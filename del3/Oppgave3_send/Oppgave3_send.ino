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




struct ACCL                     //Oppretter struct for serialisering av MPU6050 signal
{
  int16_t rawAcclZ;
  double acclZ;
};


ACCL mpu6050;                   //variabel mpu6050 av type ACCL

const int toggleId = 0x22;
const int onOffId = 0x21;
const int MPU6050Id = 0x20;

IntervalTimer timer1;


void ISR()
{
  Serial.print("faktisk verdi mpu1.acclZ(): ");
  Serial.println(mpu1.rawAcclZ());

  mpu6050.acclZ = mpu1.rawAcclZ();
  memcpy(MPU6050.buf, &mpu6050, sizeof(mpu6050));   //Serialiserer MPU6050

  Can1.write(MPU6050);
}




void setup() {
  // put your setup code here, to run once:
  delay(2000);
  Serial.println(F("Hello Teensy 3.6 dual CAN Test."));

  timer1.begin(ISR, 1000000);

  mpu1.init();



  struct CAN_filter_t defaultMask;     // defaultMask filter med egenskaper tilsvarende default i FlexCAN.h
  defaultMask.flags.remote = 0;
  defaultMask.flags.extended = 0;
  defaultMask.id = 0;

  Can0.begin(250000, defaultMask, true, true);  //alternativ pin-tilkobling av Can0

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
  led_OnOff.buf[0] = 1;



}

void loop() {
  // put your main code here, to run repeatedly:

  Can1.write(led_OnOff);

  led_OnOff.buf[0] = 1;

  delay(500);

delay(20);
}



