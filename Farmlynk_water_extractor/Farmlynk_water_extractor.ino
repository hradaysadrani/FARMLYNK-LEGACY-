

// This code is modified for FARMLYNK by Hraday Sadrani (Student of K.E.S.I.A.W. ENG. MED. SCHOOL, PANVEL).
#include <ESP8266WiFi.h>
int percentage;
int map_low = 1030;
int map_high = 300;
int Relay = D6;       // D6


void setup()
{
  Serial.begin(115200);
  pinMode(A0, INPUT); // A0 pin of arduino for data sended by moisture sensor
  pinMode(D6, OUTPUT); // Set pin D6 as OUTPUT pin, to send signal to WATER EXTRACTOR (RELAY)  
  delay(300);
}
void loop()
{
  int SensorValue= analogRead(A0);
  percentage = map(SensorValue, map_low, map_high, 0, 100); 
  Serial.print(percentage);
   
  Serial.print("% - ");
  
  if(SensorValue >= 550) 
{
   Serial.println("No need to extract water - पानी निकालने की जरूरत नहीं है"); 
   digitalWrite(D6,HIGH); //if soil moisture sensor provides HIGH value THEN IT WILL OFF WATER EXTRACTOR
   delay(1000);
  }
  if(SensorValue < 550) 
{
   Serial.println("HIGH WATER, NEED TO EXTRACT IT - उच्च पानी स्तर, अतिरिक्त पानी निकालने की जरूरत है");
   digitalWrite(D6,LOW); //if soil moisture sensor provides SUPER HIGH value THEN IT WILL ON WATER EXTRACTOR
   delay(1000);
   
  }
}
