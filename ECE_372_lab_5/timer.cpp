#include <avr/io.h>
#include "timer.h"

void initTimerMs(){
    //set waveform generation bits for CTC mode
    //WGM13 = 0; WGM12 = 1; WGM11 = 0; WGM10 = 0;
    TCCR1A &= ~(1<<WGM10);
    TCCR1A &= ~(1<<WGM11);
    TCCR1B |= (1<<WGM12);
    TCCR1B &= ~(1<<WGM13);

    // //set the prescaler to 1   2^16 = 0xFFFF = 65536
    // //CS12 = 0, CS11 = 0, CS10 = 1;
    TCCR1B |= (1<<CS10);
    TCCR1B &= ~((1<<CS11)|(1<<CS12));
  
    // OCR1A = ( 1ms * 16*10^6) / 1  = 16000
    OCR1A = 16000;
}

void delayMs( unsigned int Delay){
    unsigned int x = 0;  // delay index
    TCNT1 = 0; // begin couting the register at 0
    TIFR1 = (1<<OCF1A); // Lower the flag 

    while(x < Delay){
        if(TIFR1 & (1<<OCF1A)){
            TIFR1 |=(1<<OCF1A); //  If statment true, lower the flag
            x++;
        }
    }

}