#include <SPI.h>
#include <SdFat.h>
#include <SFEMP3Shield.h>
#include <MemoryFree.h>

const int FSR_THREASHOLD = 100;
const int FSR_TIMER_THREASHOLD = 500;

const int FSR_PIN = A0;
const int EN_GPIO1 = A2;
const int SD_CS = 9;
const int LED_PIN = 3;


SFEMP3Shield MP3player;
SdFat sd;

// Files should be called [number].mp3
char filename[5][6];
long fsr_timer = 0;
boolean fetch_fsr_data = true;

void errorLoop();

void setup () {
  Serial.begin(9600);
  
  // Set the pinmode for the LED.
  pinMode(LED_PIN, OUTPUT);
  analogWrite(LED_PIN, 0);
  
  if (initSound(10)) {
    errorLoop();
  }
  
}



void loop() {
  updateMP3();
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
