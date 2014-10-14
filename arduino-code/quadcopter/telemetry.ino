void telemetry () {
  //Telemetry info
  if (debug_mode) {
    loopcount++;
    
    if (loopcount == 25) {
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
      printRC();
      printAngles();
      printPID();
      printSystem();
      Serial1.println("devider");
      loopcount = 0;
    }
  }
}

void printPID () {
    char pidXP_s[5];
    char pidXI_s[5];
    char pidXD_s[5];
    char pidYP_s[5];
    char pidYI_s[5];
    char pidYD_s[5];
    char pidZP_s[5];
    char pidZI_s[5];
    char pidZD_s[5];
    char total[50];
    dtostrf(pidXP, 1, 2, pidXP_s);
    dtostrf(pidXI, 1, 2, pidXI_s);
    dtostrf(pidXD, 1, 2, pidXD_s);
    dtostrf(pidYP, 1, 2, pidYP_s);
    dtostrf(pidYI, 1, 2, pidYI_s);
    dtostrf(pidYD, 1, 2, pidYD_s);
    dtostrf(pidZP, 1, 2, pidZP_s);
    dtostrf(pidZI, 1, 2, pidZI_s);
    dtostrf(pidZD, 1, 2, pidZD_s);
    sprintf(total, "pid:%s:%s:%s:%s:%s:%s:%s:%s:%s", pidXP_s, pidXI_s, pidXD_s, pidYP_s, pidYI_s, pidYD_s, pidZP_s, pidZI_s, pidZD_s);
    Serial1.println(total);
}

void printAngles() {
    char anglex_s[10];
    char angley_s[10];
    char anglez_s[10];
    char total[70];
    dtostrf(anglex, 1, 1, anglex_s);
    dtostrf(angley, 1, 1, angley_s);
    dtostrf(anglez, 1, 1, anglez_s);
    sprintf(total, "angles:%s:%s:%s", anglex_s, angley_s, anglez_s);
    Serial1.println(total);
}

void printRC() {
    char total[50];
    sprintf(total, "reciever:%d:%d:%d:%d:%d:%d:%d:%d", ThrottleVal, PitchVal, RollVal, YawVal, CH5Val, CH6Val, CH7Val, CH8Val);
    Serial1.println(total);
}

void printSystem() {
  counter++;
  char total[40];
  char flat_s[15];
  char flon_s[15];
  dtostrf(flat, 3, 12, flat_s);
  dtostrf(flon, 3, 12, flon_s);
  sprintf(total, "system:%d:%d:%s:%s:%d", dtime, counter, flat_s, flon_s, armed ? 1 : 0);
  Serial1.println(total);
}
