#include<avr/io.h>
#include "uart.h"
#include<avr/interrupt.h>


void startConversion()
{
	ADCSRA |= (1 << ADSC);
}

void setupADC()
{
	ADMUX = (1 << REFS0);//Reference voltage
	ADCSRA = (1 << ADEN) | (1 << ADIE) | (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS1);
    
    DIDR0 = (1 << ADC0D);

    startConversion();
}




int main(void){
uart_init();
DDRC = 0x00;

setupADC();
sei();

TCCR0B = (1 << CS01) | (1 << CS00);

while(1){

}
}

ISR(ADC_vect){
	printf("%u\n",ADC);
	startConversion();
}



