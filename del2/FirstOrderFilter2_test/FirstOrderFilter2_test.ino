#include "FirstOrderFilter.h"
#include <math.h>




FirstOrderFilter lavpassfilter(10);
FirstOrderFilter lapass2(10);
FirstOrderFilter lapass3(10);
void setup()
{
  Serial.begin(2000000);
  
  
}

void loop()
{

  float i = 0;

  while(1)
  {
    
    i = i + 0.6283;           //oppdateres 100 ganger per sek, altså er dette 10Hz eller 62,83rad/s 
    float x = 3*sin(4*i);     //ganger vi mmed 5 eller mere vil vi få alias da vi sampler med 100Hz,
    float y = 3*sin(2*i);     // altså er nyquist frekvensen 50 Hz
    float z = 3*sin(1*i);     //i bodeplot har vi -3db ved denne frekvensen
    
    //Serial.println(millis());
    //Serial.print(" ");



    Serial.print(2.1);
    Serial.print(" ");
    Serial.print(1.34);
    Serial.print(" ");
        
    //Serial.print(lavpassfilter.lavPass(x));
    //Serial.print(" ");
    Serial.print(lapass2.lavPass(y));
    Serial.print(" ");
    Serial.println(lapass3.lavPass(z));
    
    delay(10); //ca 10 milisek mellom rundene 
 
    
  }

}
