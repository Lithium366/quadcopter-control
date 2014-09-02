#include <EEPROM.h>
#include <Firmata.h>
#include <Servo.h>
#include <PinChangeInt.h>
#include <Wire.h>
#include <ADXL345.h>
#include <L3G4200D.h>
#include <HMC5883L.h>
#include <PID_v1.h>
#include "config.h"
#include <TinyGPS.h>

int ThrottleVal, PitchVal, RollVal, YawVal, CH6Val, CH7Val, CH8Val;
boolean CH5Val;

ADXL345 acc; //Accelerometer data
L3G4200D gyro; //Gyro data
HMC5883L compass; //Compass data
TinyGPS gps;

double anglex = 0; //Roll
double angley = 0; //Pitch
double anglez = 0; //Yaw
int dtime = 0; //Loop time
int loopcount = 0; //For

Servo enginex1; //Top left
Servo enginex2; //Top right
Servo enginex3; //Bottom left
Servo enginex4; //Bottom right

// GPS data
float flat, flon;
unsigned long age;

double SetpointX, OutputX, SetpointY, OutputY, SetpointZ, OutputZ, deltaZ, errorZ;
//Roll PID
PID myPIDx(&anglex, &OutputX, &SetpointX, pidXP, pidXI, pidXD, REVERSE);
//Pitch PID
PID myPIDy(&angley, &OutputY, &SetpointY, pidYP, pidYI, pidYD, DIRECT);
//Yaw PID
PID myPIDz(&errorZ, &OutputZ, &SetpointZ, pidZP, pidZI, pidZD, DIRECT);

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
  SetpointZ = 0;
  myPIDx.SetMode(AUTOMATIC);
  myPIDx.SetSampleTime(6);
  myPIDx.SetOutputLimits(-200, 200);
  myPIDy.SetMode(AUTOMATIC);
  myPIDy.SetSampleTime(6);
  myPIDy.SetOutputLimits(-200, 200);
  myPIDz.SetMode(AUTOMATIC);
  myPIDz.SetSampleTime(6);
  myPIDz.SetOutputLimits(-200, 200);
  enginex1.attach(ENGINE1);
  enginex1.writeMicroseconds(0);
  enginex2.attach(ENGINE2);
  enginex2.writeMicroseconds(0);
  enginex3.attach(ENGINE3);
  enginex3.writeMicroseconds(0);
  enginex4.attach(ENGINE4);
  enginex4.writeMicroseconds(0);
  initGps();
  delay(1000);
}

void loop() {
  readRC();
  getAngles();
  //readGps();
  SetpointX = RollVal;
    SetpointY = PitchVal;    
    computeErrorZ();

  if (ThrottleVal > 400) {

    myPIDx.Compute(); 
    myPIDy.Compute();
    myPIDz.Compute();
    enginex1.writeMicroseconds(ThrottleVal - OutputY - OutputX - OutputZ);
    enginex2.writeMicroseconds(ThrottleVal - OutputY + OutputX + OutputZ);
    enginex3.writeMicroseconds(ThrottleVal + OutputY - OutputX + OutputZ);
    enginex4.writeMicroseconds(ThrottleVal + OutputY + OutputX - OutputZ);
  } else {
    enginex1.writeMicroseconds(0);
    enginex2.writeMicroseconds(0);
    enginex3.writeMicroseconds(0);
    enginex4.writeMicroseconds(0);
  }
//Serial.println(anglex);
  //Debug info
  loopcount++;
  if (loopcount == 1) {
    printRC();
  } else if (loopcount == 4) {
    printAngles();
  } else if (loopcount == 7) {
    printPID();
  } else if (loopcount == 10) {
    printSystem();
  } else if (loopcount == 13) {
    Serial1.println("devider");
  } else if (loopcount == 16) {
    loopcount = 0;
  }
}

void computeErrorZ () {
  if (!deltaZ) {
    deltaZ = anglez;
  }    
  if(abs(YawVal) > 5) {
    deltaZ = anglez + YawVal;
  }
  errorZ = anglez - deltaZ;
  if(errorZ > 180) {
    errorZ -= 360;
  } else if (errorZ < -180) {
    errorZ += 360;
  }
  // if wind will rotate to 180/-180 - Quad will crash  
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
    sprintf(total, "pid:%s:%s:%s:%s:%s:%s:%s:%s:%s", pidXP_s, pidXI_s, pidXD_s, pidYP_s, pidYI_s, pidYD_s, pidZP_s, pidZI_s, pidZD_s);
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
    sprintf(total, "angles:%s:%s:%s", anglex_s, angley_s, anglez_s);
    Serial1.println(total);
}

void printRC() {
    char total[100];
    sprintf(total, "reciever:%d:%d:%d:%d:%d:%d:%d:%d", ThrottleVal, PitchVal, RollVal, YawVal, CH5Val, CH6Val, CH7Val, CH8Val);
    Serial1.println(total);
}

void printSystem() {
  char total[10];
  sprintf(total, "system:%d", dtime);
  Serial1.println(total);
}
