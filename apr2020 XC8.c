// APRIL 2020
#include <xc.h>

__CONFIG(FOSC_HS & WDTE_OFF & PWRTE_ON & CP_OFF);

# define _XTAL_FREQ 3276800

/**
 100Hz znaci TMR0 izbroji do 100 za 1s
 znaci do 150 za 1,5s
 Da bi generisao interrupt mora da predje 255
 Zato setujemo TMR0 inicijalno na 255-150=105
 */


void interrupt intcode(void);

void main()
{
    OPTION_REG = 0x04 //100Hz
    INTCON = 0;
    INTCONbits.GIE = 1;
    INTCONbits.T0IE = 1;
    
    TRISB = 0xFF;
    TMR0 = 105 //PROVERI da li se ovako TMR0 podesava
    
    PORTB = 128 //1000 0000 (prvo svetli RB7)
    
    
    while(1)
    {
        
    }
}

void interrupt intcode(void)
{
    if(INTCONbits.T0IF == 1)
    {
        //BROJAC MORA koji broji
        if(PORTBbits.RB0 == 1)
        {
            PORTB = 128;
        }
        else
        {
        /*
         shiftamo na desno
          1000 0000
          0100 0000
          ...
          0000 0001
         */
            PORTB = PORTB >> 1
            
        }
        
        INTCONbits.T0IF = 0;
        TMR0 = 105;
    }
}
