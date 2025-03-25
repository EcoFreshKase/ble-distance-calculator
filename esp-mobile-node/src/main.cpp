#include <Arduino.h>
#include "BluetoothSerial.h"
#include "main.h"

BluetoothSerial SerialBT;
boolean lastButtonState = false;
boolean connected = false;

boolean experimentOngoing = false;
int experimentCounter = 1;
boolean experimentReceiving = false;
unsigned long experimentStartTime = 0;

boolean connectToBounceBackController();
void restartExperiment();

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


  // Start Experiment Section
  if (!experimentOngoing) {
    boolean buttonState = digitalRead(BUTTON_PIN) == LOW;
    if (!lastButtonState && buttonState && !experimentOngoing) {
      Serial.println("Starting Experiment.");
      experimentOngoing = true;
      experimentStartTime = millis();
    };
    lastButtonState = buttonState;
  }


  // Experiment Section
  if (experimentOngoing) {

    // Is device currently waiting for a response
    if (experimentReceiving) {
      if (SerialBT.available()) {
        int data = SerialBT.read();
        // if (data != experimentCounter - 1) {
        //   DEBUG_PRINT("Received signal %d, but expected %d. Restarting experiment....\n", data, experimentCounter - 1);
        //   restartExperiment();
        //   return;
        // }

        experimentCounter++;
        experimentReceiving = false;

        DEBUG_PRINT("Received signal %d.\n", experimentCounter);
      }
    } else {
      SerialBT.write(0b11111111);
      experimentReceiving = true;
      experimentCounter++;

      DEBUG_PRINT("Sent signal %d.\n", experimentCounter);
    }

    // End experiment
    if (experimentCounter >= EXPERIMENT_NUM_SIGNALS) {
      Serial.printf("Experiment completed after %dms.\n", millis() - experimentStartTime);
      restartExperiment();
    }
  }
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

void restartExperiment() {
  experimentOngoing = false;
  experimentReceiving = false;
  experimentCounter = 0;
  experimentStartTime = 0;
  SerialBT.flush();
}
