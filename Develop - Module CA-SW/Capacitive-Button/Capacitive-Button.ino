/* Test capticable button - Usage with OUT - */
#include <Ticker.h>
int btn1 = 13;
int btn2 = 12;
#define SmartConfig_Led_1 1
//Ticker ticker;

void setup()
{
  Serial.begin(115200);
  pinMode(btn1, INPUT);
  pinMode(btn2, INPUT);
  pinMode(SmartConfig_Led_1, OUTPUT);
}

void loop()
{
  Serial.println("___ CA-SW2 Hello to your home ____");
  Serial.println("Nothing special happened... :/");
  /*
  while ((digitalRead(btn1)==0)&&(digitalRead(btn2)==0))
    {      
      if (!clickSmartConfig())
      {
        Serial.println("Nothing special happened... :/");
        ticker.detach();
      }
      else
      {
          Serial.println("He cmn lo, thanh cong roi do con cho!! No SmartConfig duoc roi ne :))");
          ticker.attach(0.1, blinking);
      }
    }
    */
   delay(2000);
}
/*
void blinking()
{
  int state = digitalRead(SmartConfig_Led_1);
  digitalWrite(SmartConfig_Led_1,!state);
}

boolean clickSmartConfig()
{
    int out = 0;
    delay(5000);
      if ((digitalRead(btn1)==0)&&(digitalRead(btn2)==0))
        out = 1;
      else
        out = 0;
        
    return out;
}
*/
