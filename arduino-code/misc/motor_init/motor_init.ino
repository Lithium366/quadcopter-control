#include <Servo.h>
#include <Wire.h>
#include <ADXL345.h>
#include <L3G4200D.h>
#include <HMC5883L.h>
#include <PID_v1.h>

Servo enginex1;
Servo enginex2;
Servo enginex3;
Servo enginex4;

void setup(){
        enginex1.attach(4);
        enginex1.writeMicroseconds(0);
        enginex2.attach(2);  
        enginex2.writeMicroseconds(0);
        enginex3.attach(3);
        enginex3.writeMicroseconds(0);
        enginex4.attach(5);  
        enginex4.writeMicroseconds(0);
	delay(1000);
        enginex2.writeMicroseconds(1400);
        delay(5000);
        /*enginex2.writeMicroseconds(800);
        enginex3.writeMicroseconds(800);
        enginex4.writeMicroseconds(800);*/
        /*delay(2000);
        enginex1.writeMicroseconds(0); 
        enginex2.writeMicroseconds(0);
        enginex3.writeMicroseconds(0); 
        enginex4.writeMicroseconds(0);
        delay(2000);*/
}

void loop() {
        enginex2.writeMicroseconds(0); 
        /*enginex2.writeMicroseconds(900);
        enginex3.writeMicroseconds(900); 
        enginex4.writeMicroseconds(900);*/
}
