#include <Arduino.h>
#include "BluetoothSerial.h"

BluetoothSerial SerialBT;
int BUTTON_PIN = 18;
boolean lastButtonState = false;

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  Serial.begin(115200);
  SerialBT.begin("esp-mobile-node");
  Serial.println("Device is ready for connection.");
}

void loop() {
  boolean buttonState = digitalRead(BUTTON_PIN) == LOW;
  if (!lastButtonState && buttonState) {
    Serial.print("Button is pressed.");
  };
  lastButtonState = buttonState;

  if (Serial.available()) {
    SerialBT.write(Serial.read());
  }
  if (SerialBT.available()) {
    Serial.write(SerialBT.read());
  }
  delay(25);
}
