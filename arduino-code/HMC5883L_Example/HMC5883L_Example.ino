// Reference the I2C Library
#include <HMC5883L.h>

#define SDA_PORT PORTB
#define SDA_PIN 1
#define SCL_PORT PORTB
#define SCL_PIN 0
#define I2C_TIMEOUT 100
#define I2C_NOINTERRUPT 0
#define I2C_SLOWMODE 1
#define FAC 1
#define I2C_CPUFREQ (F_CPU/FAC)

#include <SoftI2CMaster.h>
#include <avr/io.h>

HMC5883L compass;
int error = 0;

void CPUSlowDown(int fac) {
  // slow down processor by a fac
    CLKPR = _BV(CLKPCE);
    CLKPR = _BV(CLKPS1) | _BV(CLKPS0);
}

// Out setup routine, here we will configure the microcontroller and compass.
void setup()
{
  #if FAC != 1
    CPUSlowDown(FAC);
  #endif

  Serial.begin(9600);

  if (!i2c_init()) 
    Serial.println(F("Initialization error. SDA or SCL are low"));
  else
    Serial.println(F("...done"));

  compass = HMC5883L(); // Construct a new HMC5883 compass.
    
  Serial.println("Setting scale to +/- 1.3 Ga");
  error = compass.SetScale(1.3); // Set the scale of the compass.
  if(error != 0) // If there is an error, print it out.
    Serial.println(compass.GetErrorText(error));
  
  Serial.println("Setting measurement mode to continous.");
  error = compass.SetMeasurementMode(Measurement_Continuous); // Set the measurement mode to Continuous
  if(error != 0) // If there is an error, print it out.
    Serial.println(compass.GetErrorText(error));
}

void loop()
{
  MagnetometerScaled scaled = compass.ReadScaledAxis();
  float heading = atan2(scaled.YAxis, scaled.XAxis);
  float declinationAngle = 0.0457;
  heading += declinationAngle;

  if(heading < 0)
    heading += 2*PI;

  if(heading > 2*PI)
    heading -= 2*PI;

  float headingDegrees = heading * 180/M_PI; 
  Output(headingDegrees);
}

// Output the data down the serial port.
void Output(float headingDegrees)
{
   Serial.print("   \tHeading:\t");
   Serial.print(headingDegrees);
   Serial.println(" Degrees   \t");
}
