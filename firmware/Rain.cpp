#include "Rain.h"

Rain &Rain::i() {
	static Rain theInstance;
	return theInstance;
}

Rain::Rain() :
    m_pid(&m_lastTemperature, &m_pidDutyCycle, &m_targetTemperature, 4., 1., 1., DIRECT) {
	m_pid.SetMode(AUTOMATIC);
	analogWrite(RAIN_PIN_HEATING, 0);
	pinMode(RAIN_PIN_CHARGE, OUTPUT);
	update(true);
}

Rain::~Rain() {
}

float Rain::R2T(float R) {
	float T0 = 87.4292636;
	float R0 = 79.1795257;
	float beta = 0.04098201;
	return T0 - 1./beta * log(R / R0);
}

float Rain::readTemperature() {
	int raw = analogRead(RAIN_PIN_TEMPERATURE);
	float V = (raw * 5.) / 1024.;
	float R = (V * RAIN_TEMPERATURE_RESISTANCE) / (12. - V);
	return R2T(R);
}

float Rain::readCapacitance() {
	mytime_t start, elapsed;
	float capacitance;

	// Discharge capacitor
	digitalWrite(RAIN_PIN_CHARGE, LOW);
	pinMode(RAIN_PIN_DISCHARGE, OUTPUT);
	digitalWrite(RAIN_PIN_DISCHARGE, LOW);
	start = micros();
	while ( analogRead(RAIN_PIN_VOLTAGE) > 0 ) {
		if ( (micros() - start) >= RAIN_TIMEOUT_MICROS ) {
			return NAN;
		}
	}

	// Charge and count time
	pinMode(RAIN_PIN_DISCHARGE, INPUT);
	start = micros();
	digitalWrite(RAIN_PIN_CHARGE, HIGH);
	while ( analogRead(RAIN_PIN_VOLTAGE) < RAIN_THRESHHOLD ) {
		if ( (micros() - start) >= RAIN_TIMEOUT_MICROS ) {
			digitalWrite(RAIN_PIN_CHARGE, LOW);
			return NAN;
		}
	}
	elapsed = micros() - start;

	// Discharge capacitor
	digitalWrite(RAIN_PIN_CHARGE, LOW);
	pinMode(RAIN_PIN_DISCHARGE, OUTPUT);
	digitalWrite(RAIN_PIN_DISCHARGE, LOW);
	start = micros();
	while ( analogRead(RAIN_PIN_VOLTAGE) > 0 ) {
		if ( (micros() - start) >= RAIN_TIMEOUT_MICROS ) {
			return NAN;
		}
	}

	capacitance = ( (float) elapsed / RAIN_RESISTANCE ) * THOUSAND * THOUSAND;
	return capacitance;
}

float Rain::capacitance2percentage(float capacitance) {
	if ( isnan(capacitance) ) {
		return 100.;
	}
	if ( capacitance < RAIN_CAPACITANCE_DRY_PF ) {
		return 0.0;
	}
	float val = 100. * 
		(capacitance - RAIN_CAPACITANCE_DRY_PF) /
		(RAIN_CAPACITANCE_WET_PF - RAIN_CAPACITANCE_DRY_PF);
	return val > 100. ? 100. : val;
}

float Rain::heatingPower() {
	update();
	return 100. * m_pidDutyCycle / 255.;
}

void Rain::update(bool force) {
	m_lastTemperature = readTemperature();
	m_targetTemperature = BME280::i().temperature() + RAIN_TEMPERATURE_OFFSET;
	if ( m_targetTemperature > RAIN_MAX_TEMPERATURE ) {
		m_targetTemperature = RAIN_MAX_TEMPERATURE;
	}
	m_pid.Compute();
	analogWrite(RAIN_PIN_HEATING, m_pidDutyCycle);

	mytime_t time = millis();
	if ( (time - m_lastReading) < RAIN_SAMPLE_INTERVAL_SECONDS * THOUSAND && ! force ) {
		return;
	}

	m_lastCapacitance = readCapacitance();
	m_lastPercentage = capacitance2percentage(m_lastCapacitance);

	m_lastReading = time;
}

float Rain::capacitance() {
	update();
	return m_lastCapacitance;
}

float Rain::percentage() {
	update();
	return m_lastPercentage;
}

float Rain::temperature() {
	update();
	return m_lastTemperature;
}
