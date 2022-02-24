/*
 * Buzzer.h
 *
 *  Created on: Feb 12, 2022
 *      Author: 20100
 */

#ifndef BUZZER_H_
#define BUZZER_H_

#include "Buzzer_Types.h"
#include "Dio.h"
#include "Buzzer_Cfg.h"

void BUZZER_Init(void);
void BUZZER_ON(void);
void BUZZER_OFF(void);
void BUZZER_TOGGLE(void);
buzzer_type Buzzer_GetStatus(void);


#endif /* BUZZER_H_ */
