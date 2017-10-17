/*
 * ADC1.cpp
 *
 * Created: 17.10.2017 12.02.29
 * Author : Jørgen Benum
 */ 

#include <avr/io.h>


int main(void)
{
    PRADC = 0;				//power recudction bit : enables ADC, ligger i PRR register, må settes til 1 før shut down?
	
	ADMUX = 0b01000001;		// første to bit: bruker AVCC som referanse, men krever ekstern kondensator at AREF pin for å redusere støy
							// siste fire bit velger ADC1 altså  PC1
			
			
							
}

