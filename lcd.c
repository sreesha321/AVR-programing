/*
 * lcd.c
 *
 * Created: 30-03-2026 23:51:20
 *  Author: DELL
 */ 

#define F_CPU 16000000UL
#include "lcd.h"

// Send command to LCD (e.g. clear screen, move cursor)
void lcd_cmd(unsigned char cmd)
{
	LCD_PORT = cmd;
	LCD_PORT &= ~(1 << RS);  // RS=0 for command
	LCD_PORT |= (1 << EN);   // EN=1 to latch
	my_delay_ms(2);
	LCD_PORT &= ~(1 << EN);  // EN=0
}

// Send data (a character) to LCD
void lcd_data(unsigned char data)
{
	LCD_PORT = data;
	LCD_PORT |= (1 << RS);   // RS=1 for data
	LCD_PORT |= (1 << EN);   // EN=1 to latch
	my_delay_ms(2);
	LCD_PORT &= ~(1 << EN);  // EN=0
}

// Initialize LCD
void lcd_init(void)
{
	LCD_DDR = 0xFF;   // Set PORTD as output
	my_delay_ms(20);

	lcd_cmd(0x38);    // 8-bit mode, 2 line, 5x7 font
	lcd_cmd(0x0C);    // Display ON, cursor OFF
	lcd_cmd(0x06);    // Cursor auto increment
	lcd_cmd(0x01);    // Clear display
	my_delay_ms(2);
}

// Print string on LCD
void lcd_print(const char *str)
{
	while (*str)
	lcd_data(*str++);
}

// Print unsigned 16-bit integer on LCD
void lcd_print_uint16(uint16_t num)
{
	char buf[6];
	int i = 0;

	if (num == 0)
	{
		lcd_data('0');
		return;
	}

	while (num > 0)
	{
		buf[i++] = (num % 10) + '0';
		num /= 10;
	}

	while (i--)
	lcd_data(buf[i]);
}

// Set cursor position (row: 0 or 1, col: 0 to 15)
void lcd_set_cursor(unsigned char row, unsigned char col)
{
	unsigned char pos = (row == 0) ? (0x80 + col) : (0xC0 + col);
	lcd_cmd(pos);
}

// Clear LCD screen
void lcd_clear(void)
{
	lcd_cmd(0x01);
	my_delay_ms(2);
}