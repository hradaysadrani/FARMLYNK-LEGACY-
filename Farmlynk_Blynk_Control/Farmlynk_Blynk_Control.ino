/********************************************************
 *  Sensor data sent to Blynk app
 *    SOIL MOISTURE CONNECTIONS - 
 *    Soil A0 -) NODEMCU A0
 *    VCC- VCC  /// GND- GND
 *    
 *    RELAY CONECTIONS -
 *    IN1 TO NODEMCU D6
 *    VCC- VCC ///// VCC-VCC
 *******************************************************/     


/* ESP & Blynk */
#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SimpleTimer.h>
#define trigger D2
#define echo D1
#define PUMP_PIN D6              //PUMP (Red LED)
#define LAMP_PIN D7              //LAMP (Green LED) (Not connected in this project)
boolean pumpStatus = 0;
boolean lampStatus = 0;
#define PUMP_ON_BUTTON D9        //push-button PUMP (Red)
#define LAMP_ON_BUTTON D10       //push-button LAMP (Green)
#define SENSORS_READ_BUTTON D4   //push-button SENSOR (yellow)
#define soilMoisterPin A0
#define soilMoisterVcc D8
int soilMoister = 0;

char auth[] = "D-LGO2_KNt9cul5_tK6VY7fIxI6Ngk5R"; // Blynk project: "Farmlynk"
char ssid[] = "Project Network";
char pass[] = "noturwifi";


WidgetLED PUMPs(V0);  // Echo signal to Sensors Tab at Blynk App
WidgetLCD lcd(V5);
SimpleTimer timer;
String data = "";
int sr = A0;
int srdata = 0; 

void myTimerEvent()
{
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V5, millis() / 1000);
}

void setup()
{
  pinMode(A0, INPUT);
  Serial.begin(115200);
  delay(10);
  Serial.println("FARMLYNK");
  Serial.println(".... Starting Setup");
  Serial.println(" ");

  pinMode(PUMP_PIN, OUTPUT);\
  pinMode(PUMP_ON_BUTTON, INPUT_PULLUP);
  pinMode(SENSORS_READ_BUTTON, INPUT_PULLUP);
  pinMode(soilMoisterVcc, OUTPUT);
  pinMode(trigger, OUTPUT);
  pinMode(echo, INPUT);
  
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(2000L,sensorvalue); 

  PUMPs.off();
  digitalWrite(PUMP_PIN, HIGH);
  digitalWrite (soilMoisterVcc, LOW);
  digitalWrite (D5, HIGH);
  
}

void loop()
{
   long duration, distance;
  digitalWrite(trigger, LOW);  
  delayMicroseconds(2); 
  
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10); 
  
  digitalWrite(trigger, LOW);
  duration = pulseIn(echo, HIGH);
  distance = (duration/2) / 29.1;
  
  if (distance <= 6) {
    Blynk.virtualWrite(V4, 255);
}
  else {
    Blynk.virtualWrite(V4, 0);
  }

 if (distance <= 15) {
    Blynk.virtualWrite(V1, 255);
}
  else {
    Blynk.virtualWrite(V1, 0);
  }

   if (distance <= 22) {
    Blynk.virtualWrite(V2, 255);
}
  else {
    Blynk.virtualWrite(V2, 0);
  }
  timer.run(); // Initiates SimpleTimer
  Blynk.run();
}

BLYNK_WRITE(3) // Pump remote control
{
  int i=param.asInt();
  if (i==1) 
  {
    pumpStatus = !pumpStatus;
    aplyCmd();
  }
  }

/***************************************************
* Receive Commands and act on actuators
****************************************************/
void aplyCmd()
{
 
  if (pumpStatus == 1) 
  {
    Blynk.notify("FARMLYNK: Warning ==>> STATE OF PUMP CHANGED"); 
    Serial.println("FARMLYNK: Warning ==>> PUMP IS NOW ON - पंप अब चालू है");
    digitalWrite(PUMP_PIN, LOW);
    PUMPs.on();
  }

  if (pumpStatus == 0) 
  {
    Blynk.notify("FARMLYNK: Warning ==>> STATE OF PUMP CHANGED"); 
    Serial.println("FARMLYNK: Warning ==>> PUMP IS NOW OFF - पंप अब बंद है");
    digitalWrite(PUMP_PIN, HIGH);
    PUMPs.off();
    }
}

void sensorvalue()
{
srdata = analogRead(sr);
data = srdata; 

 if ( (srdata > 800)) 
 {
    lcd.clear();
    lcd.print(0,0,"DRYNESS:");
    lcd.print(11,0,data);
    lcd.print(0,1,"Soil is DRY.");
 }
 
  if ( (srdata < 800)&& (srdata > 260)) 
 {
    lcd.clear();
    lcd.print(0,0,"DRYNESS:");
    lcd.print(11,0,data);
     lcd.print(0,1,"Well watered.");
 }
 
 if  ( (srdata < 260)) 
 {
    lcd.clear();
    lcd.print(0,0,"DRYNESS:");
    lcd.print(11,0,data);
     lcd.print(0,1,"HIGH WATER LEVEL.");
 }
 
 lcd.print(11,0,data);
 data = "";
 
}

BLYNK_READ(4)

     {
            Blynk.virtualWrite(4, soilMoister);
      }
