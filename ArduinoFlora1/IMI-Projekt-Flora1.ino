//Zuweisung der programminternen Namen an die physischen Adressen der jeweiligen Pins

const int Uhr = 12;
const int Kipp = 0;
const int schalterLampe = 9;
const int buzzer =10;
const int motor = 6;
const int schublade = 1;


//Setup-funktion: wird 1xMal beim Start des Mikrocontrollers ausgeführt
void setup(void) 
{
  //Eingangs und Ausgangspins für die Sensoren usw. festlegen
  pinMode(Kipp, INPUT_PULLUP);
  pinMode(schalterLampe, INPUT_PULLUP);
  pinMode(schublade, INPUT_PULLUP);
  pinMode(Uhr, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(motor,OUTPUT);

}
//Haupfuktion: wird immer immer "im loop" durchlaufen
void loop() 
{
  //Wenn der Kippschalter aktiv ist (Lampenhals nach unten gebogen), laufen die Zeiger der Uhr
  if(digitalRead(Kipp) == HIGH)
  {
    digitalWrite(Uhr, HIGH);
  }
  //Wenn der Kippschalter inaktiv ist (Lampenhals nach oben gebogen), stehen die Zeiger der Uhr
  if(digitalRead(Kipp) == LOW)
  {
    digitalWrite(Uhr, LOW);
  }
  //Lampenschalter aus --> Buzzer in Wecker an
  if(digitalRead(schalterLampe) == LOW)
  {
    tone(buzzer, 440);
  }
  //Lampenschalter an --> Buzzer in Wecker aus
  if(digitalRead(schalterLampe) == HIGH)
  {
    noTone(buzzer);
  }
  //Schubladenschalter geöfnet --> Motor an
  if (digitalRead(schublade) == LOW)
  {
    analogWrite(motor, 50);
  }
  //Schubladenscalter geschlossen -> Motor aus
  if (digitalRead(schublade) == HIGH)
  {
    analogWrite(motor, 0);
  }

  
}
