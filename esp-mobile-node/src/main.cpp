#include <Arduino.h>
#include "BluetoothSerial.h"
#include "main.h"

BluetoothSerial SerialBT;
int BUTTON_PIN = 18;
boolean lastButtonState = false;
boolean connected = false;

boolean connectToBounceBackController() {
  Serial.println("Connecting to bounce back controller....");
  if (!SerialBT.connect(DEVICE_NAME_ESP_BOUNCE_BACK)) {
    Serial.println("Failed to connect to bounce back controller.");
    return false;
  };
  Serial.println("Connected to bounce back controller.");
  return true;
}

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  Serial.begin(115200);
  SerialBT.begin("esp-mobile-node", true);
  
  // Setup connection to bounce back controller
  unsigned long startTime = millis();
  connected = connectToBounceBackController();
  while (!connected && (millis() - startTime) < 30000) {
    connected = connectToBounceBackController();
  }

  if (!connected) {
    Serial.println("Failed to connect to bounce back controller. Restarting device....");
    ESP.restart();
  }  

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
