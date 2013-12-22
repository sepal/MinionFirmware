
/**
 * Initializes the mp3 and sd card lib and fetches all sound files
 * from the sd card.
 */
boolean initSound(int volume) {
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

    // Does the filename start with char '1' through '5'?      
    if (tempfilename[0] >= '1' && tempfilename[0] <= '5')
    {
      // Yes! subtract char '1' to get an index of 0 through 4.
      index = tempfilename[0] - '1';
      
      // Copy the data to our filename array.
      strcpy(filename[index],tempfilename);
  
      Serial.print(F("found a file with a leading "));
      Serial.print(index+1);
      Serial.print(F(": "));
      Serial.println(filename[index]);
    }
      
    file.close();
  }

  // Set the VS1053 volume. 0 is loudest, 255 is lowest (off):
  MP3player.setVolume(volume, volume);
  
  // Turn on the amplifier chip:
  digitalWrite(EN_GPIO1, HIGH);
  delay(2);
}
