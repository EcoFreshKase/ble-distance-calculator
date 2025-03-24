#include <Arduino.h>
#include "BluetoothSerial.h"
#include "main.h"

BluetoothSerial SerialBT;
boolean lastButtonState = false;
boolean connected = false;

boolean experimentOngoing = false;
int experimentCounter = 0;
boolean experimentReceiving = false;
unsigned long experimentStartTime = 0;

boolean connectToBounceBackController();

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  Serial.begin(115200);
  SerialBT.begin(DEVICE_NAME, true);
  
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
    Serial.println("Starting Experiment.");
    experimentOngoing = true;
    experimentStartTime = millis();
  };
  lastButtonState = buttonState;

  // Experiment Section
  if (experimentOngoing) {

    // Is device currently waiting for a response
    if (experimentReceiving) {
      if (SerialBT.available()) {
        SerialBT.read();

        experimentCounter++;
        experimentReceiving = false;
      }
    } else {
      SerialBT.write(experimentCounter);
      experimentReceiving = true;
      experimentCounter++;
    }

    // End experiment
    if (experimentCounter > EXPERIMENT_NUM_SIGNALS) {
      Serial.printf("Experiment completed after %dms.\n", millis() - experimentStartTime);
      experimentOngoing = false;
      experimentReceiving = false;
      experimentCounter = 0;
      SerialBT.flush();
    }
  }
  delay(25);
}

boolean connectToBounceBackController() {
  Serial.println("Connecting to bounce back controller....");
  if (!SerialBT.connect(DEVICE_NAME_ESP_BOUNCE_BACK)) {
    Serial.println("Failed to connect to bounce back controller.");
    return false;
  };
  Serial.println("Connected to bounce back controller.");
  return true;
}
