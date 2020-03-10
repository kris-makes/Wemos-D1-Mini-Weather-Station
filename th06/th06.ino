#include <Wire.h>

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Wire.begin();

}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(humidity());
  Serial.println(temp());
  delay(1000);
}

// Function for humidity measurement
int humidity() {
  float h = 0;
  int r;
  Wire.beginTransmission(0x40);
  Wire.write(0xE5);
  Wire.endTransmission();
  Wire.requestFrom(0x40, 2);
  if (Wire.available() > 0) {
    r = Wire.read();
    r = r << 8;
    r = r | Wire.read();
  }
  return h = (125.0 * r) / 65536.0 - 6.0;
}

// Function for temperature measurement
int temp() {
  float t = 0;
  int r;
  Wire.beginTransmission(0x40);
  Wire.write(0xE0);
  Wire.endTransmission();
  Wire.requestFrom(0x40, 2);
  if (Wire.available() > 0) {
    r = Wire.read();
    r = r << 8;
    r = r | Wire.read();
  }
  return t = (175.72 * r) / 65536.0 - 46.85;
}
