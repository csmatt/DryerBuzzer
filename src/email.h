#pragma once

#define ENABLE_SMTP true // Allows SMTP class and data
// #define ENABLE_DEBUG
//  #define ENABLE_DEBUG true // Allows debugging
//  #define READYMAIL_DEBUG_PORT Serial

// If message timestamp and/or Date header was not set,
// the message timestamp will be taken from this source, otherwise
// the default timestamp will be used.
/* #if defined(ESP32) || defined(ESP8266)
#define READYMAIL_TIME_SOURCE time(nullptr); // Or using WiFi.getTime() in WiFiNINA and WiFi101 firmwares.
#endif */
#include "prefs.h"
#include <ReadyMail.h>
#include <WiFiClientSecure.h>

void sendEmail();
