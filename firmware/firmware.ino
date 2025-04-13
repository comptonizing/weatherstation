#include <Arduino.h>

#include "BME280.h"
#include "IR.h"
#include "Rain.h"
#include "SQM.h"
#include "Wind.h"

#define PREFIX "Weather: "
#define DELAY 500

void updateAll() {
  BME280::i().update();
  Rain::i().update();
  SQM::i().update();
  Wind::i().update();
}

template<typename T1, typename T2>
void sendValue(T1 thing, T2 value) {
  Serial.print(PREFIX);
  Serial.print(thing);
  Serial.println(value);
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
  updateAll();
}

void loop() {
  updateAll();
  sendAll();
  delay(DELAY);
}
