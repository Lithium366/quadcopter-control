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

// Turn on /off telemetry
bool debug_mode = true;
// Time that sticks must be in arm/disarm position to arm/disarm
int arm_time_ms = 5000;

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
double pitchCorrection, rollCorrection;
double yawCorrection = 0;
int minEngineRPM = 600;

float pidXP, pidXI, pidXD, pidYP, pidYI, pidYD, pidZP, pidZI, pidZD;

float declinationAngle = 0.227;
int telemetry_mode = 1; // flight instruments
