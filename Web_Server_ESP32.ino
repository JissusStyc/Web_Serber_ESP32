#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include "Wifi_Config.h"

String server_text = "";

void setup() {
  Serial.begin(115200);

  wifi_setup();
  OTA_setup();
  server_config();
}

void loop() {
  ArduinoOTA.handle();
  server_text = paginaInicio + String(hallRead()) + paginaFin;
  server_print(server_text);
}
