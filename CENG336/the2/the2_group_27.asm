list P=18F8722

;The Information of Group 27:
;Melis Ece Ünsal 2237865
;Utku Güngör 2237477
;Doruk Gerçel 2310027
;We implemented THE2_v2 on simulation enviorement.
;We tested it in the board enviorement as-well, but sometimes we have trouble wit the 7-segment
;display. Although they work fine in the simulation enviorement.
    
#include <p18f8722.inc>
config OSC = HSPLL, FCMEN = OFF, IESO = OFF, PWRT = OFF, BOREN = OFF, WDT = OFF, MCLRE = ON, LPT1OSC = OFF, LVP = OFF, XINST = OFF, DEBUG = OFF

counter   udata 0x22
counter
   
w_temp  udata 0x23
w_temp

status_temp udata 0x24
status_temp

pclath_temp udata 0x25
pclath_temp

portb_var   udata 0x26
portb_var

timer1low   udata 0x27
timer1low
   
timer1high   udata 0x28
timer1high   
   
state    udata 0x29
state
    
temp_status  udata 0x30
temp_status
  
index   udata 0x31
index
   
health   udata 0x32
health
   
ball_num1  udata 0x33
ball_num1
  
ball_num2  udata 0x34
ball_num2
    
ball_num3  udata 0x35
ball_num3
    
shift_num  udata 0x36
shift_num
  
flag  udata 0x37
flag
  
bar  udata 0x38
bar
  
ball_num4  udata 0x39
ball_num4

org     0x00
goto    main

org     0x08
goto    isr             ;go to interrupt service routine

isr:
    call    save_registers  ;Save current content of STATUS and PCLATH registers to be able to restore them later
    bcf     INTCON, 2	    ;Can't take timer interupt again
    btfss   flag, 6 
    call    display_health
    btfsc   flag , 6
    call    display_level
    btg	    flag, 6	    ;Toggle the flag to show the other one next time
    btfsc   flag, 3
    goto    timer_interrupt3
    btfsc   flag, 2
    goto    timer_interrupt2
    btfsc   flag, 1
    goto    timer_interrupt1
    
    

;;;;;;;;;;;;;;;;;;;;;;;; Timer interrupt handler part ;;;;;;;;;;;;;;;;;;;;;;;;;;
timer_interrupt1:
    incf	counter, f              ;Timer interrupt handler part begins here by incrementing count variable
    movf	counter, w              ;Move count to Working register
    sublw	d'100'                  ;Decrement 100 from Working register
    btfss	STATUS, Z               ;Is the result Zero?
    goto	timer_interrupt_exit    ;No, then exit from interrupt service routine
    clrf	counter                 ;Yes, then clear count variable
    bsf         flag, 0                 ;Is this a timer interrupt?
    goto        timer_interrupt_exit
    

timer_interrupt2:
    incf	counter, f              ;Timer interrupt handler part begins here by incrementing count variable
    movf	counter, w              ;Move count to Working register
    sublw	d'80'                   ;Decrement 100 from Working register
    btfss	STATUS, Z               ;Is the result Zero?
    goto	timer_interrupt_exit    ;No, then exit from interrupt service routine
    clrf	counter                 ;Yes, then clear count variable
    bsf         flag, 0                 ;Is this a timer interrupt?
    goto        timer_interrupt_exit

timer_interrupt3:
    incf	counter, f              ;Timer interrupt handler part begins here by incrementing count variable
    movf	counter, w              ;Move count to Working register
    sublw	d'70'                   ;Decrement 100 from Working register
    btfss	STATUS, Z               ;Is the result Zero?
    goto	timer_interrupt_exit    ;No, then exit from interrupt service routine
    clrf	counter                 ;Yes, then clear count variable
    bsf         flag, 0                 ;Is this a timer interrupt?
    goto        timer_interrupt_exit

timer_interrupt_exit:
    bcf	        INTCON, 2		;Clear TMROIF
    movlw	d'61'               ;256-61=195; 195*256*100 = 4992000 instruction cycle;
    movwf	TMR0
    call	restore_registers   ;Restore STATUS and PCLATH registers to their state before interrupt occurs
    retfie

;;;;;;;;;;;; Register handling for proper operation of main program ;;;;;;;;;;;;
save_registers:
    movwf 	w_temp          ;Copy W to TEMP register
    swapf 	STATUS, w       ;Swap status to be saved into W
    clrf 	STATUS          ;bank 0, regardless of current bank, Clears IRP,RP1,RP0
    movwf 	status_temp     ;Save status to bank zero STATUS_TEMP register
    movf 	PCLATH, w       ;Only required if using pages 1, 2 and/or 3
    movwf 	pclath_temp     ;Save PCLATH into W
    clrf 	PCLATH          ;Page zero, regardless of current page
	return

