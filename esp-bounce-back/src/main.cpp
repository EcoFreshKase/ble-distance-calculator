#include <Arduino.h>
#include "BluetoothSerial.h"
#include "main.h"

BluetoothSerial SerialBT;

boolean isNewLine(int data);

void setup() {
  Serial.begin(115200);
  SerialBT.begin(DEVICE_NAME);
  Serial.println("Device is ready for connection.");
}

void loop() {
  if (SerialBT.available()) {
    int data = SerialBT.read(); 
    if (!isNewLine(data)) {
      SerialBT.write(data);
    }
    DEBUG_PRINT(data);
  }
}

boolean isNewLine(int data) {
  return data == 13 || data == 10;
}
