#include "email.h"

WiFiClientSecure ssl_client;
SMTPClient smtp(ssl_client);

void sendEmail()
{
  ssl_client.setInsecure();

  // TODO: should make configurable
  smtp.connect("smtp.gmail.com", 465);

  Serial.println("Initializing preferences");
  initPreferences(true);
  Serial.println(smtp.isConnected());

  String senderUserNamePreference = preferences.getString("sender-username");
  String senderPasswordPreference = preferences.getString("sender-password");
  String recipientPreference = preferences.getString("email-to-notify");

  commitPreferences();
  Serial.println("ending preferences");

  smtp.authenticate(senderUserNamePreference, senderPasswordPreference, readymail_auth_password);

  SMTPMessage msg;
  msg.headers.add(rfc822_from, "DryerBuzzer <" + senderUserNamePreference + ">");
  msg.headers.add(rfc822_to, "Recipient <" + recipientPreference + ">");
  msg.headers.add(rfc822_subject, "Dryer Cycle Finished");
  msg.text.body("Dryer Cycle Finished");

  configTime(0, 0, "pool.ntp.org");
  while (time(nullptr) < 100000)
    delay(100);
  msg.timestamp = time(nullptr);

  smtp.send(msg);
}
