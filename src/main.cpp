#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

/*
Made by: Nian Luisman
if delay period is 0 time of a period is approximately 5us. 
Calculate the wait time example 100ms with pre-scaler of 1024:
  * Freq/Pre-scaler => 16MHz/1024 = 15.625KHz
  * 1/15.625KHz = 64μs
  * TCNT1 = 2^16 - (t/64μs)
  * TCNT1 = 2^16 - (100/0.064) => TCNT1 = 65536 - 1562 => TCNT1 = 63974
See link: https://denizariyan.com/interrupt-based-timer-for-atmega328p
*/


//changes these values to manipulate the Rabi oscillation.
const float delayPeriodTime = 0;
const float delayMWOn = 0.1f;
const float delayLaserOn = 5;
const float delayDeltaLaserMicrowave = 1.0f; 
//****************************************************************

ISR (TIMER1_OVF_vect) {
    PORTB ^= (1 << PB3);
    _delay_us(delayLaserOn);
    PORTB ^= (1 << PB3);

    _delay_us(delayDeltaLaserMicrowave);

    PORTB ^= (1 << PB1);
    PORTB ^= (1 << PB2); 
    _delay_us(delayMWOn);
     PORTB ^= (1 << PB1); 
    PORTB ^= (1 << PB2); 
    
    TCNT1 = 65536 - (delayPeriodTime/0.064);
}
void init_timer(){
    // Set data direction register of PORTB. 
    DDRB |= (1 << PB1) | (1 << PB2) | (1 << PB3);
   
    TCNT1 = 65536 - (delayPeriodTime/0.064);
    // Set normal counter mode
    TCCR1A = 0x00;
    // Set pre-scaler
    TCCR1B |= (1<<CS10) | (1 << CS12);

    // Set overflow interrupt enable bit
    TIMSK1 = (1 << TOIE1); 
    sei(); // Enable interrupts globally
}
int main() {

    init_timer();
    //init values relay. 
    PORTB |= (1 << PB1);
    PORTB &= ~(1 << PB2);

    while(1)
    {
        // Do anything, this timer is non-blocking.
        // It will interrupt the CPU only when needed
    }
    // Add return so compilers don't cry about it being missing.
    // Under normal circumstances this will never be hit
    return 0;
}