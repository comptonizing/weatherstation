#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_BME280.h>

#include "common.h"

#define BME280ADDR 0x76
#define BME280_SAMPLE_INTERVAL_SECONDS 5

class BME280 {
	public:
		static BME280 &i();
		void update(bool force = false);
		float temperature();
		float pressure();
		float humidity();
		float dewpoint();
	private:
		BME280();
		~BME280();
		BME280(const BME280&);
		BME280& operator=(const BME280&);

		mytime_t m_lastReading = 0;
		Adafruit_BME280 *m_bme;
		float m_lastTemperature = NAN;
		float m_lastPressure = NAN;
		float m_lastHumidity = NAN;
		float m_lastDewpoint = NAN;
};
