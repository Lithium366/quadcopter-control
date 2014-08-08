#include <PinChangeInt.h>

#define THROTTLE_IN_PIN 9
#define ROLL_IN_PIN 8
#define PITCH_IN_PIN 8
#define YAW_IN_PIN 8

uint32_t ulThrottleStart;
uint32_t ulRollStart;
uint32_t ulPitchStart;
uint32_t ulYawStart;
volatile uint16_t unThrottleInShared;
volatile uint16_t unRollInShared;
volatile uint16_t unPitchInShared;
volatile uint16_t unYawInShared;

void setup()
{
  Serial.begin(9600);
  PCintPort::attachInterrupt(THROTTLE_IN_PIN, calcThrottle,CHANGE);
  PCintPort::attachInterrupt(ROLL_IN_PIN, calcRoll,CHANGE);
  PCintPort::attachInterrupt(PITCH_IN_PIN, calcPitch,CHANGE);
  PCintPort::attachInterrupt(YAW_IN_PIN, calcYaw,CHANGE);
}

void loop()
{
  Serial.print(map(unThrottleInShared, 1058, 1860, 700, 2300));
  Serial.print(":");
  Serial.print(map(unRollInShared, 1058, 1860, -30, 30));
  Serial.print(":");
  Serial.print(map(unPitchInShared, 1058, 1860, -30, 30));
  Serial.print(":");
  Serial.println(map(unYawInShared, 1058, 1860, -500, 500));
  delay(100);
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
