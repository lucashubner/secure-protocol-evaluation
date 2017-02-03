/*
ESP-COAP Client
*/
#include <ESP8266WiFi.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Adafruit_Sensor.h>
#include "coap_client.h"

#define wifi_ssid "Adercleison"
#define wifi_password "dionecleide2015"

#define DHTPIN 13     // what digital pin we're connected to
#define DHTTYPE DHT22   // DHT 11

typedef struct TSensor{
  float value;
  int sensor_type_id;
}TSensor;
TSensor temperature;
TSensor humidity;

DHT dht(DHTPIN, DHTTYPE);
//instance for coapclient
coapClient coap;

//ip address and default port of coap server in which your interested in
IPAddress ip(179,106,223,24);//take ETH Zurich or coap.me server to run and check client 
int port = 3000;
long long errors = 0;
long long messagesSend = 0;
// coap client response callback
void callback_response(coapPacket &packet, IPAddress ip, int port);

// coap client response callback
void callback_response(coapPacket &packet, IPAddress ip, int port) {
    char p[packet.payloadlen + 1];
    memcpy(p, packet.payload, packet.payloadlen);
    p[packet.payloadlen] = NULL;

    //response from coap server
 if(packet.type==3 && packet.code==0){
      Serial.println("ping ok");
    }

    //Serial.println(p);
}

void sensorToJson(TSensor * sensor, String * json){
  *json += "{\"value\":\""+ String(sensor->value) +"\",\"sensor_type_id\":\""+ sensor->sensor_type_id+ "\"}";
}
void setup() {
   
    Serial.begin(9600);

    WiFi.begin(wifi_ssid, wifi_password);
    Serial.println(" ");

    // Connection info to WiFi network
    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(wifi_ssid);
    WiFi.begin(wifi_ssid, wifi_password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");
    // Print the IP address of client
    Serial.println(WiFi.localIP());

    // client response callback.
    // this endpoint is single callback.
    coap.response(callback_response);

    // start coap client
    coap.start();
    
    dht.begin();
    
  temperature.sensor_type_id = 3;
  humidity.sensor_type_id = 2;

}

void loop() {
  bool state;

  humidity.value = dht.readHumidity();
  temperature.value = dht.readTemperature();
  if (isnan(humidity.value) || isnan(temperature.value)) {
    Serial.println("Falha ao ler sensor DHT!");
    return;
  }
  delay(1000);
  String tJson = "";
  String hJson = "";
  
  sensorToJson(&temperature, &tJson); 
  sensorToJson(&humidity ,&hJson); 
  unsigned long begin = micros();
  
  int msgid = coap.post(ip, port,"sensor_reads",(char*)tJson.c_str(),tJson.length());
  msgid = coap.post(ip, port,"sensor_reads",(char*)hJson.c_str(),tJson.length());
  state = coap.loop();
  unsigned long end = micros();
  messagesSend++;
  if(!state){
    errors++;
    Serial.print("#########");
    Serial.print("#ERROR!#");
    Serial.print("#########");
    }
  double taxaErros = errors;
  taxaErros /=messagesSend;
  taxaErros *= 100;
  Serial.print("Taxa de erros de :");
  Serial.println (taxaErros);
  Serial.print("Time spend in micros: ");
  Serial.println(end - begin);
  
  Serial.print("Humidity: ");
  Serial.print(humidity.value);
  Serial.print("\t");
  Serial.print("Temperature: ");
  Serial.println(temperature.value);
}

