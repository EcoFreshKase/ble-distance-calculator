#ifndef MAIN_H
#define MAIN_H

// #define DEBUG

#ifdef DEBUG
  #define DEBUG_PRINT(...) Serial.printf(__VA_ARGS__)
#else
  #define DEBUG_PRINT(...)
#endif

const char DEVICE_NAME_ESP_BOUNCE_BACK[] = "esp-bounce-back";
const int BUTTON_PIN = 18;
const char DEVICE_NAME[] = "esp-mobile-node";

const int EXPERIMENT_NUM_SIGNALS = 10000;
const char EXPERIMENT_SERVICE_UUID[] = "1000";

#endif