#include <avr/io.h>
#include <avr/interrupt.h>

int main(void){
 DDRD = 0x40;

 TCCR0A |= (1 << WGM01);
 TCCR0A |= (0 << COM0A1) | (1 << COM0A0);//toggle OC0A(o/p compare pin) on compare match 
 OCR0A = 35;  
 TCCR0B |= (1 << CS02) | (1 << CS00);
 
 TIFR0 |= (1<<OCF0A);
 
 while(1){
 	
 	
 }

}