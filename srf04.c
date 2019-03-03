#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
//#include <avr/pgmspace.h>

#include "srf04.h"

/* ---------------------------------------------------------------------------
 * 
 * internal clock attiny84 : 10MHz
 * srf04 ROUTINES 
 * 
 *         Sonar interfacing:
 *                1. Send high impulse to Trig input for minimum 10us
 *                2. Sonar automatically sends eight 40kHz inpulses
 *                3. Sonar rises high on Echo output and then after some time drops 
 *                   output to low (can take a while on long distances! - must include timeouts)
 *                   38mS if no obstacle
 *                4. Based on output time difference deltaT = lowT-highT calculate: 
 *                        distance = [ deltaT * sound_speed(340m/s) ] / 2
 *                5. Make a delay before starting the next cycle to compensate for late echoes
 * 
 * Distance calculation :
 *      time period ( = time of 1 clock pulse) = 1/Clockfreq.
 *      1/16 000 000 = 0.000 000 062 5 sec = 0.0625µs
 *      1/10 000 000 = 0.000 000 1 sec = 0.1µs
 * 
 * Max. number of tiks:
 *      10mHz, no préscaler, 38mS if no obstacle
 *      [(10.000.000/0)*0.038]-1=379999
 *      16bit timer overflow = 65536
 *      379.999/65.536 = 5,798          379.999/256 = 1484.37109375
 *      379.999-(65.536*5) = 52.319     379.999-(256*1484) = 95
 *      ==> 5 overflows & 52.319 tiks
 * 
 * ---------------------------------------------------------------------------*/


void srf04_init(){
    // ------------------- ultrasonic init code --------------------
    SONAR_TRIGGER_OUTPUT_MODE();
    SONAR_ECHO_INPUT_MODE();
    SONAR_ECHO_PULL_UP();
    timerCounter = 0;
    
    cli(); //disable global interrupts
    
    // interrupt 0 initialization
    EICRA |= (0 << ISC01) | (1 << ISC00);   // enable interrupt on any(rising/droping) edge
    EIMSK |= (1 << INT0);                   // Turns on INT0
    OCR1A =  1484;                   //sets the desired count to got time-out to 38mS if prescaler = 0
    // timer 0 initialization
    TCCR0B |= (1 << CS00);                  // set up timer with no prescaling
    TCNT0 = 0;                              // initialize counter
    TIMSK0 |= (1 << TOIE0);                 // enable timer interrupt
    
    sei();                                  // Enable Global Interrupt
}

//Timer0 overflow detect
/*
 * f = speed of crystal in Hz.
 * 
 * T = 1 / f = The time of one clock-pulse (Sec).               : 1e-07
 * N = 0.038 / T = Number of clock-pulses in 38ms seconds.      : 380.000
 * no prescaler value. use of 16bit counter
 * n = number of timer overflows int(N / 65536)
 * r = rest = N - (65536 * n)
 * 
*/
ISR(TIM0_OVF_vect)
{
    if (up){
        timerCounter++;     // count the timer overflow's
        if (timerCounter >= 240){
            distance=999;
            TCCR0B |= (0 << CS00);                            //Stop Timer
            running = 0;
        }
    }
}

// interrupt for INT0 pin, to detect high/low voltage changes
// We assume, that high voltage rise comes before low drop and not vice versa
// Check change in the level at the PD3 for falling/rising edge
SIGNAL(INT0_vect){
    if(running){ //accept interrupts only when sonar was started
        if (up == 0 ) { // voltage rise, start time measurement
            up = 1;
            //Setup Timer0
            TCNT0 = 0;                  // initialize counter
        } else { // voltage drop, stop time measurement
            up = 0;
            /* use this for measurement of < 1,40m we use nbrCnt = 2 */
            distance=(((timerCounter*256)+TCNT0)*0.017);        //0010200612
            timerCounter=0;
            TCCR0B |= (0 << CS00);                            //Stop Timer
            running = 0;
        }
    }else {
        distance=0;
        TCCR0B |= (0 << CS00);                            //Stop Timer
        running = 0;
    }
}

void sonar() {
    SONAR_TRIGGER_LOW();
    _delay_us(2);
    SONAR_TRIGGER_HIGH();
    _delay_us(10);
    SONAR_TRIGGER_LOW();
    running = 1;  // sonar launched
}
