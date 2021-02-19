#pragma config OSC = HSPLL, FCMEN = OFF, IESO = OFF, PWRT = OFF, BOREN = OFF, WDT = OFF, MCLRE = ON, LPT1OSC = OFF, LVP = OFF, XINST = OFF, DEBUG = OFF

#include <xc.h>
#include "breakpoints.h"

/*The Information of Group 27:
*Melis Ece Unsal 2237865
*Utku Gungor 2237477
*Doruk Gercel 2310027*/

/*Our dessign architecture is similar to Round-Robin architecture with interrupts.
 *We set the required interrupts which are ADC, RB4, TMR0, TMR1, TMR2(10 ms rb4 handle
 *that is defined by us). As an interrupt occurs the ISR takes the conrtol, direct the
 *control flow to the correct interrupt handler. We have already created some flags,
 *and these flags are set acording to the interrupt handlers. In our main loop, there
 *are flag control points, and the flow enters to the necessary parts acording to the
 *flags that are set in the interrupt handlers. When they enter a control point the
 *flag is directly reset inorder to prevent wrong entries.*/

volatile int adc_value; //Holds converted ADC
int counter_0;          //Counts for real TMR0 interrupt
int flag_timer0_end;    //Set when TMR0 counts 50ms
int counter_1;          //Counts for real TMR1 interrupt
int flag_timer1_end;    //Set when TMR1 counts 5000ms
int high_adc_guess;     //Holds the high bits ADC guess
int low_adc_guess;      //Holds the low bits ADC guess
int high_adc;           //Holds the high bits ADC read
int low_adc;            //Holds the low bits ADC read
int flag_adc_ready;     //Set when ADC conversion is finished
int adc_step;           //The value [0,9] for ADC
int flag_make_guess;    //Set when RB4 is really pressed
int guess_no;           //Number which is guessed
int flag_end_result;    //Used for 500ms lighth or 500ms dim
int flag_finish;        //Set for finish time of TMR1
int finish_counter;     //Counts for 4 light operations
int counter_2;          //Counts for real TMR2 interrupt
int enter_500ms;        //Checks for 1 entry in 500ms
int flag_rb4_pushed;    //The state of button RB4

/*Set TMR1 configuration*/
void timer1_start(){
    T1CON = 0xF0;              /*T1CON=11110000*/
    TMR1 = 41455;
    T1CON = T1CON | 0x01;
}

/*Make first init*/
void init(){
    /*Clear PORT values and INTCON*/
    PORTB = 0x00;
    PORTC = 0x00;
    PORTD = 0x00;
    PORTE = 0x00;
    PORTH = 0x00;
    PORTJ = 0x00;
    INTCON = 0x00;
    TMR1IE = 0;
    ADIE = 0;
    
    /*Clear variables*/
    adc_value = 5;
    counter_0 = 0;
    flag_timer0_end = 0;
    counter_1 = 0;
    flag_timer1_end = 0;
    high_adc_guess = 0;
    low_adc_guess = 0;
    high_adc = 0;
    low_adc = 0;
    flag_adc_ready = 0;
    flag_make_guess = 0;
    guess_no = 0;
    flag_end_result = 0;
    flag_finish = 0;
    finish_counter = 0;
    counter_2 = 0;
    enter_500ms = 0;
    flag_rb4_pushed = 0;
    
    /*Initialize TRIS Registers*/
    TRISB = 0x10;
    TRISC = 0xF9;
    TRISD = 0xF0;
    TRISE = 0xF9;
    TRISH = 0x10;
    TRISJ = 0x00;
    
    PR2 = 124;      //Will be used for TMR2 interrupt
    /*Select PORTH*/
    PORTH = 0x08;
    
    init_complete();    //Call breakpoint function
    
    /*Initialize ADC Registers*/
    ADCON1 = 0x00;             /*ADCON1=00000000*/
    ADCON0 = 0x30;             /*ADCON0=00110000*/
    ADCON2 = 0x82;             /*ADCON2=10000010*/
    ADON = 1;                  /*ADCON0=00110001*/
    
    /*Initialize TIMER Registers*/
    T0CON = 0x47;              /*T0CON=01000111*/
    TMR0 = 0x3D;
    T0CON = T0CON | 0x80;      /*T0CON=11000111*/
    timer1_start();
    T2CON = 0x03;               /*T2CON=00000011*/
    
    /*Enable interrupts*/
    INTCON = 0xE8;             /*INTCON=11101000*/
    PIE1bits.TMR1IE = 0x1;     //Enable TMR1 interrupt
    PIE1bits.ADIE = 0x1;       //Enable ADC interrupt
    
    return;
}

