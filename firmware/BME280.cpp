#include "BME280.h"

BME280 &BME280::i() {
	static BME280 theInstance;
	return theInstance;
}

BME280::BME280() {
	m_bme = new Adafruit_BME280();
	m_bme->begin(BME280ADDR);
	update(true);
}

BME280::~BME280() {
}

void BME280::update(bool force) {
	mytime_t time = millis();
	if ( (time - m_lastReading) < BME280_SAMPLE_INTERVAL_SECONDS * THOUSAND && ! force ) {
		return;
	}
	m_lastReading = time;
	sendLog("Taking BME280 ... ");
	Wire.beginTransmission(BME280ADDR);
	if ( Wire.endTransmission() ) {
		// Could not talk to BME
		m_lastTemperature = NAN;
		m_lastPressure = NAN;
		m_lastHumidity = NAN;
		m_lastDewpoint = NAN;
		return;
	}
	m_lastTemperature = m_bme->readTemperature();
	m_lastPressure = m_bme->readPressure() / 100.;
	m_lastHumidity = m_bme->readHumidity();
	if ( m_lastTemperature == NAN || m_lastHumidity == NAN ) {
		m_lastDewpoint = NAN;
		return;
	}
	float H = (log10(m_lastHumidity)-2)/0.4343 + (17.62*m_lastTemperature)/(243.12+m_lastTemperature);
	m_lastDewpoint = 243.12*H/(17.62-H);
	sendLog("done");
}

float BME280::temperature() {
	update();
	return m_lastTemperature;
}

float BME280::pressure() {
	update();
	return m_lastPressure;
}

float BME280::humidity() {
	update();
	return m_lastHumidity;
}

float BME280::dewpoint() {
	update();
	return m_lastDewpoint;
}
