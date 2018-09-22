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
double offset;
double frequency;

int main(void){
uart_init();
DDRB = 0x1E;
PORTB = 0x02;
DDRD = 0x40;
OCR1A = 160;
TCNT1 = 0;
trigger = 1;


TIMSK1 |= (1 << OCIE1A);//Output Compare Interrupt Enable
sei();
TCCR1B |= (1 << CS10); //enable timer1 with no prescaling


for(;;){
}


}

ISR(TIMER1_COMPA_vect){
    if(trigger == 1){ 
    PORTB ^= (1<<PORTB1);
    OCR1A += 3200; //to wait for 200us
    trigger = 0;
}
    
else{
    trigger = 1;
    OCR1A += 160;
    cli();
    
    TCCR1B ^= (1 << ICES1); //capture on rising edge
    TCCR1B |= (1 << ICNC1); //i/p capture noise canceller
    
    TIMSK1 |= (1 << ICIE1); //enabling the i/p capture interrupt
    TIMSK1 |= (1 << TOIE1);

    TIFR1 |= (1 << ICF1); //initially clear Input Capture Flag
    
    
    sei();
}
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
       printf("%lu \n",pulseWidth);
       cli();

 //continuous mode
 if((PIND & 0x80) == 0x80){ //I used PORT D7 to choose between continuos and discrete
    TCCR0A |= (1 << WGM01);//CTC
 TCCR0A |= (0 << COM0A1) | (1 << COM0A0);//toggle OC0A(o/p compare pin) on compare match 
  
 TCCR0B |= (1 << CS01) | (1 << CS00);//prescaler 64
 
 TIFR0 |= (1<<OCF0A);
offset = ((pulseWidth-2500)/11000)*1046.5;
frequency = 1046.5+offset;
OCR0A = (16000000/(frequency*64))/2;
if((pulseWidth >= 2500) && (pulseWidth <= 3800)){

    PORTB = 0x00;
    PORTB |= (0 << PORTB4);
    PORTB |= (0 << PORTB3);
    PORTB |= (0 << PORTB2); 

}

else if((pulseWidth >= 3800) && (pulseWidth <= 5100)){
    PORTB = 0x00;
    PORTB |= (0 << PORTB4);
    PORTB |= (0 << PORTB3);
    PORTB |= (1 << PORTB2); 

}
else if((pulseWidth >= 5100) && (pulseWidth <= 6400)){
    PORTB = 0x00;
    PORTB |= (0 << PORTB4);
    PORTB |= (1 << PORTB3);
    PORTB |= (0 << PORTB2); 

}

else if((pulseWidth >= 6400) && (pulseWidth <= 7700)){
    PORTB = 0x00;
    PORTB |= (0 << PORTB4);
    PORTB |= (1 << PORTB3);
    PORTB |= (1 << PORTB2); 

}

else if((pulseWidth >= 7700) && (pulseWidth <= 9000)){
    PORTB = 0x00;
    PORTB |= (1 << PORTB4);
    PORTB |= (0 << PORTB3);
    PORTB |= (0 << PORTB2);

}

else if((pulseWidth >= 9000) && (pulseWidth <= 10300)){
    PORTB = 0x00;
    PORTB |= (1 << PORTB4);
    PORTB |= (0 << PORTB3);
    PORTB |= (1 << PORTB2);

}

else if((pulseWidth >= 10300) && (pulseWidth <= 11600)){
    PORTB = 0x00;
    PORTB |= (1 << PORTB4);
    PORTB |= (1 << PORTB3);
    PORTB |= (0 << PORTB2);

}

else{
    PORTB = 0x00;
    PORTB |= (1 << PORTB4);
    PORTB |= (1 << PORTB3);
    PORTB |= (1 << PORTB2);
    }
sei();
 }
 //discrete mode
 else{

if((pulseWidth >= 2500) && (pulseWidth <= 3800)){
    PORTB = 0x00;
    PORTB |= (0 << PORTB4);
    PORTB |= (0 << PORTB3);
    PORTB |= (0 << PORTB2);
    OCR0A = 118;
    TIFR0 |= (1<<OCF0A); 
TCCR0B |= (1 << CS01) | (1 << CS00);//prescaler 64 

}

else if((pulseWidth >= 3800) && (pulseWidth <= 5100)){
    PORTB = 0x00;
    PORTB |= (0 << PORTB4);
    PORTB |= (0 << PORTB3);
    PORTB |= (1 << PORTB2);
    OCR0A = 106;
    TIFR0 |= (1<<OCF0A);  
    TCCR0B |= (1 << CS01) | (1 << CS00);//prescaler 64 


}
else if((pulseWidth >= 5100) && (pulseWidth <= 6400)){
    PORTB = 0x00;
    PORTB |= (0 << PORTB4);
    PORTB |= (1 << PORTB3);
    PORTB |= (0 << PORTB2);
    OCR0A = 95;
    TIFR0 |= (1<<OCF0A); 
    TCCR0B |= (1 << CS01) | (1 << CS00);//prescaler 64 


}

else if((pulseWidth >= 6400) && (pulseWidth <= 7700)){
    PORTB = 0x00;
    PORTB |= (0 << PORTB4);
    PORTB |= (1 << PORTB3);
    PORTB |= (1 << PORTB2);
    OCR0A = 89;
    TIFR0 |= (1<<OCF0A); 
    TCCR0B |= (1 << CS01) | (1 << CS00);//prescaler 64 


}

else if((pulseWidth >= 7700) && (pulseWidth <= 9000)){
    PORTB = 0x00;
    PORTB |= (1 << PORTB4);
    PORTB |= (0 << PORTB3);
    PORTB |= (0 << PORTB2);
    OCR0A = 79;
    TIFR0 |= (1<<OCF0A);
    TCCR0B |= (1 << CS01) | (1 << CS00);//prescaler 64 
 

}

else if((pulseWidth >= 9000) && (pulseWidth <= 10300)){
    PORTB = 0x00;
    PORTB |= (1 << PORTB4);
    PORTB |= (0 << PORTB3);
    PORTB |= (1 << PORTB2);
    OCR0A = 71;
    TIFR0 |= (1<<OCF0A); 
    TCCR0B |= (1 << CS01) | (1 << CS00);//prescaler 64 


}

else if((pulseWidth >= 10300) && (pulseWidth <= 11600)){
    PORTB = 0x00;
    PORTB |= (1 << PORTB4);
    PORTB |= (1 << PORTB3);
    PORTB |= (0 << PORTB2);
    OCR0A = 63;
    TIFR0 |= (1<<OCF0A); 
    TCCR0B |= (1 << CS01) | (1 << CS00);//prescaler 64 


}

else{
    PORTB = 0x00;
    PORTB |= (1 << PORTB4);
    PORTB |= (1 << PORTB3);
    PORTB |= (1 << PORTB2);
    OCR0A = 59;
    TIFR0 |= (1<<OCF0A); 
    TCCR0B |= (1 << CS01) | (1 << CS00);//prescaler 64 

    }
    sei();

 }
}

}

ISR(TIMER1_OVF_vect){
    overflows++;
    
}



