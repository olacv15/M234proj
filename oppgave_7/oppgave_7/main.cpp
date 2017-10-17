/*
 * oppgave_7.cpp
 *
 * Created: 16.10.2017 10:24:15
 * Author : Ola Christoffer Våge
 */ 



#include <avr/io.h>
#include <avr/interrupt.h>
#define 	F_CPU   8000000UL
#include <util/delay.h>

int toggle = 1;
#define knapp 0x20


ISR(PCINT1_vect)
{
	if( ( PINC & knapp) == 0 )	// Sjekker etter 0 på PC5
	{
		toggle = -toggle;
		_delay_ms(100);		// Fungerer også uten delay, men etter litt prøving fant vi at 100ms delay gjør funskjonen bedre.
	}
}

int main(void)
{
	
	
	DDRB = 0b00000001;
	PORTB = 0x00;
	DDRC = 0x00;
	PORTC = 0b00000000;
	
	// Setter opp Pin Change Interrupt Control Register
	PCICR = 0x00;
	PCICR |= (1 << PCIE1);
	// Setter opp Pin Change Mask Register
	PCMSK1 = 0x00;
	PCMSK1 |= (1 << PCINT13);

	sei();
	

	while (1)
	{
		
		if (toggle == 1)
		{
			PORTB &= 0b11111110;	 //skrur led på
		}
		else if (toggle == -1)
		{
			PORTB |= 0b00000001;	// Skrur led av
		}
		
	}
}