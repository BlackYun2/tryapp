/*
 * Lcd.h
 *
 *  Created on: Nov 26, 2021
 *      Author: Eng_Fawzi
 */

#ifndef _LCD_H_
#define _LCD_H_
#include <util\delay.h>
#include "Dio.h"
#include "Lcd_Types.h"

#define LCD_MASK		0xF0
#define NO_CSTOM_CHAR    		(8)
#define NO_CSTOM_CHAR_BYTES 	(8)
void Lcd_Init(void);
void Lcd_Cmd(lcd_Cmd_Type cmd);
void Lcd_DisplayChr(u8 chr);
void Lcd_DisplayStr(u8* str);
void Lcd_Goto_Row_Column(u8 row, u8 col);
void Lcd_DisplayNum(u16 num);
void LcdDisplayFloat(f64 floatNum);
#endif /* 01_SOURCE_02_HAL_LCD_INC_LCD_H_ */

