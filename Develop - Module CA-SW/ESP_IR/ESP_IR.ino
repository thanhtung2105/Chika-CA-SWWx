#include <IRrecv.h>
#include <IRsend.h>
#include <IRremoteESP8266.h>
#include <IRutils.h>

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <Ticker.h>
#include <ESP8266HTTPClient.h>
#include <SoftwareSerial.h>


#define action_led D0
#define state_led D2
#define btn_config D8
//#define btn_lear
#define IR_LED D6
#define recvPin D3

unsigned int buttonTimer = 0;
unsigned int longPressTime = 2000;

boolean buttonActive = false;
boolean longPressActive= false;
boolean smartConfigStart = false;
boolean upToken = false;
boolean learning = false;
boolean sended = false;

const char* mqtt_server = "14.169.230.167";
const int mqtt_port = 2502;
const char* mqtt_user = "chika";
const char* mqtt_pass = "2502";

char* IR_module = "IR_Learning_1";
char* topicLearn = "IR_Learning_1/learn";
char* topicControl = "IR_Learning_1/control";
char* topicDone = "IR_Learning_1/done";
char* topicCancel = "IR_Learning_1/cancel";

String IRlearn_ID;
int IR_value;
String fullToken;
int ignore;


IRrecv irrecv(recvPin);
IRsend irsend(IR_LED);

decode_results results;

Ticker ticker;

WiFiClient esp;
PubSubClient client(esp);

void setup() {
  Serial.begin(115200);
  Serial.println("IR Device is ready");

  ignore = 0;
  irsend.begin();
  irrecv.enableIRIn(); // Start the receiver
  Serial.println("IRrecv is currently running and waiting for IR message");
  Serial.println(recvPin);
  
  WiFi.setAutoConnect(true);
  WiFi.setAutoReconnect(true);
  WiFi.mode(WIFI_STA);

  pinMode(action_led,OUTPUT);
  pinMode(btn_config,INPUT);
  pinMode(state_led,OUTPUT);
  
  delay(10000);
  if(!WiFi.isConnected()){
    startSmartConfig();
  }else{
    digitalWrite(state_led,HIGH);
    Serial.println("WIFI CONNECTED");
    Serial.println(WiFi.SSID());
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
  }


  client.setServer(mqtt_server,mqtt_port);
  client.setCallback(callback);
}


//-------------------------------------------------------------
void loop() {
  longPress();
  if(WiFi.status() == WL_CONNECTED){
    if(client.connected()){
      client.loop();
      HTTPClient http;
    if(!upToken){
      fullToken = getToken(http,"taiphuc","chika2502");
      Serial.println(fullToken);
    }

    if(sended){
      irsend.sendSony(IR_value,12);
      IR_value = 0;
      Serial.println("IR_control");
      sended = false;
    }
    
    while(learning){
      digitalWrite(state_led,LOW);
      tick();
      client.loop();
      if(irrecv.decode(&results)){
        Serial.print("HEX: ");
        serialPrintUint64(results.value,HEX);
        Serial.print("\t DEC:");
        serialPrintUint64(results.value,DEC);
        Serial.println("");
        if(httpPUT(http, fullToken, IRlearn_ID, results.value))
          client.publish(topicDone,"Successfull");
        else
          client.publish(topicDone,"Failure");
        learning = false;
        digitalWrite(state_led,HIGH);
        digitalWrite(action_led,LOW);
        irrecv.resume();
        delay(200);
     }
     delay(200);
    }
    defaultLed();
    
    }else{
      reconnect();
    }
  }else {
    Serial.println("WiFi Connected Fail");
    WiFi.reconnect();
  }
  irrecv.resume();
  delay(200);
}

//----------------------------------------------------------------------
void defaultLed(){
      digitalWrite(state_led,HIGH);
      digitalWrite(action_led,LOW);
}

