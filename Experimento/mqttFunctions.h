#ifndef MQTT_FUNCTIONS_H
#define MQTT_FUNCTIONS_H

#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>

#define mqtt_server "cacc.unioeste-foz.br"
#define mqtt_user "lucas"
#define mqtt_password "123456"

#define humidity_topic "lucas/humidity"
#define temperature_topic "lucas/temperature"

#define mqttPort 1883
#define mqttPortSsl 8883

Adafruit_MQTT_Client mqtt(&espClient, mqtt_server, mqttPort, mqtt_user, mqtt_password);
Adafruit_MQTT_Client mqttSecure(&espClientSecure, mqtt_server, mqttPortSsl, mqtt_user, mqtt_password);

Adafruit_MQTT_Publish temperatureTopic = Adafruit_MQTT_Publish(&mqtt, temperature_topic);
Adafruit_MQTT_Publish humidityTopic = Adafruit_MQTT_Publish(&mqtt, humidity_topic);

Adafruit_MQTT_Publish temperatureTopicSec = Adafruit_MQTT_Publish(&mqttSecure, temperature_topic);
Adafruit_MQTT_Publish humidityTopicSec = Adafruit_MQTT_Publish(&mqttSecure, humidity_topic);

int reconnectMqtt(Adafruit_MQTT_Client * client, int clientNumber) {
  String clientMqttName = "ESP8266-";
  clientMqttName += String(clientNumber);
  // Loop eterno até conectar no mqtt broker/wifi
  while (!client->connected()) {
    //Serial.print("Tentando conectar...");
    if (client->connect()) {
      //Serial.println("Conectado");
    } else {
      //Serial.print("Failed, rc=");
      //Serial.print(client->state());
      //Serial.println(" Tryng again in 5 sec...");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

#endif
