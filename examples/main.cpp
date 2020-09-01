// Config2EEPROM config save and load to  EEPROM library.
//
// This is an example of howto save and load data
// to and from EEPROM using Config2EEPROM library.
// Tested with Platform.io and Arduino with ESP8266.
//
// Author: Max Kalashnikov
// Git Repo: https://github.com/geekbrother/config2eeprom

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <config2eeprom.hpp>

// Serial incoming
String incoming;
// Config class and map hash
config2eeprom c2e;

// =There are few overloads:
// -You can pass starting address of eeprom instance if you need
// config2eeprom c2e(256);

// -You can pass starting address of eeprom and document size,
// which is 254 bytes by default
// config2eeprom c2e(256, 2000);

staticConfigDoc configHash;

void setup()
{
  // Print info
  Serial.begin(9600);
  Serial.println("config2eeprom example.");
  Serial.println("Send 'S' command to save exmaple data to eeprom.");
  Serial.println("Send 'D' command to clear data from eeprom.");

  // Load config file and check is configured
  if (!c2e.load(configHash))
  {
    Serial.println("No config in EEPROM.");
  }
  else
  {
    Serial.println("Config found in EEPROM.");

    const String ssid = configHash["wifi_ssid"].as<String>();
    const String pass = configHash["wifi_pass"].as<String>();

    // Start wifi as STA
    Serial.println("Start wifi STA ssid, pass:");
    Serial.print(ssid);
    Serial.print(",");
    Serial.print(pass);
    wifi.startSTA(ssid.c_str(), pass.c_str());
  }
}

void loop()
{
  if (Serial.available() > 0)
  {
    // read the incoming
    incoming = Serial.readString();
    // say what you got:
    Serial.println(incoming);
    // Save example data
    if (incoming == "S")
    {
      // initialize staticConfigDoc which is ArduinoJSON doc type
      staticConfigDoc c2econfig;
      // Save wifi credentials to hash
      const char *ssid = "MyWIFI";
      const char *pass = "SomePass";
      c2econfig["wifi_ssid"] = ssid;
      c2econfig["wifi_pass"] = pass;
      c2e.save(c2econfig);
      // Restart esp
      ESP.restart();
    }
    // Clear eeprom
    if (incoming == "D")
    {
      c2e.clear();
      ESP.restart();
    }
  }
}
