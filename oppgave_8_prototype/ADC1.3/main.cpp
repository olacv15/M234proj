#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>




uint16_t resultat;						//AD-konvertering er 10 bit


ISR(ADC_vect)
{
	
	resultat = ADC;						//henter verdier fra ADC-register		
										// resultat = (ADCH<<8) + ADCL; bruk av denne låser programmet til en verdi
	
	if (resultat >= 512)				//midtre verdi, resultat går fra 0 til 1023
	{
		PORTB &= 0b11111110;			//led på
	}
	else
	{
		PORTB |= 0b00000001;			//led av
	}


}



int main(void)
{
	
	
	ADMUX = 0b01000001;					// første to bit: bruker AVCC som referanse, men krever ekstern kondensator at AREF pin for å redusere støy
										// siste fire bit velger ADC1 altså  PC1
	
										//enable ADC, enable ADC interuptflag, enable autotrigger (default auto trigger er fre running mode), setter prescaler til 128 
	ADCSRA |= (1 << ADEN) | (1 << ADIE) | (1 << ADATE)| (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
		
	DDRB = 0xFF;						//Setter alle bits i port b som outputs
	
	ADCSRA |= (1 << ADSC);				//start konvertering, må settes for å starte free running mode

	sei();
	
	
	
	
	while (1)
	{
		

		
	}
	
	
	
	
	
}