restore_registers:
    movf 	pclath_temp, w  ;Restore PCLATH
    movwf 	PCLATH          ;Move W into PCLATH
    swapf 	status_temp, w  ;Swap STATUS_TEMP register into W
    movwf 	STATUS          ;Move W into STATUS register
    swapf 	w_temp, f       ;Swap W_TEMP
    swapf 	w_temp, w       ;Swap W_TEMP into W
    return
  
init:
    clrf index
    clrf timer1low
    clrf timer1high
    clrf health
    clrf ball_num1
    clrf ball_num2
    clrf ball_num3
    clrf ball_num4
    clrf shift_num  
    clrf flag
    
   
    
    CLRF PORTA ; Initialize PORTA by
		; clearing output
		; data latches
    CLRF LATA ; Alternate method
		; to clear output
		; data latches
    MOVLW 0Fh ; Configure A/D
    MOVWF ADCON1 ; for digital inputs

    movlw h'C0'
    movwf TRISA
    clrf LATA
    
    CLRF PORTB 
    CLRF PORTC 
    CLRF PORTD
    CLRF PORTH 
    CLRF PORTJ
    movlw h'C0'
    movwf TRISB
    clrf LATB
    
    movlw h'C0'
    movwf TRISC
    clrf LATC
    
    movlw h'C0'
    movwf TRISD
    clrf LATD
    
    movlw h'0D'
    movwf TRISG
    clrf LATG
    
    
    ;Disable interrupts
    clrf    INTCON
    
    ;Configure Output Ports
    
    clrf    LATH
    clrf    TRISH
    
    clrf    LATJ
    clrf    TRISJ
    

    ;Initialize Timer0
    movlw   b'01000111' ;Disable Timer0 by setting TMR0ON to 0 (for now)
                        ;Configure Timer0 as an 8-bit timer/counter by setting T08BIT to 1
                        ;Timer0 increment from internal clock with a prescaler of 1:256.
    movwf   T0CON
    ;start timer1
    movlw   b'11000001'	 ;For the c0 and c1 to be lighted, 3rd bit must be 0
    movwf   T1CON
return
     
    
enable_interrupts:
    movlw   b'10100000' ;Enable Global, peripheral, Timer0 and RB interrupts by setting GIE, PEIE, TMR0IE and RBIE bits to 1
    movwf   INTCON
    bsf     T0CON, 7    ;Enable Timer0 by setting TMR0ON to 1
    return
  
disable_interrupts:
    clrf    INTCON	;Disable INTCON
    bcf	    T0CON, 7	;Disable Timer0
    return
    
    
    
 ;This part saves the timer 1 register at the begining
 save_time:
	movff  TMR1H, timer1high
	movff  TMR1L, timer1low	
	;Used to light the bar in the begining and set the register 
	ligth_bar:
		movlw   h'00'
		movwf   PORTC
		movwf   PORTD
		movlw   h'20'
		movwf   PORTA
		movwf   PORTB
		movlw   b'00001100'
		movwf   bar
    return
		
;Used for the begining light, all lights
start_lighting:
    movlw   h'3F'
    movwf   PORTA
    movwf   PORTB
    movwf   PORTC
    movwf   PORTD
    return
    
