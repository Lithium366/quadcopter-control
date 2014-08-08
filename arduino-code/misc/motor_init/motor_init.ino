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
        enginex1.writeMicroseconds(2300);
        enginex2.attach(5);  
        enginex2.writeMicroseconds(2300);
        enginex3.attach(10);
        enginex3.writeMicroseconds(2300);
        enginex4.attach(11);  
        enginex4.writeMicroseconds(2300);
	delay(5000);
        enginex1.attach(4);
        enginex1.writeMicroseconds(700);
        enginex2.attach(5);  
        enginex2.writeMicroseconds(700);
        enginex3.attach(10);
        enginex3.writeMicroseconds(700);
        enginex4.attach(11);  
        enginex4.writeMicroseconds(700);
        delay(2000);
        enginex1.writeMicroseconds(0); 
        enginex2.writeMicroseconds(0);
        enginex3.writeMicroseconds(0); 
        enginex4.writeMicroseconds(0);
        delay(2000);
}

void loop() {
        enginex1.writeMicroseconds(800); 
        enginex2.writeMicroseconds(800);
        enginex3.writeMicroseconds(800); 
        enginex4.writeMicroseconds(800);
}
