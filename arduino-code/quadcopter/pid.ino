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