;The main function    
main:
	call init
	;The playable part that the game always comes for restart
	game:
	    call   disable_interrupts	;At the begining of the game we disable the interupt
	    clrf   counter		;And cleared the counter 
    
	    bcf    flag, 0
	    bcf    flag, 2
	    bcf    flag, 3
	    bcf    flag, 4
	    bcf    flag, 5
	    
	    bsf    flag, 1
	    
	    movlw  h'04'	    ;Ball number1 is 4 because we create the first imediately
	    movwf  ball_num1
	    
	    movlw  d'10'
	    movwf  ball_num2
	    
	    movlw  d'15'
	    movwf  ball_num3
	    
	    movlw  d'06'	    ;The number of moves for a ball to damage health
	    movwf  ball_num4
	    
	    movlw  h'05'
	    movwf  health
	    
	    call display_health		;Restart lighting
	    call display_level	
	    
	    movlw   h'00'		;This part lights the bar
	    movwf   PORTC
	    movwf   PORTD
	    movlw   h'20'
	    movwf   PORTA
	    movwf   PORTB
	    movlw   b'00001100'
	    movwf   bar
	    ;First check of the press G0
	    press:
		btfss  PORTG, 0
		goto   press
		goto   release
	    release:
		btfsc  PORTG, 0
		goto   release
		call   save_time	    ;Imediately save time
		call   randomize	    ;Imediately create ball
		call   enable_interrupts    ;Imediately enable interrupts
	    ;Main loop that that the game runs in a game cycle
	    loop:
		call   display_health
		call   display_level
		btfsc  flag, 0
		goto   levels
		btfsc  flag, 5
		goto   game_over
		press_right:
		    btfss   PORTG, 2
		    goto    press_left
		    goto    release_right
		release_right:
		    btfsc   PORTG, 2
		    goto    release_right
		    goto    move_right
		press_left:    
		    btfss   PORTG, 3
		    goto    loop
		    goto    release_left
		release_left:
		    btfsc   PORTG, 3
		    goto    release_left
		    goto    move_left
		
		;Checks if the bar can move right and directs to the lighting of the bar
		move_right:
		    btfsc   bar, 0
		    goto    press_right
		    rrncf    bar, f
		    btfsc   bar, 2
		    goto    light_bc
		    btfsc   bar, 0
		    goto    light_cd
		    goto    press_right
		    
		;Checks if the bar can move left and directs to the lighting of the bar    
		move_left:
		    btfsc   bar, 3
		    goto    press_right
		    rlncf   bar, f
		    bcf     bar, 4 
		    btfsc   bar, 3
		    goto    light_ab
		    btfsc   bar, 1
		    goto    light_bc
		    goto    press_right
		    
		
		light_ab:
		    bsf     PORTA, 5
		    bsf     PORTB, 5
		    bcf     PORTC, 5
		    bcf     PORTD, 5
		    goto    press_right
		   
		light_bc:
		    bcf     PORTA, 5
		    bsf     PORTB, 5
		    bsf     PORTC, 5
		    bcf     PORTD, 5
		    goto    press_right
		    
		light_cd:
		    bcf     PORTA, 5
		    bcf     PORTB, 5
		    bsf     PORTC, 5
		    bsf     PORTD, 5
		    goto    press_right        
		goto   loop
    goto    main  
    
  
;This part is used to shift between timer1high and timer1low     
shift_bit:	
		rrcf   timer1high, f
		movff  STATUS, temp_status
		rrcf   timer1low, f
		btfsc  STATUS, 0
		bsf    timer1high, 7
		btfss  STATUS, 0
		bcf    timer1high, 7
		btfsc  temp_status, 0
		bsf    timer1low, 7
		btfss  temp_status, 0
		bcf    timer1low, 7
		decfsz shift_num
		goto   shift_bit
		return

;Generates the random part that creates the ball is created
randomize:
		movff  timer1low, index
		bcf    index, 7
		bcf    index, 6
		bcf    index, 5
		bcf    index, 4
		bcf    index, 3
		bcf    index, 2
		call   shift_bit	;Shift for the following operation
		
	    ;Find the number in the index
	    
	    zero:
		movlw  h'00'
		cpfseq index
		goto   one
		goto   create_ball_ra
	    one:
		movlw  h'01'
		cpfseq index
		goto   two
		goto   create_ball_rb
	    two:
	        movlw  h'02'
		cpfseq index
		goto   three
		goto   create_ball_rc
	    three:
		goto   create_ball_rd
		
    ;Create the ball in the right port and return
    create_ball_ra:
	        bsf  PORTA, 0  
		return
    create_ball_rb:
		bsf  PORTB, 0 
		return
    create_ball_rc:
		bsf  PORTC, 0 
		return 
    create_ball_rd:		
		bsf  PORTD, 0 
		return
		
;The main levels part, that is called when a ball must be created or moved
levels:
	    bcf    flag, 0	   ;The enable bit for the levels is cleared
	    btfsc  flag, 4
	    goto   level4
	    btfsc  flag, 3
	    goto   level3
	    btfsc  flag, 2
	    goto   level2
	    btfsc  flag, 1
	    goto   level1
		
level1:
	    movlw  h'01'
	    movwf  shift_num	    ;The number that is used in shifting in randomize
	    call   shift_ports	    ;Move the balls
	    btfsc  flag, 5	    ;If game is over goto end the game
	    goto   game_over
	    call   randomize	    ;Create random ball
	    dcfsnz ball_num1	    ;If balls are finished level-up    
	    call   level1_up	
	    goto   loop		    ;Return the loop as cycle ends
	    
level2:	
	    movlw  h'03'
	    movwf  shift_num
	    call   shift_ports
	    btfsc  flag, 5	    ;If game is over return to loop and end the game
	    goto   game_over
	    call   randomize
	    dcfsnz ball_num2
	    call   level2_up
	    goto   loop
	    
level3:
	    movlw  h'05'
	    movwf  shift_num
	    call   shift_ports
	    btfsc  flag, 5	    ;If game is over return to loop and end the game
	    goto   game_over
	    call   randomize
	    dcfsnz ball_num3
	    call   level3_up
	    goto   loop  

