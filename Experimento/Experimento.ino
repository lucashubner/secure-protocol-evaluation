#include <DHT.h>
#include <DHT_U.h>
#include <Adafruit_Sensor.h>
#include <ESP8266WiFi.h>

#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>

#include "coap_client.h"
#include "esp8266Definitions.h"
#include "sensorStruct.h"

#include "coapFunctions.h"
#include "httpFunctions.h"
#include "mqttFunctions.h"

#define HOST "cacc.unioeste-foz.br"

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
  
  Serial.println("-------------------------------------------------------------------------------------------------------------------------------");
  Serial.println("|\t   HTTP   \t|\t HTTP SSL \t|\t   MQTT   \t|\t MQTT SSL \t|\t COAP NoSec \t|");
}

void loop() {

  humidity.value = dht.readHumidity();
  temperature.value = dht.readTemperature();

  // Verify if the value readed from DHT22 comes correctly
  // If it fails, the loop is breaked it tryes again.
  if (isnan(humidity.value) || isnan(temperature.value)) {
    delay(500);
    return;
  }

   Serial.print("|\t");
//--------------------------------------------------------------------------------------------//  
  // HTTP PROTOCOL WITHOUT SSL
  begin = micros();
  postOnWebService(espClientSecure,temperature,HOST, httpPort,false);
  end = micros();
  //TODO POST RESULT CARAI
  Serial.print(end - begin);
  Serial.print(" | ");
  begin = micros();
  postOnWebService(espClientSecure,humidity,HOST, httpPort,false);
  end = micros();
  delay(500);
  Serial.print(end - begin);
  Serial.print("\t|\t");

  
//--------------------------------------------------------------------------------------------//
  // HTTPS PROTOCOL
  begin = micros();
  postOnWebService(espClientSecure, temperature, HOST, httpPortSsl, true);
  end = micros();
  Serial.print(end - begin);
  Serial.print(" | ");
  
  begin = micros();
  postOnWebService(espClientSecure,humidity,HOST, httpPortSsl,true);
  end = micros();
  Serial.print(end - begin);
  delay(500);
  
  Serial.print("\t|\t");
//--------------------------------------------------------------------------------------------//  
  // MQTT WITHOUT SSL
  if(!mqtt.connected()){
    reconnectMqtt(&mqtt,1);
  }
  //Serial.println("mqtt no ssl");
  begin = micros();
  humidityTopic.publish(humidity.value);
  end = micros();
  Serial.print(end - begin);
  Serial.print(" | ");
  
  begin = micros();
  temperatureTopic.publish(temperature.value);
  end = micros();
  delay(500);
  
  Serial.print(end - begin);
  Serial.print("\t|\t");
//--------------------------------------------------------------------------------------------//  
  // MQTT WITH SSL
  if(!mqttSecure.connected()){
    reconnectMqtt(&mqttSecure,2);
  }
  //Serial.println("mqtt ssl");
  begin = micros();
  if(verifyFingerprint(espClientSecure, mqttPortSsl, HOST)){
    humidityTopic.publish(humidity.value);
    end = micros();
    Serial.print(end - begin);
    Serial.print(" | ");
  }
  else{
    Serial.print("ERROR");
  }
  begin = micros();
  if(verifyFingerprint(espClientSecure, mqttPortSsl, HOST)){
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
  // COAP NOSEC  
  sensorToJson(&temperature, &tJson);
  sensorToJson(&humidity, &hJson);
  
  begin = micros();
  msgid = coap.post(ip, coapPort,"sensor_reads",(char*)tJson.c_str(),tJson.length());
  state = coap.loop();
  end = micros();
  if(!state){
    Serial.print("ERROR");
  }
  else{
    Serial.print(end - begin);
  }

  begin = micros();
  msgid = coap.post(ip, coapPort,"sensor_reads",(char*)hJson.c_str(),hJson.length());
  state = coap.loop();
  end = micros();
  if(!state){
    Serial.print("ERROR");
  }
  else{
    Serial.print(end - begin);
  }
  Serial.println("\t|");
}
