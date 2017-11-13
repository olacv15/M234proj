#include "MPU6050.h"
#include "FlexCAN.h"
#include "Metro.h"
#include "Servo.h"
#include "FirstOrderFilter.h"

#ifndef __MK66FX1M0__
#error "Teensy 3.6 with dual CAN bus is required to run this example"
#endif

FirstOrderFilter filter1(10);

MPU6050 mpu1;                   

Servo servo1;                   

int servoPin = 6;

double mapp(double x, double in_min, double in_max, double out_min, double out_max);

CAN_message_t MPU6050_raw;          //Melding for å lese av verdiene på MPU6050

CAN_message_t inMsg;



struct ACCL                     //Oppretter struct for deserialisering av MPU6050 signal
{
  double acclZ;
};


struct rawAccl
{
  int16_t rawAcclZ;
};

rawAccl mpu6050;                   //variabel mpu6050 av type ACCL

const int MPU6050RawId = 0x29;

IntervalTimer timer1;

void ISR()
{
  mpu6050.rawAcclZ = filter1.lavPass(mpu1.rawAcclZ());
  memcpy(MPU6050_raw.buf, &mpu6050, sizeof(mpu6050));   //Serialiserer MPU6050

  Can1.write(MPU6050_raw);
}



void setup() {
  // put your setup code here, to run once:
  delay(2000);
  Serial.println(F("Hello Teensy 3.6 dual CAN Test."));

  timer1.begin(ISR, 10000);

  mpu1.init();

  servo1.attach(servoPin);



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

  MPU6050_raw.ext = 0;
  MPU6050_raw.id = MPU6050RawId;
  MPU6050_raw.len = sizeof(mpu6050);
}

void loop() {
  // put your main code here, to run repeatedly:
  

  Can1.read(inMsg);

  switch (inMsg.id) {
    case 0x30:

      servoActuation();

      break;

    default:

      break;
  }

  
}


void servoActuation()
{
  ACCL tmp;

  memcpy(&tmp, inMsg.buf, sizeof(tmp));           //Deserialiserer MPU6050

  double mapAcclZ = mapp(tmp.acclZ, -9.81, 9.81, 0, 180);

  servo1.write(static_cast<int>(mapAcclZ));
}


double mapp(double x, double in_min, double in_max, double out_min, double out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
