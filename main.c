/*
 * ATmega_GLCD_TextFont
 * http://electronicwings.com
 */

#define F_CPU 8000000UL			/* Define CPU clock Freq 8MHz */
#include <avr/io.h>			/* Include AVR std. library file */
#include <util/delay.h>			/* Include delay header file */


#define Data_Port	 PORTA		/* Define data port for GLCD */
#define Command_Port	 PORTC		/* Define command port for GLCD */
#define Data_Port_Dir	 DDRA		/* Define data port for GLCD */
#define Command_Port_Dir DDRC		/* Define command port for GLCD */

#define RS		 PC0		/* Define control pins */
#define RW		 PC1
#define EN		 PC2
#define CS1		 PC3
#define CS2		 PC4
#define RST		 PC5

#define TotalPage	 8

typedef enum{
	left,
	right
}dir;
typedef enum{
	h,
	vup,
	vdown
}axis;
void GLCD_Command(char Command)		/* GLCD command function */
{
	Data_Port = Command;		/* Copy command on data pin */
	Command_Port &= ~(1 << RS);	/* Make RS LOW for command register*/
	Command_Port &= ~(1 << RW);	/* Make RW LOW for write operation */
	Command_Port |=  (1 << EN);	/* HIGH-LOW transition on Enable */
	_delay_us(5);
	Command_Port &= ~(1 << EN);
	_delay_us(5);
}

void GLCD_Data(char Data)		/* GLCD data function */
{
	Data_Port = Data;		/* Copy data on data pin */
	Command_Port |=  (1 << RS);	/* Make RS HIGH for data register */
	Command_Port &= ~(1 << RW);	/* Make RW LOW for write operation */
	Command_Port |=  (1 << EN);	/* HIGH-LOW transition on Enable */
	_delay_us(5);
	Command_Port &= ~(1 << EN);
	_delay_us(5);
}

void GLCD_Init()			/* GLCD initialize function */
{
	Data_Port_Dir = 0xFF;
	Command_Port_Dir = 0xFF;
	/* Select both left & right half of display & Keep reset pin high */
	Command_Port |= (1 << CS1) | (1 << CS2) | (1 << RST);
	_delay_ms(20);
	GLCD_Command(0x3E);		/* Display OFF */
	GLCD_Command(0x40);		/* Set Y address (column=0) */
	GLCD_Command(0xB8);		/* Set x address (page=0) */
	GLCD_Command(0xC0);		/* Set z address (start line=0) */
	GLCD_Command(0x3F);		/* Display ON */
}

void GLCD_ClearAll()			/* GLCD all display clear function */
{
	int i=0,j=0;
	/* Select both left & right half of display */
	Command_Port |= (1 << CS2);
	Command_Port &= ~(1 << CS1);
	for(int q = 0; q < 2; q++)
	{


	for(i = 0; i < TotalPage; i++)
	{
		GLCD_Command((0xB8) + i);

		for(j = 0; j < 64; j++)
		{
			GLCD_Command((0x40) + j);/* Increment page */
			GLCD_Data(0x00);	/* Write zeros to all 64 column */
		}
	}
	Command_Port |= (1 << CS1);
	Command_Port &= ~(1 << CS2);
	}
	GLCD_Command(0x40);		/* Set Y address (column=0) */
	GLCD_Command(0xB8);		/* Set x address (page=0) */
}

