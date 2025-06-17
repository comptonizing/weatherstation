#include <Arduino.h>

#include "common.h"
#include "BME280.h"
#include "IR.h"
#include "Rain.h"
#include "SQM.h"
#include "Wind.h"
#include "Watchdog.h"


void updateAll() {
  watchdogReset();
  BME280::i().update();
  watchdogReset();
  Rain::i().update();
  watchdogReset();
  SQM::i().update();
  watchdogReset();
  Wind::i().update();
  watchdogReset();
}

void sendBME280() {
  auto i = &(BME280::i());
  sendValue(F("Environment Temperature: "), i->temperature());
  sendValue(F("Environment Pressure: "), i->pressure());
  sendValue(F("Environment Humidity: "), i->humidity());
  sendValue(F("Environment Dewpoint: "), i->dewpoint());
}

void sendIR() {
  auto i = &(IR::i());
  sendValue(F("IR sky: "), i->skyTemp());
  sendValue(F("IR ambient: "), i->ambientTemp());
  sendValue(F("IR diff: "), BME280::i().temperature() - i->skyTemp());
}

void sendRain() {
  auto i = &(Rain::i());
  sendValue(F("Rain capacitance: "), i->capacitance());
  sendValue(F("Rain percentage: "), i->percentage());
  sendValue(F("Rain temperature: "), i->temperature());
  sendValue(F("Rain heating: "), i->heatingPower());
  sendValue(F("Rain dutycycle: "), i->heatingDutyCycle());
}

void sendSQM() {
  auto i = &(SQM::i());
  sendValue(F("SQM ir: "), i->ir());
  sendValue(F("SQM full: "), i->full());
  sendValue(F("SQM vis: "), i->vis());
  sendValue(F("SQM mpsas: "), i->mpsas());
  sendValue(F("SQM dmpsas: "), i->dmpsas());
  sendValue(F("SQM integration: "), i->integration());
  sendValue(F("SQM gain: "), i->gain());
}

void sendWind() {
  auto i = &(Wind::i());
  sendValue(F("Wind speed: "), i->speed());
  sendValue(F("Wind gust: "), i->gust());
}

void sendAll() {
  sendBME280();
  sendIR();
  sendRain();
  sendSQM();
  sendWind();
}

void setup() {
  Serial.begin(115200);
  Serial.println("");
  sendValue(F("Startup: "), 1);
#ifdef WATCHDOG_ENABLE
  watchdogEnable();
  setWatchdogResetTimer(WATCHDOG_TIMEOUT);
  watchdogReset();
#endif
  updateAll();
}

void loop() {
  updateAll();
  sendAll();
  watchdogReset();
  delay(DELAY);
}
