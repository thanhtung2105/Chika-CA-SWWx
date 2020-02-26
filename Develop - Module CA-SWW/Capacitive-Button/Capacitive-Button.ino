/* Test capticable button - Using with NEW LED system (ticker light) - */
#include <Ticker.h>   //Specific usage with ESP

Ticker ticker;

const unsigned long timeout_1 = 7000;
const unsigned long timeout_2 = 5000;
const unsigned long timeout_3 = 3000;

int button_1 = 4;				      //@PIN_19
int button_2 = 5;				      //@PIN_20

int stateLED_control_1 = 10;	//@PIN_12
int stateLED_control_2 = 2;		//@PIN_17	

int control_1 = 16;           //@PIN_4
int control_2 = 14;           //@PIN_5

bool stateDEVICE_control_1 = 0;
bool stateDEVICE_control_2 = 0;

unsigned long lastChangedTime;

void setup()
{
  Serial.begin(115200);
  pinMode(button_1, INPUT);
  pinMode(button_2, INPUT);
  pinMode(stateLED_control_1, OUTPUT);
  pinMode(stateLED_control_2, OUTPUT);
  pinMode(control_1, OUTPUT);
  pinMode(control_2, OUTPUT);
  Serial.println("\n\n___ CA-SW2 Hello to your home ____");
}

void loop()
{
  /*
  if ((digitalRead(button_1)==0)&&(digitalRead(button_2)==0))
  {
    Serial.println("Get ready to SmartConfig ...");      
      if (!clickSmartConfig())
      {
          Serial.println("Yes! We did it!");
          ticker.attach(0.3, blinking);
      }
      else
      {
          Serial.println("Nothing special happened... :/");
          ticker.detach();
      }
  }
  */

  // check_Button_x : 0 - nothing; 1 - normal click; 2 - Setup (Button_1); 3 - Out setup (Button_2)
  int check_Button_1 = isButton_Click(button_1);
  int check_Button_2 = isButton_Click(button_2);

    if(check_Button_1 == 1)
  {
    Serial.println("\nButton 1 - Clicked!");
    digitalWrite(stateLED_control_1, stateDEVICE_control_1);
    digitalWrite(control_1, !stateDEVICE_control_1);
    
    stateDEVICE_control_1 = !stateDEVICE_control_1;
  }
    else if(check_Button_1 == 2)
  {
    Serial.println("Start SmartConfig");
    delay(2000);
    digitalWrite(stateLED_control_1,HIGH);
    digitalWrite(stateLED_control_2,LOW);
    digitalWrite(control_1,LOW);
    digitalWrite(control_2,LOW);
    ticker.attach(0.1, blinking);
  }
  
//  if(check_Button_2 == 1)
//  {
//    Serial.println("\nButton 2 - Clicked!");
//    digitalWrite(stateLED_control_2, stateDEVICE_control_2);
//    digitalWrite(control_2, !stateDEVICE_control_2);
//    
//    stateDEVICE_control_2 = !stateDEVICE_control_2;
//  }

  
   delay(100);
}

int isButton_Click(int PIN_to_read)
{
    ticker.detach();
    int out = 0;
    if (digitalRead(PIN_to_read) == 0)
    {
      lastChangedTime = millis();
      Serial.print("Last time: ");
      Serial.println(lastChangedTime);
    }
    while (digitalRead(PIN_to_read) == 0)
    {
        delay(200);
        Serial.println(millis());
        if (millis() - lastChangedTime > timeout_3)
        {
            out = 2;
            Serial.println("OK - 2 - Setup");
            break;
        }
        else
            out = 1;
    }
    return out;
}

void blinking()
{
  bool state_1 = digitalRead(stateLED_control_1);
  bool state_2 = digitalRead(stateLED_control_2);
  digitalWrite(stateLED_control_1,!state_1);
  digitalWrite(stateLED_control_2,!state_2);
}

boolean clickSmartConfig()
{
    int out = 0;
    delay(5000);
    
      if ((digitalRead(button_1)==0)&&(digitalRead(button_2)==0))
        out = 1;
      else
        out = 0;
    return out;
}
