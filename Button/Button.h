/*
 * Button.h
 *
 *  Created on: Feb 12, 2022
 *      Author: 20100
 */

#ifndef BUTTON_H_
#define BUTTON_H_

#include "Button_Types.h"
#include "Dio.h"
#include "Button_cfg.h"

void Button_Init(void);
button_type Button_GetStatus(buttonID_type button);


#endif /* BUTTON_H_ */
