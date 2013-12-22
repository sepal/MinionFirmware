#include <SPI.h>
#include <SdFat.h>
#include <SFEMP3Shield.h>
#include <MemoryFree.h>

const int FSR_THREASHOLD = 100;
const int FSR_TIMER_THREASHOLD = 500;

const int FSR_PIN = A0;
const int EN_GPIO1 = A2;
const int SD_CS = 9;

SFEMP3Shield MP3player;
SdFat sd;

// Files should be called [number].mp3
char filename[5][6];
long fsr_timer = 0;

void errorLoop();

void setup () {
  Serial.begin(9600);
  
  initSound(0);
  
}



void loop() {
}


/**
 * This function is called if something wrent wrong during the
 * initialization.
 */
void errorLoop() {
  
}
