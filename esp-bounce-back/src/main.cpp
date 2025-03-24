#include <Arduino.h>
#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

boolean isNewLine(int data);

void setup() {
  Serial.begin(115200);
  SerialBT.begin("esp-bounce-back");
  Serial.println("Device is ready for connection.");
}

void loop() {
  if (SerialBT.available()) {
    int data = SerialBT.read(); 
    if (!isNewLine(data)) {
      SerialBT.write(data);
    }
  }
}

boolean isNewLine(int data) {
  return data == 13 || data == 10;
}
