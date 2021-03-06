const float alpha = 0.5;

double fXg = 0;
double fYg = 0;
double fZg = 0;
int znak = 1;
double yaw1 = 0;
boolean holdZ = false;

long ptime = 0;

float getCompassData(double pitch, double roll)
{
  
  mag.getHeading(&mx, &my, &mz);

  pitch = pitch / 57;
  roll = roll / 57;

  float xh = mx * cos(pitch) + my * sin(roll) * sin(pitch) - mz * cos(roll) * sin(pitch);
  float yh = my * cos(roll) + mz * sin(roll);
  
  float heading = atan2(yh, xh);
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
  pitchCorrection = -1 * (anglex - pitchCorrection);
  rollCorrection = -1 * (angley - rollCorrection); 
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
  yaw = getCompassData(pitch, roll) + yawCorrection;
  
  dtime = millis() - ptime;
  if (dtime < 0) {
    dtime = 10;
  }
  ptime = millis();

  anglex = 0.98 * (anglex + gyro.g.y * 0.00875 * dtime / 1000 * -1) + 0.02 * pitch;
  angley = 0.98 * (angley + gyro.g.x * 0.00875 * dtime / 1000 * -1) + 0.02 * roll;   
  
  double k1, k2;
  
  if (yaw - yaw1 > 5 || yaw - yaw1 < -5) {
    k1 = 1;
    k2 = 0;
  } else {
    k1 = 0.98;
    k2 = 0.02;
    if(yaw > 345 || yaw < 15) {
      anglez = yaw;
    }
  }
  yaw1 = yaw;
  if (!anglez) {
    anglez = yaw;
  }
  anglez = k1 * (anglez + gyro.g.z * 0.00875 * dtime / 1000 * -1) + k2 * yaw;
}

void computeErrorZ ()  {
  if (abs(YawVal) <= 2) {
    if (!holdZ) {
      deltaZ = anglez;
      holdZ = true;
    }
    errorZ = anglez - deltaZ;
  } else {
    errorZ = (double)YawVal;
    holdZ = false;
  }
  
  if(errorZ > 180) {
    errorZ -= 360;
  } else if (errorZ < -180) {
    errorZ += 360;
  }
  // if wind will rotate to 180/-180 - Quad will crash  
}


