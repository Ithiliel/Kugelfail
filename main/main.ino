#include <Servo.h> 
#include "pindef.h"
#include "discmonitor.h"
#include "routine.h"

DiscMonitor discmonitor;
Servo servo;

void interruptPhoto()
{
  discmonitor.registerPhotoTransition(digitalRead(PIN_HALL) == 0);
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  servo.attach(9); 
  Serial.println("Hej!");  
  
  attachInterrupt(digitalPinToInterrupt(PIN_PHOTO), interruptPhoto, CHANGE);
  pinMode(PIN_SWITCH, INPUT); 
  pinMode(PIN_HALL, INPUT); 
}

int triggerPosition;
unsigned long triggerDelay;
int triggerValid; //Bestimmt, ob triggerPosition und triggerDelay gültig sind
int lastCheckPosition; //Wird verwendet, damit die Berechnung nur einmal pro Segment ausgeführt wird

void loop() {
  // put your main code here, to run repeatedly:
  if (digitalRead(PIN_SWITCH) != 0) {
    return;
  }
  if (triggerValid) {
    Serial.println("> Ausloesen...");     
    while(discmonitor.currentPosition != triggerPosition) {}; //Warte auf richtiges Segment
    unsigned long triggerTime = discmonitor.lastImpulseMicros + triggerDelay; //Summe Startzeit des richtigen Segmentes + empfolene Verzögerung = endgültige Auslösezeit
    while(micros() < triggerTime) {}; //Warte auf richtige Zeit
	
	// Bittet das Mutterschiff um Erlaubnis die Expedition zu starten
    if (routine.goLittleBall(discmonitor.getSpeed())){
		// Schickt eine kühne Stahlkugel auf eine Reise ins Unbekannte.
		servo.write(20); 
		Serial.println("> Ausgeloest!");  
		delay(200);
		servo.write(50);
		Serial.println("> Servo zurueckgefahren!");  
		triggerValid = false;
		lastCheckPosition = discmonitor.currentPosition;
	}
    
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
