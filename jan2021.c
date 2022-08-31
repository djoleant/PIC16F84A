//jan2021
#include <xc.h>

__CONFIG(FOSC_HS & WDTE_OFF & PWRTE_ON & CP_OFF);

# define _XTAL_FREQ 3276800

int brojac=0;
int displej=0;
int red=0; // da li prikaz kocke ili prikaz rezultata
int potez=0 //da li igra prvi ili drugi igrac
int igrac1=0;//broj pobeda
int igrac2=0;
int br1=0;//vrednost kockice
int br2=0;
int cifraZaPrikaz=0;//na displej nula kad osvezavamo sta se prikazuje
int lock=1;
int crtica = 10;


void osvezi();
void interrupt prekid(void);

//rez a0 igraci a1 i a2
//b7 i a3



void main()
{
    TRISA=0;
    TRISAbits.RA3=1;
    TRISB=0
    TRISBbits.RB7=1;
    
    //option register
    OPTION_REG=0x03;//refresh 200hz
    //intcon
    INTCON=0;
    INTCONbits.GIE=1;
    
    upisi()
    while(1)
    {
        while(potez==0)
        {
            if(PORTBbits.RB7==1)
            {
                potez=1;//posle drugi igrac
                INTCONbits.T0IE=1;//aktiviraj tmr0
                br1=rand()%6+1;
                cifraZaPrikaz=br1;
                while(lock==1)
                {
                }
                lock=1//reset za sledece cekanje 3 sekunde
                cifraZaPrikaz=crtica;
                INTCONbits.TOIE=0;//gasi tmro
            }
        }
        
        while(potez==1)
        {
            if(PORTAbits.RA3==1)
            {
                potez=2//prikaz
                INTCONbits.T0IE=1;
                br2=rand()%6+1;
                cifraZaPrikaz=br2;
                while(lock==1)
                {
                }
                lock=1;
                cifraZaPrikaz=crtica;
                INTCONbits.T0IE=0;
                red=1;//prikaz rezultata
            }
        }
        
        if(potez==2)
        {
            INTCONbits.T0IE=1;
            if(br2>br1)
                igrac2++;
            if(br1>br2)
                igrac1++;
            if(br1==9 || br2==9)
            {
                br1=0;
                br2=0;
                cifraZaPrikaz(0);
                //kraj
                red==2;//red sluzi dal jedan dva ili 3 displeja su upaljena
            }
            potez=0;
            red=0;
        }
    }
    
}

void osvezi()
{
    if(red==0)
    {
        PORTAbits.A1=1;
        PORTAbits.A2=0;
        PORTAbits.A3=0;
        prikazi(cifraZaPrikaz);
    }
    if(red==1)
    {
        PORTAbits.A1=0;
        if(displej%2==0)
        {
            PORTAbits.A2=0;
            PORTAbits.A3=1;
            prikazi(igrac1);
        }
        else
        {
            PORTAbits.A2=1;
            PORTAbits.A3=0;
            prikazi(igrac2);
        }
        if(displej>1)
            dispej=0;
    }
    if(red==2)
    {
        switch(dispej){
            case 0:
                PORTAbits.A1=1;
                PORTAbits.A2=0;
                PORTAbits.A3=0;
                prikazi(0);
                break;
            case 1:
                PORTAbits.A1=0;
                PORTAbits.A2=1;
                PORTAbits.A3=0;
                break;
            case 2:
                PORTAbits.A1=0;
                PORTAbits.A2=0;
                PORTAbits.A3=1;
                break;
            default:
            break;
        }
        
        if(dispej>2)
            dispej=0;
    }
    displej++;
}

void interrupt code
{
    if(INTCONbits.T0IF==1)
    {
        osvezi();
        if(red == 0)
        {
            brojac++;
            if(brojac==600)
            {
                brojac=0;
                lock=0;
            }
        }
        INTCONbits.T0IF==0;
    }
}

void upisi()
{
static const int codes[]={0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xd8, 0x80, 0x90,0x20};//0,1,2,3,4,5,6,7,8,9,-
for(int i = 4;i<15;i++)
{
      EECON1bits.WREN=1;
      EEADR=i;
      EEDATA=codes[i-4];
      EECON2=0x55;
      EECON2=0xAA;
      EECON1bits.WR=1;
      __delay_ms(10);
}
}

void prikazi(int cifra)
{
   EEADR=cifra+4;
   EECON1bits.RD=1;
   PORTB=EEDATA;
}