/*Make init in restart condition*/
void restart_init(){
    /*Clear PORT values and INTCON*/
    PORTB = 0x00;
    PORTC = 0x00;
    PORTD = 0x00;
    PORTE = 0x00;
    PORTH = 0x00;
    PORTJ = 0x00;
    INTCON = 0x00;
    TMR1IE = 0;
    ADIE = 0;
    
    /*Clear variables*/
    adc_value = 5;
    counter_0 = 0;
    flag_timer0_end = 0;
    counter_1 = 0;
    flag_timer1_end = 0;
    high_adc_guess = 0;
    low_adc_guess = 0;
    high_adc = 0;
    low_adc = 0;
    flag_adc_ready = 0;
    flag_make_guess = 0;
    guess_no = 0;
    flag_end_result = 0;
    flag_finish = 0;
    finish_counter = 0;
    counter_2 = 0;
    enter_500ms = 0;
    flag_rb4_pushed = 0;
    
    /*Initialize TRIS Registers*/
    TRISB = 0x10;
    TRISC = 0xF9;
    TRISD = 0xF0;
    TRISE = 0xF9;
    TRISH = 0x10;
    TRISJ = 0x00;
    
    PR2 = 124;      //Will be used for TMR2
    /*Select PORTH*/
    PORTH = 0x08;
        
    /*Initialize ADC Registers*/
    ADCON1 = 0x00;             /*ADCON1=00000000*/
    ADCON0 = 0x30;             /*ADCON0=00110000*/
    ADCON2 = 0x82;             /*ADCON2=10000010*/
    ADON = 1;                  /*ADCON0=00110001*/
    
    /*Initialize TIMER Registers*/
    T0CON = 0x47;              /*T0CON=01000111*/
    TMR0 = 0x3D;
    T0CON = T0CON | 0x80;      /*T0CON=11000111*/
    timer1_start();
    T2CON = 0x03;               /*T2CON=00000011*/
    
    /*Enable interrupts*/
    INTCON = 0xE8;             /*INTCON=11101000*/
    PIE1bits.TMR1IE = 0x1;     //Enable TMR1 interrupt
    PIE1bits.ADIE = 0x1;       //Enable ADC interrupt
    
    return;
}

/*Set the PORTJ values for 7-Segment Display*/
void light_port_jh(int number){
    if(number == -1){
        PORTJ = 0x00;       //Dim the lights
    }
    else if(number == 0){
        PORTJ = 0x3F;      //00111111
    }
    else if(number == 1){
        PORTJ = 0x06;      //00000110
    }
    else if(number == 2){
        PORTJ = 0x5B;      //01011011
    }
    else if(number == 3){
        PORTJ = 0x4F;      //01001111
    }
    else if(number == 4){
        PORTJ = 0x66;      //01100110
    }
    else if(number == 5){
        PORTJ = 0x6D;      //01101101
    }
    else if(number == 6){
        PORTJ = 0x7D;      //01111101
    }
    else if(number == 7){
        PORTJ = 0x07;      //00000111
    }
    else if(number == 8){
        PORTJ = 0x7F;      //01111111
    }
    else if(number == 9){
        PORTJ = 0x6F;      //01101111
    }
    latjh_update_complete(); //Call breakpoint function
    return;
}

