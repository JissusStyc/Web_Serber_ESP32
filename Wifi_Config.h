#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

void wifi_setup();
void OTA_setup();
void server_config();
void server_print(String server_text);

WiFiServer server(80);

String header;  // Variable para guardar el HTTP request

//------------------------CODIGO HTML------------------------------
String paginaInicio = "<!DOCTYPE html>"
                      "<html>"
                      "<head>"
                      "<meta charset='utf-8' />"
                      "<META HTTP-EQUIV='Refresh' CONTENT='1'>"
                      "<title>Servidor Web ESP32</title>"
                      "</head>"
                      "<body>"
                      "<center>"
                      "<h3>Servidor Web ESP32</h3>";

String paginaFin = "</center>"
                   "</body>"
                   "</html>";

void wifi_setup() {
  Serial.println("Booting");
  WiFi.mode(WIFI_STA);
  //WiFi.begin("Lizandro", "baracunatana");
  WiFi.begin("Manualidades Alexxxa", "Manualidadesalexxxa793579357935");
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }
  // Port defaults to 3232
  // ArduinoOTA.setPort(3232);
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void OTA_setup() {
  //ArduinoOTA.setHostname("Jissus");  //Nombre del ESP32
  ArduinoOTA.setPassword("2991");  // Contraseña para cargar programas

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA
    .onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
      else  // U_SPIFFS
        type = "filesystem";

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      Serial.println("Start updating " + type);
    })
    .onEnd([]() {
      Serial.println("\nEnd");
    })
    .onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    })
    .onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });

  ArduinoOTA.begin();
}

void server_config() {
  if (!MDNS.begin("ESP32-ambiental")) {
    Serial.println("Error configurando mDNS!");
    while (1) {
      delay(1000);
    }
  }
  server.begin();  // iniciamos el servidor
  MDNS.addService("http", "tcp", 80);
  Serial.print("ESP32-ambiental");
  Serial.println(".local");
}

void server_print(String server_text) {

  WiFiClient client = server.available();  // Escucha a los clientes entrantes

  if (client) {                     // Si se conecta un nuevo cliente
    Serial.println("New Client.");  //
    String currentLine = "";        //
    while (client.connected()) {    // loop mientras el cliente está conectado
      if (client.available()) {     // si hay bytes para leer desde el cliente
        char c = client.read();     // lee un byte
        Serial.write(c);            // imprime ese byte en el monitor serial
        header += c;
        if (c == '\n') {  // si el byte es un caracter de salto de linea
          // si la nueva linea está en blanco significa que es el fin del
          // HTTP request del cliente, entonces respondemos:
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            // Muestra la página web
            client.println(server_text);

            // la respuesta HTTP temina con una linea en blanco
            client.println();
            break;
          } else {  // si tenemos una nueva linea limpiamos currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // si C es distinto al caracter de retorno de carro
          currentLine += c;      // lo agrega al final de currentLine
        }
      }
    }
    // Limpiamos la variable header
    header = "";
    // Cerramos la conexión
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}