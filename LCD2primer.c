/*
 DODATNI ZADATAK LCD INTERFON
 
 RB1-RB6 za LCD
 RA0 i RA1 na spoljasnji uredjaj koji
 salje MC-u sinale koji predstavljaju
 elemente sifre
    00 - 0
    01 - 1
    10 - 2
    11 - 3
 U EEPROM je tacna sifra pocevsi
 od lokacije 0x03
 
 Pritiskom na RB0 interaptom se potvrdjuje
 unos pojedinacne sifre, uneta cifra dodaje
 sadrzaju na displeju i nakon cetvrte proverava
 sa tacnim nizom, a zatim sadrzaj displeja
 resetuje
 
 Ako je sifra tacna -> ZELENI LED (na RA3)
 Ako nije -> CRVENI LED (na RA2)
    [u trajanju od 3s]
 Zatim se startuje novi ciklus unosa
 cifre pocevsi od prve pozicije
 */
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

int brUnosa = 0;
int unos[4] = {0,0,0,0}
int CIFRA;

void upisi()
{
    int i = 0;
    static const int sifra[] = {3, 2, 3, 1};
    for(int i = 0; i<4; i++)
    {
        EECON1bits.WREN = 1;
        EEADR = 0x03 + i;
        EEDATA = sifra[i];
        EECON2 = 0x55;
        EECON2 = 0xAA;
        EECONbits.WR = 1;
        __delay_ms(10);
    }
}

int procitaj(int cifra)
{
    EEADR = 0x03 + cifra;
    EECON1bits.RD = 1;
    int vrednost = EEDATA;
    
    return vrednost;
}

void interrupt intcode()
{
    if(INTCONbits.INTF == 1)
    {
        brUnosa++;
        if(PORTAbits.RA0 == 1 && PORTAbits.RA1 == 1) CIFRA = 3;
        else if(PORTAbits.RA1 == 1) CIFRA = 2;
        else if(PORTAbits.RA0 == 1) CIFRA = 1;
        else CIFRA = 0;
        
        unos[brUnosa-1] = CIFRA; // indeks u nizu jer br unosa npr
        //treci unos ide na niz[2]
        
        Lcd_Init();
        Lcd_Set_Cursor(1, brUnosa);
        if(CIFRA == 3) Lcd_Write_String("3");
        else if(CIFRA == 2) Lcd_Write_String("2");
        else if(CIFRA == 1) Lcd_Write_String("1");
        else Lcd_Write_String("0");
        
        if (brUnosa == 4)
        {
            int i;
            int tacnaSifra = 1;
            for(i=0; i<4; i++)
            {
                int tacnaCifra = procitaj(i)
                if(tacnaCifra != unos[i]) tacnaSifra=0;
            }
            
            if(tacnaSifra)
            {
                PORTAbits.RA3 = 1;
                PORTAbits.RA2 = 0;
                __delay_ms(3000);
                PORTA = 0x00;
            }
            else
            {
                PORTAbits.RA2 = 1;
                PORTAbits.RA1 = 0;
                __delay_ms(3000);
                PORTA = 0x00;
            }
            
            Lcd_Clear();
            
            brUnosa=0;
        }
        INTCONbits.INTF = 0;
    }
}

void main()
{
    TRISA = 0x03;
    TRISB = 0x01;
    PORTA = 0;
    PORTB = 0;
    
    INTCON = 0x90;
    
    upisi()
    
    while(1) { }
    return;
}
