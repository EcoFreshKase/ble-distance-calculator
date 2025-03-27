#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BluetoothSerial.h>
#include "main.h"

// Bluetooth Classic variables
BluetoothSerial SerialBT;
boolean lastButtonState = false;
boolean connected = false;

// BLE variables
BLEServer *Server = NULL;
BLECharacteristic *pCharacteristic;
BLEService *experimentService;
bool deviceConnected = false;

// Experiment variables
boolean experimentOngoing = false;
int experimentCounter = 1;
boolean experimentReceiving = false;
unsigned long experimentStartTime = 0;

boolean connectToBounceBackController();
void restartExperiment();
void startBleServer();
BLECharacteristic* createCharacteristicAndSetValue(BLEService* service, char* uuid, char* value);

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  Serial.begin(115200);
  SerialBT.begin(DEVICE_NAME, true);

  // Setup connection to bounce back controller //
  unsigned long startTime = millis();
  connected = connectToBounceBackController();
  while (!connected && (millis() - startTime) < 30000) {
    connected = connectToBounceBackController();
  }

  if (!connected) {
    Serial.println("Failed to connect to bounce back controller. Restarting device....");
    ESP.restart();
  }

  // Setup BLE Server for phone connection
  startBleServer();
  Serial.println("BLE Server Started!");


  Serial.println("Device is ready for action.");
}

void loop() {
  // BLE Server Communication

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

void startBleServer() {
  BLEDevice::init(DEVICE_NAME);
  Server = BLEDevice::createServer();
  BLEService *experimentService = Server->createService(EXPERIMENT_SERVICE_UUID);
  
  createCharacteristicAndSetValue(experimentService, "1", "Hello from ESP32!");
  
  experimentService->start();
  Server->getAdvertising()->start();
}


BLECharacteristic* createCharacteristicAndSetValue(BLEService* service, char* uuid, char* value) {
  BLECharacteristic* characteristic = service->createCharacteristic(
    uuid, 
    BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE
  );
  characteristic->setValue(value);

  return characteristic;
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
