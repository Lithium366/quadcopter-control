const float alpha = 0.5;

double fXg = 0;
double fYg = 0;
double fZg = 0;

int znak = 1;

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
  return headingDegrees;
}

void getLevel() {
  pitchCorrection = (float) EEPROMReadlong(40) / 1000;
  rollCorrection = (float) EEPROMReadlong(44) / 1000;
}

void setLevel() {
  pitchCorrection = -1 * (angley - pitchCorrection);
  rollCorrection = -1 * (anglex - rollCorrection); 
  EEPROMWritelong(40, long(pitchCorrection * 1000));
  EEPROMWritelong(44, long(rollCorrection * 1000));
}

void getAngles()
{
  double pitch, roll, yaw, Xg, Yg, Zg;
  acc.read(&Xg, &Yg, &Zg);
  gyro.read();
  fXg = Xg * alpha + (fXg * (1.0 - alpha));
  vybrox[loopcount] = Xg;
  fYg = Yg * alpha + (fYg * (1.0 - alpha));
  vybroy[loopcount] = Yg;
  fZg = Zg * alpha + (fZg * (1.0 - alpha));
  vybroz[loopcount] = Zg;
  roll  = (atan2(-fYg, fZg)*180.0)/M_PI + rollCorrection;
  pitch = (atan2(fXg, sqrt(fYg*fYg + fZg*fZg))*180.0)/M_PI + pitchCorrection;
  yaw = getCompassData() + yawCorrection;
  
  dtime = millis() - ptime;
  if (dtime < 0) {
    dtime = 10;
  }
  ptime = millis();

  anglex = 0.98 * (anglex + gyro.g.x * 0.00875 * dtime / 1000 * -1) + 0.02 * pitch;
  angley = 0.98 * (angley + gyro.g.y * 0.00875 * dtime / 1000 * -1) + 0.02 * roll;
  if (!anglez) {
    anglez = yaw;
  }
  anglez = 0.999 * (anglez + gyro.g.z * 0.00875 * dtime / 1000 * -1) + 0.001 * yaw;
}

void computeErrorZ () {
  if (!deltaZ) {
    deltaZ = anglez;
  }    
  if(abs(YawVal) > 5) {
    deltaZ = anglez + YawVal;
  }
  errorZ = anglez - deltaZ;
  if(errorZ > 180) {
    errorZ -= 360;
  } else if (errorZ < -180) {
    errorZ += 360;
  }
  // if wind will rotate to 180/-180 - Quad will crash  
}


