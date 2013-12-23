// IMU helper variables.
const int IMU_GRAVITY = 256;

// Holds the directions.
int imu_sign[9] = {1,1,1,-1,-1,-1,1,1,1};
// Raw values.
int imu_values[6];
// Calibration values.
int imu_offset[6]={0,0,0,0,0,0};

long imu_timer = 0;

int imu_shake_counter = 0;

long imu_shake_timer = 0;

long led_timer = 0;

/**
 * Initializes the compass, which also holds the accelerometer.
 */
boolean accel_init() {
  if (!compass.init()) {
    return true;
  }
  
  compass.enableDefault();
  
  switch (compass.getDeviceType())
  {
    case LSM303::device_D:
      compass.writeReg(LSM303::CTRL2, 0x18); // 8 g full scale: AFS = 011
      break;
    case LSM303::device_DLHC:
      compass.writeReg(LSM303::CTRL_REG4_A, 0x28); // 8 g full scale: FS = 10; high resolution output mode
      break;
    default: // DLM, DLH
      compass.writeReg(LSM303::CTRL_REG4_A, 0x30); // 8 g full scale: FS = 11
  }
  
  return false;
}



/**
 * Reads the accelerometer registers from the compass
 * and saves them into accel_x, accel_y & accel_z.
 */
void accel_read() {
  compass.readAcc();

  imu_values[3] = compass.a.x >> 4; // shift left 4 bits to use 12-bit representation (1 g = 256)
  imu_values[4] = compass.a.y >> 4;
  imu_values[5] = compass.a.z >> 4;
  
  accel_x = imu_sign[3] * (imu_values[3] - imu_offset[3]);
  accel_y = imu_sign[4] * (imu_values[4] - imu_offset[4]);
  accel_z = imu_sign[5] * (imu_values[5] - imu_offset[5]);
}

void imu_calibrate() {
  // Read some values.
  for(int i=0;i<32;i++)
  {
    accel_read();
    // Cumulate values
    for(int y=0; y<6; y++) 
      imu_offset[y] += imu_values[y];
    delay(20);
  }
  
  for(int y=0; y<6; y++)
    imu_offset[y] = imu_offset[y]/32;
  
  // Uncomment the following line if you want to substract the gravity from the offset fetched by the routine above. 
  //imu_offset[3] -= IMU_GRAVITY * imu_sign[3];
}

void imu_update() {
  // Run the imu fetching at 10 Hz.
  if (millis() - imu_timer >= 100 && !led_on) {
    imu_timer = millis();
    accel_read();
    
    if (abs(accel_x) >  IMU_SHAKE_THREASHOLD) {
      #ifdef DEBUGGING
      Serial.println("Shake it!");
      #endif
      imu_shake_counter++;
      imu_shake_timer = millis();
    }
    
    if (millis()-imu_shake_timer >= IMU_SHAKE_RESET_TIME && imu_shake_counter > 0) {
      #ifdef DEBUGGING
      Serial.print("Reseting shake counter.");
      #endif
      imu_shake_counter = 0;
      imu_shake_timer = millis();
      
      #ifdef DEBUGGING
      Serial.print(imu_shake_timer);
      Serial.println(millis());
      #endif
    }
      
    
    if (imu_shake_counter >= IMU_SHAKE_MIN) {
      #ifdef DEBUGGING
      Serial.println("Lighting up led and playing sound.");
      #endif
      digitalWrite(LED_PIN, HIGH);
      led_on = true;
      led_timer = millis();
      imu_shake_counter = 0;
      if (MP3player.isPlaying()) {
        MP3player.stopTrack();
      }
      MP3player.playMP3(filename[SND_SHAKE]);
    }
  }
  
  if (millis()-led_timer >= LED_ON_TIME && led_on) {
    #ifdef DEBUGGING
    Serial.println("Turning led off.");
    #endif
    led_on = false;
    digitalWrite(LED_PIN, LOW);
  }
}



