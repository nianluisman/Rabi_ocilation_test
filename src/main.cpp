#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

/*
if delay period is 0 time of a period is approximately 5us. 
*/
const float delayPeriodTime = 0;
const float delayMWOn = 0.1f;
const float delayDelayLaserMicrowave = 1.0f; //set delay time in ms 


ISR (TIMER1_OVF_vect) {
    
    PORTB ^= (1 << PB3);
    PORTB ^= (1 << PB3);
    _delay_us(delayDelayLaserMicrowave);
    PORTB ^= (1 << PB1); // Toggle the 5th data register of PORTB
    PORTB ^= (1 << PB2); // Toggle the 5th data register of PORTB
    _delay_us(delayMWOn);
     PORTB ^= (1 << PB1); // Toggle the 5th data register of PORTB
    PORTB ^= (1 << PB2); // Toggle the 5th data register of PORTB
    
    TCNT1 = 65536 - (delayPeriodTime/0.064);
}

int main() {
    // Set 5th data direction register of PORTB. A set value means output
    DDRB |= (1 << PB1) | (1 << PB2) | (1 << PB3);
    // 50 ms for 16MHz clock
    TCNT1 = 65536 - (delayPeriodTime/0.064);
    // Set normal counter mode
    TCCR1A = 0x00;
    // Set 1024 pre-scaler
    TCCR1B |= (1<<CS10) | (1 << CS12);
    // Set overflow interrupt enable bit
    PORTB |= (1 << PB1);
    PORTB &= ~(1 << PB2);

    TIMSK1 = (1 << TOIE1); 
    sei(); // Enable interrupts globally
    while(1)
    {
        // Do anything, this timer is non-blocking.
        // It will interrupt the CPU only when needed
    }
    // Add return so compilers don't cry about it being missing.
    // Under normal circumstances this will never be hit
    return 0;
}