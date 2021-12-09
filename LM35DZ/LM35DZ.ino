int val;
int tempPin = A0;

void setup()
{
  Serial.begin(9600);
}
void loop()
{
  val = analogRead(tempPin);
  float mv = ( val/1024.0)*5000;
  float cel = mv/10;
  float farh = (cel*9)/5 + 32;
//Serial.print("TEMPRATURE = ");
  Serial.println(farh);
  //Serial.print("*F");
  //Serial.println();
  delay(2000);
}