void callback(char * topic, byte* payload , unsigned int length){
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  String data;
  String mtopic = (String)topic;
  
  for(int i = 0; i < length; i++){
    data += (char)payload[i];
  }
  Serial.println(data);

  if(mtopic == "IR_Learning_1/control"){
    Serial.println("IR_send");
    for(int i = 0; i < length; i++){
      IR_value += ((int)payload[i] - 48)*power10(length - i - 1); 
    }
    Serial.println(IR_value);
    sended = true;
  }
    
  if(mtopic == "IR_Learning_1/learn"){
    Serial.println("Learning");
    if(ignore > 0){
     IRlearn_ID = data;
     learning = true;
    }else ignore++;
  }

  if(mtopic == "IR_Learning_1/cancel"){
    learning = false;
  }
}

void reconnect(){
    Serial.println("Attempting MQTT connection ...");
    String clientId = "ESP8266Client-testX";
    clientId += String(random(0xffff),HEX);
    if(client.connect(clientId.c_str(),mqtt_user,mqtt_pass)){
      Serial.println("connected");
      client.subscribe(topicLearn);
      client.subscribe(topicControl);
      client.subscribe(topicCancel);
    }else {
      Serial.print("MQTT Connected Fail, rc = ");      Serial.print(client.state());
      Serial.println("try again in 5 seconds");
    }
}




//--------------------------------------------------------------------------------
boolean httpPUT(HTTPClient& http, String token, String id, long value){
  Serial.println("PUT Method/Procedure");
  http.begin("http://chika-server.herokuapp.com/ir");
  http.addHeader("Content-Type","application/json");
  http.addHeader("Authorization",token);

  int httpCode = http.PUT("{\"id\":\""+ id + "\",\"value\":\"" + value + "\"}");
  if(httpCode > 0){
    Serial.println("PUT success");
    String payload = http.getString();
    Serial.println(payload);
    return true;
  }else
    return false;
}

String getToken(HTTPClient& http, String user , String pass){
  String g_token;
  Serial.println("GET TOKEN");
  http.begin("http://chika-server.herokuapp.com/auth/signin");
  http.addHeader("Content-Type", "application/json");
  int httpCode = http.POST("{\"usernameOrEmail\":\"" + user + "\",\"password\":\"" + pass + "\"}");
  if(httpCode > 0)
  {
    String payload = http.getString();
    Serial.println(httpCode);
    StaticJsonDocument<500> jsonDoc;
    DeserializationError error = deserializeJson(jsonDoc,payload);
    if(error)
    {
      Serial.println("Get token fail");
      delay(3000);
      return "Fail";
    }
    String type = jsonDoc["tokenType"];
    String token = jsonDoc["accessToken"];
    g_token = type + " " + token;
    upToken = true;
    return g_token;
  }
  http.end();
}

void longPress(){
  if (digitalRead(btn_config) == HIGH) {
    Serial.println(digitalRead(btn_config));
    if (buttonActive == false) {
      buttonActive = true;
      buttonTimer = millis();
    }
    if ((millis() - buttonTimer > longPressTime) && (longPressActive == false)) {
      longPressActive = true;
      digitalWrite(state_led,LOW);
      startSmartConfig();
    }
  } else {
    if (buttonActive == true) {
      if (longPressActive == true) {
        longPressActive = false;
      } 
      buttonActive = false;
    }
  }
}

void tick(){
  int state = digitalRead(action_led);
  digitalWrite(action_led,!state);
}

boolean startSmartConfig(){
  int t = 0;
  Serial.println("Smart Config Start");
  WiFi.beginSmartConfig();
  delay(500);
  ticker.attach(0.1,tick);
  while(WiFi.status() != WL_CONNECTED){
    t++;
    Serial.print(".");
    delay(500);
    if(t > 120){
      Serial.println("Smart Config Fail");
      smartConfigStart = false;
      ticker.attach(0.5,tick);
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
  digitalWrite(action_led,LOW);
  digitalWrite(state_led,HIGH);
}

int power10(int b){
  
  switch(b){
    case 0: return 1;
    case 1: return 10;
    case 2: return 100;
    case 3: return 1000;
    case 4: return 10000;
    case 5: return 100000;
    case 6: return 1000000;
    case 7: return 10000000;
  }
//  if(b == 0) return 1;
//  else{
//      for (int i = 0; i < b ; i ++){
//          a *= base;
//      }
//     return a;
//  }

}
