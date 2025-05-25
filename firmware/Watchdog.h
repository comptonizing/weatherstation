#pragma once

#include <Arduino.h>

#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <util/atomic.h>

#include "common.h"

void watchdogEnable();
void watchdogDisable();
void watchdogReset();
void setWatchdogResetTimer(mytime_t interval);
