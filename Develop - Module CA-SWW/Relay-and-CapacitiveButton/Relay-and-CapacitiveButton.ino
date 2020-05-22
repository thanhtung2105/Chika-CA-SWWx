int READ = 7;
int WRITE = 8;
int state = 0;
void setup()
{
  pinMode(READ,INPUT);
  pinMode(WRITE,OUTPUT);
}

void loop()
{
  state = digitalRead(READ);
  digitalWrite(WRITE,!state);
  delay(100);
}
