/*
Legg merke til at denne kun fungerer i sammarbeid med LinearBlikk (må konstrueres først) og følgende instillinger

	************************************************ SETUP ********************************
	sei();
	DDRB = 0xFF;		//Setter alle bits i port b som outputs
	
	
	
	TCCR1A = 0;
	TCCR1B = 0;
	TCCR1B |= (1 << WGM12) | (1 << CS10);	//Prescaler 1, CTC mode
	TIMSK1 |= (1 << OCIE1A); //Output compare A match interrupt enable

	
	OCR1A = 399;	//Timer1 teller fra 0-99. Med prescaler på 1:1 gir dette oss en pwm frekvens på 100Hz, oppløsning på 100.
	
	********************************************** SETUP ***************************************
	
	
*/	

#ifndef __INTERUPTPWM_H__
#define __INTERUPTPWM_H__

#include <avr/io.h>
#include <avr/interrupt.h>

#include "LinearBlink.h"

class InteruptPwm
{
	public:
	InteruptPwm();
	~InteruptPwm();
	
	void pwmSignal();					// funksjon som blinker mykt med leden
	
	void ledON();
	void ledOFF();

	private:
	unsigned int interruptTeller_;
	unsigned int tillegg_;				// tillegg for teller
	unsigned int styrke_;				// denne må starte på 1 for unngå startproblem med if-setning
	unsigned int styrkeTeller_;			// styrkeverdi tas fra char i LinearBlink
	LinearBlink linearBlink_;
	



}; 

#endif //__INTERUPTPWM_H__
