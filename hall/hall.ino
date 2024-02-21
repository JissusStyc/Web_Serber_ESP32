#include <WiFi.h>

void setup(){
  Serial.begin(115200);

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
  String hallValue = String(hallRead());

  if (client.connect("192.168.20.100", 80)) {
    client.println("GET /hall?value=" + hallValue + " HTTP/1.1");
    client.println("Host: 192.168.20.100");
    client.println("Connection: close");
    client.println();
  }

  delay(1000);
}
