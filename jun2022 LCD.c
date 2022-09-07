__CONFIG(FOSC_HS & WDTE_OFF & PWRTE_ON & CP_OFF);

#define _XTAL_FREQ   3276800  
#include <htc.h>
#include <xc.h>
#include "lcd.h"

#define RS PORTBbits.RB1
#define EN PORTBbits.RB2
#define D4 PORTBbits.RB3
#define D5 PORTBbits.RB4
#define D6 PORTBbits.RB5
#define D7 PORTBbits.RB6

int mere=0;
int brojOsoba=0;
int max=0;
int stanje=0; //za razlicita stanja razlicita cekanja
int brojac=0;

void interrupt intcode();

void main()
{
	TRISA=0
	TRISAbits.RA0=1;
	TRISB=0;
	TRISBbits.RB0=1;
	
	OPTION_REGISTER=5H//bilo sta, pa se uklapa formulom, npr 50 ili 100Hz
	INTCON=0;
	INTCONbits.GIE=1;
	INTCONbits.INTE=1;
	Lcd_Init();
	while(1)
	{
        //cela logika problema ce se obradjivati u okviru interrupt-a
	}
}

void interrupt intcode()
{
	if(INTCONbits.INTF==1)
	{
		stanje=0;
		INTCONbits.INTF=0;
		brojOsoba++;
		EEADR=0x05;
		EECON1bits.RD=1;
		max=EEDATA;
		
		EEADR=0x06
		EECON1bits.RD=1;
		mere=EEDATA
		
		INTCONbits.T0IE=1;
		if(mere==1)
		{
			if(brojOsoba<max)
			{
				stanje=2;
				if(PORTAbits.RA0==1)
				{
					Lcd_Clear();
					Lcd_Set_Cursor(1,1);
					Lcd_Write_String("PROVERA OK");
					PORTAbits.RA1=1;
				}
				else
				{
					Lcd_Clear();
					Lcd_Set_Cursor(1,1);
					Lcd_Write_String("ZABRANJEN ULAZ");
					PORTAbits.RA1=0;
				}
			}
		}
		else
		{
			Lcd_Clear();
			Lcd_Set_Cursor(1,1);
			Lcd_Write_String("MERE UKINUTE");
			PORTAbits.RA1=1;
			stanje=1;
		}
		
	}
	if(INTCONbits.T0IF==1)
	{
		brojac++;
        //KOD asm brojac ne bi mogao do 500 vec do 255 max pa bi brojali npr 2 puta do 250
		if(stanje==1 && brojac==500)//10s vrata otvorena
		{
				brojac=0;
				PORTAbits.RA1=0;
				INTCONbits.T0IE=0;
		}
		if(stanje==2)
		{
			if(brojac==100)
			{
				Lcd_Clear();
				Lcd_Set_Cursor();
				Lcd_Write_String(brojOsoba)
			}
			if(brojac==250)
			{
				PORTAbits.RA1=0;
				brojac=0;
				INTCONbits.T0IE=0;
			}
		}
		INTCONbits.T0IF=0;
	}
}
