#include "MPU6050.h"
#include "FlexCAN.h"
#include "Metro.h"

#ifndef __MK66FX1M0__
#error "Teensy 3.6 with dual CAN bus is required to run this example"
#endif

MPU6050 mpu1;                   //Oppretter MPU6050 "mpu1"


CAN_message_t led_toggle;       //Melding for å toggle led ved oppstart

CAN_message_t led_OnOff;        //Meldign for å styre led av/på

CAN_message_t MPU6050;          //Melding for å lese av verdiene på MPU6050

CAN_message_t inMsg;            



float realAcclZ;

float rawToRealAccl(int16_t raw);

struct ACCL                     //Oppretter struct for serialisering av MPU6050 signal
{
  int16_t acclZ;
};


ACCL mpu6050;                   //variabel mpu6050 av type ACCL

const int toggleId = 0x22;
const int onOffId = 0x21;
const int MPU6050Id = 0x20;

void setup() {
  // put your setup code here, to run once:
  delay(2000);
  Serial.println(F("Hello Teensy 3.6 dual CAN Test."));

  mpu1.init();



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

  MPU6050.ext = 0;
  MPU6050.id = MPU6050Id;
  MPU6050.len = sizeof(mpu6050);

  led_OnOff.ext = 0;
  led_OnOff.id = onOffId;
  led_OnOff.len = 1;



}

void loop() {
  // put your main code here, to run repeatedly:
  

  Can1.read(inMsg);

  switch (inMsg.id) {
    case 0x20:

      IMUread();

      break;

    case 0x21:

      ledOnOff();
      
      break;

    default:

      break;
  }

  
  delay(20);
}


void IMUread()
{
//  Serial.print("faktisk verdi mpu1.acclZ(): ");
//  Serial.println(mpu1.acclZ());
//
//  mpu6050.acclZ = mpu1.acclZ();
//  memcpy(MPU6050.buf, &mpu6050, sizeof(mpu6050));   //Serialiserer MPU6050

  ACCL tmp;

  memcpy(&tmp, inMsg.buf, sizeof(tmp));           //Deserialiserer MPU6050

  Serial.print("Overfort verdi av acclZ (raw_value) : ");
  Serial.println(tmp.acclZ); 

  Serial.println("*********************************");

  Serial.print("acclZ i m/s^2: ");
  Serial.println(rawToRealAccl(tmp.acclZ)); 
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
   realAcclZ = ((float)(raw)*9.81) / 16384.0;
   return realAcclZ;
}

