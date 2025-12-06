#pragma once

#include <Arduino.h>
#include <PID_v1.h>

#include "common.h"
#include "BME280.h"

#define RAIN_SAMPLE_INTERVAL_SECONDS 5
#define RAIN_PIN_CHARGE              7
#define RAIN_PIN_DISCHARGE           8
#define RAIN_PIN_VOLTAGE             7
#define RAIN_PIN_TEMPERATURE         1
#define RAIN_PIN_HEATING             9
#define RAIN_RESISTANCE              4.87e6
#define RAIN_THRESHOLD               648
#define RAIN_TIMEOUT_MICROS          10000
#define RAIN_TEMPERATURE_RESISTANCE  4700.
#define RAIN_CAPACITANCE_DRY_PF      300.
#define RAIN_CAPACITANCE_WET_PF      1000.
#define RAIN_TEMPERATURE_OFFSET      10
#define RAIN_MAX_TEMPERATURE         45
#define RAIN_MIN_TEMPERATURE         10

class Rain {
	public:
		static Rain &i();
		void update(bool force = false);
		float capacitance();
		float percentage();
		float temperature();
		float heatingPower();
		float heatingDutyCycle();

	private:
		Rain();
		~Rain();
		Rain(const Rain&);
		Rain& operator=(const Rain&);

		float R2T(float R);
		float readCapacitance();
		float capacitance2percentage(float capacitance);
		float readTemperature();

		mytime_t m_lastReading = 0;
		double m_lastTemperature = NAN;
		float m_lastCapacitance = NAN;
		float m_lastPercentage = NAN;
		double m_pidDutyCycle = 0;
		double m_targetTemperature = NAN;

		PID m_pid;
};
