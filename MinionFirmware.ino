
#include <Wire.h>
#include <SPI.h>
#include <SdFat.h>
#include <SFEMP3Shield.h>
#include <L3G.h>
#include <LSM303.h>

// Uncomment the next line to print debug messages throught the Serial line.
#define DEBUGGING 1


// Minimum touch value.
const int FSR_THREASHOLD = 100;
// Minimum time until we can play a sound again.
const int FSR_TIMER_THREASHOLD = 500;

// Minimum of for the accelerometer value.
const int IMU_SHAKE_THREASHOLD = 200;
// Number of time we have to move the minion up & down before he glows.
const int IMU_SHAKE_MIN = 5;
// Time until the shake counter is reseted for the minion not beeing shaked.
const int IMU_SHAKE_RESET_TIME = 1000;
// How long should the led be on.
const int LED_ON_TIME = 5000;

const int FSR_PIN = A0;

const int EN_GPIO1 = A2;
const int SD_CS = 9;
const int LED_PIN = 3;


L3G gyro;
LSM303 compass;

SFEMP3Shield MP3player;
SdFat sd;

long fsr_timer = 0;
boolean fetch_fsr_data = true;

// special mode plays a sound and blinks the leds.
boolean special_mode = false;
long special_mode_timer = 0;
const int SPECIAL_MODE_TIME = 500;

// Sound file indeces.
enum SOUND_FILES {
  SND_RANDOM_MIN = 0,
  SND_RANDOM_MAX = 5,
  SND_SPECIAL = 1,
  SND_SHAKE = 6,
  SND_READY = 7, 
};

// Files should be called [digit].mp3
char filename[9][6];

// Holds the accelerometer values
int accel_x, accel_y, accel_z;

boolean led_on = false;

void errorLoop();

void setup () {
  #ifdef DEBUGGING
  Serial.begin(9600);
  #endif
  
  // Set the pinmode for the LED.
  pinMode(LED_PIN, OUTPUT);
  analogWrite(LED_PIN, 0);
  
  
  Wire.begin();
  
  if (accel_init()) {
    errorLoop();
    #ifdef DEBUGGING
    Serial.println("Error initializing accelerometer");
    #endif
  }
  
  delay(20);
  
  imu_calibrate();
  
  if (sound_init(10)) {
    errorLoop();
    #ifdef DEBUGGING
    Serial.println("Error initializing sound");
    #endif
  }
  
  delay(2000);
  MP3player.playMP3(filename[SND_READY]);
  #ifdef DEBUGGING
  Serial.println("Ready");
  #endif
}



void loop() {
  sound_update();
  imu_update();
}


/**
 * This function is called if something wrent wrong during the
 * initialization. the LED GPIO should be initialized, so it can
 * blink. 
 */
void errorLoop() {
  // Blink the led forever if there was an error.
  while(true) {
    digitalWrite(LED_PIN, HIGH);
    delay(1000);
    digitalWrite(LED_PIN, LOW);
    delay(1000);
  }
}
