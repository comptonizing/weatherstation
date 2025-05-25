#include "Watchdog.h"

static mytime_t __lastReset = 0;
static mytime_t __resetInterval = mytime_max;

void setWatchdogResetTimer(mytime_t interval) {
	__resetInterval = interval;
}

void(* resetFunc) (void) = 0;

ISR(WDT_vect) {
	mytime_t time = millis();
	if ( (time - __lastReset) < __resetInterval ) {
		return;
	}
	resetFunc();
}

void watchdogEnable() {
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
    MCUSR &= ~_BV(WDRF);
    WDTCSR |= _BV(WDCE) | _BV(WDE);
    WDTCSR = _BV(WDP0) | _BV(WDP3);
    WDTCSR |= (1 << WDIE);
  }
}

void watchdogReset() {
#ifdef WATCHDOG_ENABLE
	wdt_reset();
	__lastReset = millis();
#endif
}

void watchdogDisable() {
	wdt_disable();
}
