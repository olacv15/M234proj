#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h>


void setup();
void ledOFF(void);
void ledON(void);

	
uint16_t resultat;							//ADC resultat er 10 bit, 
uint16_t InterruptTeller;					//Bruker 16 bit for å unngå varsel for sammenligning av 8 og 16bit verdi
uint16_t mappet;

// tabell med lineariserte lysverdier  
const uint16_t tabell[200] = {0,0,0,0,0,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,3,3,3,3,3,3,3,4,4,4,4,4,5,5,5,5,6,6,6,6,7,7,7,7,8,8,8,9,9,9,10,10,10,11,11,11,12,12,13,13,13,14,14,15,15,16,16,17,17,18,18,19,19,20,20,21,21,22,23,23,24,25,25,26,26,27,28,29,29,30,31,31,32,33,34,35,35,36,37,38,39,40,41,41,42,43,44,45,46,47,48,49,50,51,52,53,54,56,57,58,59,60,61,62,64,65,66,67,69,70,71,72,74,75,76,78,79,81,82,84,85,86,88,89,91,93,94,96,97,99,101,102,104,106,107,109,111,112,114,116,118,120,122,123,125,127,129,131,133,135,137,139,141,143,145,147,149,152,154,156,158,160,163,165,167,170,172,174,177,179,181,184,186,189,191,194,196,199};


ISR(TIMER1_COMPA_vect)
{	
	ADCSRA |= (1 << ADSC);					//start konvertering
	
	while (ADCSRA & (1<<ADSC))
	{
											//vent på at konvertering er ferdig
	}
	
	resultat = ADC;							//resultat av kovertering
											//brukte denne tidligere : resultat = (ADCH<<8) + ADCL; da leser programmet kun en 
											//gang, altså resultat låses selv om vi får gjennom interrupt	
	mappet = round(resultat/5.145);			//endrer fra 1024 trinn til 200 trinn, makverdi for mappet er dermed 199											

	if (InterruptTeller >= tabell[mappet])	//sammenligninger med tabellverdiene
	{
		ledOFF();
	}
	else
	{
		ledON();
	}
	
	InterruptTeller++;
	
	if(InterruptTeller >= 199)
	{	
		InterruptTeller = 0;				//starter teller på nytt
	}
	


}
	
		
int main(void)
{
	setup();
	sei();
	
		while (1)
		{
		
		}

	}
	

void setup()
{
	//Oppgsett av interrupt ved faste tidspunkt
	
	DDRB = 0xFF;											//Setter alle bits i port b som outputs
	
	TCCR1A = 0;
	TCCR1B = 0;
	TCCR1B |= (1 << WGM12) | (1 << CS10);					//Prescaler 1, CTC mode
	TIMSK1 |= (1 << OCIE1A);								//Output compare A match interrupt enable

	OCR1A = 199;											//Timer1 teller fra 0-199. Med prescaler på 1:1 gir dette oss en pwm frekvens på 200Hz, oppløsning på 200.
	
	//Oppsett av ADC

	ADMUX = 0b01000001;										// første to bit: bruker AVCC som referanse, men krever ekstern kondensator på AREF pin for å redusere støy
															// siste fire bit velger ADC1 altså  PC1
							
	ADCSRA |= (1 << ADEN);									// enabler ADC, prescaler blir da 1: inputklokke har samme hastighet som systemklokke.  
}

	
void ledOFF(void)
{
	PORTB |= 0b00000001;
}


void ledON(void)
{
	PORTB &=0b11111110;
}

