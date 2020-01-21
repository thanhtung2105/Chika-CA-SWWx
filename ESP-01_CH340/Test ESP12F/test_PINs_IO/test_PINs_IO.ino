/*
* @file     test_PINs_IO.ino
* @proc_id  Test Pins IO for ESP12F
* @author   Thanh Tung Phan
* @brief    Check working all of PINs I/O and note some PIN which can not use!
* @pref     ESP12F_Schematic.png
* @corp     Chika Corporation
* @last-mod Monday, 13th Jan, 2020
*/

/*************************************     NOTE PINs    ******************************************
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
*************************************************************************************************/

//Variables - ESP12F - PINs testing:
#define PIN_4 16
#define PIN_5 14
#define PIN_6 12
#define PIN_7 13

#define PIN_12 10

#define PIN_17 2    //This PIN control onboard-LED of ESP12F
#define PIN_19 4
#define PIN_20 5
#define PIN_21 3    //GPIO3 - U0RXD - Use if necessary (sometime it affects to setup())
#define PIN_22 1    //GPIO1 - U0TXD - SPI_CS1 - Use if necessary (sometime it affects to setup())

void setup()
{
  Serial.begin(115200);
  Serial.println("\n\n__ Chika Corporation - Test PINs ESP12F program __");

  pinMode(PIN_4, OUTPUT);
  pinMode(PIN_5, OUTPUT);
  pinMode(PIN_6, OUTPUT);
  pinMode(PIN_7, OUTPUT);

  pinMode(PIN_12, OUTPUT);

  pinMode(PIN_17, OUTPUT);
  pinMode(PIN_19, OUTPUT);
  pinMode(PIN_20, OUTPUT);
//  pinMode(PIN_21, OUTPUT);
//  pinMode(PIN_22, OUTPUT);

  /*
  pinMode(PIN_4, INPUT);
  pinMode(PIN_5, INPUT);
  pinMode(PIN_6, INPUT);
  pinMode(PIN_7, INPUT);
  
  pinMode(PIN_12, INPUT);

  pinMode(PIN_17, INPUT);
  pinMode(PIN_19, INPUT);
  pinMode(PIN_20, INPUT);
  pinMode(PIN_21, INPUT);
  pinMode(PIN_22, INPUT);
   */

  Serial.println("\n  Starting ...");
  Serial.println("\n********** PIN CHECK ***********");
  Serial.println("*   1. Pin 4 - GPIO16          *");
  Serial.println("*   2. Pin 5 - GPIO14          *");
  Serial.println("*   3. Pin 6 - GPIO12          *");
  Serial.println("*   4. Pin 7 - GPIO13          *");
  Serial.println("*   5. Pin 12 - GPIO10         *");
  Serial.println("*   6. Pin 17 - GPIO02'        *");
  Serial.println("*   7. Pin 19 - GPIO4          *");
  Serial.println("*   8. Pin 20 - GPIO5          *");
  Serial.println("********************************");
}

void loop()
{
  digitalWrite(PIN_4, HIGH);
  digitalWrite(PIN_5, HIGH);
  digitalWrite(PIN_6, HIGH);
  digitalWrite(PIN_7, HIGH);

  digitalWrite(PIN_12, HIGH);

  digitalWrite(PIN_17, HIGH);
  digitalWrite(PIN_19, HIGH);
  digitalWrite(PIN_20, HIGH);
//  digitalWrite(PIN_21, HIGH);
//  digitalWrite(PIN_22, HIGH);

  delay(500);


  digitalWrite(PIN_4, LOW);
  digitalWrite(PIN_5, LOW);
  digitalWrite(PIN_6, LOW);
  digitalWrite(PIN_7, LOW);

  digitalWrite(PIN_12, LOW);

  digitalWrite(PIN_17, LOW);
  digitalWrite(PIN_19, LOW);
  digitalWrite(PIN_20, LOW);
//  digitalWrite(PIN_21, LOW);
//  digitalWrite(PIN_22, LOW);

  delay(500);
}
