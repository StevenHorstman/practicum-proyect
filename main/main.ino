// Libreria Wifi del Arduino UNO Wifi Rev2
#include <WiFiNINA.h>
#include <HttpClient.h>
#include "internet.h"
#include <ArduinoJson.h>


DynamicJsonDocument config(1024);
DynamicJsonDocument msg(256);

WiFiClient client;
HttpClient http(client);

int status = WL_IDLE_STATUS;

void setup() {
  Serial.begin(9600);
  msg["name"] = "ard1";
  msg["action"] = "configuration";

  String content = "";

  internetConnection();

  Serial.println("Connected to internet");

  if (client.connect("192.168.100.10", 3000)) {
    Serial.println("Connected to server");
    // Covierte le json y lo envia a la servidor
    serializeJson(msg, content);
    char charBuf[256];
    content.toCharArray(charBuf, 256);
    client.write(charBuf);

    Serial.println(charBuf);
    
    // Obtiene el string  Json
    String fin = "";
    delay(5000);
    while(client.available()) {
      char c = client.read();
      fin = fin + String(c);
    }

    //Deserializa el string json y lo permte deja usable
    deserializeJson(config, fin);
    String name = config["device_name"];
    Serial.print(name);

  }

  client.stop();

  Serial.println("end");
  serverTest();
}



void loop(){

}


String getConfiguration(){
  int BUFFER_SIZE = 2048;
  char buf[BUFFER_SIZE];
  String json = "";
 while (Serial.available() <= 0){}
  if (Serial.available() > 0){
    int rlen = Serial.readBytes(buf, BUFFER_SIZE);

    
    for (int i = 0; i < rlen; i++){
      json = json + String(buf[i]);
    }
  }
  return json;
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
    status = WiFi.begin("Totalplay-2.4G-f030", "32W6W96ptV8mPK2P");
    
    delay(1000);
	}
}

void serverTest(){
  for (int i = 0; i < config["server_number"]; i++){
    
    if (status != WL_CONNECTED) {
      internetConnection();
      break;
    }

    int err = 0;
    if (config["servers"][i]["type"] == "http"){
      //String t = config["servers"][i]["type"];
      //String n = config["servers"][i]["name"];
      //Serial.print(t);
      //Serial.print("://");
      //Serial.println(n);
      Serial.print("Server: ");
      Serial.println(i);
      for (int j = 1; j <= 5; j++) {
        err = http.get(config["servers"][i]["name"], config["servers"][i]["endpoint"]);
        Serial.println(i);
        int severStatus = http.responseStatusCode();
        if (severStatus > 0 && severStatus < 500) {
          break;
        }

        //delay(2000); 
        if (j == 5){
          String serverName = config["servers"][i]["name"];
            Serial.println("a");
          if (client.connect("192.168.100.10", 3000)) {
            Serial.print("Error en ");
            Serial.println(serverName);
            String errContent = "";
            delay(2000);
            msg["server"] = serverName;
            msg["action"] = "report";
            Serial.println("a");
            serializeJson(msg, errContent);
            char charBuf[256];
            errContent.toCharArray(charBuf, 256);
            client.write(charBuf);
            Serial.println(charBuf);
          }
          client.stop();
          Serial.println(serverName);
        }
      }
      

    }else{
      for (int j = 1; j <= 5; j++) {
        err = http.get(config["servers"][i]["name"], config["servers"][i]["endpoint"]);
        if (err == 0) {
          break;
        }

        //delay(2000);
        if (j==5){
          String serverName = config["servers"][i]["name"];
          if (client.connect("192.168.100.10", 3000)) {
           
            Serial.print("Error en ");
            Serial.println(serverName);
            String errContent = "";
            delay(2000);
            msg["action"] = "report";
            msg["server"] = serverName;
            serializeJson(msg, errContent);
            char charBuf[256];
            errContent.toCharArray(charBuf, 256);
            client.write(charBuf);
            Serial.println(charBuf);
          }
          client.stop();
          
          
        }
      }
    }
  }
}
