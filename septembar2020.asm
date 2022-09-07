#include p16f84a.inc

CBLOCK 0x30
   WREG_TEMP ;storage for WREG during interrupt
   STATUS_TEMP ;storage for STATUS during interrupt
   PCLATH_TEMP ;storage for PCLATH during interrupt
   FSR_TEMP ;storage for FSR during interrupt
	BROJAC
	STANJE
	SEKUNDE
	RED

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
	
	movlw 0x03
	movwf RED
	movlw 0
	movwf BROJAC
	banksel INTCON
	movwf INTCON
	bsf INTCON,INTE //omoguci rb0
	banksel TRISB
	movwf TRISB
	movwf TRISA
	banksel PORTA
	movwf PORTA
	movwf PORTB
	movwf STANJE
	banksel TRISB
	bsf TRISB,B0
	banksel OPTION_REG
	movlw 0x05
	movwf OPTION_REG //n=5 50hz
	
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
	
	BTFSS INTCON,INTF //ispitujemo zasto upao u interapt
	jmp tmrouslov 
	infc STANJE,f
	movf STANJE,w
	andlw 0x01
	BTFSS STATUS,Z //ispitujemo u koje je stanje w%2=0,1
	jmp gasisistem:
	//sistem pokrenut
	//RA3-crvena RA2-zuta RA3-zelena
	//pitamo cije je red svetla
	banksel INTCON
	bsf INTCON,TOIE //omogucimo tmro da bi brojili sekunde
	
	crvena:
	movf RED,w
	sublw 0x03
	BTFSS STATUS,Z
	jmp zuta
	decf RED,f
	banksel PORTA
	bsf PORTA,RA3
	movlw 0x96
	movwf SEKUNDE
	jmp tmrouslov
		
	zuta:
	movf RED,w
	sublw 0x02
	BTFSS STATUS,Z
	jmp zelena
	decf RED,f
	bsf PORTA,RA2
	movlw 0x64 	
	movwf SEKUNDE
	jmp tmrouslov
	
	zelena:
	movlw 0x03
	movwf RED
	bsf PORTA,RA1
	movlw 0x32
	movwf SEKUNDE
	jmp tmrouslov
	


	gasisistem:
	//stanje,red,brojac
	clr STANJE//ne mora il mora
	clr brojac
	banksel INTCON
	bcf INTCON,T0IE
	banksel PORTA
	movlw 0
	movwf PORTA
	jmp kraj
	
	tmrouslov:
	banksel INTCON
	BTFSS INTCON,T0IF
	jmp kraj
	incf BROJAC,f
	movf BROJAC,w
	subwf sekunde,w
	BTFSS STATUS,Z
	jmp kraj
	clrf BROJAC
	movf RED,w
	sublw 0x01
	BTFSS STATUS,Z
	jmp kraj
	//resetujemo red novi ciklus
	movlw 0x03
	movwf RED
	
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
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
