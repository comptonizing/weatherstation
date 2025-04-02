#include "SQM.h"

SQM &SQM::i() {
	static SQM theInstance;
	return theInstance;
}

SQM::SQM() {
	m_tsl = new SQM_TSL2591(2591);
	m_tsl->begin();
	m_tsl->config.gain = TSL2591_GAIN_LOW;
	m_tsl->config.time = TSL2591_INTEGRATIONTIME_200MS;
	m_tsl->configSensor();
	m_tsl->setCalibrationOffset(0.0);
	m_tsl->verbose = false;
	update(true);
}

SQM::~SQM() {
}

uint16_t SQM::ir() {
	return m_tsl->ir;
}

uint16_t SQM::full() {
	return m_tsl->full;
}

uint16_t SQM::vis() {
	return m_tsl->vis;
}

double SQM::mpsas() {
	return m_tsl->mpsas;
}

double SQM::dmpsas() {
	return m_tsl->dmpsas;
}

float SQM::integration() {
	return m_tsl->integrationValue;
}

float SQM::gain() {
	return m_tsl->gainValue;
}

void SQM::update(bool force) {
	mytime_t time = millis();
	if ( (time - m_lastReading) < SQM_SAMPLE_INTERVAL_SECONDS * THOUSAND && ! force ) {
		return;
	}
	m_tsl->takeReading();
	m_lastReading = time;
}
