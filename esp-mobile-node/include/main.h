#ifndef MAIN_H
#define MAIN_H

// #define DEBUG
#define NON_STOP_EXPERIMENT

#ifdef DEBUG
  #define DEBUG_PRINT(...) Serial.printf(__VA_ARGS__)
#else
  #define DEBUG_PRINT(...)
#endif

const char DEVICE_NAME_ESP_BOUNCE_BACK[] = "esp-bounce-node-5m-2";
const int BUTTON_PIN = 18;
const char DEVICE_NAME[] = "esp-mobile-node-5m-2";

const int EXPERIMENT_NUM_SIGNALS = 1000;

#endif