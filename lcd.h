/*
 * lcd.h
 *
 * Created: 30-03-2026 23:50:16
 *  Author: DELL
 */ 


#ifndef LCD_H_
#define LCD_H_

#include <avr/io.h>
#include <stdint.h>

// Define LCD control pins and port
#define LCD_PORT PORTD
#define LCD_DDR  DDRD

#define RS PD0
#define EN PD1

// Function prototypes
void lcd_cmd(unsigned char cmd);
void lcd_data(unsigned char data);
void lcd_init(void);
void lcd_print(const char *str);
void lcd_print_uint16(uint16_t num);
void lcd_set_cursor(unsigned char row, unsigned char col);
void lcd_clear(void);

// Declare delay function defined in main.c
void my_delay_ms(unsigned int ms);

#endif /* LCD_H_ */