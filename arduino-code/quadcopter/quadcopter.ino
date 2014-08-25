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

ADXL345 acc;
L3G4200D gyro;
HMC5883L compass;

double anglex = 0;
double angley = 0;
double anglez = 0;
int dtime = 0;
int looptime = 0;

Servo enginex1;
Servo enginex2;
Servo enginex3;
Servo enginex4;

double SetpointX, OutputX, SetpointY, OutputY;
//PID myPIDx(&anglex, &OutputX, &SetpointX,1.4, 0.05, 0.4, REVERSE);
//PID myPIDy(&angley, &OutputY, &SetpointY,1.4, 0.05, 0.4, DIRECT);
//Roll
PID myPIDx(&anglex, &OutputX, &SetpointX, pidXP, pidXI, pidXD, DIRECT);
//Pitch
PID myPIDy(&angley, &OutputY, &SetpointY, pidYP, pidYI, pidYD, REVERSE);

void setup() {
  Serial1.begin(57600);
  Serial.begin(57600);
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

  if (ThrottleVal > 400) {
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
  
  char debug_rc_values[50];
  char debug_angles_values[20];
  char debug_pid_values[50];
  char debug_system_values[10];
  char debug_final[130];
   
  looptime++;    
  if (looptime == 1) {
    printRC();
  } else if (looptime == 2) {
    printAngles();
  } else if (looptime == 3) {
    printPID();
  } else if (looptime == 4) {
    printSystem(); 
    looptime = 0;
  }
   
}

void printPID () {
    char pidXP_s[10];
    char pidXI_s[10];
    char pidXD_s[10];
    char pidYP_s[10];
    char pidYI_s[10];
    char pidYD_s[10];
    char pidZP_s[10];
    char pidZI_s[10];
    char pidZD_s[10];
    char total[100];
    dtostrf(pidXP, 3, 1, pidXP_s);
    dtostrf(pidXI, 3, 1, pidXI_s);
    dtostrf(pidXD, 3, 1, pidXD_s);
    dtostrf(pidYP, 3, 1, pidYP_s);
    dtostrf(pidYI, 3, 1, pidYI_s);
    dtostrf(pidYD, 3, 1, pidYD_s);
    dtostrf(pidZP, 3, 1, pidZP_s);
    dtostrf(pidZI, 3, 1, pidZI_s);
    dtostrf(pidZD, 3, 1, pidZD_s);
    sprintf(total, "pid:%s:%s:%s:%s:%s:%s:%s:%s:%s\n", pidXP, pidXI, pidXD, pidYP, pidYI, pidYD, pidZP, pidZI, pidZD);
    Serial1.println(total);
}

void printAngles() {
    char anglex_s[10];
    char angley_s[10];
    char anglez_s[10];
    char total[40];
    dtostrf(anglex, 1, 3, anglex_s);
    dtostrf(angley, 1, 3, angley_s);
    dtostrf(anglez, 1, 3, anglez_s);
    sprintf(total, "angles:%s:%s:%s\n", anglex_s, angley_s, anglez_s);
    Serial1.println(total);
}

void printRC() {
    char total[100];
    sprintf(total, "reciever:%d:%d:%d:%d:%d:%d:%d:%d", ThrottleVal, PitchVal, RollVal, YawVal, CH5Val, CH6Val, CH7Val, CH8Val);
    Serial1.println(total);
}

void printSystem() {
  char total[10];
  sprintf(total, "system:%d\n", dtime);
  Serial1.println(total);
}
