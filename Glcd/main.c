/*
 * ATmega_GLCD_TextFont
 * http://electronicwings.com
 */

#define F_CPU 8000000UL /* Define CPU clock Frequency 8MHz */
#include <avr/io.h>     /* Include AVR std. library file */
#include <util/delay.h> /* Include defined delay header file */
#include <stdio.h>      /* Include standard i/o library file */
#include "Font_Header.h"
#include <string.h>

#define Data_Port PORTA       /* Define data port for GLCD */
#define Command_Port PORTC    /* Define command port for GLCD */
#define Data_Port_Dir DDRA    /* Define data port for GLCD */
#define Command_Port_Dir DDRC /* Define command port for GLCD */

#define RS PC0 /* Define control pins */
#define RW PC1
#define EN PC2
#define CS1 PC3
#define CS2 PC4
#define RST PC5

#define TotalPage 8

typedef enum{
	left,
	right
}dir;
typedef enum{
	h,
	vup,
	vdown
}axis;


int ndigit(int n)
{
    if (n/10 == 0)
        return 1;
    return 1 + ndigit(n / 10);
}

void GLCD_Command(char Command) /* GLCD command function */
{
    Data_Port = Command;        /* Copy command on data pin */
    Command_Port &= ~(1 << RS); /* Make RS LOW to select command register */
    Command_Port &= ~(1 << RW); /* Make RW LOW to select write operation */
    Command_Port |= (1 << EN);  /* Make HIGH to LOW transition on Enable pin */
    _delay_us(5);
    Command_Port &= ~(1 << EN);
    _delay_us(5);
}

void GLCD_Data(char Data) /* GLCD data function */
{
    Data_Port = Data;           /* Copy data on data pin */
    Command_Port |= (1 << RS);  /* Make RS HIGH to select data register */
    Command_Port &= ~(1 << RW); /* Make RW LOW to select write operation */
    Command_Port |= (1 << EN);  /* Make HIGH to LOW transition on Enable pin */
    _delay_us(5);
    Command_Port &= ~(1 << EN);
    _delay_us(5);
}

void GLCD_Init() /* GLCD initialize function */
{
    Data_Port_Dir = 0xFF;
    Command_Port_Dir = 0xFF;
    /* Select both left & right half of display & Keep reset pin high */
    Command_Port |= (1 << CS1) | (1 << CS2) | (1 << RST);
    _delay_ms(20);
    GLCD_Command(0x3E); /* Display OFF */
    GLCD_Command(0x40); /* Set Y address (column=0) */
    GLCD_Command(0xB8); /* Set x address (page=0) */
    GLCD_Command(0xC0); /* Set z address (start line=0) */
    GLCD_Command(0x3F); /* Display ON */
}

void GLCD_ClearAll() /* GLCD all display clear function */
{
    int i, j;
    /* Select both left & right half of display */
    // Command_Port |= (1 << CS1) | (1 << CS2);
    Command_Port |= (1 << CS2);
    Command_Port &= ~(1 << CS1);
    for (int q = 0; q < 2; q++)
    {
        for (i = 0; i < TotalPage; i++)
        {
            GLCD_Command((0xB8) + i); /* Increment page each time after 64 column */
            for (j = 0; j < 64; j++)
            {
                GLCD_Data(0); /* Write zeros to all 64 column */
            }
        }
        Command_Port |= (1 << CS1);
        Command_Port &= ~(1 << CS2);
    }
    GLCD_Command(0x40); /* Set Y address (column=0) */
    GLCD_Command(0xB8); /* Set x address (page=0) */
}

void direction(dir d)
{
	if(d == left)
	{
		Command_Port |= (1 << CS2);
		Command_Port &= ~(1 << CS1);
	}
	else
	{
		Command_Port |= (1 << CS1);
		Command_Port &= ~(1 << CS2);
	}
}
void clearline(axis a, int x, int y, int n, dir d)
{
	direction(d);
		switch(a)
		{
		case h:
			break;
		case vdown:
			GLCD_Command((0xB8) + y);
			GLCD_Command((0x40) + x);
			break;
		case vup:
			GLCD_Command((0xB8) + y + 1);
			GLCD_Command((0x40) + x);
			break;
		}
	for (int j = 0; j < n; j++)
	{
		GLCD_Data(0);
	}
	GLCD_Command(0x40);
}

void cursor_place(int x, int y, dir d)
{
	direction(d);
	GLCD_Command((0x40) + x);
	GLCD_Command((0xB8) + y);
}


