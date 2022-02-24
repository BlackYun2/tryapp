/*
 * Button.c
 *
 *  Created on: Feb 12, 2022
 *      Author: 20100
 */

#include "Button.h"

void Button_Init(void)
{
	Dio_ConfigChannel(BUTTON0_PORT, BUTTON0_PIN, INPUT);//BUTTON 0
	//CLR_BIT(DIO_DDRB_REG,0);
	Dio_ConfigChannel(BUTTON1_PORT, BUTTON1_PIN, INPUT);//BUTTON 1
	//CLR_BIT(DIO_DDRD_REG,6);
	Dio_ConfigChannel(BUTTON2_PORT, BUTTON2_PIN, INPUT);//BUTTON 2
	//CLR_BIT(DIO_DDRD_REG,2);
}
button_type Button_GetStatus(buttonID_type button)
{
	button_type status = RELEASED;
	switch (button)
	{
	case BUTTON0:
		if(Dio_ReadChannel(BUTTON0_PORT, BUTTON0_PIN) == STD_HIGH)
		{
			status = PRESSED;
		}
		else
		{
			status = RELEASED;
		}
		break;
	case BUTTON1:
		if(Dio_ReadChannel(BUTTON1_PORT, BUTTON1_PIN) == STD_HIGH)
		{
			status = PRESSED;
		}
		else
		{
			status = RELEASED;
		}
		break;
	case BUTTON2:
		if(Dio_ReadChannel(BUTTON2_PORT, BUTTON2_PIN) == STD_HIGH)
		{
			status = PRESSED;
		}
		else
		{
			status = RELEASED;
		}
		break;


	}
	return status;
}


