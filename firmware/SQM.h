#include <Wire.h>
#include <Adafruit_Sensor.h>

#include "SQM_TSL2591.h"

#include "common.h"

#define SQM_SAMPLE_INTERVAL_SECONDS 5

class SQM {
	public:
		static SQM &i();
		void update(bool force = false);
		uint16_t ir();
		uint16_t full();
		uint16_t vis();
		double mpsas();
		double dmpsas();
		float integration();
		float gain();
	private:
		SQM();
		~SQM();
		SQM(const SQM&);
		SQM& operator=(const SQM&);

		mytime_t m_lastReading = 0;
		SQM_TSL2591 *m_tsl;
};
