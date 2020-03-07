#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Ticker.h>
/*
* @file     6e2b871e-fd51-4006-af7b-a3ab59b17c40.ino
* @proc_id  6e2b871e-fd51-4006-af7b-a3ab59b17c40
* @author   Thanh Tung Phan
* @brief    Source code for the second Chika Smart Switch - CA-SW2
* @corp     Chika Corporation
* @last-mod Wednesday, 12th Feb, 2020
*/

/*******************************************************************************
 *  INCLUDES
 ******************************************************************************/
 
Ticker ticker;

/*******************************************************************************
 *  VALUE DEFINITION
 ******************************************************************************/
//Wifi information - Just use for test wifi of module when SmartConfig meet failure
const char *ssid = "username wifi";
const char *password = "password wifi";

//Information of CA-SW2-1 and CA-SW2-2:		char[37]=6/7
const char *CA_SW2_1 = "6e2b871e-fd51-4006-af7b-a3ab59b17c40/6883dd85-c759-428c-98fe-77b00c20c710";
const char *CA_SW2_2 = "6e2b871e-fd51-4006-af7b-a3ab59b17c40/7f669cb3-2189-4c22-ae09-cb6cc663b96d";

//Config MQTT broker information:
const char *mqtt_server = "chika.gq";
const int mqtt_port = 2502;
const char *mqtt_user = "chika";
const char *mqtt_pass = "2502";

//Variables - ESP12F:
/*************************************     NOTE PINs    ******************************************
 * @@PINs CAN NOT use:
 * @REST: Pin 1.
 * @ADC: Pin 2.
 * @CH_PD: Pin 3. 
 * @VCC: Pin 8.
 * @GND: Pin 15.
 * @GPIO15: Pin 16.
 * @GPIO0: Pin 18.
 * List PINs which can not use as normal GPIO:
 * @GPIO11 - SDIO CMD - SPI_CS0: Pin 9. (~# lens/boot mode)
 * @GPIO7 - SDIO DATA0 - SPI_MISO: Pin 10. (~# wdt reset)
 * @GPIO9 - SDIO DATA2 - SPIHD - HSPIHD: Pin 11. (~# load addr fail)
 * @GPIO8 - SDIO DATA1 - SPI_MOSI - UlRXD: Pin 13. (~# chksum 0x2d)
 * @GPIO6 - SDIO CLK - SPI_CLK: Pin 14. (~# csum 0x2d/v8b899c12)
 *
 * @@PINs usable:
 * @PIN_4 16
 * @PIN_5 14
 * @PIN_6 12
 * @PIN_7 13

 * @PIN_12 10

 * @PIN_17 2    //This PIN control onboard-LED of ESP12F
 * @PIN_19 4
 * @PIN_20 5
 * @PIN_21 3    //GPIO3 - U0RXD - Use if necessary (sometime it affects to setup())
 * @PIN_22 1    //GPIO1 - U0TXD - SPI_CS1 - Use if necessary (sometime it affects to setup())
 * 
*************************************************************************************************/

int button_1 = 13;				//@PIN_7
int button_2 = 12;				//@PIN_6

int control_1 = 5;				//@PIN_20
int control_2 = 4;				//@PIN_19			

int stateLED_control_1 = 16;	//@PIN_4
int stateLED_control_2 = 14;		//@PIN_5

//Variables - MQTT:
boolean stateDEVICE_control_1 = false;
boolean stateDEVICE_control_2 = false;
boolean smartConfigStart =  false;

unsigned int longPressTime = 6000;

/*******************************************************************************
 * @func    Setup range
 *
 * @brief   All of needed function for setup
 *******************************************************************************/
//Setup Wifi:
void setup_Wifi()
{
	delay(100);
	WiFi.begin(ssid, password);
	while (WiFi.status() != WL_CONNECTED)
	{
		delay(500);
	}
}

//Setup MQTT - Wifi ESP12F:
WiFiClient esp_12F;
PubSubClient client(esp_12F);

//------------- NEEDED FUNCTIONS -------------

void blinking()
{
  bool state_1 = digitalRead(stateLED_control_1);
  bool state_2 = digitalRead(stateLED_control_2);
  digitalWrite(stateLED_control_1,!state_1);
  digitalWrite(stateLED_control_2,!state_2);
}

/**********************************************************************************
 * @func  SmartConfig
 * 
 * @brief: Initialize the information for SmartConfig
 * @param: fd - file descriptor when using function
 * 
***********************************************************************************/
void exitSmartConfig(){
	WiFi.stopSmartConfig();
	ticker.detach();
	digitalWrite(stateLED_control_1, LOW);
	digitalWrite(stateLED_control_2, LOW);
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
    //   Serial.println("Smart Config Fail");
	  smartConfigStart = false;
	  ticker.attach(0.5, blinking);
      delay(3000);
      exitSmartConfig();
      return false;
    }
  }
  smartConfigStart = true;
//   Serial.println("WIFI CONNECTED");
//   Serial.print("IP: ");
//   Serial.println(WiFi.localIP());
//   Serial.println(WiFi.SSID());
  exitSmartConfig();
  return true;
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
    while (digitalRead(GPIO_to_read) == 1)
    {
        delay(20);
        if (millis() - timer > longPressTime)
        {
        //  Serial.println("Starting smart config ...");
          startSmartConfig();
        }
        else
        out = 1;
    }
    return out;
}

