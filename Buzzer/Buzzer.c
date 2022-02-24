/*
 * Buzzer.c
 *
 *  Created on: Feb 12, 2022
 *      Author: 20100
 */
#include "Buzzer.h"

void BUZZER_Init(void)
{
	Dio_ConfigChannel(BUZZER_PORT, BUZZER_PIN, OUTPUT);
}
void BUZZER_ON(void)
{
	Dio_WriteChannel(BUZZER_PORT, BUZZER_PIN, STD_HIGH);
}

void BUZZER_OFF(void)
{
	Dio_WriteChannel(BUZZER_PORT, BUZZER_PIN, STD_LOW);
}
void BUZZER_TOGGLE(void)
{
	Dio_ToggleChannel(BUZZER_PORT, BUZZER_PIN);
}
buzzer_type Buzzer_GetStatus(void)
{
	buzzer_type status = OFF;
	if(Dio_ReadChannel(BUZZER_PORT, BUZZER_PIN) == STD_HIGH)
	{
		status = ON;
	}
	else
	{
		status = OFF;
	}
	return status;
}

