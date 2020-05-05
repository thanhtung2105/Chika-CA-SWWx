/*
* @file     6e2b871e-fd51-4006-af7b-a3ab59b17c40.ino
* @proc_id  6e2b871e-fd51-4006-af7b-a3ab59b17c40
* @author   Thanh Tung Phan
* @brief    Source code for the second Chika Smart Switch (2 buttons) - CA-SW2
* @corp     Chika Corporation
* @last-mod Thursday, 30th April, 2020
*/

/*******************************************************************************
 *  INCLUDES
 ******************************************************************************/
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>			//MQTT library
#include <Ticker.h>

/*******************************************************************************
 *  VALUE DEFINITION
 ******************************************************************************/

//Information of the communication topic
//Change this when uploading for a new product 
const char *CA_SW2_1 = "6e2b871e-fd51-4006-af7b-a3ab59b17c40/button1";
const char *CA_SW2_2 = "6e2b871e-fd51-4006-af7b-a3ab59b17c40/button2";

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
Ticker ticker;		//Ticker for SmartConfig signal

//Declare for MQTT - Wifi ESP12F:
WiFiClient esp_12F;
PubSubClient client(esp_12F);

int button_1 = 13; //@PIN_7
int button_2 = 12; //@PIN_6

int control_1 = 5; //@PIN_20
int control_2 = 4; //@PIN_19

int stateLED_control_1 = 16; //@PIN_4
int stateLED_control_2 = 14; //@PIN_5

//Variables - MQTT:
boolean stateDEVICE_control_1 = false;
boolean stateDEVICE_control_2 = false;
boolean smartConfigStart = false;

unsigned int longPressTime = 6000;		//The holding time to start SmartConfig

/*******************************************************************************
 * @func    Setup range
 *
 * @brief   All of needed function for setup
 *******************************************************************************/

//------------- NEEDED FUNCTIONS -------------

void blinking()
{
	bool state_1 = digitalRead(stateLED_control_1);
	bool state_2 = digitalRead(stateLED_control_2);
	digitalWrite(stateLED_control_1, !state_1);
	digitalWrite(stateLED_control_2, !state_2);
}

/**********************************************************************************
 * @func  SmartConfig
 * 
 * @brief: Initialize the information for SmartConfig
 * @param: fd - file descriptor when using function
 * 
***********************************************************************************/
void exitSmartConfig()
{
	WiFi.stopSmartConfig();
	ticker.detach();
	digitalWrite(stateLED_control_1, LOW);
	digitalWrite(stateLED_control_2, LOW);
}

//SmartConfig:
boolean startSmartConfig()
{
	int t = 0;
	Serial.println("Smart Config Start");
	WiFi.beginSmartConfig();
	delay(500);
	ticker.attach(0.1, blinking);
	while (WiFi.status() != WL_CONNECTED)
	{
		t++;
		Serial.print(".");
		delay(500);
		if (t > 120)
		{
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
	while (digitalRead(GPIO_to_read) == LOW)
	{
		delay(10);
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

//Callback - func for receiving message from MQTT:
void callback(char *topic, byte *payload, unsigned int length)
{
	String payload_toStr;
	for (unsigned int i = 0; i < length; i++)
	{
		payload_toStr += (char)payload[i];
	}

	// Checking button & controlling:
	if (String(topic).equals(CA_SW2_1))
	{
		if (payload_toStr == "true")
		{
			stateDEVICE_control_1 = true;
		}
		else if (payload_toStr == "false")
		{
			stateDEVICE_control_1 = false;
		}
		digitalWrite(control_1, stateDEVICE_control_1);
		digitalWrite(stateLED_control_1, stateDEVICE_control_1);
	}
	else if (String(topic).equals(CA_SW2_2))
	{
		if (payload_toStr == "true")
		{
			stateDEVICE_control_2 = true;
		}
		else if (payload_toStr == "false")
		{
			stateDEVICE_control_2 = false;
		}
		digitalWrite(control_2, stateDEVICE_control_2);
		digitalWrite(stateLED_control_2, stateDEVICE_control_2);
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

		//Also check button clicking to control offline linked device
		check_Button_1 = isButton_Click(button_1);
		if (check_Button_1)
		{
			stateDEVICE_control_1 = !stateDEVICE_control_1;
			digitalWrite(stateLED_control_1, stateDEVICE_control_1);
			digitalWrite(control_1, stateDEVICE_control_1);
		}

		check_Button_2 = isButton_Click(button_2);
		if (check_Button_2)
		{
			stateDEVICE_control_2 = !stateDEVICE_control_2;
			digitalWrite(stateLED_control_2, stateDEVICE_control_2);
			digitalWrite(control_2, stateDEVICE_control_2);
		}

		if (client.connect(clientId.c_str(), mqtt_user, mqtt_pass))
		{
			Serial.println("Connected");
			client.subscribe(CA_SW2_1);
			client.subscribe(CA_SW2_2);
		}
		else
		{
			// Serial.print("Failed, rc=");
			// Serial.print(client.state());
			// Serial.println("Try again in 3 seconds");
			delay(500);
		}
	}
}

//General setup:
void setup()
{
	Serial.begin(115200);
	Serial.println("\n\n_ CA-SW2 say hello to your home _");
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
void loop()
{
	if (WiFi.status() == WL_CONNECTED)
	{
		if (!client.connected())
		{
			reconnect_mqtt();
		}
		else
			client.loop();
	}

	boolean check_Button_1 = isButton_Click(button_1);
	boolean check_Button_2 = isButton_Click(button_2);

	if (check_Button_1)
	{
		stateDEVICE_control_1 = !stateDEVICE_control_1;
		digitalWrite(stateLED_control_1, stateDEVICE_control_1);
		digitalWrite(control_1, stateDEVICE_control_1);
		
		if (stateDEVICE_control_1)
		{
			client.publish(CA_SW2_1, "true", true);
		}	
		else
		{
			client.publish(CA_SW2_1, "false", true);
		}
	}

	if (check_Button_2)
	{
		stateDEVICE_control_2 = !stateDEVICE_control_2;
		digitalWrite(stateLED_control_2, stateDEVICE_control_2);
		digitalWrite(control_2, stateDEVICE_control_2);
		
		if (stateDEVICE_control_2)
		{
			client.publish(CA_SW2_2, "true", true);
		}	
		else
		{
			client.publish(CA_SW2_2, "false", true);
		}
	}
}
