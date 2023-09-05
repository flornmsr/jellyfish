/*
 *  This sketch sends data via HTTP GET requests to data.sparkfun.com service.
 *
 *  You need to get streamId and privateKey at data.sparkfun.com and paste them
 *  below. Or just customize this script to talk to other HTTP servers.
 *
 */

#include <WiFi.h>

#include "web_service.h"

const char* ssid     = "Chickenhouse";
const char* password = "surfeninderWG1";




void connect_WiFi()
{ WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  { delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


void setup()
{
  Serial.begin(115200);
  connect_WiFi();
  initialize_web_server();
  initialize_web_server_loop();
  initialize_party_mode_loop();
  pinMode(JELLY_LED_PIN, OUTPUT);
  digitalWrite(JELLY_LED_PIN, HIGH); 

}

int value = 0;

void loop()
{

}

