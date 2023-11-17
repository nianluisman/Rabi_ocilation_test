#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


#define delayTime 100
ISR (TIMER1_OVF_vect) {
    PORTB ^= (1 << PB1); // Toggle the 5th data register of PORTB
    _delay_ms(10.0f);
    PORTB ^= (1 << PB2); // Toggle the 5th data register of PORTB
    
    TCNT1 = 65536 - (delayTime/0.064);
}

int main() {
    // Set 5th data direction register of PORTB. A set value means output
    DDRB = (1 << PB1) | (1 << PB2);
    // 50 ms for 16MHz clock
    TCNT1 = 65536 - (delayTime/0.064);
    // Set normal counter mode
    TCCR1A = 0x00;
    // Set 1024 pre-scaler
    TCCR1B = (1<<CS10) | (1<<CS12);
    // Set overflow interrupt enable bit
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