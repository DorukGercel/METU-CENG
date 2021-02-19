LIST    P=18F8722

#INCLUDE <p18f8722.inc> 
    
; CONFIG1H
  CONFIG  OSC = HSPLL, FCMEN = OFF, IESO = OFF
; CONFIG2L
  CONFIG  PWRT = OFF, BOREN = OFF, BORV = 3
; CONFIG2H
  CONFIG  WDT = OFF, WDTPS = 32768
; CONFIG3L
  CONFIG  MODE = MC, ADDRBW = ADDR20BIT, DATABW = DATA16BIT, WAIT = OFF
; CONFIG3H
  CONFIG  CCP2MX = PORTC, ECCPMX = PORTE, LPT1OSC = OFF, MCLRE = ON
; CONFIG4L
  CONFIG  STVREN = ON, LVP = OFF, BBSIZ = BB2K, XINST = OFF
; CONFIG5L
  CONFIG  CP0 = OFF, CP1 = OFF, CP2 = OFF, CP3 = OFF, CP4 = OFF, CP5 = OFF
  CONFIG  CP6 = OFF, CP7 = OFF
; CONFIG5H
  CONFIG  CPB = OFF, CPD = OFF
; CONFIG6L
  CONFIG  WRT0 = OFF, WRT1 = OFF, WRT2 = OFF, WRT3 = OFF, WRT4 = OFF
  CONFIG  WRT5 = OFF, WRT6 = OFF, WRT7 = OFF
; CONFIG6H
  CONFIG  WRTC = OFF, WRTB = OFF, WRTD = OFF
; CONFIG7L
  CONFIG  EBTR0 = OFF, EBTR1 = OFF, EBTR2 = OFF, EBTR3 = OFF, EBTR4 = OFF
  CONFIG  EBTR5 = OFF, EBTR6 = OFF, EBTR7 = OFF
; CONFIG7H
  CONFIG  EBTRB = OFF
    

    
;*******************************************************************************
; Variables & Constants
;*******************************************************************************
no_a  udata  0x20
no_a
  
opr1  udata  0x24
opr1
  
opr2  udata  0x28
opr2
  
opr3  udata  0x32
opr3
  
UDATA_ACS
  x	res 1	; used in delay
  y	res 1	; used in delay
  z	res 1	; used in delay
  state res 1	; controlled by RB0 button
;*******************************************************************************
; Reset Vector
;*******************************************************************************

RES_VECT  CODE    0x0000            ; processor reset vector
    GOTO    main                   ; go to beginning of program

   
; INIT FUNCTION ***************************************************************
init
    clrf LATA
    clrf LATB
    clrf LATC
    clrf LATD
    clrf LATE
    
    movlw b'00010000'
    movwf TRISA

    movlw b'00000000'
    movwf TRISB
    
    movlw b'00000000'
    movwf TRISC
    
    movlw b'00000000'
    movwf TRISD

    movlw b'00011000'
    movwf TRISE
    
    clrf LATA
    clrf LATB
    clrf LATC
    clrf LATD
    clrf LATE
    
    movlw b'00000000'
    movwf  no_a
    movwf  opr1
    movwf  opr2
    movwf  opr3
    
    return
  
; FIRST LEDS FUNCTION ***************************************************************
first_leds
    movlw h'0f'
    movwf LATB
    movwf LATC
    
    movlw h'ff'
    movwf LATD
    
    call DELAY
    
    movlw h'00'
    movwf LATB
    movwf LATC
    movwf LATD
    
    return
    
; PRESS A LABEL **************************************************************
CHOOSE_OPERATION
press_a_start:
    btfss   PORTA, 4	;check if ra4 pressed
    goto    press_a_start	;skip if pressed
    goto    relase_a
press_a:
    btfss   PORTA, 4	;check if ra4 pressed
    goto    press_e3_a	;skip if pressed
relase_a:
    btfsc   PORTA, 4	;check if ra4 released
    goto    relase_a	;skip if released
    incf    no_a
press_e3_a:
    btfss   PORTE, 3
    goto    press_a
relase_e3_a:
    btfsc   PORTE, 3
    goto    relase_e3_a
    goto    OPR1

; CHOOSE Opr 1 FUNCTION **************************************************************
OPR1
press_e3_opr1_begin:
    btfss   PORTE, 3
    goto    press_e4_opr1
relase_e3_opr1_begin:
    btfsc   PORTE, 3
    goto    relase_e3_opr1_begin
    goto    OPR2
press_e4_opr1:
    btfss   PORTE, 4
    goto    press_e3_opr1_end
relase_e4_opr1:
    btfsc   PORTE, 4
    goto    relase_e4_opr1
    incf    opr1
;CHECK IF THE NUMBER MUST BE RESET*************************************************************
    btfss   opr1, 2
    goto    LED_OPR1
    btfss   opr1, 0
    goto    LED_OPR1
    movlw   d'0'
    movwf   opr1
;LED OPR1*************************************************************************************
LED_OPR1
    btfsc opr1, 2
    goto  light_4_leds1
    btfsc opr1, 1
    goto  upstairs1
    goto  downstairs1
    upstairs1:
	btfsc	opr1, 0
	goto	light_3_leds1
	goto	light_2_leds1
    downstairs1:
	btfsc	opr1, 0
	goto	light_1_leds1
	goto	light_0_leds1
    light_4_leds1:
	movlw	b'1111'
	movwf	LATB
	goto	press_e3_opr1_end
    light_3_leds1:
	movlw	b'0111'
	movwf	LATB
	goto	press_e3_opr1_end
    light_2_leds1:
	movlw	b'0011'
	movwf	LATB
	goto	press_e3_opr1_end
    light_1_leds1:
	movlw	b'0001'
	movwf	LATB
	goto	press_e3_opr1_end
    light_0_leds1:
	movlw	b'0000'
	movwf	LATB
	goto	press_e3_opr1_end
