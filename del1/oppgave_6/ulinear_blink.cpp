/*
 * softblink2.cpp
 *
 * Created: 10.10.2017 10.36.07
 * Author : Jørgen Benum
 */ 


#define F_CPU 1000000UL  // 1 MHz
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

void ledON(void);
void ledOFF(void);

	
uint8_t InterruptTeller = 0;
uint8_t tillegg = 1;
uint8_t styrke = 0;

ISR(TIMER1_COMPA_vect)
{	
	if (InterruptTeller >= styrke)
	{
		ledOFF();
	}
	else
	{
		ledON();
	}
		
	
	InterruptTeller++;
	
	if(InterruptTeller >= 200)		//Legger på 1 lysstyrke for hver gang interrupten har trigget 200 ganger
	{
			styrke = styrke + tillegg;
			
			if (styrke == 200 || styrke == 0)	//Inverterer "tillegg" hver gang lysstyrken når endepunktene
			{
				tillegg = -tillegg;		
			}
		
		InterruptTeller = 0;
	}
	
	
}
	
	
	
int main(void)
{
	
	
	DDRB = 0xFF;							//Setter alle bits i port b som outputs
	
	TCCR1A = 0;								//Forsikrer oss om at alle bits i TCCR1A = 0
	TCCR1B = 0;								//Forsikrer oss om at alle bits i TCCR1B = 0
	TCCR1B |= (1 << WGM12) | (1 << CS10);	//CTC mode, Prescaler 1:1
	TIMSK1 |= (1 << OCIE1A);				//Output compare A match interrupt enable

	OCR1A = 199;							//Timer1 teller fra 0-199. => cycle = 200Hz ved 200 lysstyrker
	sei();




	
		while (1)
		{
			
		}

	}
	
	
void ledON(void)
{
	PORTB |= 0b00000001;
}

void ledOFF(void)
{
	PORTB &=0b11111110;
}

