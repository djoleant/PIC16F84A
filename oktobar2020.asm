;oktobar2020

#include p16f84a.inc

CBLOCK 0x30
   WREG_TEMP ;storage for WREG during interrupt
   STATUS_TEMP ;storage for STATUS during interrupt
   PCLATH_TEMP ;storage for PCLATH during interrupt
   FSR_TEMP ;storage for FSR during interrupt
	BROJAC
	LOCK
ENDC

RST org 0x0000
    goto ResetCode
    org 0x0004
    goto InterruptCode

;====================================================================
; CODE SEGMENT
;====================================================================

PGM   code
ResetCode:
   clrf PCLATH
Start:

	banksel INTCON
	
	movlw 0 // W <- 0
	movwf BROJAC // BROJAC <- W
	movwf LOCK
	movwf INTCON
	bsf INTCON,GIE
	bsf INTCON,INTE
	banksel OPTION_REG
	movlw 0x05
	movwf OPTION_REG
	banksel TRISB
    bsf TRISB, 0
	bsf TRISB,1
	bsf TRISB,2
	
	petlja:
	
	
	jmp petlja
	
	
InterruptCode:
   movwf WREG_TEMP ;save WREG
   swapf STATUS,W ;store STATUS in WREG
   clrf STATUS ;select file register bank0
   movwf STATUS_TEMP ;save STATUS value
   movf PCLATH,W ;store PCLATH in WREG
   movwf PCLATH_TEMP ;save PCLATH value
   clrf PCLATH ;select program memory page0
   movf FSR,W ;store FSR in WREG
   movwf FSR_TEMP ;save FSR value

	BTSFSS INTCON,INTF
	jmp drugiuslov:
	banksel PORTB
	BTFCS PORTB,RB1
	jmp dalje
	BTFCS PORTB,RB2
	jmp dalje
	//proso test
	banksel PORTA
	bsf PORTA,RA2
	bcf PORTA,RA1
	bsf PORTA,RA0
	banksel INTCON
	bsf INTCON,TOIE //dozvoljava tmro,sad treba ceka 5 sekunde
	bcf INTCON,INTE //zabrani rbo da se ne mesaju interapti
	jmp kraj
	
	drugiuslov:
	BTFSS INTCON,TOIF
	jmp kraj
	incf BROJAC,f
	movf BROJAC,w
	sublw 0xFA
	btfss STATUS,Z //w=250 250-250 = 0
	jmp kraj
	banksel INTCON
	bcf INTCON,TOIE //gasi tmro
	bsf INTCON,INTE //dozvoli opet rb0
	bansekl PORTA
	bcf PORTA,RA2
	bsf PORTA,RA1
	bcf PPRTA,RA0
	movlw 0
	movwf BROJAC //reset brojac
	
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
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
