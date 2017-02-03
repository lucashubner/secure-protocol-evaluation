#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>

#define mqtt_server "cacc.unioeste-foz.br"
#define mqtt_user "lucas"
#define mqtt_password "123456"

#define humidity_topic "lucas/humidity"
#define temperature_topic "lucas/temperature"

#define mqttPort 1883

#define mqttPortSsl 8883

int reconnectMqtt(Adafruit_MQTT_Client * client, int clientNumber) {
  String clientMqttName = "ESP8266-";
  clientMqttName += String(clientNumber);
  // Loop eterno até conectar no mqtt broker/wifi
  while (!client->connected()) {
    //Serial.print("Tentando conectar...");
    if (client->connect()) {
      //Serial.println("Conectado");
    } else {
      /*Serial.print("Falhou, rc=");
      Serial.print(client->state());
      Serial.println(" Tentando novamente em 5 segundos...");
      */
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

