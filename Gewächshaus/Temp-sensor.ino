/* How to use the DHT-22 sensor with Arduino uno
   Temperature and humidity sensor
*/

//Libraries
#include <DHT.h>;

//Constants
#define DHTPIN 3     // mit pin ? verbunden
#define DHTTYPE DHT22   // Daten typ
DHT dht(DHTPIN, DHTTYPE); //// DHT Sensor initialisieren

#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

float hum;  //Luftfeuchtigkeit
float temp; //Tempratur

int  Wasserlevel;

void setup()
{
  dht.begin();
  lcd.begin(16, 2);
  pinMode(2, OUTPUT);
  pinMode(A5, INPUT);
}

void loop()
{
Wasserlevel = analogRead(A5);
  
    hum = dht.readHumidity();
    temp= dht.readTemperature();
    
    if(Wasserlevel < 150){
      lcd.clear();
      lcd.print("Wasser: Low");
      digitalWrite(2, HIGH); //Pumpe an
      delay(2500);
    }else if(Wasserlevel > 150 && Wasserlevel < 350 ){
      lcd.clear();
      lcd.print("Wasser: High");
      digitalWrite(2, LOW); //pumpe aus
      delay(2500);
    }
    
    lcd.clear();
    //Gibt Luftfeuchtigkeit aus
    lcd.print("Humidity: ");
    lcd.print(hum);
    lcd.print(" %");
    delay(2500);
    lcd.clear();
    //Gibt Tempratur aus
    lcd.print("Temp: ");
    lcd.print(temp);
    lcd.print(" Celsius");
    delay(2500);
    lcd.clear();
}

   
