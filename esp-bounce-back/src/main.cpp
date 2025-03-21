#include <Arduino.h>
#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

boolean isNewLine(int data);
int incrementData(int data);

void setup() {
  Serial.begin(115200);
  SerialBT.begin("esp-bounce-back");
  Serial.println("Device is ready for connection.");
}

void loop() {
  if (SerialBT.available()) {

    int data = SerialBT.read();
    Serial.println("Received: " + String(data));
    SerialBT.write(incrementData(data));
  }
  delay(25);
}

int incrementData(int data) {
  return isNewLine(data) ? data : (data + 1) % 256;
}

boolean isNewLine(int data) {
  return data == 13 || data == 10;
}
