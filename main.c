#define F_CPU 16000000UL

#include <avr/io.h>
#include <stdint.h>
#include "lcd.h"

// Custom delay function (approximate ms delay)
void my_delay_ms(unsigned int ms)
{
	unsigned int i, j;

	for (i = 0; i < ms; i++)
	{
		for (j = 0; j < 1600; j++);
	}
}

// Initialize UART for 9600 baud, 8-bit data, no parity, 1 stop bit
void uart_init(void)
{
	UBRR0H = 0;
	UBRR0L = 103; // For 16MHz clock

	UCSR0B = (1 << TXEN0); // Enable transmitter
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // 8-bit data
}

// Transmit one byte over UART
void uart_tx(char data)
{
	while (!(UCSR0A & (1 << UDRE0))); // Wait until buffer empty
	UDR0 = data;
}

// Transmit string over UART
void uart_print(const char *s)
{
	while (*s)
	uart_tx(*s++);
}

// Transmit unsigned 16-bit integer as decimal string over UART
void uart_print_uint16(uint16_t v)
{
	char buf[6];
	int i = 0;

	if (v == 0)
	{
		uart_tx('0');
		return;
	}

	while (v > 0)
	{
		buf[i++] = (v % 10) + '0';
		v /= 10;
	}

	while (i--)
	uart_tx(buf[i]);
}

// Initialize ADC to use AVcc as reference and ADC0 channel
void adc_init(void)
{
	ADMUX = (1 << REFS0); // AVcc reference, ADC0
	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1); // Enable ADC, prescaler 64
}

// Read ADC value from ADC0 channel
uint16_t adc_read(void)
{
	ADCSRA |= (1 << ADSC);            // Start conversion
	while (ADCSRA & (1 << ADSC));     // Wait for conversion to finish
	return ADC;
}

// Convert ADC reading to temperature in Celsius
uint16_t get_temperature(void)
{
	uint16_t adc_val = adc_read();

	// LM35 outputs 10mV per degree Celsius
	// ADC resolution: 5V / 1023 steps
	// temp = (adc_val * 5V * 100) / 1023 = (adc_val * 500) / 1023
	return (adc_val * 500UL) / 1023;
}

int main(void)
{
	uint16_t temp;

	lcd_init();
	uart_init();
	adc_init();

	DDRB |= (1 << PB0); // Configure PB0 as output for LED

	while (1)
	{
		temp = get_temperature();

		// Display temperature on LCD
		lcd_set_cursor(0, 0);
		lcd_print("Temp: ");
		lcd_print_uint16(temp);
		lcd_print(" C   ");

		// Send temperature via UART
		uart_print("Temperature: ");
		uart_print_uint16(temp);
		uart_print(" C\r\n");

		// Blink LED to indicate system running
		PORTB ^= (1 << PB0);

		// Wait 0.5 seconds using custom delay
		my_delay_ms(500);
	}
}