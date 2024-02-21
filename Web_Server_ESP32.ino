#include <WiFi.h>
#include <ESPmDNS.h>

//------------------Servidor Web en puerto 80---------------------

WiFiServer server(80);

//---------------------Credenciales de WiFi-----------------------

//const char* ssid = "Lizandro";
//const char* password = "baracunatana";
const char* ssid = "Manualidades Alexxxa";
const char* password = "Manualidadesalexxxa793579357935";
const char* nombre_host = "ambiental";

//---------------------VARIABLES GLOBALES-------------------------
int contconexion = 0;

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

//---------------------------SETUP--------------------------------

void setup() {
  Serial.begin(115200);
  Serial.println("");

  // Conexión WIFI
  WiFi.begin(ssid, password);
  //Cuenta hasta 50 si no se puede conectar lo cancela
  while (WiFi.status() != WL_CONNECTED and contconexion < 50) {
    ++contconexion;
    delay(500);
    Serial.print(".");
  }
  if (contconexion < 50) {
    //para usar con ip fija
    //IPAddress ip(192,168,1,180);
    //IPAddress gateway(192,168,1,1);
    //IPAddress subnet(255,255,255,0);
    //WiFi.config(ip, gateway, subnet);

    Serial.println("");
    Serial.println("WiFi conectado");
    Serial.println(WiFi.localIP());
    server.begin();  // iniciamos el servidor
  } else {
    Serial.println("");
    Serial.println("Error de conexion");
  }
  if (!MDNS.begin(nombre_host)) {
    Serial.println("Error configurando mDNS!");
    while (1) {
      delay(1000);
    }
  }
  MDNS.addService("http", "tcp", 80);
  Serial.print(nombre_host);
  Serial.println(".local");
}

//----------------------------LOOP----------------------------------

void loop() {
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
            client.println(paginaInicio + String(hallRead()) + paginaFin);

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