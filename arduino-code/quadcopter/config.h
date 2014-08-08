
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
#define ENGINE1 12
#define ENGINE2 6
#define ENGINE3 4
#define ENGINE4 5

// Values range from RC reciever
int rcMin = 1058;
int rcMax = 1860;

// Max angles for X/Y/Z axes, throttle
int maxPitch = 30;
int maxRoll = 30;
int maxYaw = 50;
int minThrottle = 300;
int maxThrottle = 2300;

// Correction values to keep horison position
int pitchCorrection = 0;
int rollCorrection = 0;

float pidXP = 0;
float pidXI = 0;
float pidXD = 0;

float pidYP = 0;
float pidYI = 0;
float pidYD = 0;

float pidZP = 0;
float pidZI = 0;
float pidZD = 0;
