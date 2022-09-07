#include pic16f84.a

CBLOCK 0x20
 
	FR0
	FR1
	FR2
	OLD
	RBPOZ
	COUNT
ENDC

RST org 0x0000
	goto ResetCode
	org 0x0004
	goto InterruptCode
	
;MAIN

Start:

	BANKSEL OPTION_REG
	MOVLW 0x02
	MOVWF OPTION_REG
	; 0000 0100
	
	MOVLW 0x04 
	MOVWF TRISA
	CLRF TRISB
	
	; Inicijalizacija
	
	MOVLW 0x02
	MOVWF RBPOZ
	MOVLW .200
	MOVWF COUNT
	MOVLW .84
	MOVWF TMR0
	
	CLRF OLD ;old<-0
	
Petlja:
	BTFSS PORTA,2
	goto Nastavak0
	BTFSC OLD,0
	goto Nastavak1
	BSF OLD, 0
	;FREK++
	INCF FR0, F
	MOVF FR0, W
	XORLW 0x0A
	BTFSS STATUS, Z
	goto 
	CLRF FR0 
	INCF FR1, F 
	MOVF FR1, W
	XORLW 0x0A
	BTFSS STATUS, Z
	goto 
	CLRF FR1 
	INCF FR2
	;pp da ne ide preko 999
	
Nastavak0:
	CLRF OLD, 0
	
Nastavak1:
	;OSVEZI DISP
	BTFSS INTCON, T0IF
	goto
	BCF INTCON, T0IF
	MOVLW .84
	MOVWF TMR0
	
	MOVF RBPOZ, F
	BTFSS STATUS, Z
	goto IdiNaD1
	
	MOVF D0, W
	ADDWF .3
	
	BANKSEL EEADR
	MOVWF EEADR
	BSF EECON1, RD
	MOVF EEDATA, W
	
	BANKSEL PORTB
	MOVWF PORTB
	
	;zavrsili D0
	goto Dalje
	
IdiNaD1:
	BTFSS RBPOZ, 0
	goto IdiNaD2
	;osvezi d1
	
	MOVF D1, W
	ADDWF .3
	
	BANKSEL EEADR
	MOVWF EEADR
	BSF EECON1, RD
	MOVF EEDATA, W
	
	BANKSEL PORTB
	MOVWF PORTB
	
	;zavrsili
	goto Dalje
	
IdiNaD2:
	MOVF D2, W
	ADDWF .3
	
	BANKSEL EEADR
	MOVWF EEADR
	BSF EECON1, RD
	MOVF EEDATA, W
	
	BANKSEL PORTB
	MOVWF PORTB
	
	CLRF RBPOZ ;vracamo na 0
	
Dalje:
	DECFSZ COUNT
	goto Petlja
	MOVF FR0, W
	MOVWF D0
	MOVF FR1, W
	MOVWF D1
	MOVF FR2, W
	MOVWF D2
	
	CLRF FR0
	CLRF FR1
	CLRF FR2
	
	goto Petlja
	


	
	
	
	
	
	
	