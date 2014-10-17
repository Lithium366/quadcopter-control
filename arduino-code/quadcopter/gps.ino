void readGps () {
  gps.f_get_position(&flat, &flon, &age);

  while (Serial2.available())
      gps.encode(Serial2.read());
}