//Callback:
void callback(char *topic, byte *payload, unsigned int length)
{
	//Topic list test is the value of variables: CA_SW2_1 and CA_SW2_2
	// Serial.print("Topic [");
	// Serial.print(topic);
	// Serial.print("]");
	//Print message of button ID:
	// for (unsigned int i = 0; i < length; i++)
	// {
	// 	Serial.print((char)payload[i]);
	// }
	// Serial.println();

  //Differenate the button ID: 1 - 6883dd85-c759-428c-98fe-77b00c20c710 & 2 - 7f669cb3-2189-4c22-ae09-cb6cc663b96d
	if ((char)topic[37] == '6')
		switch ((char)payload[0])
		{
			case '1':
			digitalWrite(control_1, HIGH);
			stateDEVICE_control_1 = true;
			digitalWrite(stateLED_control_1,stateDEVICE_control_1);
			break;
			case '0':
			digitalWrite(control_1, LOW);
			stateDEVICE_control_1 = false;
			digitalWrite(stateLED_control_1,stateDEVICE_control_1);
			break;
		}
   
	else if ((char)topic[37] == '7')
		switch ((char)payload[0])
		{
			case '1':
			digitalWrite(control_2, HIGH);
			stateDEVICE_control_2 = true;
			digitalWrite(stateLED_control_2,stateDEVICE_control_2);
			break;
			case '0':
			digitalWrite(control_2, LOW);
			stateDEVICE_control_2 = false;
			digitalWrite(stateLED_control_2,stateDEVICE_control_2);
			break;
		}
   
}

/**********************************************************************************
 * @func  reconnect_mqtt()
 * 
 * @brief: Reconnect MQTT when connect meet failure
 * @param: fd - file descriptor when using the func
 * 
***********************************************************************************/
void reconnect_mqtt()
{
	boolean check_Button_1;
	boolean check_Button_2;
	while (!client.connected())
	{
		Serial.print("Attempting MQTT connection...");
        String clientId = "CA-SW2 - ";
        clientId += String(random(0xffff), HEX);
        Serial.println(clientId);
		
		check_Button_1 = isButton_Click(button_1);			
		if (check_Button_1)
		{
			// Serial.println("\nButton 1 - Clicked!");
			digitalWrite(stateLED_control_1, stateDEVICE_control_1);
			digitalWrite(control_1, !stateDEVICE_control_1);
			
			stateDEVICE_control_1 = !stateDEVICE_control_1;
			// Serial.print("Relay 1 change state to: ");
			// Serial.print(stateDEVICE_control_1);
		}
		
		check_Button_2 = isButton_Click(button_2);
		if (check_Button_2)
		{
			// Serial.println("\nButton 2 - Clicked!");
			digitalWrite(stateLED_control_2, stateDEVICE_control_2);
			digitalWrite(control_2, !stateDEVICE_control_2);
			
			stateDEVICE_control_2 = !stateDEVICE_control_2;
			// Serial.print("Relay 2 change state to: ");
			//Serial.print(stateDEVICE_control_2);
		}

		if (client.connect(clientId.c_str(), mqtt_user, mqtt_pass))
        {
            // Serial.println("Connected");
            client.subscribe(CA_SW2_1);
            client.subscribe(CA_SW2_2);
        }
        else
        {
            // Serial.print("Failed, rc=");
            // Serial.print(client.state());
            // Serial.println("Try again in 3 seconds");
            delay(1000);
        }
    }
}


//General setup:
void setup()
{
	Serial.begin(115200);
	// Serial.println("\n\n_ CA-SW2 say hello to your home _");
	pinMode(button_1, INPUT);
	pinMode(button_2, INPUT);
	
	WiFi.setAutoConnect(true);
	WiFi.setAutoReconnect(true);
	WiFi.mode(WIFI_STA);
	
	pinMode(control_1, OUTPUT);
	pinMode(control_2, OUTPUT);
	pinMode(stateLED_control_1, OUTPUT);
	pinMode(stateLED_control_2, OUTPUT);
	pinMode(stateDEVICE_control_1, OUTPUT);
	pinMode(stateDEVICE_control_2, OUTPUT);

//  setup_Wifi();
  
	delay(8000);

	digitalWrite(stateLED_control_1, HIGH);
	digitalWrite(stateLED_control_2, HIGH);
	// Serial.println("WIFI CONNECTED");
	// Serial.println(WiFi.SSID());
	// Serial.print("IP: ");
	// Serial.println(WiFi.localIP());
 	
	// Serial.println("Trying connect MQTT ...");
	client.setServer(mqtt_server, mqtt_port);
	client.setCallback(callback);
}


/*************************************     Main Loop    ******************************************
 * @desc: process the physical touch and send request to Broker.
 * @para: none
 * 
*************************************************************************************************/
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

	boolean check_Button_1 = isButton_Click(button_1);
	boolean check_Button_2 = isButton_Click(button_2);
			
		if (check_Button_1)
		{
			digitalWrite(stateLED_control_1, stateDEVICE_control_1);
			digitalWrite(control_1, !stateDEVICE_control_1);
			if (stateDEVICE_control_1)
				client.publish(CA_SW2_1, "0");
			else
				client.publish(CA_SW2_1, "1");
			
			stateDEVICE_control_1 = !stateDEVICE_control_1;
		}
		
		if (check_Button_2)
		{
			digitalWrite(stateLED_control_2, stateDEVICE_control_2);
			digitalWrite(control_2, !stateDEVICE_control_2);
			if (stateDEVICE_control_2)
				client.publish(CA_SW2_2, "0");
			else
				client.publish(CA_SW2_2, "1");
			
			stateDEVICE_control_2 = !stateDEVICE_control_2;
		}

	delay(100);
}
