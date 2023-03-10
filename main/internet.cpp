#include "internet.h"
#include <WiFiNINA.h>

void internetConnection(int status, const char* ssid, const char* pass) {

  while(status != WL_CONNECTED) {
    Serial.println("Conetado a internet");
    status = WiFi.begin(ssid, pass);
    delay(1000);
	}
}
