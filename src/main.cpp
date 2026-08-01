#include "main.h"

const int LED_PIN = 3;              // D1;
const int VIBRATION_SENSOR_PIN = 4; // D2;
const int BUZZER_PIN = 5;           // D3;
const int CHARGING_PIN = 10;        // D10;
const int ADC_PIN = 2;              // A0;

const int BUZZER_TIME = 500;
const float CHARGING_VOLTAGE_MIN = 4;
const bool DISABLE_WHEN_CHARGING = true;
const long VIBRATING_DEBOUNCE_TIME = 500;

// TODO: not currently used
float getVoltage()
{
  uint32_t Vbatt = 0;
  for (int i = 0; i < 16; i++)
  {
    Vbatt = Vbatt + analogReadMilliVolts(A0); // ADC with correction
  }
  float voltage = 2 * Vbatt / 16 / 1000.0; // attenuation ratio 1/2, mV --> V
  Serial.println(voltage, 3);
  return voltage;
}

void displayIsCharging()
{
  digitalWrite(LED_PIN, HIGH);
  delay(500);
  digitalWrite(LED_PIN, LOW);
  delay(500);
}

boolean getIsCharging()
{
  // TODO: I don't think this is working as expected
  return digitalRead(CHARGING_PIN);
}

boolean previous = false;
boolean current = false;
boolean wasVibrating = false;

long startedVibratingTime = 0;
long lastVibrationTime = 0;
long stoppedVibratingTime = 0;

void setup()
{
  Serial.begin(115200);

  pinMode(VIBRATION_SENSOR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(ADC_PIN, INPUT);
  pinMode(CHARGING_PIN, INPUT);

  initPortal();
  initHandlers();
}

void loop()
{
  // NetWizard Loop Task
  NW.loop();

  if (getIsCharging())
  {
    if (!DISABLE_WHEN_CHARGING)
    {
      displayIsCharging();
      return;
    }
  }

  previous = current;
  current = digitalRead(VIBRATION_SENSOR_PIN) == 1;
  long now = millis();

  if (current != previous)
  {
    lastVibrationTime = now;
    if (startedVibratingTime == 0)
    {
      startedVibratingTime = now;
    }
    else
    {
      long hasBeenVibratingTime = now - startedVibratingTime;
      // Serial.print("hasBeenVibratingTime: ");
      // Serial.println(hasBeenVibratingTime);
      initPreferences(true);
      int startVibrationPreference = preferences.getString("start-vibration", String(VIBRATING_TIME_MIN)).toInt() * 1000;
      commitPreferences();
      if (hasBeenVibratingTime > startVibrationPreference)
      {
        digitalWrite(LED_PIN, HIGH);
        stoppedVibratingTime = 0;
        wasVibrating = true;
      }
    }
  }
  else if (wasVibrating)
  {
    if (stoppedVibratingTime == 0)
    {
      stoppedVibratingTime = now;
    }
    else
    {
      long hasNotBeenVibratingTime = now - stoppedVibratingTime;
      // Serial.print("hasNotBeenVibratingTime: ");
      // Serial.println(hasNotBeenVibratingTime);
      initPreferences(true);
      int stopVibrationPreference = preferences.getString("stop-vibration", String(NOT_VIBRATING_TIME_MIN)).toInt() * 1000;
      commitPreferences();
      if (hasNotBeenVibratingTime > stopVibrationPreference)
      {
        wasVibrating = false;
        startedVibratingTime = 0;
        digitalWrite(LED_PIN, LOW);
        digitalWrite(BUZZER_PIN, HIGH);
        delay(BUZZER_TIME);
        digitalWrite(BUZZER_PIN, LOW);
        sendEmail();
      }
    }
  }
  else
  {
    if (now - lastVibrationTime > VIBRATING_DEBOUNCE_TIME)
    {
      startedVibratingTime = 0;
    }
  }
}