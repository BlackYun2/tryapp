/*
 * led.h
 *
 *  Created on: Feb 12, 2022
 *      Author: 20100
 */
#include "Led_Types.h"
#include "Dio.h"
#include "Led_Cfg.h"

#ifndef LED_H_
#define LED_H_

void LED_Init(void);
void LED_ON(led_type ledId);
void LED_OFF(led_type ledId);
void LED_TOGGLE(led_type ledId);

#endif /* LED_H_ */
