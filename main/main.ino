// Libreria Wifi del Arduino UNO Wifi Rev2
#include <WiFiNINA.h>
#include <HttpClient.h>
//#include <ArduinoJson.h>

// TODO. Hacer que el te de el ssid y la constraseña se reciba por comunicacion serial
const char ssid[] = "Totalplay-2.4G-f030";
const char pass[] = "32W6W96ptV8mPK2P";
const char *servers[]  = {"google.com", "arduinofrfrr.cc"};
//const char *servers[1]  = {"google.com"};
const char *ips[] = {"192.168.100.11"};
WiFiClient client;
HttpClient http(client);
int status = WL_IDLE_STATUS;
int serversNum = sizeof(servers) / sizeof(int);
int ipsNum = sizeof(ips) / sizeof(int);

void setup(){
  // indica el pueto donde se hace la comunicacion serial
  Serial.begin(9600);

  Serial.println("Inicio");
  internetConnection();
  serverTry();
  Serial.println("Fin");
}


void loop(){

}

void internetConnection() {

  while(status != WL_CONNECTED) {
    status = WiFi.begin(ssid, pass);
    Serial.println("Conetado a internet");
    delay(1000);
	}
}

void ipsTry(){
  for (int i = 0; i <= ipsNum; i++){

    if (status != WL_CONNECTED) {
      internetConnection();
      break;
    }
    int err = 0;
    for (int j = 1; j <= 5; j++) {
      err = http.get(ips[i], 8081, "/");
      
      if (err == 0) {
        if (http.responseStatusCode() == 200) {
          Serial.println("Connected");
          break;
        }
        Serial.print("Intento: ");
        Serial.println(j);
      }
    }
  }
}
  
void serverTry(){
  Serial.println(serversNum);
  for (int i = 0; i <= serversNum -1; i++){
    Serial.println(servers[i]);
    Serial.println(serversNum);
    if (status != WL_CONNECTED) {
      internetConnection();
      break;
    }
    int err = 0;
    for (int j = 1; j <= 5; j++) {
      err = http.get(servers[i], "/");
      if (err == 0) {
        break;
      }
      Serial.print("Intento: ");
      Serial.println(j);
    }

  }
}
