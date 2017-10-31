#include "FirstOrderFilter.h"
#include <math.h>
#include <arduino.h>


FirstOrderFilter::FirstOrderFilter(double breakFrequency )
  : breakFrequency_(breakFrequency)
{
  //lager tidskonstanten filteret må ha for å ha riktig knekkfrekvens
  timeConstant_ = 1/(2*M_PI*breakFrequency_);

  /*setter startverdier for første gjennomkjøring av lavPass funksjon, første output blir 
    i dette tilfellet "ikke brukbar" data, pga output settes til null og har ingen sammenheng med 
    previousTime som settes her */ 
  previousTime_ = (millis()/(1000.0f));              
  previousOutput_ = 0;
  
}

FirstOrderFilter::~FirstOrderFilter()
{
  
}

// dynamisk filter, endrer vektefaktorene avhengig av tiden mellom gjennomkjøringene
double FirstOrderFilter::lavPass(double input)
{
  //lese ut "nåtiden"
  timE = (millis()/(1000.0f));
  
  //genererer vektefaktorer for filter
  weightFactor1_ = 1/(1 + (timeConstant_/(timE - previousTime_)));
  weightFactor2_ = 1 - weightFactor1_;

  //filter output
  output_ = weightFactor1_*input + weightFactor2_*previousOutput_;

  //"lagrer" nåverende verdier til bruk i nest gjennomkjøring 
  previousOutput_ = output_;
  previousTime_ = timE;

  return output_;
  


  
}

