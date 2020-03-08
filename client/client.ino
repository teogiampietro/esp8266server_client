#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <DHT.h>    // importa la Librerias DHT
#include <DHT_U.h>

//variables globales
int SENSOR1 = 12;     // pin DATA de DHT22 a pin digital 2
float TEMPERATURA;
float HUMEDAD;
int MOVIMIENTO;
String NAME = "D1";

//constantes
const int KY017 = 13;
const int LED = 15;
const char* ssid = "ESP8266_SSID";
const char* password = "12345678";
const char* host = "192.168.2.1";
DHT dht(SENSOR1, DHT22);   // creacion del objeto, cambiar segundo parametro por DHT11 si se utiliza en lugar del DHT22
WiFiClient client;

void setup() {
  Serial.begin(115200);   // inicializacion de monitor serial
  pinMode(KY017, INPUT);
  pinMode(LED, OUTPUT);
  dht.begin();      // inicializacion de sensor

}

void loop() {
  if(WiFi.status() != WL_CONNECTED){
    conectarWifi();
  } 
  capturarValores();
  enviarValores();
  delay(10000);
}

void conectarWifi() {
  //conexión a la red wifi
  WiFi.begin(ssid, password);
  Serial.println();
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
}

void capturarValores() {
  HUMEDAD = dht.readHumidity();   // obtencion de valor de humedad
  MOVIMIENTO = digitalRead(KY017); // obtencion de valor del sensor movimiento
  TEMPERATURA = dht.readTemperature();  // obtencion de valor de temperatura
  Serial.println(TEMPERATURA);
  Serial.println(MOVIMIENTO);
  Serial.println(HUMEDAD);
}

void enviarValores() {
  if (client.connect(host, 80)) {
    String url = "/update?value1=";
    url += String(TEMPERATURA);
    url += "&value2=";
    url += String(HUMEDAD);
    url += "&value3=";
    url += String(MOVIMIENTO);
    url += "&value4=";
    url += NAME;
    url += " ";
    client.print(String("GET ") + url + "HTTP\1.1/r/n" + "Host: " + host + "\r\n" +
                 "Connection: keep-alive\r\n\r\n");
    delay(10);
    Serial.println("Response: ");
    while (client.available()) {
      String line = client.readStringUntil('\r');
      Serial.println(line);
    }
  }
}
//digitalWrite(LED, ESTADO);  // escribe valor leido en pin 3
