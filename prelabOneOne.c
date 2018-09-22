#include <avr/io.h>
#include <avr/interrupt.h>

int main(void)
{


DDRB = 0x20;
PORTB = 0x01;

//TCCR1B = (1 << WGM12); //set CTC
TCCR1B |= (0 << ICES1); //capture on falling edge
TCCR1B |= (1 << ICNC1);
TCCR1B |= (1 << CS12) | (1 << CS10); //enable timer1 with prescaler
TIFR1 |= (1 << ICF1); //initially clear Input Capture Flag

TIMSK1 = (1 << ICIE1); //enabling the interrupt
sei();
while(1){

}

}
ISR(TIMER1_CAPT_vect)
{
if(!(PINB & 0x01))
{
	PORTB |= 0x20;

}
else{
	PORTB &= 0xDF;
}

TIFR1 |= (1 << ICF1); //clearing Input Capture Flag
TCNT1 = 0;
TCCR1B ^= (1 << ICES1);
}