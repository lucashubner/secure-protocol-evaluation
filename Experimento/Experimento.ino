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

#define DHTPIN 13     // what digital pin we're connected to
#define DHTTYPE DHT22   // DHT 22

String host = "cacc.unioeste-foz.br";

int httpReturnCode;
bool mqttPublished;
TSensor temperature;
TSensor humidity;

DHT dht(DHTPIN, DHTTYPE);

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
  
  Serial.println("-------------------------------------------------------------------------------------------------------------------------------------------------------");
  Serial.println("|\t   HTTP   \t\t|\t HTTP SSL \t\t|\t   MQTT   \t\t|\t MQTT SSL \t\t|\t COAP NoSec \t|");
}

void loop() {
  delay(1000);
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
  httpReturnCode = postOnWebService(espClientSecure,temperature,host, httpPort, false);
  end = micros();
  if(httpReturnCode == 200 || httpReturnCode == 302){
    postResult(end-begin, HTTP_PROTOCOL, host, 3000);
    Serial.print(end - begin);
  }  
  else{
    Serial.print("Error:");
    Serial.print(httpReturnCode);
  }
  Serial.print(" | ");
  begin = micros();
  httpReturnCode = postOnWebService(espClientSecure,humidity,host, httpPort,false);
  end = micros();
  if(httpReturnCode == 200 || httpReturnCode == 302){
    //postResult(end-begin, HTTP_PROTOCOL, host, 3000);
    Serial.print(end - begin);
  }  
  else{
    Serial.print("Error:");
    Serial.print(httpReturnCode);
  }
  delay(500);
  Serial.print("\t|\t");

//--------------------------------------------------------------------------------------------//
  // HTTPS PROTOCOL
  begin = micros();
  httpReturnCode = postOnWebService(espClientSecure, temperature, host, httpPortSsl, true);
  end  = micros();
  if(httpReturnCode == 200 || httpReturnCode == 302){
      Serial.print(end - begin);
    postResult(end-begin, HTTPS_PROTOCOL, host, 3000);
  }
    else{
    Serial.print("Error:");
    Serial.print(httpReturnCode);
  }

  Serial.print(" | ");
  
  begin = micros();
  httpReturnCode = postOnWebService(espClientSecure,humidity,host, httpPortSsl,true);
  end = micros();
  if(httpReturnCode == 200 || httpReturnCode == 302){
    //postResult(end-begin, HTTPS_PROTOCOL, host, 3000);
    Serial.print(end - begin);
  }
  else{
    Serial.print("Error:");
    Serial.print(httpReturnCode);
  }
  
  delay(500);
  
  Serial.print(" \t\t|\t");
//--------------------------------------------------------------------------------------------//  
  // MQTT WITHOUT SSL
  if(!mqtt.connected()){
    reconnectMqtt(&mqtt,1);
  }
  //Serial.println("mqtt no ssl");
  begin = micros();
  mqttPublished = humidityTopic.publish(humidity.value);
  end = micros();
  if(mqttPublished){
    //postResult(end-begin, MQTT_PROTOCOL, host, 3000);
  }
  Serial.print(end - begin);
  Serial.print(" | ");
  
  begin = micros();
  mqttPublished = temperatureTopic.publish(temperature.value);
  end = micros();
  if(mqttPublished){
    postResult(end-begin, MQTT_PROTOCOL, host, 3000);
    Serial.print(end - begin);
  }
  else{
    Serial.print("Timeout?");
  }
  delay(500);
  
  Serial.print(" \t\t|\t");
//--------------------------------------------------------------------------------------------//  
  // MQTT WITH SSL
  if(!mqttSecure.connected()){
    reconnectMqtt(&mqttSecure,2);
  }
  //Serial.println("mqtt ssl");
  begin = micros();
  if(verifyFingerprint(espClientSecure, host, mqttPortSsl)){
    mqttPublished = humidityTopic.publish(humidity.value);
    end = micros();
    if(mqttPublished){
      //postResult(end-begin, MQTTS_PROTOCOL, host, 3000);
    }
    Serial.print(end - begin);
    Serial.print(" | ");
  }
  else{
    Serial.print("ERROR");
  }
  begin = micros();
  if(verifyFingerprint(espClientSecure, host, mqttPortSsl)){
    mqttPublished = temperatureTopic.publish(temperature.value);
    end = micros();
    if(mqttPublished){
      postResult(end-begin, MQTTS_PROTOCOL, host, 3000);
      Serial.print(end - begin);
    }
    else{
      Serial.print("Timeout?");
    }
  }
  else{
    Serial.print("ERROR");
  }

  delay(500);
  
  Serial.print(" \t\t|\t");
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
    coapMessagesSent++;
    coapErrors++;
    Serial.print("ERROR");
  }
  else{
    coapMessagesSent++;
    postResult(end-begin, COAP_PROTOCOL, host, 3000);
    Serial.print(end - begin);
    
  }
  Serial.print(" | ");
  begin = micros();
  msgid = coap.post(ip, coapPort,"sensor_reads",(char*)hJson.c_str(),hJson.length());
  state = coap.loop();
  end = micros();
  if(!state){    
    coapMessagesSent++;
    coapErrors++;
    Serial.print("ERROR");
  }
  else{
    coapMessagesSent++;
    //postResult(end-begin, COAP_PROTOCOL, host, 3000);
    Serial.print(end - begin);
  }
  Serial.print("\t|");
  Serial.print("Tentativas:"); 
  Serial.print(coapMessagesSent);
  Serial.print("  Erros:"); 
  Serial.print(coapErrors); 
  Serial.println("tentativas = msgs enviadas com sucesso + erros");

}
