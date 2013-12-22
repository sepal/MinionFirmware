
/**
 * Initializes the mp3 and sd card lib and fetches all sound files
 * from the sd card.
 * @param volume Volume of the mp3 layer, 0 beeing the loudest and 256 beeing off.
 * @return true if there was an error.
 */
boolean sound_init(int volume) {
  int index;
  SdFile file;
  byte result;
  char tempfilename[13];
  
  // Goto mp3 mode
  pinMode(EN_GPIO1,OUTPUT);
  digitalWrite(EN_GPIO1,LOW);
  
  result = sd.begin(SD_CS,SPI_HALF_SPEED);
  
  if (result != 1) {
    return true;
  }
  
  result = MP3player.begin();
  
  if ((result != 0) && (result != 6)) {
    return true;
  }
  
  // Go through the sd card.
  sd.chdir("/", true);
  while (file.openNext(sd.vwd(),O_READ))
  {
    // Get filename
    file.getFilename(tempfilename);
    // We can have up to 10 sound files, althought not all must be used.
    // See the SOUND_FILES enumeration to see which files are used for what.
    if (tempfilename[0] >= '0' && tempfilename[0] <= '9')
    {
      // Yes! subtract char '1' to get an index of 0 through 4.
      index = tempfilename[0] - '1';
      
      // Copy the data to our filename array.
      strcpy(filename[index],tempfilename);
  
      #ifdef DEBUGGING
      Serial.print(F("found a file with a leading "));
      Serial.print(index+1);
      Serial.print(F(": "));
      Serial.println(filename[index]);
      #endif
    }
      
    file.close();
  }

  // Set the VS1053 volume. 0 is loudest, 255 is lowest (off):
  MP3player.setVolume(volume, volume);
  
  // Turn on the amplifier chip:
  digitalWrite(EN_GPIO1, HIGH);
  delay(2);
  return false;
}

/**
 * Plays a sound if the FSR Sensor was touched.
 */
void sound_update() {
  if (MP3player.isPlaying() && fetch_fsr_data) {
    fetch_fsr_data = false;
    fsr_timer = millis();
  }
  if (millis() - fsr_timer > FSR_TIMER_THREASHOLD && !fetch_fsr_data) {
    fetch_fsr_data = true;
  }
  
  int fsr_value = analogRead(FSR_PIN);
  if (fsr_value > FSR_THREASHOLD && fetch_fsr_data) {
    #ifdef DEBUGGING
    Serial.print("Pressed fsr sensor with a value of:");
    Serial.println(fsr_value);
    #endif
    int file = random(SND_RANDOM_MIN, SND_RANDOM_MAX);
    MP3player.playMP3(filename[file]);
  }
}
