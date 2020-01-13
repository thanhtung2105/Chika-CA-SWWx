//ESP8266
#include <ESP8266WiFi.h>
//----------Setup Wifi Connect-------------//
const char* ssid = "Thanh Tung";
const char* pass = "1357908642";

void setup() {
  Serial.begin(115200);
  Serial.println("Start Smart Config...");
//  WiFi.beginSmartConfig();
  delay(500);
 
 WiFi.begin(ssid, pass);

    Serial.println("Connecting...");
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("...");
        delay(500);
    }
    Serial.println("Connect successfully!!");
}

void loop() {
  Serial.println("OK!");
  delay(1000);
}
