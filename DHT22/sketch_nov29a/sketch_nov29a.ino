#include <DHT_U.h>
#include <DHT.h>

#include <DHT_U.h>
#include <DHT.h>

#include <DHT.h>
#include <DHT_U.h>
#define DHTPIN 2
#define DHTTYPE DHT22 

DHT dht(DHTPIN, DHTTYPE);

void setup(){
  Serial.begin(9600); 
  dht.begin(); 
  delay(2000);
}
void loop(){
    Serial.print("Umidade: ");
    Serial.print(dht.readHumidity());
    Serial.print("%");
    Serial.print("\nTemperatura: ");
    Serial.print(dht.readTemperature(), 0); 
    Serial.println("*C"); 
    delay(2000); //não mexer no delay
}
