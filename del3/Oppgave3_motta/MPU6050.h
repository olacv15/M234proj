#ifndef MPU6050_h
#define MPU6050_h

#include "Arduino.h"
#include "inttypes.h"
#include<Wire.h>

#define ACCL_Z_H    0x3F  //Rådataene er delt opp i to byte(16 bit) 0x3F og 0x40
#define ACCL_Z_L    0x40
#define PWR_MGMT_1  0x6B  //register 107, power management 1
#define CONFIG 0x1A       //brukes til modifisere dlpf
#define ACCEL_CONFIG 0x1C //aksellerometer konfigurasjon. Velger også modus på høypassfilter i bit 0-2


class MPU6050{
  public:

  MPU6050();

  void init();
  
  float acclZ();
  
  private:
  int16_t acclZ_;
  float actualAcclZ_;
  const uint8_t MPU_addr_ = 0x68;  // I2C address of the MPU-6050
 
};

#endif

