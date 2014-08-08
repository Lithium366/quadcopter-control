#include <Servo.h>
#include <Wire.h>
#include <ADXL345.h>
#include <L3G4200D.h>
#include <HMC5883L.h>
#include <PID_v1.h>
#include <PinChangeInt.h>

const float alpha = 0.5;

double fXg = 0;
double fYg = 0;
double fZg = 0;

double anglex = 0;
double angley = 0;
double anglez = 0;
int dtime = 0;
long ptime = 0;

#define THROTTLE_IN_PIN 11
#define ROLL_IN_PIN 8
#define PITCH_IN_PIN 9
#define YAW_IN_PIN 10

uint32_t ulThrottleStart;
uint32_t ulRollStart;
uint32_t ulPitchStart;
uint32_t ulYawStart;
volatile uint16_t unThrottleInShared;
volatile uint16_t unRollInShared;
volatile uint16_t unPitchInShared;
volatile uint16_t unYawInShared;

int ThrottleVal, PitchVal, RollVal, YawVal; 

Servo enginex1;
Servo enginex2;
Servo enginex3;
Servo enginex4;

ADXL345 acc;
L3G4200D gyro;
HMC5883L compass;

double SetpointX, OutputX, SetpointY, OutputY;
PID myPIDx(&anglex, &OutputX, &SetpointX,1.4, 0.05, 0.4, REVERSE);
PID myPIDy(&angley, &OutputY, &SetpointY,1.4, 0.05, 0.4, DIRECT);
//PID myPIDx(&anglex, &OutputX, &SetpointX,0.8, 0.02, 0.6, REVERSE);
//PID myPIDy(&angley, &OutputY, &SetpointY,0.8, 0.02, 0.6, DIRECT);

void setup()
{
	acc.begin();
        gyro.enableDefault();
        compass = HMC5883L();
        compass.SetScale(1.3);
        compass.SetMeasurementMode(Measurement_Continuous);
	Serial.begin(9600);
        /*SetpointX = 0;
        SetpointY = 0;
        myPIDx.SetMode(AUTOMATIC);
        myPIDx.SetOutputLimits(-50, 50);
        myPIDy.SetMode(AUTOMATIC);
        myPIDy.SetOutputLimits(-200, 200);
        enginex1.attach(12);
        enginex1.writeMicroseconds(0);
        enginex2.attach(6);
        enginex2.writeMicroseconds(0);
        enginex3.attach(4);
        enginex3.writeMicroseconds(0);
        enginex4.attach(5);
        enginex4.writeMicroseconds(0);*/
        /*PCintPort::attachInterrupt(THROTTLE_IN_PIN, calcThrottle,CHANGE);
        PCintPort::attachInterrupt(ROLL_IN_PIN, calcRoll,CHANGE);
        PCintPort::attachInterrupt(PITCH_IN_PIN, calcPitch,CHANGE);
        PCintPort::attachInterrupt(YAW_IN_PIN, calcYaw,CHANGE);*/
	delay(1000);
}

float getCompassData()
{
  MagnetometerRaw raw = compass.ReadRawAxis();
  MagnetometerScaled scaled = compass.ReadScaledAxis();
  int MilliGauss_OnThe_XAxis = scaled.XAxis;
  float heading = atan2(scaled.YAxis, scaled.XAxis);
  float declinationAngle = 0.227;
  heading += declinationAngle;
  
  if(heading < 0)
    heading += 2*PI;
    
  if(heading > 2*PI)
    heading -= 2*PI;
    
  float headingDegrees = heading * 180/M_PI;
  return headingDegrees;
}

void getAngles()
{
  //double pitch, roll, Xg, Yg, Zg;
  //acc.read(&Xg, &Yg, &Zg);
  gyro.read();
  /*fXg = Xg * alpha + (fXg * (1.0 - alpha));
  fYg = Yg * alpha + (fYg * (1.0 - alpha));
  fZg = Zg * alpha + (fZg * (1.0 - alpha));
  roll  = (atan2(-fYg, fZg)*180.0)/M_PI ;
  pitch = (atan2(fXg, sqrt(fYg*fYg + fZg*fZg))*180.0)/M_PI - 3.5;
  
  dtime = millis() - ptime;
  if (dtime < 0) {
    dtime = 10;
  }
  ptime = millis();*/
Serial.println(gyro.g.x);
  /*anglex = 0.98 * (anglex + gyro.g.x * 0.00875 * dtime / 1000 * -1) + 0.02 * roll;
  angley = 0.98 * (angley + gyro.g.y * 0.00875 * dtime / 1000 * -1) + 0.02 * pitch;
  anglez = getCompassData();*/
  
}

void loop()
{
        getAngles(); 
        /*ThrottleVal = map(unThrottleInShared, 1058, 1860, 300, 2300);
        if (ThrottleVal < 0) {
          ThrottleVal = 0;
        }
        RollVal = map(unRollInShared, 1058, 1860, -30, 30);
        PitchVal = map(unPitchInShared, 1058, 1860, -30, 30);
        YawVal = map(unYawInShared, 1058, 1860, -50, 50); 
  
        SetpointX = RollVal;
        SetpointY = PitchVal;
        
	getAngles();        
 
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
        }*/
      
        //Serial.println(unYawInShared);
}

void calcThrottle()
{
  if(digitalRead(THROTTLE_IN_PIN) == HIGH) { 
    ulThrottleStart = micros();
  } else {
    unThrottleInShared = (uint16_t)(micros() - ulThrottleStart);
  }
}

void calcRoll()
{
  if(digitalRead(ROLL_IN_PIN) == HIGH) { 
    ulRollStart = micros();
  } else {
    unRollInShared = (uint16_t)(micros() - ulRollStart);
  }
}

void calcPitch()
{
  if(digitalRead(PITCH_IN_PIN) == HIGH) { 
    ulPitchStart = micros();
  } else {
    unPitchInShared = (uint16_t)(micros() - ulPitchStart);
  }
}

void calcYaw()
{
  if(digitalRead(YAW_IN_PIN) == HIGH) { 
    ulYawStart = micros();
  } else {
    unYawInShared = (uint16_t)(micros() - ulYawStart);
  }
}
