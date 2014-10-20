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
#include <BMP085NB.h>

// Values of RC stick controls
int ThrottleVal, PitchVal, RollVal, YawVal, CH6Val, CH7Val, CH8Val;
boolean CH5Val;

ADXL345 acc; //Accelerometer data
L3G4200D gyro; //Gyro data
HMC5883L compass; //Compass data
TinyGPS gps; //GPS data
BMP085NB bmp;
      
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
PID myPIDx(&anglex, &OutputX, &SetpointX, pidXP, pidXI, pidXD, REVERSE); //Roll PID
PID myPIDy(&angley, &OutputY, &SetpointY, pidYP, pidYI, pidYD, DIRECT); //Pitch PID
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
  myPIDx.SetSampleTime(6);
  myPIDx.SetOutputLimits(-200, 200);
  myPIDy.SetMode(AUTOMATIC);
  myPIDy.SetSampleTime(6);
  myPIDy.SetOutputLimits(-200, 200);
  myPIDz.SetMode(AUTOMATIC);
  myPIDz.SetSampleTime(6);
  myPIDz.SetOutputLimits(-200, 200);
  getPid();
  updatePid();
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

void getvspeed () {
  dtimes += dtime;
  if (Altitude != prevAlts) {
    counter++;
    if (counter <= 50) {
      prevAlts += Altitude;
    } else if (counter > 50) {
      Alts +=Altitude;
      if (counter == 100) {
        vspeed = ((Alts - prevAlts) / 50) * 1000 / dtimes;
        prevAlts = 0;
        Alts = 0;
        counter = 0;
        dtimes = 0;
      }
    } 
  }
}

void getAltitude () {
  bmp.pollData(&Temperature, &Pressure, &Altitude);  
}

void calculatePID () {
  if (armed) {
    SetpointX = RollVal;
    SetpointY = PitchVal;
    computeErrorZ();
    myPIDx.Compute();
    myPIDy.Compute();
    myPIDz.Compute();
  }
  errorx[loopcount] = SetpointX - anglex;
  errory[loopcount] = SetpointY - angley;
  errorz[loopcount] = SetpointZ - errorZ;
}

void engineVelocities () {
  if (armed) {
    enginex1.writeMicroseconds(minEngineRPM + ThrottleVal - minThrottle - OutputY - OutputX - OutputZ);
    enginex2.writeMicroseconds(minEngineRPM + ThrottleVal - minThrottle - OutputY + OutputX + OutputZ);
    enginex3.writeMicroseconds(minEngineRPM + ThrottleVal - minThrottle + OutputY - OutputX + OutputZ);
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

void setPid(String pidr) {
  int sepPosition;
  long pids[10];
  int cnt = 0;
  long chksumm = 0;
  int startingAddress = 1;
  do {
    sepPosition = pidr.indexOf(':');
    int wd;
    if(sepPosition != -1) {
        pids[cnt] = pidr.substring(0, sepPosition).toInt();
        chksumm += pids[cnt];
        pidr = pidr.substring(sepPosition + 1, pidr.length());
        cnt++;
    } else {
        pids[cnt] = pidr.toInt();
    }
  } while(sepPosition >= 0);
  if (chksumm == pids[9]) {
    for (int i = 0; i < 9; i++) {
      EEPROMWritelong(startingAddress + 4*i, pids[i]);
    }
    Serial1.println("pidsaved");
    getPid();
    updatePid();
  } 
}

void updatePid() {
  myPIDx.SetTunings(pidXP, pidXI, pidXD);
  myPIDy.SetTunings(pidYP, pidYI, pidYD);
  myPIDz.SetTunings(pidZP, pidZI, pidZD);
}

void getPid () {
  int startingAddress = 1;
  pidXP = (float) EEPROMReadlong(startingAddress + 4*startingAddress*0) / 1000;
  pidXI = (float) EEPROMReadlong(startingAddress + 4*startingAddress*1) / 1000;
  pidXD = (float) EEPROMReadlong(startingAddress + 4*startingAddress*2) / 1000;
  pidYP = (float) EEPROMReadlong(startingAddress + 4*startingAddress*3) / 1000;
  pidYI = (float) EEPROMReadlong(startingAddress + 4*startingAddress*4) / 1000;
  pidYD = (float) EEPROMReadlong(startingAddress + 4*startingAddress*5) / 1000;
  pidZP = (float) EEPROMReadlong(startingAddress + 4*startingAddress*6) / 1000;
  pidZI = (float) EEPROMReadlong(startingAddress + 4*startingAddress*7) / 1000;
  pidZD = (float) EEPROMReadlong(startingAddress + 4*startingAddress*8) / 1000;
}

void setMode () {
  unsigned long tmr = millis();
  String mode;
  if (Serial1.available() > 0) {
    while (Serial1.available())    
      mode += (char)Serial1.read();
  }  
  
  long pres = 0;
  String pidr = ""; 
  
  switch (mode.charAt(0)) {
    case 'f':
      telemetry_mode = 1;
      break;
    case 'p': // Send PID and errors
      telemetry_mode = 2;
      break;
    case 'v': // Send raw accelerometer data
      telemetry_mode = 3;
      break; 
    case 't':
      telemetry_mode = 4;
      break;
    case 'l':
      setLevel();
      break;
    case 's':
      pidr = mode.substring(1, mode.length());
      setPid(pidr);
      break;
    case 'a': // Arm/Disarm from a console
      if (ThrottleVal <= (minThrottle + 100)) {
        armed = !armed;
      }
      break;
    case 'w':
      pres = mode.substring(1, mode.length()).toInt();
      if (pres > 80000) {
        bmp.setSeaLevelPressure(pres);
      }
      break;
    default:
      break;
  }
}

void EEPROMWritelong(int address, long value) {
  //Decomposition from a long to 4 bytes by using bitshift.
  //One = Most significant -> Four = Least significant byte
  byte four = (value & 0xFF);
  byte three = ((value >> 8) & 0xFF);
  byte two = ((value >> 16) & 0xFF);
  byte one = ((value >> 24) & 0xFF);
  
  //Write the 4 bytes into the eeprom memory.
  EEPROM.write(address, four);
  EEPROM.write(address + 1, three);
  EEPROM.write(address + 2, two);
  EEPROM.write(address + 3, one);
}

long EEPROMReadlong(long address){
  //Read the 4 bytes from the eeprom memory.
  long four = EEPROM.read(address);
  long three = EEPROM.read(address + 1);
  long two = EEPROM.read(address + 2);
  long one = EEPROM.read(address + 3);

  //Return the recomposed long by using bitshift.
  return ((four << 0) & 0xFF) + ((three << 8) & 0xFFFF) + ((two << 16) & 0xFFFFFF) + ((one << 24) & 0xFFFFFFFF);
}
