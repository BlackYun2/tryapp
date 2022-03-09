/*
 * ATmega 16 frequency and duty cycle measurment using input capture
 * www.electronicwings.com
 */


#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>

void PWM_init()
{
	/*set fast PWM mode with non-inverted output*/
	TCCR0 = (1<<WGM00) | (1<<WGM01) | (1<<COM01) | (1<<CS00)| (1<<CS01);
	DDRB|=(1<<PB3);  /*set OC0 pin as output*/
}
void PWM_set_duty(char x)
{
	OCR0 = x;
}