/*Set PORT_CDE for Arrows*/
void light_port_cde(int direction){
    if(direction == 0){         //Turn off the lights
        PORTC = 0x00;
        PORTD = 0x00;
        PORTE = 0x00;
    }
    else if(direction < 0){      //Down arrow
        PORTC = 0x04;
        PORTD = 0x0F;
        PORTE = 0x04;
    }
    else{                   //Up arrow
        PORTC = 0x02;
        PORTD = 0x0F;
        PORTE = 0x02;
    }
    latcde_update_complete();   //Call breakpoint function
    return;
}

/*Count 50 ms for ADC to start*/
void timer0_time(){
    TMR0IF = 0;
    counter_0 += 1;
    if(counter_0 == 10){        //50 ms
        flag_timer0_end = 1;
        counter_0 = 0;
        GODONE = 1;             //Start ADC conversion
        return;
    }
    else{
        TMR0 = 0x3D;    //TMR0 = 61
        return;
    }
}

/*Count 5s for game-over*/
void timer1_time(){                 //End the game
    TMR1IF = 0;
    counter_1 += 1;
    if(counter_1 == 96){          //5000 ms
        flag_timer1_end = 1;
        
        counter_1 = 0;          //Reset counter
        TMR1 = 30359;           //Set for timer1_finish
        return;
    }
    else{
        return;
    }
}

/*Count 500 ms intervals for Dim and Ligth*/
void timer1_finish(){
    TMR1IF = 0;
    counter_1 += 1;
    if(counter_1 == 10){
        //flag_end_result = flag_end_result ^ 1;
        finish_counter += 1;    //1 operation is done again
        
        if(finish_counter < 4){
            enter_500ms = 1;        //500 ms passed
        }
        
        hs_passed();        //Call breakpoint function
        
        counter_1 = 0;      //Reset counter
        TMR1 = 30359;       //Set for timer1_finish
        return;
    }
    else{
        return;
    }
}

/*Count for 10ms for Debouncing*/
void timer2_time(){
    TMR2IF = 0;
    counter_2 += 1;
    if(counter_2 == 50){    //Check for 10ms
        if(flag_rb4_pushed == 0){
            if(PORTBbits.RB4 == 1){ //Check for if still pressed
            
                rb4_handled();       //Call Breakpoint function
            
                flag_make_guess = 1;    //Guess made
                flag_rb4_pushed = 1;    //The button is now in pressed state
            }
            counter_2 = 0;
            TMR2ON = 0;             //Close TMR2
            PIE1bits.TMR2IE = 0;    //Disable TMR2 interrupt
        }
        else if(flag_rb4_pushed == 1){
            if(PORTBbits.RB4 == 0){ //Check for if still not pressed
                
                flag_make_guess = 0;    
                flag_rb4_pushed = 0;    //The button is now in un-pressed state
            }
            counter_2 = 0;
            TMR2ON = 0;             //Close TMR2
            PIE1bits.TMR2IE = 0;    //Disable TMR2 interrupt
        }
    }
    TMR2 = 0;   //Reset TMR2
    return;
    
    
}


void adc_action(){
    ADIF = 0;
    flag_adc_ready = 1;
    high_adc = ADRESH;          //Make the ADC readings
    high_adc = high_adc & 0x03;
    low_adc = ADRESL;
    
    adc_value = high_adc * 256 + low_adc;   //Compute the value
    
    adc_complete();         //Call breakpoint function
    
    TMR0 = 0x3D;            //Start initial TMR0
    counter_0 = 0;          //Start timer0 count
    return;
}

