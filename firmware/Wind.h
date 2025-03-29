#include <Arduino.h>

#include "common.h"

#define WIND_PERIOD_SECONDS 600
#define WIND_SAMPLE_INTERVAL_SECONDS 5
#define WIND_HISTORY_SIZE WIND_PERIOD_SECONDS / WIND_SAMPLE_INTERVAL_SECONDS
#define WIND_NAN -1
#define WIND_PIN 0

#define RAW2SPEED(raw) (raw / 1023. * 108.)

class Wind {
	public:
		static Wind &i();
		float avgRaw();
		int maxRaw();
		float speed();
		float gust();
		void update(bool force = false);

	private:
		Wind();
		~Wind();
		Wind(const Wind&);
		Wind& operator=(const Wind&);

		int readRaw();
		int m_historyRaw[WIND_HISTORY_SIZE];
		mytime_t m_historyTime[WIND_HISTORY_SIZE];
};
