#pragma once
#include <ESPAsyncWebServer.h>
#include <NetWizard.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include "portal.h"
#include "prefs.h"
#include "email.h"

#ifndef WEB_SERVER_H
#define WEB_SERVER_H

extern AsyncWebServer server;

void initHandlers();

#endif