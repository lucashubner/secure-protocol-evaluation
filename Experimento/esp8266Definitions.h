#define wifi_ssid "Adercleison"
#define wifi_password "dionecleide2015"
// Gerar fingerprint do certificado do server.
// openssl x509 -fingerprint -in  mqttserver.crt 
const char* fingerprint = "6a 2b 89 93 fe da 57 a1 a7 fd 7d b5 8b 49 b9 57 22 f3 70 30";
//6A 2B 89 93 FE DA 57 A1 A7 FD 7D B5 8B 49 B9 57 22 F3 70 30";

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

//------------------------------------------------------------------------------------//
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

//------------------------------------------------------------------------------------//
bool verifyFingerprint(WiFiClientSecure wifiClient, int port) {
  String host = "cacc.unioeste-foz.br";
  // Use WiFiClientSecure class to create TLS connection
  if (!wifiClient.connect("cacc.unioeste-foz.br", port)) {
    return false;
  }
    if (wifiClient.verify(fingerprint, host.c_str())) {
     return true;
  } else {
    return false;
  }
}
