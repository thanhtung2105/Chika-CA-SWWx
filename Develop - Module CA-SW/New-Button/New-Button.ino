/* Test new capticable button with seperate LED system */
/*	USING WITH ARDUINO PIN	*/

int button_1 = 4;				      
int button_2 = 5;				    

int stateLED_control_1 = 8;	
int stateLED_control_2 = 9;		

bool stateDEVICE_control_1 = 0;
bool stateDEVICE_control_2 = 0;

void setup()
{
  Serial.begin(9600);
  pinMode(button_1, INPUT);
  pinMode(button_2, INPUT);
  pinMode(stateLED_control_1, OUTPUT);
  pinMode(stateLED_control_2, OUTPUT);
  Serial.println("\n\n___ New capticable testing ____");
}

void loop()
{
  int check_Button_1 = isButton_Click(button_1);
  int check_Button_2 = isButton_Click(button_2);

    if(check_Button_1 == 1)
  {
    Serial.println("\nButton 1 - Clicked!");
    digitalWrite(stateLED_control_1, stateDEVICE_control_1);
    digitalWrite(control_1, !stateDEVICE_control_1);
    
    stateDEVICE_control_1 = !stateDEVICE_control_1;
  }
 
  if(check_Button_2 == 1)
  {
    Serial.println("\nButton 2 - Clicked!");
    digitalWrite(stateLED_control_2, stateDEVICE_control_2);
    digitalWrite(control_2, !stateDEVICE_control_2);
    
    stateDEVICE_control_2 = !stateDEVICE_control_2;
  }

   delay(100);
}

int isButton_Click(int PIN_to_read)
{
    int out = 0;
	while (digitalRead(PIN_to_read) == 0)
	{
		delay(200);
        out = 1;
	}
    return out;
}
