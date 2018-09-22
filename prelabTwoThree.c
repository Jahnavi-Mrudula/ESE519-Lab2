#include <avr/io.h>
#include <avr/interrupt.h>
#include<stdio.h>
#include <util/delay.h>
#include "uart.h"
#define F_CPU 16000000
#define BAUD 9600
#define BRC ((F_CPU/16/BAUD)-1)

unsigned long timeOfCapture = 0;
unsigned long overflows = 0;
int numberOfEdges = 0;
unsigned long edge1 = 0;
unsigned long edge2 = 0;
unsigned long pulseWidth = 0;
unsigned long a = 12222;
int check = 50;
int trigger = 0;


int main(void){
uart_init();
DDRB = 0x1E;
PORTB = 0x02;

OCR1A = 160;
TCNT1 = 0;


//TCCR1B |= (1 << WGM12);//CTC
TIMSK1 |= (1 << OCIE1A);//Output Compare Interrupt Enable
sei();
TCCR1B |= (1 << CS10); //enable timer1 with no prescaling


for(;;){

}


}

ISR(TIMER1_COMPA_vect){
    PORTB ^= (1<<PORTB1);
    //TCNT1 = 0;
    cli();
    
    TCCR1B ^= (1 << ICES1); //capture on rising edge
    TCCR1B |= (1 << ICNC1); //i/p capture noise canceller
    
    TIMSK1 |= (1 << ICIE1); //enabling the i/p capture interrupt
    TIMSK1 |= (1 << TOIE1);

    TIFR1 |= (1 << ICF1); //initially clear Input Capture Flag
    
    //TCCR1B |= (1 << CS10); //enable timer1 with no prescaling
    sei();
 }


 ISR(TIMER1_CAPT_vect){
    numberOfEdges += 1;
    if(numberOfEdges % 2 ==1){
        edge1 = ICR1;
        
        TCCR1B ^= (1 << ICES1); //altering the edge to capture
        TIFR1 |= (1 << ICF1); //initially clear Input Capture Flag
        overflows = 0;
         }
    else{
        edge2 = ICR1;
        
        TIFR1 |= (1 << ICF1); //initially clear Input Capture Flag
        TCCR1B ^= (1 << ICES1);//altering the edge to capture
        numberOfEdges = 0;
        if(((edge1>0)&&(edge1<=65535))&&(overflows >= 1)){
    pulseWidth = ((overflows-1)*(65536)+ (65535-edge1)+edge2);  //*256/(16*1000000);
}
else if(((edge1>0)&&(edge1<=65535))&&(overflows ==0)){
    pulseWidth = edge2 - edge1;
}
else{
    pulseWidth = ((overflows)*(65536)+edge2);    //*256/(16*1000000);
}
     //printf("%lu \t %lu \t %lu \n",overflows,edge1,edge2); 
       printf("%lu \n",pulseWidth);
       cli();

DDRD = 0x40;

 TCCR0A |= (1 << WGM01);//CTC
 TCCR0A |= (0 << COM0A1) | (1 << COM0A0);//toggle OC0A(o/p compare pin) on compare match 
  
 TCCR0B |= (1 << CS01) | (1 << CS00);//prescaler 64
 
 TIFR0 |= (1<<OCF0A);


if((pulseWidth >= 2500) && (pulseWidth <= 3800)){
    PORTB = 0x00;
    PORTB |= (0 << PORTB4);
    PORTB |= (0 << PORTB3);
    PORTB |= (0 << PORTB2);
    OCR0A = 118;
    TIFR0 |= (1<<OCF0A); 

}

else if((pulseWidth >= 3800) && (pulseWidth <= 5100)){
    PORTB = 0x00;
    PORTB |= (0 << PORTB4);
    PORTB |= (0 << PORTB3);
    PORTB |= (1 << PORTB2);
    OCR0A = 106;
    TIFR0 |= (1<<OCF0A);  

}
else if((pulseWidth >= 5100) && (pulseWidth <= 6400)){
    PORTB = 0x00;
    PORTB |= (0 << PORTB4);
    PORTB |= (1 << PORTB3);
    PORTB |= (0 << PORTB2);
    OCR0A = 95;
    TIFR0 |= (1<<OCF0A); 

}

else if((pulseWidth >= 6400) && (pulseWidth <= 7700)){
    PORTB = 0x00;
    PORTB |= (0 << PORTB4);
    PORTB |= (1 << PORTB3);
    PORTB |= (1 << PORTB2);
    OCR0A = 89;
    TIFR0 |= (1<<OCF0A); 

}

else if((pulseWidth >= 7700) && (pulseWidth <= 9000)){
    PORTB = 0x00;
    PORTB |= (1 << PORTB4);
    PORTB |= (0 << PORTB3);
    PORTB |= (0 << PORTB2);
    OCR0A = 79;
    TIFR0 |= (1<<OCF0A); 

}

else if((pulseWidth >= 9000) && (pulseWidth <= 10300)){
    PORTB = 0x00;
    PORTB |= (1 << PORTB4);
    PORTB |= (0 << PORTB3);
    PORTB |= (1 << PORTB2);
    OCR0A = 71;
    TIFR0 |= (1<<OCF0A); 

}

else if((pulseWidth >= 10300) && (pulseWidth <= 11600)){
    PORTB = 0x00;
    PORTB |= (1 << PORTB4);
    PORTB |= (1 << PORTB3);
    PORTB |= (0 << PORTB2);
    OCR0A = 63;
    TIFR0 |= (1<<OCF0A); 

}

else{
    PORTB = 0x00;
    PORTB |= (1 << PORTB4);
    PORTB |= (1 << PORTB3);
    PORTB |= (1 << PORTB2);
    OCR0A = 59;
    TIFR0 |= (1<<OCF0A); 
    }
    sei();
}

}

ISR(TIMER1_OVF_vect){
    overflows++;
    //printf("%lu \n",overflows);

    //printf("%lu",overflows);
    
}