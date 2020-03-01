/*
* @file     Offline-Mode.ino
* @author   Thanh Tung Phan
* @brief    Source code for first offline mode of CA-SW2
* @corp     Chika Corporation
* @last-mod Wednesday, 12th Feb, 2020
* @NOTE     USING WITH PHYSICAL BUTTON - If you want to check with touch button, let's change 
* in isButtonClick() to LOW level.
*/

/*******************************************************************************
 *  INCLUDES
 ******************************************************************************/
 
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Ticker.h>

Ticker ticker;

/*******************************************************************************
 *  VALUE DEFINITION
 ******************************************************************************/
//Wifi information - Just use for test wifi of module when SmartConfig meet failure

const char *ssid = "username wifi";
const char *password = "password wifi";

//Information of CA-SW2-1 and CA-SW2-2:
const char *CA_SW2_1 = "0c38a97d-1564-4707-935c-18b4e9bcb0db";
const char *CA_SW2_2 = "7f669cb3-2189-4c22-ae09-cb6cc663b96d";

//Config MQTT broker information:
const char *mqtt_server = "chika.gq";
const int mqtt_port = 2502;
const char *mqtt_user = "chika";
const char *mqtt_pass = "2502";

int button = 5;
int button2 = 4;				
int control = 2;
int control2 = 14;	

int stateLED_control = 16;		

//Variables - MQTT:
boolean stateDEVICE_control = false;
boolean stateDEVICE_control2 = false;

boolean smartConfigStart =  false;

unsigned int longPressTime = 6000;

//Setup MQTT - Wifi ESP12F:
WiFiClient esp_12F;
PubSubClient client(esp_12F);

//General setup:
void setup()
{
	Serial.begin(115200);
	Serial.println("\n\n_ CA-SW2 say hello to your home _");
	pinMode(button, INPUT);
  pinMode(button2, INPUT);

  pinMode(control, OUTPUT);
  pinMode(control2, OUTPUT);
  pinMode(stateLED_control, OUTPUT);
 
	WiFi.setAutoConnect(true);
	WiFi.setAutoReconnect(true);
	WiFi.mode(WIFI_STA);

	delay(8000);
	
		Serial.println("WIFI CONNECTED");
		Serial.println(WiFi.SSID());
		Serial.print("IP: ");
		Serial.println(WiFi.localIP());
    	
	Serial.println("Trying connect MQTT ...");
	client.setServer(mqtt_server, mqtt_port);
}

//------------- MAIN LOOP -------------
void loop()
{ 
	if (WiFi.status() == WL_CONNECTED)
	{
	  if(!client.connected())	{
		reconnect_mqtt();
	}
	else
	client.loop();
	}

	boolean check_Button = isButton_Click(button);
	boolean check_Button2 = isButton_Click(button2);
  		
		if (check_Button)
		{
			Serial.println("\nButton 1 - Clicked!");
			digitalWrite(stateLED_control, stateDEVICE_control);
			digitalWrite(control, !stateDEVICE_control);
			if (stateDEVICE_control)
				client.publish(CA_SW2_1, "0");
			else
				client.publish(CA_SW2_1, "1");
			
			stateDEVICE_control = !stateDEVICE_control;
		}

   if (check_Button2)
   {
      Serial.println("\nButton 2 - Clicked!");
      digitalWrite(control2, !stateDEVICE_control2);
      if (stateDEVICE_control2)
        client.publish(CA_SW2_2, "0");
      else
        client.publish(CA_SW2_2, "1");
      
      stateDEVICE_control2 = !stateDEVICE_control2;
    }
   
	delay(100);
}

//------------- OTHER FUNCTIONS -------------
void reconnect_mqtt()
{
  boolean check_Button;
  boolean check_Button2;
	while (!client.connected())
	{
		    Serial.print("Attempting MQTT connection...");
        String clientId = "CA-SW2 - ";
        clientId += String(random(0xffff), HEX);
        Serial.println(clientId);  
    check_Button = isButton_Click(button); 
    if (check_Button)
    {
      Serial.println("\nButton 1 - Clicked!");
      digitalWrite(stateLED_control, stateDEVICE_control);
      digitalWrite(control, !stateDEVICE_control);
      if (stateDEVICE_control)
        client.publish(CA_SW2_1, "0");
      else
        client.publish(CA_SW2_1, "1");
      
      stateDEVICE_control = !stateDEVICE_control;
    }

    check_Button2 = isButton_Click(button2); 
    if (check_Button2)
    {
      Serial.println("\nButton 2 - Clicked!");
      digitalWrite(control2, !stateDEVICE_control2);
      if (stateDEVICE_control2)
        client.publish(CA_SW2_2, "0");
      else
        client.publish(CA_SW2_2, "1");
      
      stateDEVICE_control2 = !stateDEVICE_control2;
    }
    
		if (client.connect(clientId.c_str(), mqtt_user, mqtt_pass))
        {
            Serial.println("Connected");
            client.subscribe(CA_SW2_1);
            client.subscribe(CA_SW2_2);
        }
        else
        {
            Serial.print("Failed, rc=");
            Serial.print(client.state());
            Serial.println("Try again in 3 seconds");
            delay(3000);
        }
    }
}


/**********************************************************************************
 * @func  isButton_Click(int GPIO_to_read)
 * 
 * @brief: checking state of pin which connect to touch button
 * @param: fd - file descriptor when using the func
 * 
***********************************************************************************/
int isButton_Click(int GPIO_to_read)
{
    int out = 0;
    unsigned int timer;
    timer = millis();
    while (digitalRead(GPIO_to_read) == HIGH)
    {
        delay(20);
        if (millis() - timer > longPressTime)
        {
          Serial.println("Starting smart config ...");
          startSmartConfig();
        }
        else
        out = 1;
    }
    return out;
}

void blinking()
{
  bool state = digitalRead(stateLED_control);
  digitalWrite(stateLED_control,!state);
}

//SmartConfig:
boolean startSmartConfig(){
  int t = 0;
  Serial.println("Smart Config Start");
  WiFi.beginSmartConfig();
  delay(500);
  ticker.attach(0.1, blinking);
  while(WiFi.status() != WL_CONNECTED){
    t++;
    Serial.print(".");
    delay(500);
    if(t > 120){
      Serial.println("Smart Config Fail");
	  smartConfigStart = false;
	  ticker.attach(0.5, blinking);
      delay(3000);
      exitSmartConfig();
      return false;
    }
  }
  smartConfigStart = true;
  Serial.println("WIFI CONNECTED");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
  Serial.println(WiFi.SSID());
  exitSmartConfig();
  return true;
}

void exitSmartConfig(){
	WiFi.stopSmartConfig();
	ticker.detach();
}
