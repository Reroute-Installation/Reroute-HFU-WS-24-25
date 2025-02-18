//importieren der LED-library
#include <FastLED.h>

// Konstanten und Variablen
#define SCREEN_PIN 6
#define SCREEN_PIN2 12
#define LED_PIN 9      // Pin, an dem die Onboard-NeoPixel-LED angeschlossen ist
#define NUM_LEDS 24     // Anzahl der LEDs im LED-Ring
CRGB leds[NUM_LEDS];  // Array zur Steuerung der LEDs

//Variablen vorbereitet
int XPos = 0; // Speichert den Wert von X
int YPos = 0; // Speichert den Wert von Y
const int Sensor = 10;
int sensorValue;
int timesClicked = 0;
int timeCounter;
int DrehAktivStatus;
int DrehImpulsStatus;
int DreherAktiv;
int oldcount;
int newcount;
int finalcount;

void setup() 
{
  //Eingangs und Ausgangspins für die Sensoren usw. festlegen
  pinMode(Sensor, INPUT);
  pinMode(SCREEN_PIN, INPUT_PULLUP);
  pinMode(SCREEN_PIN2, INPUT_PULLUP);
  Serial.begin(9600); // Serielle Kommunikation initialisieren
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS); // Initialisiere die LED
  FastLED.clear();  // Alle LEDs ausschalten
  FastLED.show();
}

void loop() 
{
  checkPhotoSensor();
  checkScreen();

    while (Serial.read() != 'S')
    {
    checkPhotoSensor();
    checkScreen();
    Serial.print("Ü");
    Serial.print(finalcount);
    Serial.print("Ä");
    Serial.println();
    }
    // Daten von der seriellen Schnittstelle lesen
    if (Serial.available() > 0) 
    {
      String input = Serial.readStringUntil('X');

      if (parseData(input)) 
      {
        // Wenn erfolgreich geparst, LED basierend auf Werten steuern
        for(int i = 0; i < 24; i++)
        {
          leds[i] = CRGB(XPos % 256, YPos % 256, 0); // RGB-Werte setzen
        }
        FastLED.show();
      } 
      else 
      {
        Serial.println("Fehler beim Parsen der Daten");
      }
    }
}

// Funktion zum Parsen der Daten
bool parseData(String data) {
  // Überprüfen, ob das Format stimmt
  if (data.length() == 8) {
    // X-Wert extrahieren
    String xValue = data.substring(0, 4);

    // Y-Wert extrahieren
    String yValue = data.substring(4, 8);

    // In Ganzzahlen umwandeln
    XPos = xValue.toInt();
    YPos = yValue.toInt();

    return true; // Erfolgreiches Parsen
  }

  return false; // Fehlerhaftes Format
}
//Subfunktion zum prüfen des zustands des Photosensors
void checkPhotoSensor()
{
  //Sensorwert auslesen
  sensorValue = analogRead(Sensor);
  //Je nach ausgelsenem Wert Code für "aktiv" oder "inaktiv" über die Serielle Schnittstelle senden
  if(sensorValue >= 2)
  {
    Serial.println("Q1G");
  }
  if(sensorValue <= 0)
  {
    Serial.println("Q0G");
  }
  delay(50);
}
//Subfunktion zum auslesn der Zustandsdaten der Wählscheibe
void checkScreen()
{
  //Auslesen der beiden Schalter in der Wählscheibe
  DrehAktivStatus = digitalRead(SCREEN_PIN);
  DrehImpulsStatus = digitalRead(SCREEN_PIN2);

  if(DrehAktivStatus == LOW)
  {
    DreherAktiv = true;
    finalcount = 0;
    if(DrehImpulsStatus == HIGH && oldcount == newcount && DreherAktiv == true)
    {
      newcount++;
    }
    if(DrehImpulsStatus == LOW && newcount > 0 && DreherAktiv == true)
    {
      oldcount = newcount;
    }
  }
  else
  if(DrehAktivStatus == HIGH && DreherAktiv == true)
  {
    finalcount = oldcount;
    if(finalcount >= 10)
    {
      finalcount = 0;
    }
    DreherAktiv = false;
    oldcount = 0;
    newcount = 0; 
  }  
}