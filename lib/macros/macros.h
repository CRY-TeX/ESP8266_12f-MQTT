#pragma once

#include "Arduino.h"

#define DEBUG 0

#if DEBUG
#define BEGIN_SERIAL() Serial.begin(9600)
#define LOG_SERIAL(x) Serial.println(x)
#else
#define BEGIN_SERIAL()
#define LOG_SERIAL(x)
#endif