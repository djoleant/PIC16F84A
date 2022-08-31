#include <htc.h> // KAD XC.H KAD HTC.H

__CONFIG(FOSC_HS & WDTE_OFF & PWRTE_ON & CP_OFF);
#define _XTAL_FREQ   3276800

int cifra1 = 0;
int cifra2 =0;
int cifra3 = 0;
int displej = 0;
int broj = 0;
int brojac = 0;

int stanje = 0;

void upisi();
void prikazi(int cifra);
void osvezi();

void main()
{
    TRISA=0;
    TRISA.bitsRA2=1;
    TRISB=0;
    
    /*
     
     Neka je n=6
     2^15 * 100Hz
     ------------ = 600
     2^2 * X * 2^7
    
     ... sredjivanjem X = 10,67
     */
    OPTION_REG=6;
    INTCON=0;
    INTCONbits.GIE=1;
    INTCONbits.T0IE=1;
    
    /* TMR0 = 255 - 10 = 245
     (10 jeiznad izracunato X)*/
    TMRO=245;
    while(1)
    {
    }
}

void upisi()
{
static const int codes[]={0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xd8, 0x80, 0x90};
    for(int i = 3;i<13;i++)
    {
        EECON1bits.WREN=1;
        EEADR=i;
        EEDATA=codes[i];
        EECON2=0x55;
        EECON2=0xAA;
        EECON1bits.WR=1;
        __delay_ms(10);
    }
}

void prikazi(int cifra)
{
   EEADR=cifra+3;
   EECON1bits.RD=1;
   PORTB=EEDATA;
}

osvezi()
{
    if(displej>3)
        displej=1
    switch(displej)
    {
        case 1:
            prikazi(cifra1);
            PORTAbits.RA0=1
            PORTAbits.RA1=0
            PORTAbits.RA3=0
            brojac++;
            break;
        case 2:
            prikazi(cifra2);
            PORTAbits.RA0=0
            PORTAbits.RA1=1
            PORTAbits.RA3=0
            break;
        
        case 3:
            prikazi(cifra3);
            PORTAbits.RA0=0
            PORTAbits.RA1=0
            PORTAbits.RA3=1
            break;
    }
    displej++;
}

void interrupt intcode()
{
    if(INTCONbits.T0IF)
    {
        /*
         DA LI NAREDNA DVA IF BLOKA
         IDU OVDE ILI U WHILE(1) U MAINU
         */
        if(PORTAbits.RA2==1){
            if(stanje==0)
            {
                stanje=1;
                // [1]
            }
        }
        if(PORTAbits.RA2==0{
            if(stanje==1)
            {
                stanje=0;
                broj++;
                /*
                 brojimo prelaze ili samo ovde
                 ili samo u zakomentarisanom
                 delu iznad [1]
                 */
            }
        }
        if(brojac==200)
        {
            brojac=0;
            cifra1=broj % 10;
            cifra2=(broj/10) % 10;
            cifra3=(broj/100) % 10;
            broj=0;
        }
        osvezi();
    TMRO=245;
    INTCONbits.TOIF=0;
    }
}
