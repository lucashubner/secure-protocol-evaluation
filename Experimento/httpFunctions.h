#include<ESP8266HTTPClient.h>
#define httpPort 3000
#define httpPortSsl 3001

//------------------------------------------------------------------------------------//
void postResult(WiFiClient client, int value, int protocolType){
    String request = "";
    String json = "";
    json  = "{\r\n\t\"value\" : \"" + String(value)  + "\",\r\n";
    json += "\t\"protocol_type_id\" : \"" + String(protocolType) +  "\"\r\n}\r\n";
  
    request += "POST /statistics"; 
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
      }
    }
}
//------------------------------------------------------------------------------------//
String postOnWebService(WiFiClient client, TSensor sensor, int port, bool secure = false){
  HTTPClient http;
  if(secure)
    http.begin("https://cacc.unioeste-foz.br:3001/sensor_reads", "6a 2b 89 93 fe da 57 a1 a7 fd 7d b5 8b 49 b9 57 22 f3 70 30");
    else
    http.begin("http://cacc.unioeste-foz.br:3000/sensor_reads");
 
  http.addHeader("Content-Type", "application/json");
  http.addHeader("Host", "cacc.unioeste-foz.br:" + String(port));
  
  String json  = "{\"value\":\"" + String(sensor.value)  + "\",";
  json += "\"sensor_type_id\":\"" + String(sensor.sensor_type_id) +  "\"}";

  int httpCode = http.POST(json);
  String payload = http.getString();
  http.end();

  return payload;
}
