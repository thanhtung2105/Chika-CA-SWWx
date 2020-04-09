int W = 7;

void setup()
{
  pinMode(W,OUTPUT);
}
void loop()
{
  digitalWrite(W,HIGH);
  delay(500);
  digitalWrite(W,LOW);
  delay(500);
}
