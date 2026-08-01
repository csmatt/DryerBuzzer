#pragma once
#include "web-server.h"
#define NW_ASYNC
#include <NetWizard.h>

#ifndef PORTAL_H
#define PORTAL_H
// Initialize NetWizard
extern NetWizard NW;
void initPortal();
#endif