#pragma once
#include <Preferences.h>

#ifndef PREFERENCES_H
#define PREFERENCES_H

extern Preferences preferences;
const long VIBRATING_TIME_MIN = 2;
const long NOT_VIBRATING_TIME_MIN = 120;
void initPreferences(boolean readOnly);
void commitPreferences();
#endif