void GLCD_String(char page_no, char *str)/* GLCD string write function */
{
	unsigned int i, column;
	unsigned int Page = ((0xB8) + page_no);
	unsigned int Y_address = 0;
	float Page_inc = 0.5;
	unsigned int FontWidth = 1;
	Command_Port |= (1 << CS1);	/* Select Left half of display */
	Command_Port &= ~(1 << CS2);

	GLCD_Command(Page);
	for(i = 0; str[i] != 0; i++)	/* Print char in string till null */
	{
		if (Y_address > (1024-(((page_no)*128))))
		break;
		if (str[i]!=32)
		{
			for (column=1; column<=FontWidth; column++)
			{
				if ((Y_address+column)==(128*((int)(Page_inc+0.5))))
				{
					if (column == FontWidth)
					break;
					GLCD_Command(0x40);
					Y_address = Y_address + column;
					Command_Port ^= (1 << CS1);
					Command_Port ^= (1 << CS2);
					GLCD_Command(Page + Page_inc);
					Page_inc = Page_inc + 0.5;
				}
			}
		}
		if (Y_address>(1024-(((page_no)*128))))
		break;
		if((((str[i]-32))+4)==0 || str[i]==32)
		{
			for(column=0; column<FontWidth; column++)
			{
				GLCD_Data(str[i]-32);
				if((Y_address+1)%64==0)
				{
					Command_Port ^= (1 << CS1);
					Command_Port ^= (1 << CS2);
					GLCD_Command((Page+Page_inc));
					Page_inc = Page_inc + 0.5;
				}
				Y_address++;
			}
		}
		else
		{
			for(column=0; column<FontWidth; column++)
			{
				GLCD_Data(str[i]-32);
				if((Y_address+1)%64==0)
				{
					Command_Port ^= (1 << CS1);
					Command_Port ^= (1 << CS2);
					GLCD_Command((Page+Page_inc));
					Page_inc = Page_inc + 0.5;
				}
				Y_address++;
			}
			GLCD_Data(0);
			Y_address++;
			if((Y_address)%64 == 0)
			{
				Command_Port ^= (1 << CS1);
				Command_Port ^= (1 << CS2);
				GLCD_Command((Page+Page_inc));
				Page_inc = Page_inc + 0.5;
			}
		}
	}
	GLCD_Command(0x40);	/* Set Y address (column=0) */
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
void cursor_place(int x, int y, dir d)
{
	direction(d);
	GLCD_Command((0x40) + x);
	GLCD_Command((0xB8) + y);
}

int write_me_char(char c, int x, int y, dir d)
{
	int moves = 0;
	direction(d);

	GLCD_Command((0xB8) + y);
	GLCD_Command((0x40) + x);

	switch (c)
	{
		case 'F':
			GLCD_Data(0x7f);
			moves++;
			GLCD_Command((0x40) +x+moves);
			GLCD_Data(0x9);//0000 1001
			moves++;
			GLCD_Command((0x40) +x+moves);
			GLCD_Data(0x9);
			moves++;
			break;
		case 'K':
			GLCD_Data(0x7f);
			moves++;
			GLCD_Command((0x40) +x+moves);
			GLCD_Data(0x14);//0000 1001
			moves++;
			GLCD_Command((0x40) +x+moves);
			GLCD_Data(0x22);
			moves++;
			GLCD_Command((0x40) +x+moves);
			GLCD_Data(0x41);
			moves++;
			break;
		case 'H':
			GLCD_Data(0x7f);
			moves++;
			GLCD_Command((0x40) +x+moves);
			GLCD_Data(0x8);//0000 1001
			moves++;
			GLCD_Command((0x40) +x+moves);
			GLCD_Data(0x8);//0000 1001
			moves++;
			GLCD_Command((0x40) +x+moves);
			GLCD_Data(0x7f);
			moves++;
			break;
		case 'D':
			GLCD_Data(0x7f);
			moves++;
			GLCD_Command((0x40) +x+moves);
			GLCD_Data(0x41);//0000 1001
			moves++;
			GLCD_Command((0x40) +x+moves);
			GLCD_Data(0x41);//0000 1001
			moves++;
			GLCD_Command((0x40) +x+moves);
			GLCD_Data(0x3E);
			moves++;
			break;
		case 'T':
			GLCD_Data(0x01);
			moves++;
			GLCD_Command((0x40) +x+moves);
			GLCD_Data(0x01);//0000 1001
			moves++;
			GLCD_Command((0x40) +x+moves);
			GLCD_Data(0x7f);
			moves++;
			GLCD_Command((0x40) +x+moves);
			GLCD_Data(0x01);//0000 1001
			moves++;
			GLCD_Command((0x40) +x+moves);
			GLCD_Data(0x01);//0000 1001
			moves++;
			break;
		case 'Y':
			GLCD_Data(0x03);
			moves++;
			GLCD_Command((0x40) +x+moves);
			GLCD_Data(0x04);//0000 1001
			moves++;
			GLCD_Command((0x40) +x+moves);
			GLCD_Data(0x78);
			moves++;
			GLCD_Command((0x40) +x+moves);
			GLCD_Data(0x04);//0000 1001
			moves++;
			GLCD_Command((0x40) +x+moves);
			GLCD_Data(0x03);//0000 1001
			moves++;
			break;
		case 'E':
			GLCD_Data(0x7f);
			moves++;
			GLCD_Command((0x40) +x+moves);
			GLCD_Data(0x49);//0000 1001
			moves++;
			GLCD_Command((0x40) +x+moves);
			GLCD_Data(0x49);
			moves++;
			GLCD_Command((0x40) +x+moves);
			GLCD_Data(0x49);//0000 1001
			moves++;
			break;
		case 'L':
			GLCD_Data(0x7f);
			moves++;
			GLCD_Command((0x40) +x+moves);
			GLCD_Data(0x40);//0000 1001
			moves++;
			GLCD_Command((0x40) +x+moves);
			GLCD_Data(0x40);
			moves++;
			GLCD_Command((0x40) +x+moves);
			GLCD_Data(0x40);//0000 1001
			moves++;
			break;
		case 'c':
			GLCD_Data(0x3c);
			moves++;
			GLCD_Command((0x40) +x+moves);
			GLCD_Data(0x42);//0000 1001
			moves++;
			GLCD_Command((0x40) +x+moves);
			GLCD_Data(0x42);
			moves++;
			GLCD_Command((0x40) +x+moves);
			GLCD_Data(0x42);//0000 1001
			moves++;
			break;
		case 'u':
			GLCD_Data(0x7e);
			moves++;

			GLCD_Command((0x40) +x+moves);
			GLCD_Data(0x40);//0000 1001
			moves++;
			GLCD_Command((0x40) +x+moves);
			GLCD_Data(0x40);//0000 1001
			moves++;
			GLCD_Command((0x40) +x+moves);
			GLCD_Data(0x7e);
			moves++;
			break;
		case 'z':
			GLCD_Data(0x64);
			moves++;
			GLCD_Command((0x40) +x+moves);
			GLCD_Data(0x54);
			moves++;
			GLCD_Command((0x40) +x+moves);
			GLCD_Data(0x4c);
			moves++;
			break;
		case '%':
			GLCD_Data(0x86);
			moves++;
			GLCD_Command((0x40) +x+moves);
			GLCD_Data(0x49);
			moves++;
			GLCD_Command((0x40) +x+moves);
			GLCD_Data(0x29);
			moves++;
			GLCD_Command((0x40) +x+moves);
			GLCD_Data(0x16);
			moves++;
			GLCD_Command((0x40) +x+moves);
			GLCD_Data(0x68);
			moves++;
			GLCD_Command((0x40) +x+moves);
			GLCD_Data(0x94);
			moves++;
			GLCD_Command((0x40) +x+moves);
			GLCD_Data(0x92);
			moves++;
			GLCD_Command((0x40) +x+moves);
			GLCD_Data(0x61);
			moves++;
			break;
		case ':':
			GLCD_Data(0x24); //0100 0100
			moves++;
			break;
		case ' ':
			GLCD_Data(0x0); //0100 0100
			moves++;
			break;
	}
	moves++;
	return moves;

}
int write_me_num(int c, int x, int y, dir d)
{
	int moves = 0;
	direction(d);

	GLCD_Command((0xB8) + y);
	GLCD_Command((0x40) + x);

	switch (c)
	{
		case 0:
			GLCD_Data(0x3E);
			moves++;
			GLCD_Command((0x40) +x+moves);
			GLCD_Data(0x41);//0000 1001
			moves++;
			GLCD_Command((0x40) +x+moves);
			GLCD_Data(0x41);//0000 1001
			moves++;
			GLCD_Command((0x40) +x+moves);
			GLCD_Data(0x3E);
			moves++;
			break;
		case 1:
			GLCD_Data(0);
			moves++;
			GLCD_Command((0x40) +x+moves);
			GLCD_Data(0x42);
			moves++;
			GLCD_Command((0x40) +x+moves);
			GLCD_Data(0x7f);
			moves++;
			GLCD_Command((0x40) +x+moves);
			GLCD_Data(0x40);
			moves++;
			GLCD_Command((0x40) +x+moves);
			GLCD_Data(0);
			moves++;
			break;
		case 2:
			GLCD_Data(0x42);
			moves++;
			GLCD_Command((0x40) +x+moves);
			GLCD_Data(0x61);
			moves++;
			GLCD_Command((0x40) +x+moves);
			GLCD_Data(0x51);
			moves++;
			GLCD_Command((0x40) +x+moves);
			GLCD_Data(0x49);
			moves++;
			GLCD_Command((0x40) +x+moves);
			GLCD_Data(0x46);
			moves++;
			break;
		case 3:
			GLCD_Data(0x21);
			moves++;
			GLCD_Command((0x40) +x+moves);
			GLCD_Data(0x41);
			moves++;
			GLCD_Command((0x40) +x+moves);
			GLCD_Data(0x45);
			moves++;
			GLCD_Command((0x40) +x+moves);
			GLCD_Data(0x4b);
			moves++;
			GLCD_Command((0x40) +x+moves);
			GLCD_Data(0x31);
			moves++;
			break;
		case 4:
			GLCD_Data(0x18);
			moves++;
			GLCD_Command((0x40) +x+moves);
			GLCD_Data(0x14);
			moves++;
			GLCD_Command((0x40) +x+moves);
			GLCD_Data(0x12);
			moves++;
			GLCD_Command((0x40) +x+moves);
			GLCD_Data(0x7f);
			moves++;
			GLCD_Command((0x40) +x+moves);
			GLCD_Data(0x10);
			moves++;
			break;
		case 5:
			GLCD_Data(0x27);
			moves++;
			GLCD_Command((0x40) +x+moves);
			GLCD_Data(0x45);
			moves++;
			GLCD_Command((0x40) +x+moves);
			GLCD_Data(0x45);
			moves++;
			GLCD_Command((0x40) +x+moves);
			GLCD_Data(0x45);
			moves++;
			GLCD_Command((0x40) +x+moves);
			GLCD_Data(0x39);
			moves++;
			break;
		case 6:
			GLCD_Data(0x3c);
			moves++;
			GLCD_Command((0x40) +x+moves);
			GLCD_Data(0x4a);
			moves++;
			GLCD_Command((0x40) +x+moves);
			GLCD_Data(0x49);
			moves++;
			GLCD_Command((0x40) +x+moves);
			GLCD_Data(0x49);
			moves++;
			GLCD_Command((0x40) +x+moves);
			GLCD_Data(0x30);
			moves++;
			break;
		case 7:
			GLCD_Data(0x1);
			moves++;
			GLCD_Command((0x40) +x+moves);
			GLCD_Data(0x71);
			moves++;
			GLCD_Command((0x40) +x+moves);
			GLCD_Data(0x9);
			moves++;
			GLCD_Command((0x40) +x+moves);
			GLCD_Data(0x5);
			moves++;
			GLCD_Command((0x40) +x+moves);
			GLCD_Data(0x3);
			moves++;
			break;
		case 8:
			GLCD_Data(0x36);
			moves++;
			GLCD_Command((0x40) +x+moves);
			GLCD_Data(0x49);
			moves++;
			GLCD_Command((0x40) +x+moves);
			GLCD_Data(0x49);
			moves++;
			GLCD_Command((0x40) +x+moves);
			GLCD_Data(0x49);
			moves++;
			GLCD_Command((0x40) +x+moves);
			GLCD_Data(0x36);
			moves++;
			break;
		case 9:
			GLCD_Data(0x6);
			moves++;
			GLCD_Command((0x40) +x+moves);
			GLCD_Data(0x49);
			moves++;
			GLCD_Command((0x40) +x+moves);
			GLCD_Data(0x49);
			moves++;
			GLCD_Command((0x40) +x+moves);
			GLCD_Data(0x29);
			moves++;
			GLCD_Command((0x40) +x+moves);
			GLCD_Data(0x1e);
			moves++;
			break;
	}
	moves++;
	return moves;

}

int present_num(int num, int x, int y, dir direction)
{
	int moves = 0;
	if(num/10 == 0)
	{
		moves += write_me_num(num, x, y, direction);
	}
	else if(num/100 == 1)
	{
		moves += write_me_num(1, x, y, direction);
		moves += write_me_num(0, x+moves, y, direction);
		moves += write_me_num(0, x+moves, y, direction);
	}
	else
	{
		moves += write_me_num(num/10, x, y, direction);
		moves += write_me_num(num%10, x+moves, y, direction);
	}
	return moves;
}


int line(axis a, int x, int y)
{
	int moves = 0;

	GLCD_Command((0xB8) + y);
	GLCD_Command((0x40) + x);
	switch(a)
	{
	case h:
		GLCD_Data(0xff);
		GLCD_Command((0x40) + x);
		GLCD_Command((0xB8)+ y  + 1);
		GLCD_Data(0xff);
		GLCD_Command((0xB8)+ y);
		moves++;
		break;
	case vup:
		moves++;
		GLCD_Command((0xB8)+ y  + 1);
		GLCD_Data(0x80);
		GLCD_Command((0xB8)+ y);
		break;
	case vdown:
		moves++;
		GLCD_Data(0x1);
		break;
	}
	return moves;
}


int main(void)
{
	GLCD_Init();		/* Initialize GLCD */
	GLCD_ClearAll();	/* Clear all GLCD display */
	dir dd = left;
	direction(dd);
	int x = 0;
	int y = 4;
	int countxleft = 0;
	int countxright = 0;
	int countnum = 0;
	x+=line(h, x, y);
	while(1)
	{
		dd = left;
		direction(dd);
		x+=line(vup, x, y);
		countxleft += write_me_char('F', countxleft, 0, left);
		countxleft += write_me_char(':', countxleft, 0, left);
		//number here
		countxleft += present_num(8, countxleft, 0, left);

		countxleft += write_me_char('K', countxleft, 0, left);
		countxleft += write_me_char('H', countxleft, 0, left);
		countxleft += write_me_char('z', countxleft, 0, left);

		countxright += write_me_char('D', countxright, 0, right);
		countxright += write_me_char('u', countxright, 0, right);
		countxright += write_me_char('T', countxright, 0, right);
		countxright += write_me_char('Y', countxright, 0, right);
		countxright += write_me_char(' ', countxright, 0, right);
		countxright += write_me_char('c', countxright, 0, right);
		countxright += write_me_char('Y', countxright, 0, right);
		countxright += write_me_char('c', countxright, 0, right);
		countxright += write_me_char(':', countxright, 0, right);

		countxright += present_num(50, countxright, 0, right);
		countxright += write_me_char('%', countxright, 0, right);


		//x+= write_me_char('D', x, y, dd);
		//x+= write_me_char('T', x, y, dd);


		if(x >= 64)
		{
			x = 0;
			dd = !dd;

		}


		if(y == 8)
		{
			y = 0;
			if(dd == left)
				dd = right;
			else
				dd = left;
		}
		countxright = 0;
		countxleft = 0;
	}
}
