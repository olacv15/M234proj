/* 
* LinearBlink.h
*
* Created: 12.10.2017 09.09.44
* Author: Jørgen Benum
*/


#ifndef __LINEARBLINK_H__
#define __LINEARBLINK_H__




class LinearBlink
{
	public:
	
	LinearBlink(unsigned int antallVerdier);            // antall lysnivåer

	~LinearBlink();										// destruktor

	unsigned int hentNivaa(unsigned int tabellNummer);	// Funksjon for å hente ut en lysstyrke fra lysstyrke-tabell som genereres av konstruktoren


	private:

	unsigned int antallVerdier_;						 // antall verdier man vil ha i tabell
	double y_;                                           // reel lysstyrke fra 0 til 100 ( 100 = full lysstryke)

	unsigned int lysVerdier [200];                       // NB MÅ SETTES MANUELT: tabell med resultat av invers oppfatet lys (tabell med lysstryker som gir linært utsende)

	double iter_;										 //iterasjonsvariabel, itererer parallelt med i
	double funksjonsVerdi_;                              //utregningsvariabel



}; 

#endif //__LINEARBLINK_H__
