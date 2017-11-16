#include "FlexCAN.h"
#include "Metro.h"

#ifndef __MK66FX1M0__
#error "Teensy 3.6 with dual CAN bus is required to run this example"
#endif

CAN_message_t inMsg;            //inngående can melding       


float rawToRealAccl(int16_t raw);

struct accl                     //Oppretter struct for deserialisering av MPU6050 signal
{
  int16_t rawAcclZ;
  double acclZ;
};


const uint32_t toggleId = 0x21;
const uint32_t onOffId = 0x22;
const uint32_t MPU6050Id = 0x20;


void setup() {
  // put your setup code here, to run once:
  delay(2000);
  Serial.println(F("Hello Teensy 3.6 dual CAN Test."));


  struct CAN_filter_t defaultMask;     // defaultMask filter med egenskaper tilsvarende default i FlexCAN.h
  defaultMask.flags.remote = 0;
  defaultMask.flags.extended = 0;
  defaultMask.id = 0;

  Can0.begin(250000, defaultMask, false, false);  //alternativ pin-tilkobling av Can0

  Can1.begin(250000, defaultMask, false, false);


  //if using enable pins on a transceiver they need to be set on
  pinMode(28, OUTPUT);
  pinMode(35, OUTPUT);
  pinMode(13, OUTPUT);

  digitalWrite(28, LOW);    //NB!!!! LOW == High speed mode
  digitalWrite(35, LOW);
  digitalWrite(13, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  

  Can1.read(inMsg);

  switch (inMsg.id) {
    case MPU6050Id:       //0x20

      IMUread();

      break;

    case onOffId:         //0x22

      ledOnOff();
      
      break;

    default:

      break;
  }

  
  delay(10);
}


void IMUread()
{
  accl mpu6050;

  memcpy(&mpu6050, inMsg.buf, sizeof(mpu6050));           //Deserialiserer MPU6050

  Serial.print("Overfort verdi av acclZ (raw_value) : ");
  Serial.println(mpu6050.rawAcclZ); 

  Serial.println("*********************************");

  Serial.print("acclZ i m/s^2: ");
  Serial.println(rawToRealAccl(mpu6050.acclZ));
}


void ledOnOff()
{
  if (inMsg.buf[0] == 0)
  {
    digitalWrite(13, LOW); 
  }

  else if (inMsg.buf[0] == 1)
  {
    digitalWrite(13, HIGH);
  }
}


float rawToRealAccl(int16_t raw)
{
   float realAcclZ;
   realAcclZ = ((float)(raw)*9.81) / 16384.0;
   return realAcclZ;
}

