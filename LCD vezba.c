/*
 1.5s
 MIKRORACUNARSKI
 SISTEMI
 
 1.5s
 2020/21.
 ELFAK
 
 Ime i prezime
    -u jednom redu
    -na po 300ms ulevo
    -na po 300ms udesno
 
 Pauza 2s
 */

#define _XTAL_FREQ 8000000
#include <htc.h>
#include <xc.h>

__CONFIG(FOSC_HS & WDTE_OFF & PWRTE_ON & CP_OFF);
#define RS PORTBbits.RB1
#define EN PORTBbits.RB2
#define D4 PORTBbits.RB3
#define D5 PORTBbits.RB4
#define D6 PORTBbits.RB5
#define D7 PORTBbits.RB6

#include "lcd.h"

int main()
{
    unsigned int a;
    TRISB = 0x00;
    Lcd_Init();
    while(1)
    {
        Lcd_Clear();
        Lcd_Set_Cursor(1,1); //prva vrsta prva kolona
        Lcd_Write_String("MIKRORACUNARSKI");
        Lcd_Set_Cursor(2,1); //druga vrsta prva kolona
        Lcd_Write_String("SISTEMI");
        
        __delay_ms(1500);
        Lcd_Clear();
        Lcd_Set_Cursor(1,1);
        Lcd_Write_String("2020/21.");
        Lcd_Set_Cursor(2,1);
        Lcd_Write_String("ELFAK");
        __delay_ms(1500);
        Lcd_Clear
        Lcd_Set_Cursor(1,1);
        Lcd_Write_String("Djordje Antic");
        
        for (a = 0; a < 13; a++)
        {
            //IDE DO DUZINE STRINGA
            //Da li moze strlen da se korist??
            __delay_ms(300);
            Lcd_Shift_Left();
        }
        for (a = 0; a < 13; a++)
        {
            __delay_ms(300);
            Lcd_Shift_Right();
        }
        
        __delay_ms(1000)
    }
    return 0;
}
