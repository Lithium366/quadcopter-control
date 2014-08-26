
// RC pins
#define THROTTLE_IN_PIN A10 //CH3
#define ROLL_IN_PIN A8 //CH1
#define PITCH_IN_PIN A9 //CH2
#define YAW_IN_PIN A11 //CH4
#define CH5_IN_PIN A12
#define CH6_IN_PIN A13
#define CH7_IN_PIN A14
#define CH8_IN_PIN A15

//ESC connected to
#define ENGINE1 4
#define ENGINE2 2
#define ENGINE3 3
#define ENGINE4 5

// Values range from RC reciever
int rcMin = 1058;
int rcMax = 1860;

// Max angles for X/Y/Z axes, throttle
int maxPitch = 30;
int maxRoll = 30;
int maxYaw = 30;
int minThrottle = 300;
int maxThrottle = 2300;

// Correction values to keep horison position
int pitchCorrection = -7.2;
int rollCorrection = -2.4;
float yawCorrection = 0;

//Roll
float pidXP = 1.4;//0.9;
float pidXI = 0.5;//0.2;
float pidXD = 0.6;//0.85;

//Pitch
float pidYP = 1.4;//0.9;
float pidYI = 0.5;//0.2;
float pidYD = 0.6;//0.85;

//Yaw
float pidZP = 0;
float pidZI = 0;
float pidZD = 0;

float declinationAngle = 0.227;
