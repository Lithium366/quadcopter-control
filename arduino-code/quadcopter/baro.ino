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
