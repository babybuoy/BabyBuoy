// Baby Buoy project!





// Program header files
#include <Wire.h>
//#include <WiFi.h>
//#include <WebServer.h>
#include <ArduCAM.h>
#include <SPI.h>
//#include <FirebaseArduino.h>
#include "memorysaver.h"
#include <Adafruit_MMA8451.h>
#include <Adafruit_Sensor.h>

// Database/Wi-Fi configuration
#define FIREBASE_HOST "babybuoy.firebaseio.com"
#define FIREBASE_AUTH "mXS1eKO9f478AK9cqBRKrepxyoNAnxDxbbsTIFrl"
#define WIFI_SSID "BLANK"
#define WIFI_PASSWORD "BLANK"

// Variables to keep track of previous and current time to create a delay
unsigned long currentTime = 0;
unsigned long prevTime = 0;

unsigned long bool_waitForAccel = 0; // Used to flag the accelerometer
unsigned long bool_waitForPIR = 0; // Used to flag the PIR
unsigned long num_timeAtPIR = 0; // Used to keep track of the time the PIR sensor detected motion
unsigned long period = 5000; // Amount of time given to check after motion is detected


int statusLED = 26; // Choose the pin for status LED
int buzzer = 12; // Choose the pin for Piezzo Buzzer. Connected from MOSFET gate.
int tempSensor = 2; // Choose pin for Temperature Sensor
int pirSensor = 27; // Choose pin for controller PIR Sensor

int PIR_Gate = 25; // Controls Power gate for PIR Sensor
int ArduCAM_Gate = 32; // Controls Power gate for ArduCAM

const int CS = 5; // GPIO5 as Slave Select for ArduCAM

// Set up buzzer sound
int freq = 2000; // 2000 Hz frequency for the buzzer
int channel = 0; // LED PWM Channel on the ESp32
int resolution = 8; // 50% High and 50% Low
int dutyCycle = 128;

// Initial temperature reading
int temp = 0;

// Threshhold for declaring a fall on the accelerometer
float sensitivity = 11.0;

float magnitude = 0.0; // Stores the calculated magnitude


// Accelerometer and semaphore instances
Adafruit_MMA8451 mma = Adafruit_MMA8451();
SemaphoreHandle_t syncSemaphore; // Interrupt based

// Interrupt service routine to unblock Arduino main loop.
// Used for created a PWM signal
//************* MIGHT NOT BE USED DEPENDING ON THE BUZZER *************//
void IRAM_ATTR handleInterrupt() 
{
  xSemaphoreGiveFromISR(syncSemaphore, NULL);
}


void setup() 
{
  Serial.begin(115200);
  
  //pinMode(PIR_Gate, OUTPUT); // Set mosfet gate to output for PIR Sensor
  //pinMode(ArduCAM_Gate, OUTPUT); // Set mosfet gate to output for ArduCAM
  
  pinMode(statusLED, OUTPUT);      // Declare LED as output
  pinMode(buzzer, OUTPUT);         // Declare Piezo Buzzer as output
  pinMode(pirSensor, INPUT_PULLUP);        // Declare PIR Sensor as input

  
  syncSemaphore = xSemaphoreCreateBinary();

  // Attach interrupt to pir pin and PWM channel to buzzer
  attachInterrupt(digitalPinToInterrupt(pirSensor), handleInterrupt, CHANGE);
  ledcSetup(channel, freq, resolution);
  ledcAttachPin(buzzer, channel); // Attach channel to buzzer pin

  Serial.println();

  // Check if accelerometer is properly connected/found
  if(!mma.begin())
  {
    Serial.println("Couldn't start MMA8451 Accelerometer.");
    Serial.println("Check your connections.");
  }
  Serial.println("MMA8451 Accelerometer found!");

  // Set the g-range for the accelerometer. Can be adjusted to 2g, 4g, or 8g.
  mma.setRange(MMA8451_RANGE_2_G);

  
  //************* WI-FI CONNECTION CODE HERE *************//


  // Turn on status LED to show that the system is on
  digitalWrite(statusLED, HIGH);
  
  Serial.println("Sensors booting up...");
  delay(10000); // Let sensors boot up, 10s
  Serial.println("Sensors ready!");
}

void loop()
{
  
  // Read data from accelerometer
  mma.read();  
  sensors_event_t event;
  mma.getEvent(&event);


  // Store magnitude of all axis
  magnitude = calculateMagnitude(event.acceleration.x, event.acceleration.y, event.acceleration.z);

  // Motion is detected! 5 seconds will be given to detect a fall from the accelerometer
  if(digitalRead(pirSensor))
  {
    num_timeAtPIR = millis(); // Time that PIR sensor was activated with respect to the system time
    bool_waitForAccel = 1; // Set flag to true to wait for accelerometer
    Serial.println("Motion detected!");
  }
  else // No motion is detected
  {
    Serial.println("No motion detected! :(");
    bool_waitForPIR = 1;
  }

  // Motion and Fall are detected. Alarm is activated and picture/video is captured and save onto website.
  if(bool_waitForAccel == 1 && (magnitude > sensitivity))
  {
    
    Serial.println("Motion and Fall detected!");
    Serial.print("Magnitude is: ");
    Serial.println(magnitude);
    bool_waitForAccel = 0; // No longer waiting for accelerometer
    Serial.println();


    //************* ADD CODE FOR TAKING PICTURE/VIDEO TO WEB **************//
  }

  // Only fall is detected. Alarm is activated and picture/video is captured to save onto website.
  else if(bool_waitForPIR == 1 && (magnitude > sensitivity))
  {
    Serial.println("Fall detected");
    Serial.print("Magnitude is: ");
    Serial.println(magnitude);
    Serial.println();

    //************* ADD CODE FOR TAKING PICTURE/VIDEO TO WEB *************//
    
  }

  // 5 second timer to check if a Fall is detected after Motion is detected.
  if(millis() > (num_timeAtPIR + period))
  {
    bool_waitForAccel = 0; // Set flag to false
  }

  
}


// Calculate the magnitude with the x, y, and z axis of the accelerometer.
// Gives an overall value that is better than just using the z-axis
float calculateMagnitude(float x, float y, float z)
{
  float magnitude = sqrt(sq(x) + sq(y) + sq(z));

  return magnitude;
}
