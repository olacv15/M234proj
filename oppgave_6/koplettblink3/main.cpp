
#include <avr/io.h>
#include <avr/interrupt.h>

#include "LinearBlink.h"
#include "InteruptPwm.h"


void setup();					// funksjon med oppsett av register og utganger 

InteruptPwm objekt;				// lager objekt av InteruptPwm, LinearBlink brukes i InteruptPwm


ISR(TIMER1_COMPA_vect)
{
	objekt.pwmSignal();	
}



int main(void)
{
	setup();
	sei();				// legges i slutt av setup eller etter setup for å unngå ukontrollert interupts i register (god praksis å legge den synlig i main) 

	
	while (1)
	{

	}

}


void setup()
{
	
	DDRB = 0xFF;							//Setter alle bits i port b som outputs
	
	
	
	TCCR1A = 0;
	TCCR1B = 0;
	TCCR1B |= (1 << WGM12) | (1 << CS10);	//Prescaler 1, CTC mode
	TIMSK1 |= (1 << OCIE1A);				//Output compare A match interrupt enable

	
	OCR1A = 199;							//Timer1 teller fra 0-199. Med prescaler på 1:1 gir dette oss en pwm frekvens på 200Hz, oppløsning på 200.
	
	
}




