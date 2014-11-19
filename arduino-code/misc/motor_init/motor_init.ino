#include <Servo.h>
#include <Wire.h>

Servo enginex1;
Servo enginex2;
Servo enginex3;
Servo enginex4;

void setup(){
        //enginex1.attach(4);
        //enginex2.attach(2);  
        enginex3.attach(3);
        //enginex4.attach(5);
        /*enginex1.writeMicroseconds(0);
        enginex2.writeMicroseconds(0);
        enginex3.writeMicroseconds(0);
        enginex4.writeMicroseconds(0);*/ 
        enginex3.writeMicroseconds(2300);
        /*enginex2.writeMicroseconds(2300);
        enginex3.writeMicroseconds(2300);
        enginex4.writeMicroseconds(2300);*/
        delay(2000);
        enginex3.writeMicroseconds(300); 
        /*enginex2.writeMicroseconds(300);
        enginex3.writeMicroseconds(300); 
        enginex4.writeMicroseconds(300);*/
        delay(4000);
}

void loop() {
        enginex3.writeMicroseconds(600); 
        //enginex2.writeMicroseconds(600);
        //enginex3.writeMicroseconds(600); 
        //enginex4.writeMicroseconds(600);
}
