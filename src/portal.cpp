#include "portal.h"

// Initialize NetWizard
NetWizard NW(&server);

void initPortal()
{
  NW.setStrategy(NetWizardStrategy::BLOCKING);

  // Start NetWizard
  NW.autoConnect("Dryer Buzzer", "");

  // Check if configured
  if (NW.isConfigured())
  {
    Serial.println("Device is configured");
    // Check if we are connected to WiFi
    if (NW.getConnectionStatus() == NetWizardConnectionStatus::CONNECTED)
    {
      // Print network details
      Serial.print("Connected to ");
      Serial.println(NW.getSSID());
      Serial.print("IP address: ");
      Serial.println(WiFi.localIP());
    }
    else
    {
      Serial.println("Not connected to any WiFi");
    }
  }
  else
  {
    Serial.println("Device is not configured");
  }
}