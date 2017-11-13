#include "MPU6050.h"
#include "FirstOrderFilter.h"

FirstOrderFilter filter1(10);

MPU6050 mpu1;

IntervalTimer timer1;   //deklarer timer1 en av typen IntervallTimer. 

int Samplerate = 10000; //Sampler signalet fra MPU6050 med rate på 10000 µs (100Hz)

const int led = 13;  // the pin with a LED

int timer = 0;


void signalRead()
  {
    if((filter1.lavPass(mpu1.acclZ())) < 3 ){   //aksellerasjon under 3 m/s^2 er en empirisk verdi som tilsier at sykkelen er veltet.
      
      timer++;

      if (timer > 500 )
      digitalWrite(led, HIGH);
    }

    else{
      timer = 0;
    }
    Serial.println(filter1.lavPass(mpu1.acclZ())); 
  }


  void setup() {
    Serial.begin(9600);
    timer1.begin(signalRead, Samplerate);   //Starter en "interrupt" kalt signalRead og kjører denne med "Samplerate" (100Hz)
    mpu1.init();                            //initialiserer og setter opp kommunikasjon med mpu1
    pinMode(led, OUTPUT);
    digitalWrite(led, LOW);
  }


  void loop() {
    

  }

