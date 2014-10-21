void readGps () {
  gps.f_get_position(&flat, &flon, &age);

  while (Serial2.available())
      gps.encode(Serial2.read());
}

float getSpeed () {
  return gps.f_speed_knots();
}
