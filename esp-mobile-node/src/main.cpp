#include <Arduino.h>
#include "BluetoothSerial.h"

BluetoothSerial SerialBT;
int BUTTON_PIN = 18;
boolean lastButtonState = false;

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  Serial.begin(115200);
  SerialBT.begin("esp-mobile-node", true);
  
  // Setup connection to bounce back controller
  if (!SerialBT.connect("esp-bounce-back")) {
    Serial.println("Failed to connect to bounce back controller.");
    abort();
  };
  Serial.println("Connected to bounce back controller.");

  Serial.println("Device is ready for action.");

}

void loop() {
  boolean buttonState = digitalRead(BUTTON_PIN) == LOW;
  if (!lastButtonState && buttonState) {
    Serial.print("Button is pressed.");
    SerialBT.write(97);
  };
  lastButtonState = buttonState;

  if (SerialBT.available()) {
    Serial.write(SerialBT.read());
  }
  delay(25);
}
