#include "prefs.h"

Preferences preferences;

void initPreferences(boolean readOnly = true)
{
  // Start Preferences
  preferences.begin("dryer-buzzer", readOnly);
}

void commitPreferences()
{
  preferences.end();
}