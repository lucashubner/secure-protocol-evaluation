#ifndef HTTP_FUNCTIONS_H
#define HTTP_FUNCTIONS_H

#include<ESP8266HTTPClient.h>
#define httpPort 3000
#define httpPortSsl 3001

String www_user = "sensors";
String www_pass = "123456";

#define HTTP_PROTOCOL   5
#define HTTPS_PROTOCOL  6
#define MQTT_PROTOCOL   1
#define MQTTS_PROTOCOL  2
#define COAP_PROTOCOL   3

//------------------------------------------------------------------------------------//
int postResult(int value, int protocolType, String host, int port){
    HTTPClient http;
    int httpCode;
    http.begin("http://"+ host + ":" + String(port) +"/statistics");
    
    http.setAuthorization(www_user.c_str(), www_pass.c_str());
    http.addHeader("Content-Type", "application/json");
    http.addHeader("Host", host + ":" + String(port));
    
    httpCode = http.POST("{\r\n\"value\" : \"" + String(value) + "\",\r\n" + "\"protocol_type_id\" : \"" + String(protocolType) +  "\"\r\n}\r\n");
    
    String payload = http.getString();
    http.end();

    return httpCode;
}
//------------------------------------------------------------------------------------//
int postOnWebService(WiFiClient client, TSensor sensor, String host, int port, bool secure = false){
  HTTPClient http;
  if(secure){
    http.begin("https://"+host+":" + String(port) +"/sensor_reads", "6a 2b 89 93 fe da 57 a1 a7 fd 7d b5 8b 49 b9 57 22 f3 70 30");
  }
  else
    http.begin("http://"+ host + ":" + String(port) +"/sensor_reads");
    
  http.setAuthorization(www_user.c_str(), www_pass.c_str());
  http.addHeader("Content-Type", "application/json");
  http.addHeader("Host", host + ":" + String(port));
  
  String json  = "{\"value\":\"" + String(sensor.value)  + "\",";
  json += "\"sensor_type_id\":\"" + String(sensor.sensor_type_id) +  "\"}";

  int httpCode = http.POST(json);
  String payload = http.getString();
  
  
  http.end();

  return httpCode;
}

#endif
