uint32_t ulThrottleStart;
uint32_t ulRollStart;
uint32_t ulPitchStart;
uint32_t ulYawStart;
uint32_t ulCH5Start;
uint32_t ulCH6Start;
uint32_t ulCH7Start;
uint32_t ulCH8Start;
volatile uint16_t unThrottleInShared;
volatile uint16_t unRollInShared;
volatile uint16_t unPitchInShared;
volatile uint16_t unYawInShared;
volatile uint16_t unCH5InShared;
volatile uint16_t unCH6InShared;
volatile uint16_t unCH7InShared;
volatile uint16_t unCH8InShared;

void readRC() {
  PCintPort::attachInterrupt(THROTTLE_IN_PIN, calcThrottle,CHANGE);
  PCintPort::attachInterrupt(ROLL_IN_PIN, calcRoll,CHANGE);
  PCintPort::attachInterrupt(PITCH_IN_PIN, calcPitch,CHANGE);
  PCintPort::attachInterrupt(YAW_IN_PIN, calcYaw,CHANGE);
  PCintPort::attachInterrupt(CH5_IN_PIN, calcCH5,CHANGE);
  PCintPort::attachInterrupt(CH6_IN_PIN, calcCH6,CHANGE);
  PCintPort::attachInterrupt(CH7_IN_PIN, calcCH7,CHANGE);
  PCintPort::attachInterrupt(CH8_IN_PIN, calcCH8,CHANGE);
  
  ThrottleVal = map(unThrottleInShared, rcMin, rcMax, minThrottle, maxThrottle - minEngineRPM);
  if (ThrottleVal < 0) {
    ThrottleVal = 0;
  }
  RollVal = map(unRollInShared, rcMin, rcMax, -maxPitch, maxPitch);
  if (RollVal < -100) {
    RollVal = 0;
  }
  PitchVal = map(unPitchInShared, rcMin, rcMax, -maxPitch, maxPitch);
  if (PitchVal < -100) {
    PitchVal = 0;
  }
  YawVal = map(unYawInShared, rcMin, rcMax, -maxYaw, maxYaw);
  if (YawVal < -100) {
    YawVal = 0;
  }
  CH5Val = map(unCH5InShared, rcMin, rcMax, false, true);
  CH6Val = map(unCH6InShared, rcMin, rcMax, 0, 255);
  if (CH6Val < 0) {CH6Val = 0;}
  CH7Val = map(unCH7InShared, rcMin, rcMax, 0, 255);
  if (CH7Val < 0) {CH7Val = 0;}
  CH8Val = map(unCH8InShared, rcMin, rcMax, 0, 255);
  if (CH8Val < 0) {CH8Val = 0;}
}

void calcThrottle()
{
  if(digitalRead(THROTTLE_IN_PIN) == HIGH) { 
    ulThrottleStart = micros();
  } else {
    unThrottleInShared = (uint16_t)(micros() - ulThrottleStart);
  }
}

void calcRoll()
{
  if(digitalRead(ROLL_IN_PIN) == HIGH) { 
    ulRollStart = micros();
  } else {
    unRollInShared = (uint16_t)(micros() - ulRollStart);
  }
}

void calcPitch()
{
  if(digitalRead(PITCH_IN_PIN) == HIGH) { 
    ulPitchStart = micros();
  } else {
    unPitchInShared = (uint16_t)(micros() - ulPitchStart);
  }
}

void calcYaw()
{
  if(digitalRead(YAW_IN_PIN) == HIGH) { 
    ulYawStart = micros();
  } else {
    unYawInShared = (uint16_t)(micros() - ulYawStart);
  }
}

void calcCH5()
{
  if(digitalRead(CH5_IN_PIN) == HIGH) { 
    ulCH5Start = micros();
  } else {
    unCH5InShared = (uint16_t)(micros() - ulCH5Start);
  }
}

void calcCH6()
{
  if(digitalRead(CH6_IN_PIN) == HIGH) { 
    ulCH6Start = micros();
  } else {
    unCH6InShared = (uint16_t)(micros() - ulCH6Start);
  }
}

void calcCH7()
{
  if(digitalRead(CH7_IN_PIN) == HIGH) { 
    ulCH7Start = micros();
  } else {
    unCH7InShared = (uint16_t)(micros() - ulCH7Start);
  }
}

void calcCH8()
{
  if(digitalRead(CH8_IN_PIN) == HIGH) { 
    ulCH8Start = micros();
  } else {
    unCH8InShared = (uint16_t)(micros() - ulCH8Start);
  }
}
