#include <Adafruit_BMP280.h>
#include <WiFi.h>

Adafruit_BMP280 bmp;

void setup(){
  Serial.begin(115200);

  if (!bmp.begin()) {
    Serial.println("BMP280 ERROR");
    while (1);
  }

  //WiFi.begin("Lizandro", "baracunatana");
  //WiFi.begin("Manualidades Alexxxa", "Manualidadesalexxxa793579357935");
  WiFi.begin("NARIRA", "RiveraR1990");

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
}

void loop(){
  WiFiClient client;
  String bmpValue = String(bmp.readTemperature());

  if (client.connect("192.168.20.100", 80)) {
    client.println("GET /bmp?value=" + bmpValue + " HTTP/1.1");
    client.println("Host: 192.168.20.100");
    client.println("Connection: close");
    client.println();
  }
  delay(1000);
}