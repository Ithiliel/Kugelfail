#include <Servo.h> 
#include "pindef.h"
#include "discmonitor.h"

DiscMonitor discmonitor;
Servo servo;

void interruptPhoto()
{
  discmonitor.registerPhotoTransition();
}

void interruptHall()
{
  Serial.println("Hall-interrupt!");
  if (digitalRead(PIN_HALL) == 0) // In dem Segment, in dem das Loch unter dem Vereinzeler steht, gibt es eine fallende Flanke des Hallsensors (stimmt das?)
  {
    discmonitor.registerAbsolutePosition();
  }
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  servo.attach(9); 
  Serial.println("Hej!");  
  
  attachInterrupt(digitalPinToInterrupt(PIN_PHOTO), interruptPhoto, CHANGE);
  attachInterrupt(digitalPinToInterrupt(PIN_HALL), interruptHall, CHANGE);
  pinMode(PIN_SWITCH, INPUT); 
}

int triggerPosition;
unsigned long triggerDelay;
int triggerValid; //Bestimmt, ob triggerPosition und triggerDelay gültig sind
int lastCheckPosition; //Wird verwendet, damit die Berechnung nur einmal pro Segment ausgeführt wird

void loop() {
  // put your main code here, to run repeatedly:
  if (digitalRead(PIN_SWITCH) == 0){
    return;
  }
  if (triggerValid) {
    Serial.println("> Ausloesen...");     
    while(discmonitor.currentPosition != triggerPosition) {}; //Warte auf richtiges Segment
    unsigned long triggerTime = discmonitor.lastImpulseMicros + triggerDelay; //Summe Startzeit des richtigen Segmentes + empfolene Verzögerung = endgültige Auslösezeit
    while(micros() < triggerTime) {}; //Warte auf richtige Zeit

    // Schickt eine kühne Stahlkugel auf eine Reise ins Unbekannte.
    servo.write(20); 
    Serial.println("> Ausgeloest!");  
    delay(200);
    servo.write(50);
    Serial.println("> Servo zurueckgefahren!");  
    triggerValid = false;
    lastCheckPosition = discmonitor.currentPosition;
    
  }
  else {
    if (discmonitor.currentPosition != lastCheckPosition) {
      Serial.print("> Pruefe - Jetzt in Segment ");
      Serial.println(discmonitor.currentPosition);
      
      triggerValid = discmonitor.getRecommendedTriggerPos(&triggerPosition, &triggerDelay); //Pointer: Die Funktion schreibt ihre Erbenisse in unsere Variablen
      int nextPos = discmonitor.currentPosition + 1;
      if (nextPos > 11) nextPos -= 12; //Normalisieren
      
      if (triggerValid && triggerPosition == nextPos) { //Heißt: Wir erwarten noch eine Flanke, bis der Timer läuft und dann ausgelöst wird
        triggerValid = true;
        Serial.println("> Ausloesen vorbereitet!");
      }
      else {
        Serial.println("> Ergebnis verworfen - Prüfe später erneut!");     
        triggerValid = false;   
      }
      lastCheckPosition = discmonitor.currentPosition;
      
    }
    
  }
  
}
