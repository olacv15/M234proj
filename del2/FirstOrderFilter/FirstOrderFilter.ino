#include "FirstOrderFilter.h"
#include <math.h>
#include <arduino.h>


FirstOrderFilter lavpassfilter(10);
FirstOrderFilter lapass2(10);
FirstOrderFilter lapass3(10);
void setup()
{
  Serial.begin(2000000);
}

void loop()
{
  float a = 1;
  float i = 1;

  while(1)
  {
    i = i + 0.01;
    a = a + 0.01;
    
    float x = 3.0f*sin(1000*i);
    float y = 3.0f*sin(75*i);
    float z = 3.0f*sin(50*i);
    
   



    
    Serial.print(lavpassfilter.lavPass(x , a));
    Serial.print(" ");
    Serial.print(lapass2.lavPass(y , a));
    Serial.print(" ");
    Serial.println(lapass3.lavPass(z , a));
    delay(10);
    
  }

}
