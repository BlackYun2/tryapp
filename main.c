#define F_CPU 16000000UL /* Define CPU clock Frequency 8MHz */
#include <avr/io.h>     /* Include AVR std. library file */
#include <util/delay.h> /* Include defined delay header file */
#include <avr/interrupt.h>
#include <stdlib.h>
#include <stdio.h>      /* Include standard i/o library file */

int main(void)
{

	PWM_init();
	PWM_set_duty(127);
	unsigned int a,b,c,high,period;
	char frequency[14],duty_cy[7];
	PORTD = 0xFF;			/* Turn ON pull-up resistor */


	char breaks = 2;
	char cap = 128 - breaks;
    int f = 16;
    int cycle = 25;
    int x = 90;
    int move = 0;
    char space[20] = "    ";
    char firstLine[128];

    GLCD_Init();         /* Initialize GLCD */
    GLCD_ClearAll();     /* Clear all GLCD display */
    while (1)
    {
    	TCCR1A = 0;
        TCNT1=0;
        TIFR = (1<<ICF1);  	/* Clear ICF (Input Capture flag) flag */

        TCCR1B = 0x41;  	/* Rising edge, no prescaler */
        while ((TIFR&(1<<ICF1)) == 0);
        a = ICR1;  		/* Take value of capture register */
        TIFR = (1<<ICF1);  	/* Clear ICF flag */

        TCCR1B = 0x01;  	/* Falling edge, no prescaler */
        while ((TIFR&(1<<ICF1)) == 0);
        b = ICR1;  		/* Take value of capture register */
        TIFR = (1<<ICF1);  	/* Clear ICF flag */

        TCCR1B = 0x41;  	/* Rising edge, no prescaler */
        while ((TIFR&(1<<ICF1)) == 0);
        c = ICR1;  		/* Take value of capture register */
        TIFR = (1<<ICF1);  	/* Clear ICF flag */

        TCCR1B = 0;  		/* Stop the timer */

        if(a<b && b<c)  	/* Check for valid condition,
        			to avoid timer overflow reading */
        {
        	high=b-a;
        	period=c-a;

        	long freq= F_CPU/period;/* Calculate frequency */

        				/* Calculate duty cycle */
        	    		float duty_cycle =((float) high /(float)period)*100;
        	ltoa(freq,frequency,10);

        	itoa((int)duty_cycle,duty_cy,10);

        	f = freq;
        	cycle = duty_cycle;

        }

        else
        {
        	f = 0;
        	cycle = 0;
        	GLCD_String(0, "error");
        }








        strcat(' ', ' ');
    	strcpy(firstLine, get_firstline(f, cycle, space));
    	GLCD_String(0, firstLine); /* Print String on 0th page of display */
    	draw_line(f, cycle);

    }
}