;*************************************************************************************
press_e3_opr1_end:
    btfss   PORTE, 3
    goto    press_e4_opr1
relase_e3_opr1_end:
    btfsc   PORTE, 3
    goto    relase_e3_opr1_end
    goto OPR2

; CHOOSE Opr 2 FUNCTION **************************************************************
OPR2
press_e3_opr2_begin:
    btfss   PORTE, 3
    goto    press_e4_opr2
relase_e3_opr2_begin:
    btfsc   PORTE, 3
    goto    relase_e3_opr2_begin
    goto    RESULT
press_e4_opr2:
    btfss   PORTE, 4
    goto    press_e3_opr2_end
relase_e4_opr2:
    btfsc   PORTE, 4
    goto    relase_e4_opr2
    incf    opr2
;CHECK IF THE NUMBER MUST BE RESET*************************************************************
    btfss   opr2, 2
    goto    LED_OPR2
    btfss   opr2, 0
    goto    LED_OPR2
    movlw   d'0'
    movwf   opr2
;LED OPR2*************************************************************************************
LED_OPR2
    btfsc opr2, 2
    goto  light_4_leds2
    btfsc opr2, 1
    goto  upstairs2
    goto  downstairs2
    upstairs2:
	btfsc	opr2, 0
	goto	light_3_leds2
	goto	light_2_leds2
    downstairs2:
	btfsc	opr2, 0
	goto	light_1_leds2
	goto	light_0_leds2
    light_4_leds2:
	movlw	b'1111'
	movwf	LATC
	goto	press_e3_opr2_end
    light_3_leds2:
	movlw	b'0111'
	movwf	LATC
	goto	press_e3_opr2_end
    light_2_leds2:
	movlw	b'0011'
	movwf	LATC
	goto	press_e3_opr2_end
    light_1_leds2:
	movlw	b'0001'
	movwf	LATC
	goto	press_e3_opr2_end
    light_0_leds2:
	movlw	b'0000'
	movwf	LATC
	goto	press_e3_opr2_end
;*************************************************************************************

press_e3_opr2_end:
    btfss   PORTE, 3
    goto    press_e4_opr2
relase_e3_opr2_end:
    btfsc   PORTE, 3
    goto    relase_e3_opr2_end
    goto    RESULT

RESULT
    btfss   no_a, 0
    goto    subtraction
    goto    addition
    addition:
	movlw	h'00'
	addwf	opr1, w
	addwf	opr2, w
	movwf	opr3
	goto	LED_OPR3
    subtraction:
	movlw	h'00'
	addwf	opr2, w
	subwf	opr1, w
	movwf	opr3
	bnn	LED_OPR3
    alt_subtraction:
	movlw	h'00'
	addwf	opr1, w
	subwf	opr2, w
	movwf	opr3
	goto	LED_OPR3
	
;LED OPR3*************************************************************************************
LED_OPR3
    btfsc opr3, 3
    goto  light_8_leds3
    btfsc opr3, 2
    goto  upstairs
    goto  downstairs
    upstairs:
	btfsc	opr3, 1
	goto	high_h
	goto	high_l
	high_h:
	    btfsc   opr3, 0
	    goto    light_7_leds3
	    goto    light_6_leds3
	high_l:
	    btfsc   opr3, 0
	    goto    light_5_leds3
	    goto    light_4_leds3
    downstairs:
	btfsc   opr3, 1
	goto	low_h
	goto	low_l
	low_h:
	    btfsc   opr3, 0
	    goto    light_3_leds3
	    goto    light_2_leds3
	low_l:
	    btfsc   opr3, 0
	    goto    light_1_leds3
	    goto    light_0_leds3
    light_8_leds3:
	movlw	b'11111111'
	movwf	LATD
	goto	RESTART
    light_7_leds3:
	movlw	b'01111111'
	movwf	LATD
	goto	RESTART
    light_6_leds3:
	movlw	b'00111111'
	movwf	LATD
	goto	RESTART
    light_5_leds3:
	movlw	b'00011111'
	movwf	LATD
	goto	RESTART
    light_4_leds3:
	movlw	b'00001111'
	movwf	LATD
	goto	RESTART
    light_3_leds3:
	movlw	b'00000111'
	movwf	LATD
	goto	RESTART
    light_2_leds3:
	movlw	b'00000011'
	movwf	LATD
	goto	RESTART
    light_1_leds3:
	movlw	b'00000001'
	movwf	LATD
	goto	RESTART
    light_0_leds3:
	movlw	b'00000000'
	movwf	LATD
	goto	RESTART
;*************************************************************************************
RESTART
    call DELAY
    call init
    goto CHOOSE_OPERATION
  
; DELAY FUNCTION **************************************************************
DELAY	; Time Delay Routine with 3 nested loops
    movlw 82	; Copy desired value to W
    movwf z	; Copy W into t3
    l3:
	movlw 0xA0  ; Copy desired value to W
	movwf y    ; Copy W into t2
	l2:
	    movlw 0x9F	; Copy desired value to W
	    movwf x	; Copy W into t1
	    l1:
		decfsz x,F ; Decrement t1. If 0 Skip next instruction
		goto l1 ; ELSE Keep counting down
		decfsz y,F ; Decrement t2. If 0 Skip next instruction
		goto l2 ; ELSE Keep counting down
		decfsz z,F ; Decrement t3. If 0 Skip next instruction
		goto l3 ; ELSE Keep counting down
		return
   
main:
    call init
    call first_leds
    goto CHOOSE_OPERATION
end






