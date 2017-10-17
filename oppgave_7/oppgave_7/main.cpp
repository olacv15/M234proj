/*
 * oppgave_7.cpp
 *
 * Created: 16.10.2017 10:24:15
 * Author : Ola Christoffer Våge
 */ 

#include <avr/io.h>



int main(void)
{
	#define knapp PC5
	DDRB = 0b00000001;
    PORTB = 0x00;
	DDRC = 0x00;
	PORTC = 0b00000000;
	
	PCICR = 0x00;
	PCICR |= (1 << PCIE1);
	
	PCMSK1 = 0x00;
	PCMSK1 |= (1 << PCINT13);
	
	
	sei();
	
	
    while (1) 
    {	
		if(PINC & (1 << knapp))		// sjekker etter 1 på PC5
		{
			PORTB |= 0b00000001;	// Skrur led av
		}
		
		else
		{
			PORTB &= 0b11111110;	 //skrur led på
		}
		
    }
}