void __interrupt() my_isr(){
    if(TMR0IF == 1){    //Check if TMR0 interrupt
        TMR0IF = 0;
        timer0_time();
    }
    else if(TMR1IF == 1){   //Check if TMR1 interrupt
        TMR1IF = 0;
        if(flag_finish == 0){   //5s case
            timer1_time();
        }
        else{                   //500ms case
            timer1_finish(); 
        }
    }
    else if(TMR2IF == 1){       //Check if TMR2 interrupt
        timer2_time();
    }
    else if(RBIF == 1){         ////Check if RB4 interrupt
        RBIF = 0;
        if(flag_rb4_pushed == 0){       //Press Operation
            if(PORTBbits.RB4 == 1){           //Start TMR2 for RB4 Handle
                TMR2 = 0;               //TMR2 initialize
                TMR2ON = 1;             //TMR2ON
                PIE1bits.TMR2IE = 1;    //TMR2 interrupt enable      
            }
            else if(PORTBbits.RB4 == 0){                       //Debounce noise reset TMR2
                TMR2 = 0;               //TMR2 initialize
                TMR2ON = 0;             //TMR2ON
                PIE1bits.TMR2IE = 0;    //TMR2 interrupt enable
            }
        }
        else if(flag_rb4_pushed == 1){      //Relase Check
            if(PORTBbits.RB4 == 0){           //Start TMR2 for RB4 Handle
                TMR2 = 0;               //TMR2 initialize
                TMR2ON = 1;             //TMR2ON
                PIE1bits.TMR2IE = 1;    //TMR2 interrupt enable      
            }
            else if(PORTBbits.RB4 == 1){                       //Debounce noise reset TMR2
                TMR2 = 0;               //TMR2 initialize
                TMR2ON = 0;             //TMR2ON
                PIE1bits.TMR2IE = 0;    //TMR2 interrupt enable
            }
        }
    }
    else if(ADIF == 1){     //Check if ADC interrupt
        ADIF = 0;
        adc_action();
    }
}


void finish(){ 
    counter_1 = 0;
    TMR1 = 30359;
    
    finish_counter = 0;
    
    flag_finish = 1;
    flag_end_result = 1;
    enter_500ms = 1;
    while(finish_counter < 4){        //Call this for break
        if(flag_end_result == 1 && enter_500ms == 1){       //Call this for 500 ms light
            enter_500ms = 0;
            flag_end_result = 0;
            light_port_jh(special_number());
        }
        else if(flag_end_result == 0 && enter_500ms == 1){   //Call this for 500 ms dim
            enter_500ms = 0;
            flag_end_result = 1;
            light_port_jh(-1);
        }        
    }
    restart();          //Call breakpoint function
    
    restart_init();     //Restart initialization
}

void main(void) {
    init();             //Make first initialization
    while(1){
        if(flag_adc_ready == 1){        //Make Mapping, ADC value is ready
            flag_adc_ready = 0;
            adc_step = (adc_value - 1)/102;
            if(adc_value == 0){
                adc_step = 0;
            }
            else if(adc_step == 10){
                adc_step = 9;
            }
            light_port_jh(adc_step);
        }
        if(flag_make_guess == 1){       //RB4 handled, and a guess was made
            flag_make_guess = 0;
            guess_no = adc_step;
            if(guess_no == special_number()){       //Correct Guess
                correct_guess();
               
                /*Disable interrupts expect TMR1*/
                INTCON = 0xC0;             /*INTCON=11000000*/
                PIE1bits.ADIE = 0x0;       //Disable ADC interrupt
                PIE1bits.TMR2IE = 0;       //TMR2 interrupt disable
                
                light_port_cde(0);          //Turn off the leds

                finish();                   //Call finish process
            }
            else{                                   //Wrong Guess
                light_port_cde(special_number() - guess_no);        //Light arrows
            }
        }
        if(flag_timer1_end == 1){           //5 seconds passed game over
            flag_timer1_end = 0;
            
            game_over();
            
            /*Disable interrupts expect TMR1*/
            INTCON = 0xC0;             /*INTCON=11000000*/
            PIE1bits.ADIE = 0x0;       //Disable ADC interrupt
            PIE1bits.TMR2IE = 0;       //TMR2 interrupt disable
            
            light_port_cde(0);          //Turn of the leds
            
            finish();                   //Call finish process
        }
    }
    return;
}
