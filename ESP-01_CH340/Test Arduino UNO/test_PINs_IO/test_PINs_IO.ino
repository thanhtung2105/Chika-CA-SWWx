/*
* @file     test_PINs_IO.ino
* @proc_id  Test Pins IO for Arduino UNO
* @author   Thanh Tung Phan
* @brief    Check working all of PINs I/O and note some PIN which can not use!
* @pref     UNO_Schematic.png
* @corp     Chika Corporation
* @last-mod Thursday, 23th Jan, 2020
*/


//Variables - UNO - PINs testing:
#define PIN_A0 A0
#define PIN_A1 A1
#define PIN_A2 A2
#define PIN_A3 A3
#define PIN_A4 A4
#define PIN_A5 A5

#define PIN_D2 2
#define PIN_D3 3
#define PIN_D4 4  
#define PIN_D5 5  
#define PIN_D6 6  
#define PIN_D7 7  
#define PIN_D8 8  
#define PIN_D9 9  
#define PIN_D10 10  
#define PIN_D11 11  
#define PIN_D12 12  
#define PIN_D13 13      

void setup()
{
  Serial.begin(9600);
  Serial.println("\n\n__ Chika Corporation - Test PINs UNO program __");

  pinMode(PIN_A0, OUTPUT);
  pinMode(PIN_A1, OUTPUT);
  pinMode(PIN_A2, OUTPUT);
  pinMode(PIN_A3, OUTPUT);
  pinMode(PIN_A4, OUTPUT);
  pinMode(PIN_A5, OUTPUT);
  
  pinMode(PIN_D2, OUTPUT);
  pinMode(PIN_D3, OUTPUT);
  pinMode(PIN_D4, OUTPUT);
  pinMode(PIN_D5, OUTPUT);
  pinMode(PIN_D6, OUTPUT);
  pinMode(PIN_D7, OUTPUT);
  pinMode(PIN_D8, OUTPUT);
  pinMode(PIN_D9, OUTPUT);
  pinMode(PIN_D10, OUTPUT);
  pinMode(PIN_D11, OUTPUT);
  pinMode(PIN_D12, OUTPUT);
  pinMode(PIN_D13, OUTPUT);

  Serial.println("\n  Starting ...");
  Serial.println("\n********** PIN CHECK ***********");
  Serial.println("*   ANALOG PIN	             *");
  Serial.println("*   1. A0      			     *");
  Serial.println("*   2. A1      			     *");
  Serial.println("*   3. A2      			     *");
  Serial.println("*   4. A3      			     *");
  Serial.println("*   5. A4      			     *");
  Serial.println("*   6. A5      			     *");
  Serial.println("*   DIGITAL PIN	             *");
  Serial.println("*   1. D2   			         *");
  Serial.println("*   2. D3   			         *");
  Serial.println("*   3. D4   			         *");
  Serial.println("*   4. D5   			         *");
  Serial.println("*   5. D6   			         *");
  Serial.println("*   6. D7   			         *");
  Serial.println("*   7. D8   			         *");
  Serial.println("*   8. D9   			         *");
  Serial.println("*   9. D10   			         *");
  Serial.println("*   10. D11   			     *");
  Serial.println("*   11. D12   			     *");
  Serial.println("*   12. D13   		         *");
  Serial.println("********************************");
}

void loop()
{
  digitalWrite(PIN_A0, HIGH);
  digitalWrite(PIN_A1, HIGH);
  digitalWrite(PIN_A2, HIGH);
  digitalWrite(PIN_A3, HIGH);
  digitalWrite(PIN_A4, HIGH);
  digitalWrite(PIN_A5, HIGH);

  digitalWrite(PIN_D2, HIGH);
  digitalWrite(PIN_D3, HIGH);
  digitalWrite(PIN_D4, HIGH);
  digitalWrite(PIN_D5, HIGH);
  digitalWrite(PIN_D6, HIGH);
  digitalWrite(PIN_D7, HIGH);
  digitalWrite(PIN_D8, HIGH);
  digitalWrite(PIN_D9, HIGH);
  digitalWrite(PIN_D10, HIGH);
  digitalWrite(PIN_D11, HIGH);
  digitalWrite(PIN_D12, HIGH);
  digitalWrite(PIN_D13, HIGH);

  delay(1500);

  digitalWrite(PIN_A0, LOW);
  digitalWrite(PIN_A1, LOW);
  digitalWrite(PIN_A2, LOW);
  digitalWrite(PIN_A3, LOW);
  digitalWrite(PIN_A4, LOW);
  digitalWrite(PIN_A5, LOW);

  digitalWrite(PIN_D2, LOW);
  digitalWrite(PIN_D3, LOW);
  digitalWrite(PIN_D4, LOW);
  digitalWrite(PIN_D5, LOW);
  digitalWrite(PIN_D6, LOW);
  digitalWrite(PIN_D7, LOW);
  digitalWrite(PIN_D8, LOW);
  digitalWrite(PIN_D9, LOW);
  digitalWrite(PIN_D10, LOW);
  digitalWrite(PIN_D11, LOW);
  digitalWrite(PIN_D12, LOW);
  digitalWrite(PIN_D13, LOW);
  
  delay(1500);
}
