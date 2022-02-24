/*
 * SSD.c
 *
 *  Created on: Feb 12, 2022
 *      Author: 20100
 */
#include"SSD.h"

void SSD_Init(void)
{
	// CONFIGURE DATA LINES AS OUTPUT
	Dio_ConfigChannel(DIO_PORTA, DIO_PIN4, OUTPUT);
	Dio_ConfigChannel(DIO_PORTA, DIO_PIN5, OUTPUT);
	Dio_ConfigChannel(DIO_PORTA, DIO_PIN6, OUTPUT);
	Dio_ConfigChannel(DIO_PORTA, DIO_PIN7, OUTPUT);


	// CONFIGURE CONTROL LINES AS OUTPUT
	Dio_ConfigChannel(DIO_PORTB, DIO_PIN1, OUTPUT);
	Dio_ConfigChannel(DIO_PORTB, DIO_PIN2, OUTPUT);

}


/*screen flicker on first digit if you put delay
because the second one is at the end so it
will stay on untill the function work again
but the first one will be off at
that time so your eye will notice*/
SSD_DisplayNumber(u8 num)
{
	u8 loc_FirstDigit = (num/10)<<4;
	u8 loc_SecondtDigit = (num%10)<<4;
	Dio_WriteGroup(DIO_PORTA, SSD_MASK, loc_FirstDigit);
	_delay_ms(1);
	//enable ssd1
	Dio_WriteChannel(DIO_PORTB, DIO_PIN2, STD_HIGH);
	//disable ssd2
	Dio_WriteChannel(DIO_PORTB, DIO_PIN1, STD_LOW);
	//DIO_PORTA_REG = (num/10)<<4;
	Dio_WriteGroup(DIO_PORTA, SSD_MASK, loc_FirstDigit);
	//wait (1m)
	_delay_ms(1);
	//disable ssd1
	Dio_WriteChannel(DIO_PORTB, DIO_PIN2, STD_LOW);
	//enable ssd2
	Dio_WriteChannel(DIO_PORTB, DIO_PIN1, STD_HIGH);
	//DIO_PORTA_REG = (num%10)<<4;
	Dio_WriteGroup(DIO_PORTA, SSD_MASK, loc_SecondtDigit);
	//wait (1m)
	_delay_ms(1);
}


