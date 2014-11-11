void telemetry () {
  //Telemetry info
  if (debug_mode) {
    loopcount++;
    if (telemetry_mode == 3) { // Accelerometer data (vibrations)
      if (loopcount >= 7) {
        printAccel();
        loopcount = 0;
      }
    } else if (telemetry_mode == 2) { // PID data (error)
      if (loopcount >= 7) {
        if (loopcount >= 7) {
          printError();
          loopcount = 0;
        }
      }
    } else if (telemetry_mode == 5) {
      printEngines();
      loopcount = 0;
    } else if (telemetry_mode == 4) { // Send PID values 
      if (loopcount >= 7) {
        printPID();
        Serial1.println("devider");
        loopcount = 0;
      }
    } else {
      // flight instruments
      if (loopcount >= 7) {
        printRC();
        printAngles();
        printSystem();
        Serial1.println("devider");
        loopcount = 0;
      }
    }        
  }
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
    case 'e': // Print PID engine outputs
      telemetry_mode = 5;
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

void printAccel() {
  vybroxsum = "";
  vybroysum = "";
  vybrozsum = "";
  for (int i = 0; i < loopcount; i++) {
    char tmp[5];
    dtostrf(vybrox[i] * 1000, 3, 0, tmp);
    vybroxsum += ":";
    vybroxsum += tmp;
    dtostrf(vybroy[i] * 1000, 3, 0, tmp);
    vybroysum += ":";
    vybroysum += tmp;
    dtostrf(vybroz[i] * 1000, 3, 0, tmp);
    vybrozsum += ":";
    vybrozsum += tmp;
  }
  Serial1.print("vybrox");
  Serial1.println(vybroxsum);
  Serial1.print("vybroy");
  Serial1.println(vybroysum);
  Serial1.print("vybroz");
  Serial1.println(vybrozsum);
  Serial1.println("devider");
}

void printError () {
  vybroxsum = "";
  vybroysum = "";
  vybrozsum = "";
  for (int i = 0; i < loopcount; i++) {
    char tmp[5];
    dtostrf(errorx[i] * 100, 5, 0, tmp);
    vybroxsum += ":";
    vybroxsum += tmp;
    dtostrf(errory[i] * 100, 5, 0, tmp);
    vybroysum += ":";
    vybroysum += tmp;
    dtostrf(errorz[i] * 100, 5, 0, tmp);
    vybrozsum += ":";
    vybrozsum += tmp;
  }
  Serial1.print("errorx");
  Serial1.println(vybroxsum);
  Serial1.print("errory");
  Serial1.println(vybroysum);
  Serial1.print("errorz");
  Serial1.println(vybrozsum);
  Serial1.println("devider");
}

void printEngines () {
  vybroxsum = "";
  vybroysum = "";
  vybrozsum = "";
  for (int i = 0; i < loopcount; i++) {
    char tmp[5];
    dtostrf(enginex[i], 5, 0, tmp);
    vybroxsum += ":";
    vybroxsum += tmp;
    dtostrf(enginey[i], 5, 0, tmp);
    vybroysum += ":";
    vybroysum += tmp;
    dtostrf(enginez[i], 5, 0, tmp);
    vybrozsum += ":";
    vybrozsum += tmp;
  }
  Serial1.print("enginex");
  Serial1.println(vybroxsum);
  Serial1.print("enginey");
  Serial1.println(vybroysum);
  Serial1.print("enginez");
  Serial1.println(vybrozsum);
  Serial1.println("devider");
}

void printPID () {
    char pidXP_s[8];
    char pidXI_s[8];
    char pidXD_s[8];
    char pidYP_s[8];
    char pidYI_s[8];
    char pidYD_s[8];
    char pidZP_s[8];
    char pidZI_s[8];
    char pidZD_s[8];
    char total[100];
    dtostrf(pidXP, 7, 3, pidXP_s);
    dtostrf(pidXI, 7, 3, pidXI_s);
    dtostrf(pidXD, 7, 3, pidXD_s);
    dtostrf(pidYP, 7, 3, pidYP_s);
    dtostrf(pidYI, 7, 3, pidYI_s);
    dtostrf(pidYD, 7, 3, pidYD_s);
    dtostrf(pidZP, 7, 3, pidZP_s);
    dtostrf(pidZI, 7, 3, pidZI_s);
    dtostrf(pidZD, 7, 3, pidZD_s);
    sprintf(total, "pid:%s:%s:%s:%s:%s:%s:%s:%s:%s", pidXP_s, pidXI_s, pidXD_s, pidYP_s, pidYI_s, pidYD_s, pidZP_s, pidZI_s, pidZD_s);
    Serial1.println(total);
}

void printAngles() {
    char anglex_s[10];
    char angley_s[10];
    char anglez_s[10];
    char alt_s[10];
    char vspeed_s[10];
    char total[90];
    dtostrf(anglex, 1, 1, anglex_s);
    dtostrf(angley, 1, 1, angley_s);
    dtostrf(anglez, 1, 1, anglez_s);
    dtostrf(Altitude, 1, 1, alt_s);
    dtostrf(vspeed, 1, 1, vspeed_s);
    sprintf(total, "angles:%s:%s:%s:%s:%s", anglex_s, angley_s, anglez_s, alt_s, vspeed_s);
    Serial1.println(total);
}

void printRC() {
    char total[50];
    sprintf(total, "reciever:%d:%d:%d:%d:%d:%d:%d:%d", ThrottleVal, PitchVal, RollVal, YawVal, CH5Val, CH6Val, CH7Val, CH8Val);
    Serial1.println(total);
}

void printSystem() {  
  Serial1.print("system:");
  Serial1.print(dtime);
  Serial1.print(":");
  Serial1.print(flat, 10);
  Serial1.print(":");
  Serial1.print(flon, 10);
  Serial1.print(":");  
  Serial1.print(getSpeed(), 1);
  Serial1.print(":");
  Serial1.println(armed ? 1 : 0);
}
