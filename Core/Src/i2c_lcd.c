/*
 * Funktionen zum Midas I2C-LCD 2x8 (MC20805A6W-BNMLWI-V2)
 *
 * Version 1.0
 *
 * Joenen / HS Osnabrück 10.01.2024
 */
#include "i2c_lcd.h"

#define LCD_I2C_ADDR 0x78           // I2C Adresse der LCD Anzeige
I2C_HandleTypeDef* lcd_hi2c=NULL;   // Zeigerhandle zum I2C

/*************************************************
* lcd_init: Initiierung der RW1063 Register
*           für das 2x8 LCD
*
*  Parameter: Zeiger auf I2C Handler
*************************************************/
void lcd_init(I2C_HandleTypeDef* hi2c)
{
	uint8_t buf[7];
	lcd_hi2c=hi2c;
	HAL_Delay(5);
	buf[0] = 0x00; // CO=0, A0=0       (control byte)
	buf[1] = 0x38; // Function_set    DL=1 (I2C),  N=1 (2 Lines), F=0 (5x8 dots)
	buf[2] = 0x14; // Cursor shift right
	buf[3] = 0x0F; // Display on (D=1), Cursor on (C=1), Blink on (B=1)
	buf[4] = 0x06; // Entry_mode_set   ID=1, S=0
	buf[5] = 0x02; // return home
	buf[6] = 0x01; // Display clear
    HAL_I2C_Master_Transmit(lcd_hi2c, LCD_I2C_ADDR, buf, sizeof(buf), HAL_MAX_DELAY);
    HAL_Delay(5);
}

/**
 * write_data
 *
 *  interne Funktion zum senden von Daten
 **/
static void write_data(uint8_t data)
{
	uint8_t buf[2];

	buf[0] = 0x40; // CO=0
	buf[1] = data;
    HAL_I2C_Master_Transmit(lcd_hi2c, LCD_I2C_ADDR, buf, 2, HAL_MAX_DELAY);
}

/**
 *  write_command
 *
 *  interne Funktion zum senden von Kommandos
 *
 **/
static void write_command(uint8_t cmd)
{
	uint8_t buf[2];

	buf[0] = 0x80; // CO=1
	buf[1] = cmd;
    HAL_I2C_Master_Transmit(lcd_hi2c, LCD_I2C_ADDR, buf, 2, HAL_MAX_DELAY);
}

/**
 * lcd_clr
 *
 *  Sende Kommando clear Display
 *  (schreibt in alle DDRAM-Adressen 0x20)
 **/
void lcd_clr(void)
{
	write_command(0x01);
	HAL_Delay(5);
}

/**
 * lcd_putc
 *
 *  Ein Zeichen c an aktueller Stelle ausgeben
 *
 *  Parameter:
 *              c : Zeichen aus dem CGROM
 *
 **/
void lcd_putc(char c)
{
	write_data(c);
}

/**
 * lcd_put_st
 *
 *  Schreibt einen String (in ASCII)
 *  Maximale Laenge 8 Zeichen
 *
 *  Parameter:
 *              str : Null-terminierter String
 **/
void lcd_put_str(char* str)
{   uint8_t idx=0, buf[9];

	buf[0] = 0x40; // CO=0, RS=1
	for(idx=1;idx<=8;idx++) // maximal 8 Zeichen
	{
		buf[idx]=*(str++);
		if (buf[idx]==0) // String kürzer als 8 Zeichen?
		{
			break;
		}
	}
	HAL_I2C_Master_Transmit(lcd_hi2c, LCD_I2C_ADDR, buf, idx, HAL_MAX_DELAY);

}

/**
 * lcd_cur_pos
 *
 *  Cursor an Stelle x,y
 *
 *  Parameter: x : Spalte 0-7
 *             y : Zeile 0-1
 **/
void lcd_cur_pos(uint8_t x, uint8_t y)
{
	write_command(0x80 + (y * 0x40) + x);

}

/**
 * lcd_set_cur_blink
 *
 * Ein-/Aus des Blink-Cursors
 *
 * Parameter:
 *             cur:   Cursor Unterstrich (on/off)
 *             blink: Cursor blinken (on/off)
 **/
void lcd_set_cur_blink(cursor_mode_t cur, cursor_mode_t blink)
{
  uint8_t cmd=0x0C | (cur<<1) | blink;
  write_command(cmd);
}

