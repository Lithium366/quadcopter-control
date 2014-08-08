#include <EEPROM.h>
#include <Servo.h>
#include <PinChangeInt.h>
#include <Wire.h>
#include <ADXL345.h>
#include <L3G4200D.h>
#include <HMC5883L.h>
#include <PID_v1.h>
#include "config.h"

int ThrottleVal, PitchVal, RollVal, YawVal, CH6Val, CH7Val, CH8Val;
boolean CH5Val;
char debug_rc_values[100];

ADXL345 acc;
L3G4200D gyro;
HMC5883L compass;

double anglex = 0;
double angley = 0;
double anglez = 0;
int dtime = 0;

Servo enginex1;
Servo enginex2;
Servo enginex3;
Servo enginex4;

double SetpointX, OutputX, SetpointY, OutputY;
//PID myPIDx(&anglex, &OutputX, &SetpointX,1.4, 0.05, 0.4, REVERSE);
//PID myPIDy(&angley, &OutputY, &SetpointY,1.4, 0.05, 0.4, DIRECT);
PID myPIDx(&anglex, &OutputX, &SetpointX, pidXP, pidXI, pidXD, REVERSE);
PID myPIDy(&angley, &OutputY, &SetpointY, pidYP, pidYI, pidYD, DIRECT);

void setup() {
  Serial1.begin(57600);
  acc.begin();
  gyro.enableDefault();
  compass = HMC5883L();
  compass.SetScale(1.3);
  compass.SetMeasurementMode(Measurement_Continuous);
  SetpointX = 0;
  SetpointY = 0;
  myPIDx.SetMode(AUTOMATIC);
  myPIDx.SetOutputLimits(-100, 100);
  myPIDy.SetMode(AUTOMATIC);
  myPIDy.SetOutputLimits(-100, 100);
  enginex1.attach(ENGINE1);
  enginex1.writeMicroseconds(0);
  enginex2.attach(ENGINE2);
  enginex2.writeMicroseconds(0);
  enginex3.attach(ENGINE3);
  enginex3.writeMicroseconds(0);
  enginex4.attach(ENGINE4);
  enginex4.writeMicroseconds(0);
  delay(1000);
}

void loop() {
  readRC();
  getAngles();
 
  SetpointX = RollVal;
  SetpointY = PitchVal;

  if (ThrottleVal > 500) {
    myPIDx.Compute(); 
    myPIDy.Compute();
    enginex1.writeMicroseconds(ThrottleVal - OutputY - OutputX - YawVal);
    enginex2.writeMicroseconds(ThrottleVal - OutputY + OutputX + YawVal);
    enginex3.writeMicroseconds(ThrottleVal + OutputY - OutputX + YawVal);
    enginex4.writeMicroseconds(ThrottleVal + OutputY + OutputX - YawVal);
  } else {
    enginex1.writeMicroseconds(0);
    enginex2.writeMicroseconds(0);
    enginex3.writeMicroseconds(0);
    enginex4.writeMicroseconds(0);
  }
        
  sprintf(debug_rc_values, "reciever:%d:%d:%d:%d:%d:%d:%d:%d", ThrottleVal, PitchVal, RollVal, YawVal, CH5Val, CH6Val, CH7Val, CH8Val);  
  Serial1.println(debug_rc_values);
  
  sprintf(debug_rc_values, "angles:%d:%d:%d", anglex, angley, anglez);  
  Serial1.println(debug_rc_values);
  
  sprintf(debug_rc_values, "pid:%d:%d:%d:%d:%d:%d:%d:%d:%d", pidXP, pidXI, pidXD, pidYP, pidYI, pidYD, pidXP, pidZI, pidZD);  
  Serial1.println(debug_rc_values);
  
  sprintf(debug_rc_values, "system:%d", dtime);  
  Serial1.println(debug_rc_values);
   
}