level4:
	    call   shift_ports
	    btfsc  flag, 5	    ;If game is over return to loop and end the game
	    goto   game_over
	    
	    dcfsnz ball_num4
	    bsf	   flag, 5	    ;Ind?cates that the game is over as all the balls are finished
	    btfsc  flag, 5
	    goto   game_over
	    goto   loop
	    
    
    ;Are used in-order to level-up and are returned
    level1_up:
	bsf    flag, 2
	call   display_level
	return
    level2_up:
	bsf    flag, 3
	call   display_level
	return
    level3_up:
	bsf    flag, 4
	call   display_level
	return
	
;This is used to move the balls
shift_ports:
	    rlcf   PORTA, f	    ;Right rotate
	    rlcf   PORTB, f
	    rlcf   PORTC, f
	    rlcf   PORTD, f
	checka:			    ;Check the bar positions and catch ball conditions
	    btfsc  bar, 3
	    goto   bar_a5
	checkb:    
	    btfsc  bar, 2
	    goto   bar_b5
	checkc:    
	    btfsc  bar, 1
	    goto   bar_c5
	checkd:    
	    btfsc  bar, 0
	    goto   bar_d5
	    goto   lose_health_a
	    
	    bar_a5:		    ;Clear if bar catched the ball
		bcf     LATA, 6
		bsf     PORTA, 5
		goto    checkb
	    bar_b5:
		bcf     LATB, 6
		bsf     PORTB, 5
		goto	checkc
	    bar_c5:
		bcf     LATC, 6
		bsf     PORTC, 5
		goto	checkd
	    bar_d5:
		bcf     LATD, 6
		bsf     PORTD, 5
	
		
	    lose_health_a:	    ;Lose health if the 6th bit is set
		btfsc  LATA, 6	    ;Check if a 6th bit is set
		call   decr	    ;Call health decrementing function
		btfsc  flag, 5	    ;If the game-over flag is set goto clean and return
		goto   clear
	    lose_heatlh_b:
		btfsc  LATB, 6
		call   decr
		btfsc  flag, 5
		goto   clear
	    lose_health_c:
		btfsc  LATC, 6
		call   decr
		btfsc  flag, 5
		goto   clear
	    lose_health_d:
		btfsc  LATD, 6
		call   decr
		btfsc  flag, 5
		goto   clear
	    clear:
		bcf    LATA, 6
		bcf    LATB, 6
	        bcf    LATC, 6
		bcf    LATD, 6
		bcf    PORTA, 0
		bcf    PORTB, 0
		bcf    PORTC, 0
		bcf    PORTD, 0    
	    
	    return
		
	;Health decreasing function
	decr:
	    dcfsnz  health	   ;Decrease heatlh
	    bsf flag ,5	;If health is zero initialise game-over flag
	    call display_health
	    
	    return
	
	display_health:
	    movlw   b'00001000'
	    movwf   PORTH
	    clrf    PORTJ
	hfive:
	    movlw   d'5'
	    cpfseq  health
	    goto    hfour
	    call    display_five
	    return
	hfour:
	    movlw   d'4'
	    cpfseq  health
	    goto    hthree
	    call    display_four
	    return
	hthree:
	    movlw   d'3'
	    cpfseq  health
	    goto    htwo
	    call    display_three
	    return
	htwo:
	    movlw   d'2'
	    cpfseq  health
	    goto    hone
	    call    display_two
	    return
	hone:
	    movlw   d'1'
	    cpfseq  health
	    return
	    call    display_one
	    return
	 
	display_level:
	    movlw   b'00000001'
	    movwf    PORTH
	    clrf    PORTJ
	    btfsc   flag, 3
	    goto    display_three
	    btfsc   flag, 2
	    goto    display_two
	    btfsc   flag, 1
	    goto    display_one
	    
	    return
	    
	    
	display_one:
	    movlw  b'00000110'
	    movwf  PORTJ
	    return
	display_two:
	    movlw  b'01011011'
	    movwf  PORTJ
	    return
	display_three:
	    movlw  b'01001111'
	    movwf  PORTJ
	    return
	display_four:
	    movlw  b'01100110'
	    movwf  PORTJ
	    return
	display_five:
	    movlw  b'01101101'
	    movwf  PORTJ
	    return
	   
;Game over condition goto game and wait for the start
game_over:
	movlw   h'00'
	movwf   PORTC
	movwf   PORTD
	movlw   h'20'
	movwf   PORTA
	movwf   PORTB
	movlw   b'00001100'
	movwf   bar
	
	bcf    flag, 0			;To set the level back to one
	bcf    flag, 2
	bcf    flag, 3
	bcf    flag, 4
	bcf    flag, 5
	bsf    flag, 1
	
	movlw  h'05'			;Reset the health to 5
	movwf  health
	
	call display_health		;Restart lighting
	call display_level		
	
	goto    game

end