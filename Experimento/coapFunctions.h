#include <coap_client.h>

IPAddress ip(179,106,223,24); 
#define coapPort 3002
#define coapPortCert 3102
#define coapPortPreShared 3103
#define coapPortRawPub 3104

long long errors = 0;
long long messagesSend = 0;

String tJson = "";
String hJson = "";

bool state = true;
int msgid;
//------------------------------------------------------------------------------------//
void sensorToJson(TSensor * sensor, String * json){
  *json = "{ \"value\":\""+ String(sensor->value) +"\", \"sensor_type_id\": \""+ sensor->sensor_type_id+ "\"}";
}
//------------------------------------------------------------------------------------//
// coap client response callback
void callback_response(coapPacket &packet, IPAddress ip, int port) {
  char p[packet.payloadlen + 1];
  memcpy(p, packet.payload, packet.payloadlen);
  p[packet.payloadlen] = NULL;
  //response from coap server
  //if(packet.type==3 && packet.code==0){
    //Serial.println("ping ok");
  //}
  //Serial.println(p);
}


