void initGps() {
  Serial2.begin(9600);
}

void readGps () {
  bool newData = false;
  unsigned long chars;
  unsigned short sentences, failed;

  while (Serial2.available()) {
    char c = Serial2.read();
    if (gps.encode(c))
      newData = true;
  }

  if (newData) {
    gps.f_get_position(&flat, &flon, &age);
  }
}
