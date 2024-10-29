
// Customized code of FARMLYNK according to new BLYNK update.

/* Connections done in this project are as follows-:
Relay. D3
Btn.   D7
Soil.  A0
PIR.   D5
SDA.   D2
SCL.   D1
Temp.  D4
*/

//Include the library files
#define BLYNK_TEMPLATE_ID "TMPL3jDBxki-b"
#define BLYNK_TEMPLATE_NAME "FARMLYNK"
#define BLYNK_AUTH_TOKEN "v7kfOTuekrplO7XuhkowEmdhXMSZWRl4"

#include <LiquidCrystal_I2C.h>
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

//Initialize the LCD display
LiquidCrystal_I2C lcd(0x27, 16, 2);


char auth[] = BLYNK_AUTH_TOKEN;  //Enter your Blynk Auth token
char ssid[] = "Noice";  //Enter your WIFI SSID
char pass[] = "mirzapurwale";  //Enter your WIFI Password

DHT dht(D4, DHT22);//Configured as (DHT sensor pin,sensor type).Here, we are using DHT22 at D4. So (D4,DHT22)
BlynkTimer timer;
WidgetLCD lcd1(V6);

//Define component pins
#define soil A0     //A0 Soil Moisture Sensor

void checkPhysicalButton();
int relay1State = LOW;
int pushButton1State = HIGH;
#define RELAY_PIN_1       D3   //D3 Relay
#define PUSH_BUTTON_1     D7   //D7 Button
#define VPIN_BUTTON_1    V12 

//Create three variables for pressure
double T, P;
char status;



void setup() {
  Serial.begin(115200);
  lcd.begin();
  lcd.backlight();

 pinMode(RELAY_PIN_1, OUTPUT);
 digitalWrite(RELAY_PIN_1, LOW);
  pinMode(PUSH_BUTTON_1, INPUT_PULLUP);
  digitalWrite(RELAY_PIN_1, relay1State);


  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  dht.begin();

  lcd.setCursor(0, 0);
  lcd.print("  Initializing  ");
  for (int a = 5; a <= 10; a++) {
    lcd.setCursor(a, 1);
    lcd.print(".");
    delay(500);
  }
  lcd.clear();
  lcd.setCursor(11, 1);
  lcd.print("W:OFF");
  //Call the function
  timer.setInterval(100L, soilMoistureSensor);
  timer.setInterval(100L, DHT22sensor);
  timer.setInterval(500L, checkPhysicalButton);
}


//Get the DHT22 sensor values
void DHT22sensor() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  Blynk.virtualWrite(V0, t);
  Blynk.virtualWrite(V1, h);

  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(t);

  lcd.setCursor(9, 0);
  lcd.print("H:");
  lcd.print(h);

}


//Get the soil moisture values
void soilMoistureSensor() {
  int value = analogRead(soil);
  value = map(value, 0, 1024, 0, 100);
  value = (value - 100) * -1;

  Blynk.virtualWrite(V3, value);
  lcd.setCursor(0, 1);
  lcd.print("S:");
  lcd.print(value);
  lcd.print(" ");

if (value <= 30)
{
   lcd1.clear();
   lcd1.print(0,0, "WATER LEVEL IS:");
   lcd1.print(0,1,"LOW");
   Serial.println("WATER LVL IS LOW"); 
   delay(1000); 
}
else if (30 < value < 45)
{
   lcd1.clear();
   lcd1.print(0,0, "WATER LEVEL IS:");
   lcd1.print(0,1,"MODERATE");    
   Serial.println("WATER LVL IS OK");
   delay(1000);
}
else
{
  lcd1.clear();
   lcd1.print(0,0, "WATER LEVEL IS:");
   lcd1.print(0,1,"HIGH");   
   Serial.println("WATER LVL IS HIGH"); 
   delay(1000); 
}
  
}

BLYNK_CONNECTED() {
  // Request the latest state from the server
  Blynk.syncVirtual(VPIN_BUTTON_1);
}

BLYNK_WRITE(VPIN_BUTTON_1) {
  relay1State = !param.asInt();
  digitalWrite(RELAY_PIN_1, relay1State);
}

void checkPhysicalButton()
{
  if (digitalRead(PUSH_BUTTON_1) == LOW) {
    // pushButton1State is used to avoid sequential toggles
    if (pushButton1State != LOW) {

      // Toggle Relay state
      relay1State = !relay1State;
      digitalWrite(RELAY_PIN_1, relay1State);

      // Update Button Widget
      Blynk.virtualWrite(VPIN_BUTTON_1, relay1State);
    }
    pushButton1State = LOW;
  } else {
    pushButton1State = HIGH;
  }
}


void loop() {
if (relay1State == LOW)
{
  lcd.setCursor(8, 1);
  lcd.print("PUMP:ON ");
  }
  else if (relay1State == HIGH)
  {
    lcd.setCursor(8, 1);
    lcd.print("PUMP:OFF");
    }
    
  Blynk.run();//Run the Blynk library
  timer.run();//Run the Blynk timer

  }
