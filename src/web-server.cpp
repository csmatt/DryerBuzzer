#include "web-server.h"

// Create WebServer object on port 80
AsyncWebServer server(80);

const char *mdnsName = "dryerbuzzer";

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>Dryer Buzzer Configuration</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
</head>
<body>
  <h1 style="color: green;">%s</h1>
  <h2>Dryer Buzzer Configuration</h2>
  <form method="POST" action="/" style="display: flex; flex-direction: column;">
    <div>
      <label for="sender-username">From email username</label>
      <input type="email" id="sender-username" name="sender-username" value="%s" />
    </div>
    <div>
      <label for="sender-password">From email password</label>
      <input type="password" id="sender-password" name="sender-password" value="" />
    </div>
    <div>
      <label for="email-to-notify">Email address to notify</label>
      <input type="email" id="email-to-notify" name="email-to-notify" value="%s" />
    </div>
    <div>
      <label for="start-vibration">Start vibration buffer (s)</label>
      <input type="number" id="start-vibration" name="start-vibration" value="%s" />
    </div>
    <div>
      <label for="stop-vibration">Stop vibration buffer (s)</label>
      <input type="number" id="stop-vibration" name="stop-vibration" value="%s" />
    </div>
    <div>
      <button type="button" onclick="fetch('/email', {method: 'POST'});">Test Email</button>
      <button type="submit">Save</button>
      <button type="button" onclick="fetch('/', {method: 'DELETE'});">Reset Device</button>
      <button type="button" onclick="location.replace(location.href);">Cancel</button>
    </div>
  </form>
</body>
</html>
)rawliteral";

String fields[5] = {"sender-username", "sender-password", "email-to-notify", "start-vibration", "stop-vibration"};

void sendIndex(AsyncWebServerRequest *request, String notification)
{
  char page[1500];
  initPreferences(true);
  std::snprintf(page, sizeof(page), index_html,
                notification.c_str(),
                preferences.getString("sender-username", "").c_str(),
                preferences.getString("email-to-notify", "").c_str(),
                preferences.getString("start-vibration", String(VIBRATING_TIME_MIN)).c_str(),
                preferences.getString("stop-vibration", String(NOT_VIBRATING_TIME_MIN)).c_str());
  commitPreferences();
  request->send(200, "text/html", page);
}

void initHandlers()
{
  if (!MDNS.begin(mdnsName))
  {
    Serial.println("Error setting up MDNS responder!");
    while (1)
    {
      delay(1000);
    }
  }
  MDNS.addService("_http", "_tcp", 80);

  // Route for root / web page
  server.on("/", WebRequestMethod::HTTP_GET, [](AsyncWebServerRequest *request)
            { sendIndex(request, ""); });

  // Route for handling form POST
  server.on("/", WebRequestMethod::HTTP_POST, [](AsyncWebServerRequest *request)
            {
    initPreferences(false);
    for (int i = 0; i < 5; ++i) {
      if (request->hasParam(fields[i], true)) {
        String value = request->getParam(fields[i], true)->value();
        if (value != "") {
          preferences.putString(fields[i].c_str(), value);
        }
      }
    }
    commitPreferences();

    sendIndex(request, "Updated settings"); });

  // Route for handling reset
  server.on("/", WebRequestMethod::HTTP_DELETE, [](AsyncWebServerRequest *request)
            {
              initPreferences(false);
              for (int i = 0; i < 5; i++)
              {
                preferences.putString(fields[i].c_str(), "");
              }
              delay(50);
              preferences.putString("start-vibration", String(VIBRATING_TIME_MIN));
              preferences.putString("stop-vibration", String(NOT_VIBRATING_TIME_MIN));
              commitPreferences();
              sendIndex(request, "Cleared settings"); });

  // Route for testing email
  server.on("/email", WebRequestMethod::HTTP_POST, [](AsyncWebServerRequest *request)
            {
    //Serial.println("hit /email endpoint");
    sendEmail();
    sendIndex(request, "Email Sent"); });

  server.begin();
}