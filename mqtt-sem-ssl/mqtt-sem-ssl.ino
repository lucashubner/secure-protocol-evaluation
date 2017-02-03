#include <DHT.h>
#include <DHT_U.h>

#include <Adafruit_Sensor.h>

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define wifi_ssid "Adercleison"
#define wifi_password "dionecleide2015"

#define mqtt_server "cacc.unioeste-foz.br"
#define mqtt_user "lucas"
#define mqtt_password "123456"

#define humidity_topic "lucas/humidity"
#define temperature_topic "lucas/temperature"

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
WiFiClient espClient;
PubSubClient client(espClient);
DHT dht(DHTPIN, DHTTYPE);

void reconnect() {
  // Loop eterno até conectar no mqtt broker/wifi
  while (!client.connected()) {
    Serial.print("Tentando conectar...");
    // Para conectar sem senha utilizar o codigo abaixo.
    // if (client.connect("ESP8266Client")) {
    if (client.connect("ESP8266Client", mqtt_user, mqtt_password)) {
      Serial.println("Conectado");
    } else {
      Serial.print("Falhou, rc=");
      Serial.print(client.state());
      Serial.println(" Tentando novamente em 5 segundos...");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
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

// Conecta na Wifi e no MQTT Broker
  setup_wifi();
  client.setServer(mqtt_server, 1883);

  dht.begin();
}


void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
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
  unsigned long begin = micros();
  client.publish(temperature_topic, String(t).c_str(), true);
  client.publish(humidity_topic, String(h).c_str(), true);
  unsigned long end = micros();
  Serial.print("Time spend in micros: ");
  Serial.println(end - begin);
  
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print("\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.println();
}

