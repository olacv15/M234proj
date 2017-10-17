


#include "InteruptPwm.h"
#include "LinearBlink.h"



InteruptPwm::InteruptPwm()
: interruptTeller_(0)
, tillegg_(1)						// tillegg for teller
, styrke_(1)						// denne må starte på 1 for unngå startproblem med if-setning
, styrkeTeller_(1)					// styrkeverdi tas fra char i LinearBlink
, linearBlink_(200U)				// setter opp tabell med 200 styrker, er et objekt av LinearBlink klassen
{
		
} 


InteruptPwm::~InteruptPwm()
{
	
} 

void InteruptPwm::pwmSignal()
{	
	
		
	if (interruptTeller_ > styrke_)
	{
		ledOFF();
	}
	else
	{
		ledON();
	}
	
	
	interruptTeller_++;
	
	if(interruptTeller_ >= 199)
	{
		
		styrke_ = linearBlink_.hentNivaa(styrkeTeller_);		// henter verdier fra LinearBlink klasse
		
		if (styrkeTeller_ == 199 || styrkeTeller_ == 0)
		{
			tillegg_ = -tillegg_;
		}
		
		styrkeTeller_ = styrkeTeller_ + tillegg_;
		
		interruptTeller_ = 0;
	}
	
	
	
}


void InteruptPwm::ledOFF()
{
	PORTB |= 0b00000001;
}

void InteruptPwm::ledON()
{
	PORTB &=0b11111110;
}