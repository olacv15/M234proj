#include "Arduino.h"
#include "MPU6050.h"
#include<Wire.h>

MPU6050::MPU6050(){
 
}

void MPU6050::init()
{
  Wire.begin();
  Wire.beginTransmission(MPU_addr_);
  Wire.write(PWR_MGMT_1);  // PWR_MGMT_1 registeret
  Wire.write(0);     // setter clock source til 8 mhz og vekker til live MPU(6050) når den settes til 0
  Wire.endTransmission(true);
  
}

float MPU6050::acclZ()
{
  Wire.beginTransmission(MPU_addr_);
  Wire.write(ACCL_Z_H);  // Starter med å hente høy-bitene til z-aksellerasjonen
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr_,2,true);  // ,2, indikerer at den henter både 0x3F og 0x40
  rawAcclZ_ = Wire.read() << 8;
  rawAcclZ_ |= Wire.read();
  actualAcclZ_ = ((float)(rawAcclZ_)*9.81) / 16384.0;
  return actualAcclZ_;
}

int16_t MPU6050::rawAcclZ()
{
   Wire.beginTransmission(MPU_addr_);
  Wire.write(ACCL_Z_H);  // Starter med å hente høy-bitene til z-aksellerasjonen
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr_,2,true);  // ,2, indikerer at den henter både 0x3F og 0x40
  rawAcclZ_ = Wire.read() << 8;
  rawAcclZ_ |= Wire.read();
  return rawAcclZ_;
}

