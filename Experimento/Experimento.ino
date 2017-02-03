#include <DHT.h>
#include <DHT_U.h>
#include <Adafruit_Sensor.h>
#include <ESP8266WiFi.h>

#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>

//#include "coap_client.h"
#include "esp8266Definitions.h"
#include "sensorStruct.h"

#include "coapFunctions.h"
#include "httpFunctions.h"
#include "mqttFunctions.h"

#define DHTPIN 13     // what digital pin we're connected to
#define DHTTYPE DHT22   // DHT 22

TSensor temperature;
TSensor humidity;
  
WiFiClientSecure espClientSecure;
WiFiClient espClient;

coapClient coap;
DHT dht(DHTPIN, DHTTYPE);

Adafruit_MQTT_Client mqtt(&espClient, mqtt_server, mqttPort, mqtt_user, mqtt_password);
Adafruit_MQTT_Client mqttSecure(&espClientSecure, mqtt_server, mqttPortSsl, mqtt_user, mqtt_password);

Adafruit_MQTT_Publish temperatureTopic = Adafruit_MQTT_Publish(&mqtt, temperature_topic);
Adafruit_MQTT_Publish humidityTopic = Adafruit_MQTT_Publish(&mqtt, humidity_topic);

Adafruit_MQTT_Publish temperatureTopicSec = Adafruit_MQTT_Publish(&mqttSecure, temperature_topic);
Adafruit_MQTT_Publish humidityTopicSec = Adafruit_MQTT_Publish(&mqttSecure, humidity_topic);

unsigned long begin, end;
bool error = false;

void setup() {
  Serial.begin(9600);
  //Serial.setDebugOutput(true);
  setup_wifi();
  dht.begin();

  // client response callback. this endpoint is single callback.
  coap.response(callback_response);
  coap.start(); // start coap client 

  temperature.sensor_type_id = 3;
  humidity.sensor_type_id = 2;
  
  Serial.println("-------------------------------------------------------------------------------");//------------------------------------------------");
  Serial.println("|   HTTP   \t| HTTP SSL \t|   MQTT   \t| MQTT SSL \t| COAP NoSec \t|");// COAP PreShar \t| COAP RawRub \t| COAP Cert |");
}

void loop() {

  humidity.value = dht.readHumidity();
  temperature.value = dht.readTemperature();

  // Verifica se o sensor retornou um resultado valido,
  // caso contrario, quebra o loop e volta ao inicio.
  //Serial.println("Verify if reads correctly");
  if (isnan(humidity.value) || isnan(temperature.value)) {
    //Serial.println("Falha ao ler sensor DHT!");
    delay(500);
    return;
  }

   Serial.print("|\t");
//--------------------------------------------------------------------------------------------//  
  // PROTOCOLO HTTP SEM SSL
  //Serial.println("Post HTTP no ssl");
  
  begin = micros();
  postOnWebService(espClientSecure,temperature,httpPort,false);
  end = micros();
  //TODO POST RESULT CARAI
  begin = micros();
  postOnWebService(espClientSecure,humidity,httpPort,false);
  end = micros();
  delay(500);
  Serial.print(end - begin);
  Serial.print("\t|\t");

  
//--------------------------------------------------------------------------------------------//
  
  // PROTOCOLO HTTP COM SSL
  //Serial.println("Post HTTP ssl");
  begin = micros();
  postOnWebService(espClientSecure,temperature,httpPortSsl,true);
  postOnWebService(espClientSecure,humidity,httpPortSsl,true);
  end = micros();
  Serial.print(end - begin);

  delay(500);
  
  Serial.print("\t|\t");


//--------------------------------------------------------------------------------------------//  
  // MQTT Sem SSL
  if(!mqtt.connected()){
    reconnectMqtt(&mqtt,1);
  }
  //Serial.println("mqtt no ssl");
  begin = micros();
  humidityTopic.publish(humidity.value);
  temperatureTopic.publish(temperature.value);
  end = micros();
  delay(500);
  
  Serial.print(end - begin);
  Serial.print("\t|\t");
//--------------------------------------------------------------------------------------------//  
  // MQTT Com SSL
  if(!mqttSecure.connected()){
    reconnectMqtt(&mqttSecure,2);
  }
  //Serial.println("mqtt ssl");
  begin = micros();
  if(verifyFingerprint(espClientSecure, mqttPortSsl)){
    humidityTopic.publish(humidity.value);
    temperatureTopic.publish(temperature.value);
    end = micros();
    Serial.print(end - begin);
  }
  else{
    Serial.print("ERROR");
  }
  delay(500);
  
  
  Serial.print("\t|\t");
  
//--------------------------------------------------------------------------------------------//  
  // COAP 
  sensorToJson(&temperature, &tJson);
  sensorToJson(&humidity, &hJson);
  // COAP NOSEC  
  //Serial.print("COAP NoSec");
  error = false;
  begin = micros();
  msgid = coap.post(ip, coapPort,"sensor_reads",(char*)tJson.c_str(),tJson.length());
  state = coap.loop();
  if(!state){
    Serial.print("E");
    error=true;
  }
  msgid = coap.post(ip, coapPort,"sensor_reads",(char*)hJson.c_str(),hJson.length());
  state = coap.loop();
  if(!state){
    Serial.print("E");
    error = true;
  }
  end = micros();
  if(!error){
    Serial.print(end - begin);
  }
  Serial.print("\t|\t");
  delay(1000);
//--------------------------------------------------------------------------------------------//
  /*
  // COAP PRESHARED
  //Serial.print("COAP Preshared");
  error = false;
  begin = micros();
  msgid = coap.post(ip, coapPortPreShared,"sensor_reads",(char*)tJson.c_str(),tJson.length());
  state = coap.loop();
  if(!state){
    Serial.print("E");
    error = true;
  }
  msgid = coap.post(ip, coapPortPreShared,"sensor_reads",(char*)hJson.c_str(),hJson.length());
  state = coap.loop();
  end = micros();  
  delay(500);
  if(!state){
    Serial.print("E");
    error = true;
  }
    if(!error){
    Serial.print(end - begin);
  }
  Serial.print("\t|\t ");
  //--------------------------------------------------------------------------------------------//
  // COAP RAWPUBLIC
  //Serial.print("COAP RawPub");
  error = false;
  begin = micros();
  msgid = coap.post(ip, coapPortRawPub,"sensor_reads",(char*)tJson.c_str(),tJson.length());
  state = coap.loop();
  if(!state){
    Serial.print("E");
    error = true;
  }
  msgid = coap.post(ip, coapPortRawPub,"sensor_reads",(char*)hJson.c_str(),hJson.length());
  state = coap.loop();
  end = micros();
  delay(500);
  if(!state){
    Serial.print("E");
    error = true;
  }
  if(!error){
    Serial.print(end - begin);
  }
  Serial.print("\t|\t ");
//--------------------------------------------------------------------------------------------//  
  // COAP CERTIFICATE
  //Serial.print("COAP Cert");
  error = false;
  begin = micros();
  msgid = coap.post(ip, coapPortCert,"sensor_reads",(char*)tJson.c_str(),tJson.length());
  state = coap.loop();
  if(!state){
    Serial.print("E");
    error = true;
  }
  msgid = coap.post(ip, coapPortCert,"sensor_reads",(char*)hJson.c_str(),hJson.length());
  state = coap.loop();
  end = micros();
  delay(500);
  if(!state){
    Serial.print("E");
    error = true;
  }
  if(!error){
    Serial.print(end - begin);
  }
  */  
  Serial.println("\t|");

}
