/*
 * Dio_Types.h
 *
 *  Created on: Feb 12, 2022
 *      Author: 20100
 */

#ifndef DIO_TYPES_H_
#define DIO_TYPES_H_

typedef enum{
	DIO_PORTA,
	DIO_PORTB,
	DIO_PORTC,
	DIO_PORTD
}port_type; /**/
typedef enum{
	DIO_PIN0,
	DIO_PIN1,
	DIO_PIN2,
	DIO_PIN3,
	DIO_PIN4,
	DIO_PIN5,
	DIO_PIN6,
	DIO_PIN7
}pin_type; /**/
typedef enum{
	INPUT,
	OUTPUT
}direction_type; /**/
typedef enum{
	STD_LOW,
	STD_HIGH
}level_type; /**/

#endif /* DIO_TYPES_H_ */
