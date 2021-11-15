#include "timer.h"
//You many use any timer you wish for the microsecond delay and the millisecond delay


/* Initialize timer 1, you should not turn the timer on here. Use CTC mode  .*/
void initTimer1(){
	//set waveform generation bits for CTC mode
    //      WGM13 = 0; WGM12 = 0; WGM11 = 1; WGM10 = 0;
    TCCR1A &= ~(1<<WGM10);
    TCCR1B &= ~(1<<WGM12);
    TCCR1A |= (1<<WGM11);

    // //set the prescaler to 1024
    // //CS12 = 1, CS11 = 0, CS10 = 1;
    TCCR1B &= ~(1<<CS11);
    TCCR1B |= (1<<CS12)|(1<<CS10);

    //count needed given the prescaler
    OCR1A = 250;
}


void delayMs(unsigned int delay){
    unsigned int delayCnt = 0;

    TCNT1 = 0;
    TIFR1 |= (1<<OCF1A);

    while(delayCnt<delay){
        if (TIFR1 & (1<<OCF1A)){
            delayCnt++;
            TIFR1 |= (1<<OCF1A);
        }
    }
}
