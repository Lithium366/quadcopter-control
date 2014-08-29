
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
double pitchCorrection = -7;
double rollCorrection = -2;
double yawCorrection = 0;

//Roll
float pidXP = 7;//2.2;//7;//3;//0.9;
float pidXI = 1;//2;//0.2;
float pidXD = 0.9;//0.65;//0.95;//0.71;//0.85;

//Pitch
float pidYP = 7;//7;//3;//7;//0.9;
float pidYI = 1;//2;//3;//0.2;
float pidYD = 0.9;//0.9;//0.71;//3;//0.85;

//Yaw
float pidZP = 0;//10;
float pidZI = 0;//0;
float pidZD = 0;//1.2;

float declinationAngle = 0.227;
