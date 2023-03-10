// Libreria Wifi del Arduino UNO Wifi Rev2
#include <WiFiNINA.h>
#include <HttpClient.h>
#include "internet.h"
#include <ArduinoJson.h>

// TODO. Hacer que el te de el ssid y la constraseña se reciba por comunicacion serial
String json = "{\"ssid\": \"Steven\", \"password\": \"12345678\", \"server-number\":3, \"servers\": [{\"type\":\"https\", \"name\":\"arduino.cc\", \"endpoint\":\"/\"},{\"type\":\"http\", \"name\":\"192.168.100.13\", \"endpoint\":\"/\"}, {\"type\":\"https\", \"name\":\"arduiterterteno.cc\", \"endpoint\":\"/\"}]}";
StaticJsonDocument<2048> config;

const int BUFFER_SIZE = 2048;
char buf[BUFFER_SIZE];

int numberOfServers = config["server_number"];
const char ssid[] = "Totalplay-2.4G-f030";
const char pass[] = "32W6W96ptV8mPK2P";


WiFiClient client;
HttpClient http(client);

int status = WL_IDLE_STATUS;

void setup(){
  // indica el pueto donde se hace la comunicacion serial
  Serial.begin(9600);
  Serial.println("Inicio");
  while (Serial.available() <= 0){}
  if (Serial.available() > 0){
    int rlen = Serial.readBytes(buf, BUFFER_SIZE);

    Serial.print("I recived: ");
    for (int i = 0; i < rlen; i++){
      Serial.print(buf[i]);
    }
  }

  
  DeserializationError error = deserializeJson(config, json);
  config_print();
  /*
  internetConnection();
  ipsTry();*/
  Serial.println("Fin");
}


void loop(){

}

void config_print(){
  String ssid = config["ssid"];
  String pass = config["password"];
  int sn = config["server-number"];
  Serial.println(ssid);
  Serial.println(pass);
  Serial.println(sn);
  for (int i= 0; i < sn; i++){
    String ser = config["servers"][i];
    Serial.println(ser);
  }
}
void internetConnection() {

  while(status != WL_CONNECTED) {
    status = WiFi.begin(config["ssid"], config["password"]);
    Serial.println("Conetado a internet");
    delay(1000);
	}
}

void ipsTry(){
  for (int i = 0; i < config["server-number"]; i++){
    
    if (status != WL_CONNECTED) {
      internetConnection();
      break;
    }

    int err = 0;
    if (config["servers"][i]["type"] == "http"){
      String t = config["servers"][i]["type"];
      String n = config["servers"][i]["name"];
      Serial.print(t);
      Serial.print("://");
      Serial.println(n);
      
      for (int j = 1; j <= 5; j++) {
        err = http.get(config["servers"][i]["name"], "/");

        if (err == 0) {
         break;
        }
        Serial.print("Intento: ");
        Serial.println(j);
        delay(2000);
      }
      
    }else{
      Serial.println("Se conecta por https");
      
      for (int j = 1; j <= 5; j++) {
        err = http.get(config["servers"][i]["name"], "/");
        String t = config["servers"][i]["type"];
         String n = config["servers"][i]["name"];
        Serial.print(t);
        Serial.print("://");
        Serial.println(n);
        
        if (err == 0) {
          break;
        }

        Serial.print("Intento: ");
        Serial.println(j);
        delay(2000);
      }
    }
  }
}
