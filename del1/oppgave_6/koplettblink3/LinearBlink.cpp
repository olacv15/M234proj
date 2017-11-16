/* 
* LinearBlink.cpp
*
* Created: 12.10.2017 09.09.44
* Author: Jørgen Benum
*/


#include "LinearBlink.h"
#include "math.h"

LinearBlink::LinearBlink(unsigned int antallVerdier)
: antallVerdier_ (antallVerdier-1)									//koorigerer med -1 for å tilpasse index til array størrelse
{

	iter_ = 0;
	
	for ( unsigned int i = 0; i <= antallVerdier_; ++i)				// fyller tabell/array med lysverdier
	{
		y_=(iter_/antallVerdier_)*100;								// fordeler interval for y
					
		if( y_ <= 8 )												// Den inverse funksjonen av lysoppfatning
		{
			funksjonsVerdi_ = y_/903.3;
		}
		else
		{
			funksjonsVerdi_ = pow(((y_+16.0)/116.0),3);
		}


		lysVerdier [i] = round(funksjonsVerdi_*antallVerdier_);		//runder av verdi for å få heltall


		iter_ = 1.0 + iter_;										// Inkrementerer teller for hver runde, bruker +1.0 i stedet for ++iter for å unngå varsel


	}

}


LinearBlink::~LinearBlink()
{

}


unsigned int LinearBlink::hentNivaa(unsigned int tabellNummer)		// Funksjon for å hente ut ønsket verdi i tabell
{
	return lysVerdier[tabellNummer];

}
