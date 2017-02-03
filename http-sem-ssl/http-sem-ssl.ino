#include <DHT.h>
#include <DHT_U.h>
#include <Adafruit_Sensor.h>
#include <ESP8266WiFi.h>

typedef struct TSensor{
  float value;
  int sensor_type_id;
}TSensor;

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
DHT dht(DHTPIN, DHTTYPE);
void reconnect() {
  // Loop eterno até conectar no mqtt broker/wifi
  while (espClient.connected()) {
    Serial.print("Tentando conectar...");
    delay(5000);
    if(!espClient.connected())
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

void postResult(WiFiClient client, int value){
  
}
String postOnWebService(WiFiClient client, TSensor sensor){
    String request = "";
    String json = "";
    
    //Serial.println("Faz POST");

    json  = "{\r\n\t\"value\" : \"" + String(sensor.value)    + "\",\r\n";
    json += "\t\"sensor_type_id\" : \"" + String(sensor.sensor_type_id) +  "\"\r\n}\r\n";
  
    request += "POST /sensor_reads"; 
    request += " HTTP/1.1\r\n";
    request += "Host: cacc.unioeste-foz.br:3000\r\n";
    request += "User-Agent: Arduino/1.0\r\n";
    request += "Content-Type: application/json\r\n";
    request += "Content-Length: "+ String(json.length())+"\r\n";
    request += "Connection: close\r\n\r\n";
    request+=json;
    if(client.connect("cacc.unioeste-foz.br",3000)){   
      //Se o arduino conseguiu se conectar no server, então começa a pegar os dados
      if(client.connected()){
         client.println(request);
         //Serial.println(request);  
      }
    }u
  return json;     
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
  
  
  //String postOnWebService(WiFiClient client, TSensor sensor){
  TSensor temperature;
  temperature.sensor_type_id = 3;
  temperature.value = t;
  
  TSensor humidity;
  humidity.sensor_type_id = 2;
  humidity.value = h;

  unsigned long begin = micros();
  postOnWebService(espClient,temperature);
  postOnWebService(espClient,humidity);
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