void GLCD_String(char page_no, char *str) /* GLCD string write function */
{
    unsigned int i, column;
    unsigned int Page = ((0xB8) + page_no);
    unsigned int Y_address = 0;
    float Page_inc = 0.5;

    direction(left); /* Select first Left half of display */

    GLCD_Command(Page);
    for (i = 0; str[i] != 0; i++) /* Print each char in string till null */
    {
        if (Y_address > (1024 - (((page_no)*128) + FontWidth))) /* Check Whether Total Display get overflowed */
            break;                                              /* If yes then break writing */
        if (str[i] != 32)                                       /* Check whether character is not a SPACE */
        {
            for (column = 1; column <= FontWidth; column++)
            {
                if ((Y_address + column) == (128 * ((int)(Page_inc + 0.5)))) /* If yes then check whether it overflow from right side of display */
                {
                    if (column == FontWidth) /* Also check and break if it overflow after 5th column */
                        break;
                    GLCD_Command(0x40);             /* If not 5th and get overflowed then change Y address to START column */
                    Y_address = Y_address + column; /* Increment Y address count by column no. */
                    Command_Port ^= (1 << CS1);     /* If yes then change segment controller to display on other half of display */
                    Command_Port ^= (1 << CS2);
                    GLCD_Command(Page + Page_inc); /* Execute command for page change */
                    Page_inc = Page_inc + 0.5;     /* Increment Page No. by half */
                }
            }
        }
        if (Y_address > (1024 - (((page_no)*128) + FontWidth)))           /* Check Whether Total Display get overflowed */
            break;                                                        /* If yes then break writing */
        if ((font[((str[i] - 32) * FontWidth) + 4]) == 0 || str[i] == 32) /* Check whether character is SPACE or character last column is zero */
        {
            for (column = 0; column < FontWidth; column++)
            {
                GLCD_Data(font[str[i] - 32][column]); /* If yes then then print character */
                if ((Y_address + 1) % 64 == 0)        /* check whether it gets overflowed  from either half of side */
                {
                    Command_Port ^= (1 << CS1); /* If yes then change segment controller to display on other half of display */
                    Command_Port ^= (1 << CS2);
                    GLCD_Command((Page + Page_inc)); /* Execute command for page change */
                    Page_inc = Page_inc + 0.5;       /* Increment Page No. by half */
                }
                Y_address++; /* Increment Y_address count per column */
            }
        }
        else /* If character is not SPACE or character last column is not zero */
        {
            for (column = 0; column < FontWidth; column++)
            {
                GLCD_Data(font[str[i] - 32][column]); /* Then continue to print hat char */
                if ((Y_address + 1) % 64 == 0)        /* check whether it gets overflowed  from either half of side */
                {
                    Command_Port ^= (1 << CS1); /* If yes then change segment controller to display on other half of display */
                    Command_Port ^= (1 << CS2);
                    GLCD_Command((Page + Page_inc)); /* Execute command for page change */
                    Page_inc = Page_inc + 0.5;       /* Increment Page No. by half */
                }
                Y_address++; /* Increment Y_address count per column */
            }
            GLCD_Data(0);              /* Add one column of zero to print next character next of zero */
            Y_address++;               /* Increment Y_address count for last added zero */
            if ((Y_address) % 64 == 0) /* check whether it gets overflowed  from either half of side */
            {
                Command_Port ^= (1 << CS1); /* If yes then change segment controller to display on other half of display */
                Command_Port ^= (1 << CS2);
                GLCD_Command((Page + Page_inc)); /* Execute command for page change */
                Page_inc = Page_inc + 0.5;       /* Increment Page No. by half */
            }
        }
    }
    GLCD_Command(0x40); /* Set Y address (column=0) */
}


int line(axis a, int x, int y, int n, dir d)
{
	int moves = 0;
	direction(d);
	GLCD_Command((0xB8) + y);
	GLCD_Command((0x40) + x);
	for(int i = 0; i < n; i++)
	switch(a)
	{
	case h:
		GLCD_Data(0xff);
		GLCD_Command((0x40) + x);
		GLCD_Data(0xff);
		GLCD_Command((0xB8)+ y);
		moves++;
		break;
	case vdown:
		moves++;
		GLCD_Data(0x80);
		break;
	case vup:
		moves++;
		GLCD_Data(0x01);
		break;
	}
	GLCD_Command(0x40);
	return moves;
}

char* get_firstline(int f, int cycle, char* space)
{
	char fstr[4];
	sprintf(fstr, "%d", f);
	char cyclestr[4];
    sprintf(cyclestr, "%d", cycle);
	char firstLine[128] = "";
    strcat(firstLine, "F:");
    strcat(firstLine, fstr);
    strcat(firstLine, " KHz");
    strcat(firstLine, space);
    strcat(firstLine, "Duty:");
    strcat(firstLine, cyclestr);
    strcat(firstLine, "%");
    return firstLine;
}
void draw_line(int f, int cycle)
{
	char l = 120;
	char moves = 0;
	dir d = left;
	char r = 128;
	if(cycle == 100)
	{
		line(vup, 0, 3, 64, d);
		d = !d;
		line(vup, 0, 3, 64, d);
	}
	else if(cycle == 0)
	{
		line(vdown, 0, 5, 64, d);
		d = !d;
		line(vdown, 0, 5, 64, d);
	}
	else
	{
		moves += line(h, 0, 4, 1, d);
		r = r - moves;
		if(r - l <= 64)
		{
			r = r - l;
			l = 64 - moves;
			line(vup, moves, 3, l, d);
			d = !d;
			moves = 0;
			line(vup, moves, 3, r, d);
		}
		else
		{
			line(vup, moves, 3, l, d);
		}
	}
}

int main(void)
{
	char breaks = 2;
	char cap = 128 - breaks;
    int f = 16;
    int cycle = 50;
    int x = 90;
    int move = 0;
    char space[20] = "    ";
    char firstLine[128];

    GLCD_Init();         /* Initialize GLCD */
    GLCD_ClearAll();     /* Clear all GLCD display */
    while (1)
    {
        strcat(' ', ' ');
    	strcpy(firstLine, get_firstline(f, cycle, space));
    	GLCD_String(0, firstLine); /* Print String on 0th page of display */

		line(vup, 0, 3, 8, left);
		line(vdown, 8, 3, 8, left);
		line(vup, 16, 3, 8, left);
		line(vdown, 24, 3, 8, left);

    }
}
