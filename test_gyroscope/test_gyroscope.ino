
#include <Wire.h>

#define MPU_ADDR 0x68

// Read one register
bool readRegister(uint8_t reg, uint8_t &value) {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(reg);

  if (Wire.endTransmission(false) != 0) {
    return false;
  }

  if (Wire.requestFrom(MPU_ADDR, 1) != 1) {
    return false;
  }

  value = Wire.read();
  return true;
}

// Write to one register
void writeRegister(uint8_t reg, uint8_t value) {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(reg);
  Wire.write(value);
  Wire.endTransmission();
}

// Read multiple registers
bool readBytes(uint8_t reg, uint8_t *buffer, int length) {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(reg);

  if (Wire.endTransmission(false) != 0) {
    return false;
  }

  if (Wire.requestFrom(MPU_ADDR, length) != length) {
    return false;
  }

  for (int i = 0; i < length; i++) {
    buffer[i] = Wire.read();
  }

  return true;
}

int16_t toInt16(uint8_t high, uint8_t low) {
  return (int16_t)((high << 8) | low);
}

void setup() {
  Serial.begin(115200);
  delay(1500);

  Serial.println("Hydration Homie - GY-6500 Test");

  Wire.begin();
  Wire.setClock(100000);

  // Check sensor identity
  uint8_t whoami = 0;

  if (!readRegister(0x75, whoami)) {
    Serial.println("ERROR: Sensor not detected!");
    return;
  }

  Serial.print("WHO_AM_I: 0x");
  Serial.println(whoami, HEX);

  // Wake up MPU6500
  writeRegister(0x6B, 0x00);
  delay(100);

  // Accelerometer range: +/- 2g
  writeRegister(0x1C, 0x00);

  // Gyroscope range: +/- 250 degrees/s
  writeRegister(0x1B, 0x00);

  Serial.println("Sensor initialized!");
}

void loop() {
  uint8_t data[14];

  if (!readBytes(0x3B, data, 14)) {
    Serial.println("ERROR: Failed to read sensor");
    delay(500);
    return;
  }

  int16_t ax = toInt16(data[0], data[1]);
  int16_t ay = toInt16(data[2], data[3]);
  int16_t az = toInt16(data[4], data[5]);

  int16_t gx = toInt16(data[8], data[9]);
  int16_t gy = toInt16(data[10], data[11]);
  int16_t gz = toInt16(data[12], data[13]);

  // Convert raw values
  float accelX = ax / 16384.0;
  float accelY = ay / 16384.0;
  float accelZ = az / 16384.0;

  float gyroX = gx / 131.0;
  float gyroY = gy / 131.0;
  float gyroZ = gz / 131.0;

  // Calculate total acceleration magnitude
  float magnitude = sqrt(
    accelX * accelX +
    accelY * accelY +
    accelZ * accelZ
  );

  Serial.println("---------------------------");

  Serial.print("Accel X: ");
  Serial.print(accelX, 2);
  Serial.print(" g | Y: ");
  Serial.print(accelY, 2);
  Serial.print(" g | Z: ");
  Serial.println(accelZ, 2);

  Serial.print("Gyro X: ");
  Serial.print(gyroX, 2);
  Serial.print(" deg/s | Y: ");
  Serial.print(gyroY, 2);
  Serial.print(" deg/s | Z: ");
  Serial.println(gyroZ, 2);

  Serial.print("Acceleration magnitude: ");
  Serial.print(magnitude, 2);
  Serial.println(" g");

  delay(500);
}
