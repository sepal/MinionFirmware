// IMU helper variables.
int imu_sign[9] = {1,1,1,-1,-1,-1,1,1,1};
int imu_values[6];
int imu_offset[6]={0,0,0,0,0,0};


/**
 * Initializes the compass, which also holds the accelerometer.
 */
void accel_init() {
  compass.init();
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
}

/**
 * Reads the accelerometer registers from the compass
 * and saves them into accel_x, accel_y & accel_z.
 */
void read_accel() {
  compass.readAcc();

  imu_values[3] = compass.a.x >> 4; // shift left 4 bits to use 12-bit representation (1 g = 256)
  imu_values[4] = compass.a.y >> 4;
  imu_values[5] = compass.a.z >> 4;
  accel_x = imu_sign[3] * (imu_values[3] - imu_offset[3]);
  accel_y = imu_sign[4] * (imu_values[4] - imu_offset[4]);
  accel_z = imu_sign[5] * (imu_values[5] - imu_offset[5]);
}

