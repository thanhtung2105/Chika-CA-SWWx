/*
* @file     b36e333e-3800-4450-84cd-6f90e7e0f721.ino
* @proc_id  b36e333e-3800-4450-84cd-6f90e7e0f721
* @author   Thanh Tung Phan
* @brief    Source code for first Chika Smart Switch - CA-SW2
* @corp     Chika Corporation
* @last-mod Monday, 13th Jan, 2020
*/

/*******************************************************************************
 *  INCLUDES
 ******************************************************************************/
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

/*******************************************************************************
 *  VALUE DEFINITION
 ******************************************************************************/
//Wifi information - Just use for test wifi of module when SmartConfig meet failure

const char *ssid = "Thanh Tung";
const char *password = "1357908642";

//Information of CA-SW2-1 and CA-SW2-2:
const char *CA_SW2_1 = "0c38a97d-1564-4707-935c-18b4e9bcb0db";
const char *CA_SW2_2 = "7f704fdf-fa4b-44e2-b359-5ef19294196a";

//Config MQTT broker information:
const char *mqtt_server = "14.169.230.167";
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

int button_1 = 4;				//@PIN_19
int button_2 = 5;				//@PIN_20

int control_1 = 16;				//@PIN_4
int control_2 = 14;				//@PIN_5

int stateLED_control_1 = 10;	//@PIN_12
int stateLED_control_2 = 2;		//@PIN_17

//Variables - MQTT:
bool state_button_1 = 0;
bool state_button_2 = 0;

//Variables - Func:
unsigned long previousMillis = 0;
long interval = 500;

/*******************************************************************************
 * @func    Setup range
 *
 * @brief   All of needed function for setup
 *******************************************************************************/
//Setup Wifi:
void setup_Wifi()
{
	delay(100);
	Serial.println();
	Serial.print("Connecting to ... ");
	Serial.println(ssid);
	WiFi.begin(ssid, password);
	while (WiFi.status() != WL_CONNECTED)
	{
		delay(500);
		Serial.print(".");
	}
	Serial.println("");
    Serial.println("WiFi connected!");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

//Setup MQTT - Wifi ESP12F - ESP8266MOD:
WiFiClient esp_12F;
PubSubClient client(esp_12F);

//General setup:
void setup()
{
	pinMode(button_1, INPUT);
	pinMode(button_2, INPUT);
	
	pinMode(control_1, OUTPUT);
	pinMode(control_2, OUTPUT);
	pinMode(stateLED_control_1, OUTPUT);
	pinMode(stateLED_control_2, OUTPUT);
	
	Serial.begin(115200);
	setup_Wifi();
	
//	WiFi.setAutoConnect(true);
//	WiFi.setAutoReconnect(true);
//	delay(10000);
/*
	if(!WiFi.isConnected())
	{
		startSmartConfig();
	}
	else
	{
 */
		Serial.println("WIFI CONNECTED");
		Serial.println(WiFi.SSID());
		Serial.print("IP: ");
		Serial.println(WiFi.localIP());
//	}
 	
  Serial.println("Trying connect MQTT ...");
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
	int check_Button_1 = isButton_Click(button_1);
	int check_Button_2 = (button_2);
 
	if (!client.connected())
	{
		reconnect_mqtt();
	}
	client.loop();
	
	unsigned long currentMillis = millis();
	if (currentMillis - previousMillis > interval)
	{
		previousMillis = currentMillis;
		
		if (check_Button_1)
		{
			Serial.println("\nButton 1 - Clicked!");
			digitalWrite(control_1, !state_button_1);
			if (state_button_1)
				client.publish(CA_SW2_1, "1");
			else
				client.publish(CA_SW2_1, "0");
			
			state_button_1 = !state_button_1;
			Serial.print("Relay 1 change state to: ");
			Serial.print(state_button_1);
		}
		
		if (check_Button_2)
		{
			Serial.println("\nButton 2 - Clicked!");
			digitalWrite(control_2, !state_button_2);
			if (state_button_1)
				client.publish(CA_SW2_2, "1");
			else
				client.publish(CA_SW2_2, "0");
			
			state_button_2 = !state_button_2;
			Serial.print("Relay 2 change state to: ");
			Serial.print(state_button_2);
		}
	}
	delay(500);
}

//------------- OTHER FUNCTIONS -------------
//Callback:
void callback(char *topic, byte *payload, unsigned int length)
{
	//Topic list test is the value of variables: CA_SW2_1 and CA_SW2_2
	Serial.print("Topic [");
	Serial.print(topic);
	Serial.print("]");
	//Print message of button ID:
	for (int i = 0; i < length; i++)
	{
		Serial.print((char)payload[i]);
	}
	Serial.println();

  //Differenate the button ID: 1 - 0c38a97d-1564-4707-935c-18b4e9bcb0db & 2 - 7f704fdf-fa4b-44e2-b359-5ef19294196a
	if ((char)topic[0] == '0')
		switch ((char)payload[0])
		{
			case '1':
			digitalWrite(control_1, HIGH);
			Serial.println("Change online CA-SW2-1 to ON");
			state_button_1 = true;
			break;
			case '0':
			digitalWrite(control_1, LOW);
			Serial.println("Change online CA-SW2-1 to OFF");
			state_button_1 = false;
			break;
		}
   
	else if ((char)topic[0] == '7')
		switch ((char)payload[0])
		{
			case '1':
			digitalWrite(control_2, HIGH);
			Serial.println("Change online CA-SW2-2 to ON");
			state_button_1 = true;
			break;
			case '0':
			digitalWrite(control_2, LOW);
			Serial.println("Change online CA-SW2-2 to OFF");
			state_button_1 = false;
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
	while (!client.connected())
	{
		Serial.print("Attempting MQTT connection...");
        String clientId = "CA-SW2 - ";
        clientId += String(random(0xffff), HEX);
        Serial.println(clientId);
		
		if (client.connect(clientId.c_str(), mqtt_user, mqtt_pass))
        {
            Serial.println("Connected");

            client.publish(CA_SW2_1, "1");
            client.publish(CA_SW2_2, "1");
            client.subscribe(CA_SW2_1);
            client.subscribe(CA_SW2_2);
        }
        else
        {
            Serial.print("Failed, rc=");
            Serial.print(client.state());
            Serial.println("Try again in 5 seconds");
            delay(5000);
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
    while (digitalRead(GPIO_to_read) == 1)
    {
        delay(150);
        out = 1;
    }
    return out;
}


/**********************************************************************************
 * @func  SmartConfig
 * 
 * @brief: Initialize the information for SmartConfig
 * @param: fd - file descriptor when using function
 * 
***********************************************************************************/
//SmartConfig:
boolean startSmartConfig(){
  int t = 0;
  Serial.println("Smart Config Start");
  WiFi.beginSmartConfig();
  delay(500);
  while(WiFi.status() != WL_CONNECTED){
    t++;
    Serial.print(".");
    delay(500);
    if(t > 120){
      Serial.println("Smart Config Fail");;
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

void exitSmartConfig(){
  WiFi.stopSmartConfig();
}
