#define F_CU 20000000L
#include <avr/io.h>
#include <avr/interrupt.h>
#include<util/delay.h>

// double dutyCycle = 0;
// int main(void){
// 	DDRD = (1 << PORTD6);
// 	TCCR0A = (1 << COM0A1) | (1 << WGM00) | (1 << WGM01);
// 	TIMSK0 = (1 << TOIE0);
// 	OCR0A = (dutyCycle/100)*255;
// 	sei();
// 	TCCR0B = (1 << CS00);
// 	while(1){
// 		_delay_ms(100);
// 		dutyCycle += 10;
// 		if(dutyCycle > 100){
// 			dutyCycle = 0;
// 		}

// 	}
// }

// ISR(TIMER0_OVF_vect){
// 	OCR0A = (dutyCycle/100)*255;

//}
 int main(void){
 DDRD = 0x40;
 //TCCR0A = (1 << WGM01);
 //TCCR0A |= (0 << COM0A1) | (1 << COM0A0);//toggle OC0A(o/p compare pin) on compare match
 OCR0A = 17;
 TIMSK0 = (1 << OCIE0A);//Output Compare Interrupt Enable
 sei();
 TCCR0B = (1 << CS02) | (1 << CS00);
 while(1){
	
 }

}

 ISR(TIMER0_COMPA_vect){
 	PORTD ^= (1<<PORTD6);
 	//OCR0A = 17;
 	TCNT0 = 0;
		
	
 }