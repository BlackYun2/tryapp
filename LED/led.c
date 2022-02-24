/*
 * led.c
 *
 *  Created on: Feb 12, 2022
 *      Author: 20100
 */
#include"led.h"


void LED_Init(void)
{
	Dio_ConfigChannel(LED0_PORT, LED0_PIN, OUTPUT);//LED 0
	//SET_BIT(DIO_DDRC_REG,2);
	Dio_ConfigChannel(LED1_PORT, LED1_PIN, OUTPUT);//LED 1
	//SET_BIT(DIO_DDRC_REG,7);
	Dio_ConfigChannel(LED2_PORT, LED2_PIN, OUTPUT);//LED 2
	//SET_BIT(DIO_DDRD_REG,3);

}
void LED_ON(led_type ledId)
{
	switch (ledId)
	{
	case LED0:
		Dio_WriteChannel(LED0_PORT, LED0_PIN, STD_HIGH);
		break;
	case LED1:
		Dio_WriteChannel(LED1_PORT, LED1_PIN, STD_HIGH);
		break;
	case LED2:
		Dio_WriteChannel(LED2_PORT, LED2_PIN, STD_HIGH);
		break;

	}
}
void LED_OFF(led_type ledId)
{
	switch (ledId)
	{
	case LED0:
		Dio_WriteChannel(LED0_PORT, LED0_PIN, STD_LOW);
		break;
	case LED1:
		Dio_WriteChannel(LED1_PORT, LED1_PIN, STD_LOW);
		break;
	case LED2:
		Dio_WriteChannel(LED2_PORT, LED2_PIN, STD_LOW);
		break;
	}


}
void LED_TOGGLE(led_type ledId)
{

	switch (ledId)
	{
	case LED0:
		Dio_ToggleChannel(LED0_PORT, LED0_PIN);
		break;
	case LED1:
		Dio_ToggleChannel(LED1_PORT, LED1_PIN);
		break;
	case LED2:
		Dio_ToggleChannel(LED2_PORT, LED2_PIN);
		break;
	}
}


