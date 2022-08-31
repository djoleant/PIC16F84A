;// APR 2020 ASM
#include p16f84a.inc

CBLOCK 0x30
    WREG_TEMP
    STATUS_TEMP
    PCLATH_TEMP
    FSR_TEMP
    RANDOM
    BROJAC
    STANJE //0 za red na prvi klik, 1 za red na drugi klik
ENDC

RST org 0x0000
    goto ResetCode
    org 0x0004
    goto InterruptCode

;/*CODE SEGMENT*/

PGM CODE
ResetCode:
    clrf PCLATH
Start:
    CLRF BROJAC
    CLRF STANJE

    BANKSEL OPTION_REG
    MOVLW 0x45 //0100 0101, na bit6 zbog INTEDG sa 0 na 1
    MOVWF OPTION_REG
    
    BANKSEL INTCON
    CLRF INTCON
    BSF INTCON, GIE
    BSF INTCON, T0IE
    BSF INTCON, INTE

    BANKSEL TRISB
    MOVLW 0xEF //0111 1111
    MOVWF TRISB

    //Da li treba da se osvezava displej posto nije
    //naglaseno, ako da sa koliko Hz i da li onda
    //da koristimo sa PORTA neki pin
    //ako ne, da li svejedno treba da imamo pin
    //sa porta koji aktivira 7seg disp?

    //PITANJE: DA LI TREBA ODAVDE
    //JMP na KRAJ ako se ne javi interrupt
    //iako vrv hoce, ako se ipak desi da se ne javi
    //da se ne bi izvrsio kod na dalje odavde

InterruptCode:
    MOVWF WREG_TEMP
    SWAPF STATUS, W
    CLRF STATUS
    MOVWF STATUS_TEMP
    MOVF PCLATH, W
    MOVWF PCLATH_TEMP
    CLRF PCLATH
    MOVF FSR, W
    MOVWF FSR_TEMP

    //DA LI DA ONEMOGUCIMO PREKIDE
    //DOK OBRADJUJEMO PREKIDE

    BTFSS INTCON, INTF
    goto drugiuslov

prviklik:

    BTFSC STANJE, 0
    goto drugiklik
    BANKSEL PORTB
    //da li treba za nase promenljive banksel???

    MOVF TMR0, W
    XORLW F8 //da ga dovedemo bar na 0000 0XXX oblik
    /*
     sada imamo bar 7, sad mu dodamo +2 i onda je sig izmedju
     2 i 9, u smislu ne prelazi iznad opsega koji moze
     biti prikazan
     */
    ADDLW .2
    MOVWF RANDOM //za dalju upotrebu AKO ZATREBA
    BSF STANJE, 0 //0000 0001

drugiklik:
    BTFSS STANJE, 0
    goto kraj
    BCF STANJE, 0 //0000 0000

//POCETAK INTERRUPTA T0IF
//TADA I OSVEZI 7seg DISPLAY

    BANKSEL EEADR
    MOVF RANDOM, W //jel se pise W ILI WREG kod xc8 proveri
    MOVLW EEADR
    BANKSEL EECON1
    BSF EECON1, RD //nakon toga u EEDATA je kod za broj RANDOM
    MOVF EEDATA, W
    MOVWF PORTB

drugiuslov:
    BTFSS INTCON, T0IF
    goto kraj

    BTFSS STANJE, 0
    goto kraj
odbrojavaj:
    DECFSZ RANDOM
    goto odbrojavaj

    MOVLW .205
    MOVWF TMR0 //PROVERI JEL MOZE MOVWF TMR0 OVAKO
    BCF INTCON, T0IF

/*tmr0 inic uvek na 205*/
/*znaci kad se javi ovaj prekid prosla sekunda*/


//KRAJ INTERAPTA T0IF




kraj:
EndInt:
   bcf 3,5 ;select bank 0
   movf FSR_TEMP,W ;get saved FSR value
   movwf FSR ;restore FSR
   movf PCLATH_TEMP,W ;get saved PCLATH value
   movwf PCLATH ;restore PCLATH
   swapf STATUS_TEMP,W ;get saved STATUS value
   movwf STATUS ;restore STATUS
   swapf WREG_TEMP,F ;prepare WREG to be restored
   swapf WREG_TEMP,W ;restore WREG without affecting STATUS
   retfie ;return from interrupt
    
    


    
