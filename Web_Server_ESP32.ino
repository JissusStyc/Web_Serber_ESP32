#include <Wire.h>
#include <Adafruit_BMP280.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>

WebServer server(80);

Adafruit_BMP280 bmp;

IPAddress local_IP(192, 168, 20, 100);
IPAddress gateway(192, 168, 20, 1);
IPAddress subnet(255, 255, 0, 0);

const String HTML = R"====(
<!DOCTYPE html>
<html>
<head>
  <title>Servidor Web ESP32</title>
    <style>
      body { background-color: #464444; font-family: Arial, sans-serif; color: #000; margin: 2%; padding: 0; }
        .column {
          width: 100%;
          height: 50%;
          box-sizing: border-box;
          padding: 15px;
          text-align: center;
        }
        #column1 {
          background-color: #8A8A8A;
        }
        #column2 {
          background-color: #B8B8B8;
        }
        #column3 {
          background-color: #B8B8B8;
        }
        @media (min-width: 600px) {
          .column {
            width: auto;
            height: auto;
          }
        }
    </style>
  </head>
<body>
  <center>
    <div class="column" id="column1">
      <h1>
        Servidor Web ESP32
      </h1>
    </div>
    <div class="column" id="column2">
      <h2>
        Lectura del Sensor de Efecto Hall:
      </h2>
      <h3>
        <span id="hallValue">0</span>
      </h3>
    </div>
    <div class="column" id="column3">
      <h2>
        Lectura del Sensor de Temperatura BMP280:
      </h2>
      <h3>
        <span id="bmpValue">0</span>
      </h3>
    </div>
  </center>
  <script>
    function updateValues() {
      fetch('/values').then(function(response) {
        return response.json();
      }).then(function(data) {
        document.getElementById('hallValue').textContent = data.hall;
        document.getElementById('bmpValue').textContent = data.bmp;
      });
    }
    setInterval(updateValues, 1000);
  </script>
</body>
</html>
)====";



void handleRoot() {
  server.send(200, "text/html", HTML);
}

void handleValues() {
  int hallValue = hallRead();
  float bmpValue = bmp.readTemperature();
  String json = "{\"hall\":";
  json += hallValue;
  json += ",\"bmp\":";
  json += bmpValue;
  json += "}";
  server.send(200, "application/json", json);
}

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
  if (!WiFi.config(local_IP, gateway, subnet)) {
    Serial.println("NO IP CONFIG");
  } else {
    Serial.println("IP CONFIG");
  }
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/values", handleValues);
  if (!MDNS.begin("ESP32_Jissus")) {
    Serial.println("mDNS Error");
    while (1) {
      delay(1000);
    }
  }else{Serial.println("mDNS Correct");}
  server.begin();
}

void loop(){
  server.handleClient();
}
