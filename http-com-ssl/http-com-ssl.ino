#include <DHT.h>
#include <DHT_U.h>
#include <Adafruit_Sensor.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

typedef struct TSensor {
  float value;
  int sensor_type_id;
} TSensor;

#define wifi_ssid "Adercleison"
#define wifi_password "dionecleide2015"

// Define pinos do ESP8266 - NodeMCU
#define D0 16
#define D1 5
#define D2 4
#define D3 0
#define D4 2
#define D5 14
#define D6 12
#define D7 13
#define D8 15
#define D9 3
#define D10 1

#define DHTPIN 13     // what digital pin we're connected to
#define DHTTYPE DHT22   // DHT 11

// Variaveis Globais
//WiFiClient espClient;
WiFiClientSecure espClient;
HTTPClient http;

DHT dht(DHTPIN, DHTTYPE);
void reconnect() {
  // Loop eterno até conectar no mqtt broker/wifi
  while (espClient.connected()) {
    Serial.print("Tentando conectar...");
    delay(5000);
    if (!espClient.connected())
      break;
  }
  Serial.println("Conectado");
}

void setup_wifi() {
  delay(10);
  // Conecta na WiFi
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(wifi_ssid);

  WiFi.begin(wifi_ssid, wifi_password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi conectada");
  Serial.println("Endereco IP: ");
  Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(9600);
  setup_wifi();
  dht.begin();
}


void loop() {
  if (!espClient.connected()) {
    reconnect();
  }
  delay(1000);

  float h = dht.readHumidity();
  float t = dht.readTemperature();

  // Verifica se o sensor retornou um resultado valido,
  // caso contrario, quebra o loop e volta ao inicio.
  if (isnan(h) || isnan(t)) {
    Serial.println("Falha ao ler sensor DHT!");
    return;
  }

  // Publica temperatura e humidade nos topicos do mqtt broker
  TSensor temperature;
  temperature.sensor_type_id = 3;
  temperature.value = t;

  TSensor humidity;
  humidity.sensor_type_id = 2;
  humidity.value = h;

  unsigned long begin = micros();


  
  http.begin("https://cacc.unioeste-foz.br:3001/sensor_reads", "6a 2b 89 93 fe da 57 a1 a7 fd 7d b5 8b 49 b9 57 22 f3 70 30");
  //http.begin("http://cacc.unioeste-foz.br:3000/sensor_reads");
 
  http.addHeader("Content-Type", "application/json");
  http.addHeader("Host", "cacc.unioeste-foz.br:3001");
  
  String json  = "{\"value\":\"" + String(temperature.value)  + "\",";
  json += "\"sensor_type_id\":\"" + String(temperature.sensor_type_id) +  "\"}";

  int httpCode = http.POST(json);
  String payload = http.getString();
  http.end();

  unsigned long end = micros();
  Serial.print("Time spend in micros: ");
  Serial.println(end - begin);
  Serial.println(httpCode);
  Serial.println(payload);

  delay(5000);
}

