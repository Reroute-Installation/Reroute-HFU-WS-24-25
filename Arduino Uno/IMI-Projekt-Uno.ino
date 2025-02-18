
//Importiern der LCD library
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
//Anschlusspin für den Telefontaster
const int tasterPin = 7;
//Hilfsvariablen für das Programm
int LCDStatus = 0;
int currentMessage = 0;
unsigned long startMillis;
unsigned long currentMillis;
const unsigned long period = 5000;
unsigned long endMillis = 5000;
int switchStatus;
//Arrays mit den darzustellenden Daten auf dem LCD
String messagesTop[10] = { "Karlsruhe", "Stuttgart", "New York", "Berlin", "Hong Kong", "Wakanda", "Venus", "Pluto", "Sonne", "Minas Tirith" };
String messagesBottom[10] = { "12", "8", "-6", "5", "24", "36", "464", "-233", "ca. 6000", "25" };
String Celsius = "C";

void setup() 
{
  //Setup für das LCD (2 Zeilen mit jeweils 16 Zeichen)
  lcd.begin(16, 2);
  //Moduszuweisung für den Pin des Telefontasters
  pinMode(tasterPin, INPUT_PULLUP);
  //Starten des LCD
  StartLCD();
}

// the loop routine runs over and over again forever:
void loop() 
{
  LCDControl();
  if(millis() > endMillis && switchStatus == 0)
  {
    StartLCD();
    switchStatus = 1;

  }
}

void StartLCD()
{
  startMillis = millis();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Furtwangen");
  lcd.setCursor(0,1);
  lcd.print("-2");
  lcd.print((char)223);
  lcd.print("C");
  LCDStatus =0;
}
  void LCDControl()
  {

    if(digitalRead(tasterPin) == LOW && LCDStatus == 0)
    {

      currentMillis = millis();
      endMillis = currentMillis + period;
      LCDStatus = 1;
      switchStatus = 0;
      currentMessage++;
      if(currentMessage == 10)
      {
        currentMessage = 0;
      }

    }
    if(digitalRead(tasterPin) == HIGH && LCDStatus == 1)
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print(messagesTop[currentMessage]);
      lcd.setCursor(0,1);
      lcd.print(messagesBottom[currentMessage]);
      lcd.print((char)223);
      lcd.print(Celsius);
      LCDStatus =0;
    }
  }

  
