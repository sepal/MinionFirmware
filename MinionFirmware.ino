#include <SPI.h>
#include <SdFat.h>
#include <SFEMP3Shield.h>
#include <MemoryFree.h>

const int FSR_THREASHOLD = 100;
const int FSR_TIMER_THREASHOLD = 500;

const int FSR_PIN = A0;
const int EN_GPIO1 = A2;
const int SD_CS = 9;

// Files should be called [number].mp3
char filename[5][6];
long fsr_timer = 0;

void setup () {
  Serial.begin(9600);
}



void loop() {
}
