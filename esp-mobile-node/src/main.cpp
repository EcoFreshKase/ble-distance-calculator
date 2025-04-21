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
unsigned long experimentId = 0;

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

  Serial.printf("%s is ready for action.\n\n", DEVICE_NAME);
}

void loop() {


  // Start Experiment Section
  if (!experimentOngoing) {
    boolean buttonState = digitalRead(BUTTON_PIN) == LOW;
    if (!lastButtonState && buttonState && !experimentOngoing) {
      Serial.println("Starting Experiment.");
      Serial.println("id,start_time,end_time,num_of_passes,");

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
      DEBUG_PRINT("Experiment completed after %dms.\n", millis() - experimentStartTime);
      Serial.printf("%d,%lu,%lu,%d,\n", experimentId, experimentStartTime, millis(), experimentCounter);
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
#ifdef NON_STOP_EXPERIMENT
  experimentOngoing = true;
#else
experimentOngoing = false;
#endif

  experimentCounter = 0;
  experimentStartTime = millis();
  experimentId++;
  SerialBT.flush();
}
