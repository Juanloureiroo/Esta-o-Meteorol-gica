
const int LM35 = A0; 
float temperatura; 
 

void setup() {
Serial.begin(9600); 
}
 
//Função que será executada continuamente
void loop() {
temperatura = (float(analogRead(LM35))*5/(1023))/0.01;
Serial.println(temperatura);
delay(2000);
}
