#include <Ticker.h>
#include <ESP8266WiFi.h>

#define SmartConfig_Led_1 0
//#define SmartConfig_Led_2 
int button_1 = 14;
int button_2 = 16;

bool clickSC = false; 
Ticker ticker;

void setup() {
  Serial.begin(115200);
  Serial.println("CA-SW2 hello your home!");
  pinMode(button_1, INPUT);
  pinMode(button_2, INPUT);
  pinMode(SmartConfig_Led_1, OUTPUT);
//  pinMode(SmartConfig_Led_2, OUTPUT);
  
/*  WiFi.setAutoConnect(true);
  WiFi.setAutoReconnect(true);
  WiFi.mode(WIFI_STA);
  delay(10000);
  */
  clickSC = clickSmartConfig();
  if(!WiFi.isConnected()&&clickSC)
    startSmartConfig();
  else
  {
    Serial.println("WIFI CONNECTED");
    Serial.println(WiFi.SSID());
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
  }
}

/*____________________________________________*/

void loop() {
  if(WiFi.status() == WL_CONNECTED){
      Serial.println("Testing Done!");
      Serial.println(WiFi.SSID());
      Serial.print("IP: ");
      Serial.println(WiFi.localIP());
      delay(100);
    } 
  delay(1000);
}

/*____________________________________________*/

boolean clickSmartConfig()
{
    int out = 0;
    while ((digitalRead(button_1)==1)&&(digitalRead(button_2)==1))
    {
      delay(5000);
      if ((digitalRead(button_1)==1)&&(digitalRead(button_2)==1))
      {
        out = 1;
        break;
      }
      else
      {
        out = 0;
        break;
      }
    }
    return out;
}

void blinking()
{
  int state = digitalRead(SmartConfig_Led_1);
  digitalWrite(SmartConfig_Led_1,!state);
}

boolean startSmartConfig()
{
  int t = 0;
  Serial.println("Smart Config Start");
  WiFi.beginSmartConfig();
  delay(500);
  ticker.attach(0.1, blinking);
  while(WiFi.status() != WL_CONNECTED){
    t++;
    Serial.print(".");
    delay(500);
    if(t > 120)
    {
      Serial.println("Smart Config Fail");
      ticker.attach(0.5, blinking);
      delay(3000);
      exitSmartConfig();
      return false;
    }
  }

  Serial.println("WIFI CONNECTED");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
  Serial.println(WiFi.SSID());
  exitSmartConfig();
  return true;
}

void exitSmartConfig()
{
  WiFi.stopSmartConfig();
  ticker.detach();
  digitalWrite(SmartConfig_Led_1, LOW);
}
