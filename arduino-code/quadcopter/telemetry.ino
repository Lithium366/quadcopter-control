void telemetry () {
  //Telemetry info
  if (debug_mode) {
    loopcount++;
    if (loopcount == 1) {
      printRC();
    } else if (loopcount == 4) {
      vybroxsum = vybrox / 16;
      vybroysum = vybroy / 16;
      vybrozsum = vybroz / 16;
      vybrox = 0;
      vybroy = 0;
      vybroz = 0;
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
    char vybroxsum_s[10];
    char vybroysum_s[10];
    char vybrozsum_s[10];
    char total[70];
    dtostrf(anglex, 1, 3, anglex_s);
    dtostrf(angley, 1, 3, angley_s);
    dtostrf(anglez, 1, 3, anglez_s);
    dtostrf(vybroxsum, 1, 3, vybroxsum_s);
    dtostrf(vybroysum, 1, 3, vybroysum_s);
    dtostrf(vybrozsum, 1, 3, vybrozsum_s);
    sprintf(total, "angles:%s:%s:%s", anglex_s, angley_s, anglez_s, vybroxsum_s, vybroysum_s, vybrozsum_s);
    Serial1.println(total);
}

void printRC() {
    char total[50];
    sprintf(total, "reciever:%d:%d:%d:%d:%d:%d:%d:%d", ThrottleVal, PitchVal, RollVal, YawVal, CH5Val, CH6Val, CH7Val, CH8Val);
    Serial1.println(total);
}

void printSystem() {
  char total[40];
  char flat_s[15];
  char flon_s[15];
  dtostrf(flat, 12, 3, flat_s);
  dtostrf(flon, 12, 3, flon_s);
  sprintf(total, "system:%d:%s:%s", dtime, flat_s, flon_s);
  Serial1.println(total);
}
