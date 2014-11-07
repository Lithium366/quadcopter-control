#include <EEPROM.h>
#include <Firmata.h>
#include <Servo.h>
#include <PinChangeInt.h>
#include <Wire.h>
#include <ADXL345.h>
#include <L3G4200D.h>
#include <HMC5883L.h>
#include <PID_v1.h>
#include <TinyGPS.h>
#include <BMP085NB.h>
#include "config.h"

// Values of RC stick controls
int ThrottleVal, PitchVal, RollVal, YawVal, CH6Val, CH7Val, CH8Val;
boolean CH5Val;

ADXL345 acc; //Accelerometer data
L3G4200D gyro; //Gyro data
HMC5883L compass; //Compass data
TinyGPS gps; //GPS data
BMP085NB bmp; // pressure sensor
      
// Temperature / pressure related vars      
int Temperature = 0;
long Pressure = 0;
float Altitude = 0;
float prevAlts = 0;
float Alts = 0;
float vspeed = 0;
int dtimes = 0;
int counter = 0;

double anglex = 0; //Roll angle in degrees
double angley = 0; //Pitch angle in degrees
double anglez = 0; //Yaw angle in degrees relatively to the North
double vybrox[7];
double vybroy[7];
double vybroz[7];
double errorx[7];
double errory[7];
double errorz[7];
String vybroxsum;
String vybroysum;
String vybrozsum;
int dtime = 0; //Loop time
int loopcount = 0; //Telemetry loop counter
int armcounter = 0;
bool armed = false;

Servo enginex1; //Top left engine
Servo enginex2; //Top right engine
Servo enginex3; //Bottom left engine
Servo enginex4; //Bottom right engine

// GPS data
float flat, flon;
  unsigned long age;

double SetpointX, OutputX, SetpointY, OutputY, SetpointZ, OutputZ, deltaZ, errorZ; // PID setpoints and errors
PID myPIDx(&anglex, &OutputX, &SetpointX, pidXP, pidXI, pidXD, REVERSE); //Pitch PID
PID myPIDy(&angley, &OutputY, &SetpointY, pidYP, pidYI, pidYD, DIRECT); //Roll PID
PID myPIDz(&errorZ, &OutputZ, &SetpointZ, pidZP, pidZI, pidZD, DIRECT); //Yaw PID

void setup() {
  Serial1.begin(57600); //3DR telemetry (always 57600)
  Serial.begin(57600); //USB serial
  Serial2.begin(9600); //init GPS
  acc.begin();
  acc.setRange(ADXL345::RANGE_2G);
  gyro.enableDefault();
  compass = HMC5883L();
  compass.SetScale(1.3);
  compass.SetMeasurementMode(Measurement_Continuous);
  bmp.initialize();
  SetpointX = 0;
  SetpointY = 0;
  SetpointZ = 0; //Must be always 0
  myPIDx.SetMode(AUTOMATIC);
  myPIDx.SetSampleTime(4);
  myPIDx.SetOutputLimits(-200, 200);
  myPIDy.SetMode(AUTOMATIC);
  myPIDy.SetSampleTime(4);
  myPIDy.SetOutputLimits(-200, 200);
  myPIDz.SetMode(AUTOMATIC);
  myPIDz.SetSampleTime(4);
  myPIDz.SetOutputLimits(-200, 200);
  getPid();
  updatePid();
  getLevel();
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
  setMode(); // Listen to a console commands
  readRC(); //Read data from RC
  getAltitude();
  getAngles(); //Read angles from sensors
  armDisarm(); //Arm/disarm
  readGps(); //Read GPS sensor
  calculatePID();
  engineVelocities();
  getvspeed();
  telemetry();
}

void engineVelocities () {
  if (armed) {
    enginex1.writeMicroseconds(minEngineRPM + ThrottleVal - minThrottle - OutputY - OutputX - OutputZ);
    enginex2.writeMicroseconds(minEngineRPM + ThrottleVal - minThrottle + OutputY - OutputX + OutputZ);
    enginex3.writeMicroseconds(minEngineRPM + ThrottleVal - minThrottle - OutputY + OutputX + OutputZ);
    enginex4.writeMicroseconds(minEngineRPM + ThrottleVal - minThrottle + OutputY + OutputX - OutputZ);
  } else {
    enginex1.writeMicroseconds(0);
    enginex2.writeMicroseconds(0);
    enginex3.writeMicroseconds(0);
    enginex4.writeMicroseconds(0);
  }
}

void armDisarm () {
  // Hold throttle and pitch sticks in bottom position for arm_time_ms time
  if (ThrottleVal <= (minThrottle + 100) && PitchVal <= (maxPitch * -1 + 1)) {
    armcounter++;
    if (armcounter * dtime >= arm_time_ms) {
      armcounter = 0;
      if (armed) {
        armed = false;
      } else {
        armed = true;
      }
    }
  } else {
    armcounter = 0;
  }
}

