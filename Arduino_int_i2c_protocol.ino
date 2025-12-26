#include <Wire.h>

#define I2C_ADDR 0x08

volatile int32_t latestValue = 0;
volatile bool hasNew = false;

void onRequest() {
  int32_t v = latestValue;

  Wire.write((uint8_t*)&v, sizeof(v));

  uint8_t flag = hasNew ? 1 : 0;
  Wire.write(flag);
  hasNew = false;
}

void setup() {
  Serial.begin(9600);
  while (!Serial) {}

  Wire.begin(I2C_ADDR);
  Wire.onRequest(onRequest);

  Serial.println("UNO ready. Type a number then press Enter.");
}

void loop() {
  if (Serial.available()) {
    String s = Serial.readStringUntil('\n');
    s.trim();
    if (s.length() > 0) {
      long v = s.toInt();
      latestValue = (int32_t)v;
      hasNew = true;

      Serial.print("Stored value = ");
      Serial.println((int32_t)v);
    }
  }
}
