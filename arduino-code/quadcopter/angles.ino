const float alpha = 0.5;

double fXg = 0;
double fYg = 0;
double fZg = 0;

long ptime = 0;

float getCompassData()
{
  MagnetometerScaled scaled = compass.ReadScaledAxis();
  float heading = atan2(scaled.YAxis, scaled.XAxis);
  heading += declinationAngle;
  
  if(heading < 0)
    heading += 2*PI;
    
  if(heading > 2*PI)
    heading -= 2*PI;
    
  float headingDegrees = heading * 180/M_PI;
  Serial.println(headingDegrees);
  return headingDegrees;
}

void getAngles()
{
  double pitch, roll, yaw, Xg, Yg, Zg;
  acc.read(&Xg, &Yg, &Zg);
  gyro.read();
  fXg = Xg * alpha + (fXg * (1.0 - alpha));
  fYg = Yg * alpha + (fYg * (1.0 - alpha));
  fZg = Zg * alpha + (fZg * (1.0 - alpha));
  roll  = (atan2(-fYg, fZg)*180.0)/M_PI + rollCorrection;
  pitch = (atan2(fXg, sqrt(fYg*fYg + fZg*fZg))*180.0)/M_PI + pitchCorrection;
  yaw = getCompassData() + yawCorrection;
  
  dtime = millis() - ptime;
  if (dtime < 0) {
    dtime = 10;
  }
  ptime = millis();

  anglex = 0.98 * (anglex + gyro.g.x * 0.00875 * dtime / 1000 * -1) + 0.02 * roll;
  angley = 0.98 * (angley + gyro.g.y * 0.00875 * dtime / 1000 * -1) + 0.02 * pitch;
  anglez = 0.98 * (anglez + gyro.g.z * 0.00875 * dtime / 1000 * -1) + 0.02 * yaw;
}


