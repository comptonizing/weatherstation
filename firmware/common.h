#pragma once

#include <limits.h>
#include <Arduino.h>

#define mytime_t unsigned long
#define mytime_max ULONG_MAX
#define THOUSAND 1000UL

#define PREFIX "Weather: "
#define DELAY 500

#define WATCHDOG_ENABLE
#define WATCHDOG_TIMEOUT 120000

template<typename T1, typename T2>
void sendValue(T1 thing, T2 value) {
  Serial.print(PREFIX);
  Serial.print(thing);
  Serial.println(value);
}

template<typename T>
void sendLog(T thing) {
  Serial.print("LOG: ");
  Serial.println(thing);
}
