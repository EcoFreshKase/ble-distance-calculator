#ifndef MAIN_H
#define MAIN_H

// #define DEBUG

#ifdef DEBUG
  #define DEBUG_PRINT(...) Serial.printf(__VA_ARGS__)
#else
  #define DEBUG_PRINT(...)
#endif

const char DEVICE_NAME[] = "esp-bounce-node-5m-2";

#endif