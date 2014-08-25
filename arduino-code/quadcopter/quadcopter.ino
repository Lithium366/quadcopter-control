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
  
  char debug_rc_values[50];
  char debug_angles_values[20];
  char debug_pid_values[50];
  char debug_system_values[10];
  char debug_final[130];
   
  looptime++;    
  if (looptime >= 10) {
    printRC();
    printAngles();
    printPID();
    printSystem(); 
    looptime = 0;
  }
   
}

void printPID () {
    Serial1.print("pid:");
    Serial1.print(pidXP);
    Serial1.print(":");
    Serial1.print(pidXI);
    Serial1.print(":");
    Serial1.print(pidXD);
    Serial1.print(pidYP);
    Serial1.print(":");
    Serial1.print(pidYI);
    Serial1.print(":");
    Serial1.print(pidYD);
    Serial1.print(pidZP);
    Serial1.print(":");
    Serial1.print(pidZI);
    Serial1.print(":");
    Serial1.println(pidZD);
}

void printAngles() {
    Serial1.print("angles:");
    Serial1.print(anglex);
    Serial1.print(":");
    Serial1.print(angley);
    Serial1.print(":");
    Serial1.println(anglez);
}

void printRC() {
    Serial1.print("reciever:");
    Serial1.print(ThrottleVal);
    Serial1.print(":");
    Serial1.print(PitchVal);
    Serial1.print(":");
    Serial1.print(RollVal);
    Serial1.print(":");
    Serial1.print(YawVal);
    Serial1.print(":");
    Serial1.print(CH5Val);
    Serial1.print(":");
    Serial1.print(CH6Val);
    Serial1.print(":");
    Serial1.print(CH7Val);
    Serial1.print(":");
    Serial1.println(CH8Val);
}

void printSystem() {
  Serial1.print("system:");
  Serial1.println(dtime);
}
