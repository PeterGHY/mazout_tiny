/****************************************************************************
    LCD-AVR-4f.c  - Use an HD44780U based LCD with an Atmel ATmega processor
 
    Copyright (C) 2013 Donald Weiman    (weimandn@alfredstate.edu)
 
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
 
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
 
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
/****************************************************************************
         File:    LCD-AVR-4f.c
         Date:    September 16, 2013
 
       Target:    ATmega328
     Compiler:    avr-gcc (AVR Studio 6)
       Author:    Donald Weiman
 
      Summary:    8-bit data interface, with busy flag implemented.
                  Any LCD pin can be connected to any available I/O port.
                  Includes a simple write string routine.
 */
/******************************* Program Notes ******************************
 
            This program uses a 4-bit data interface and it uses the busy
              flag to determine when the LCD controller is ready.  The LCD
              RW line (pin 5) must therefore be connected to the uP.
 
            The use of the busy flag does not mean that all of the software
              time delays have been eliminated.  There are still several
              required in the LCD initialization routine where the busy flag
              cannot yet be used.  These delays are have been implemented at
              least twice as long as called for in the data sheet to
              accommodate some of the out of spec displays that may show up.
              There are also some other software time delays required to
              implement timing requirements such as setup and hold times for
              the various control signals.
 
  ***************************************************************************
 
            The four data lines as well as the three control lines may be
              implemented on any available I/O pin of any port.  These are
              the connections used for this program:
 
                 -----------                   ----------
                | ATmega328 |                 |   LCD    |
                |           |                 |          |
                |        PD7|---------------->|D7        |
                |        PD6|---------------->|D6        |
                |        PD5|---------------->|D5        |
                |        PD4|---------------->|D4        |
                |           |                 |D3        |
                |           |                 |D2        |
                |           |                 |D1        |
                |           |                 |D0        |
                |           |                 |          |
                |        PB1|---------------->|E         |
                |        PB2|---------------->|RW        |
                |        PB0|---------------->|RS        |
                 -----------                   ----------
 
  **************************************************************************/

#pragma once

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>

// LCD interface (should agree with the diagram above)
#define lcd_D7_port     PORTA                   // lcd D7 connection
#define lcd_D7_bit      PORTA7
#define lcd_D7_ddr      DDRA
#define lcd_D7_pin      PINA                    // busy flag

#define lcd_D6_port     PORTA                   // lcd D6 connection
#define lcd_D6_bit      PORTA6
#define lcd_D6_ddr      DDRA

#define lcd_D5_port     PORTA                   // lcd D5 connection
#define lcd_D5_bit      PORTA5
#define lcd_D5_ddr      DDRA

#define lcd_D4_port     PORTA                   // lcd D4 connection
#define lcd_D4_bit      PORTA4
#define lcd_D4_ddr      DDRA

#define lcd_E_port      PORTA                   // lcd Enable pin
#define lcd_E_bit       PORTA1
#define lcd_E_ddr       DDRA

#define lcd_RS_port     PORTA                   // lcd Register Select pin
#define lcd_RS_bit      PORTA3
#define lcd_RS_ddr      DDRA

#define lcd_RW_port     PORTA                   // lcd Read/Write pin
#define lcd_RW_bit      PORTA2
#define lcd_RW_ddr      DDRA

// LCD module information
#define lcd_LineOne     0x00                    // start of line 1
#define lcd_LineTwo     0x40                    // start of line 2
//#define   lcd_LineThree   0x14                  // start of line 3 (20x4)
//#define   lcd_lineFour    0x54                  // start of line 4 (20x4)
//#define   lcd_LineThree   0x10                  // start of line 3 (16x4)
//#define   lcd_lineFour    0x50                  // start of line 4 (16x4)

// LCD instructions
#define lcd_Clear           0b00000001          // replace all characters with ASCII 'space'
#define lcd_Home            0b00000010          // return cursor to first position on first line
#define lcd_EntryMode       0b00000110          // shift cursor from left to right on read/write
#define lcd_DisplayOff      0b00001000          // turn display off
#define lcd_DisplayOn       0b00001100          // display on, cursor off, don't blink character
#define lcd_FunctionReset   0b00110000          // reset the LCD
#define lcd_FunctionSet4bit 0b00101000          // 4-bit data, 2-line display, 5 x 7 font
#define lcd_SetCursor       0b10000000          // set cursor position
#define lcd_SetCursor2      0b10000001          // set cursor position

// Function Prototypes
void lcd_write(uint8_t);
void lcd_write_instruction(uint8_t);
void lcd_write_character(uint8_t);
void lcd_write_string(uint8_t *);
void lcd_init(void);
void lcd_check_BF(void);

/************************
 * Peter 20/02/2016
 * **********************/

// void lcd_goto(uint8_t x, uint8_t y);            // put LCD cursor on specified line 
// void lcd_Hex(int data);                         // display hex value
// void lcd_TwoDigits(uint8_t data);
// void lcd_string_format(char *szFormat, ...);
// 
// // void lcdWriteInt(int val,unsigned int field_length);
// void lcdGotoXY(uint8_t x,uint8_t y);

/***************************************************
    M A C R O S
***************************************************/
// #define lcdWriteIntXY(x,y,val,fl) {\
//  lcdGotoXY(x,y);\
//  lcdWriteInt(val,fl);\
// }
/***************************************************/
