#include "Wind.h"

Wind &Wind::i() {
	static Wind theInstance;
	return theInstance;
}

Wind::Wind() {
	for (int ii=0; ii<WIND_HISTORY_SIZE; ii++) {
		m_historyRaw[ii] = WIND_NAN;
		m_historyTime[ii] = 0;
	}
	update(true);
}

Wind::~Wind() {
}

int Wind::readRaw() {
	return analogRead(WIND_PIN);
}

void Wind::update(bool force) {
	mytime_t time = millis();

	if ( (time - m_historyTime[0]) < WIND_SAMPLE_INTERVAL_SECONDS * THOUSAND && ! force ) {
		// Not enough time passed, skip measurement
		return;
	}

	sendLog("Taking wind ... ");

	// Shift everything in the buffer back
	for (int ii=WIND_HISTORY_SIZE-1; ii>0; ii--) {
		m_historyRaw[ii] = m_historyRaw[ii-1];
		m_historyTime[ii] = m_historyTime[ii-1];
	}

	m_historyRaw[0] = readRaw();
	m_historyTime[0] = time;

	sendLog("done");
}

float Wind::avgRaw() {
	mytime_t time = millis();
	float avg = 0;
	int n = 0;
	for (int ii=0; ii<WIND_HISTORY_SIZE; ii++) {
		if (
				(time - m_historyTime[ii]) > WIND_PERIOD_SECONDS * THOUSAND ||
				m_historyRaw[ii] == WIND_NAN
		   ) {
			break;
		}
		avg += m_historyRaw[ii];
		n++;
	}
	avg /= n;
	return avg;
}

int Wind::maxRaw() {
	mytime_t time = millis();
	int thismax = 0;
	for (int ii=0; ii<WIND_HISTORY_SIZE; ii++) {
		if (
				(time - m_historyTime[ii]) > WIND_PERIOD_SECONDS * THOUSAND ||
				m_historyRaw[ii] == WIND_NAN
		   ) {
			break;
		}
		if ( m_historyRaw[ii] > thismax ) {
			thismax = m_historyRaw[ii];
		}
	}
	return thismax;
}

float Wind::speed() {
	update();
	return RAW2SPEED(avgRaw());
}

float Wind::gust() {
	update();
	return RAW2SPEED(maxRaw());
}
