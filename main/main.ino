#include "servocontroller.h"
#include "pindef.h"
#include "discmonitor.h"
#include "routine.h"

DiscMonitor discmonitor;
ServoController servocontroller;
Routine routine;

void interruptPhoto()
{
  discmonitor.registerPhotoTransition(digitalRead(PIN_HALL) == 0);
}


void setup() {
  // Initialisiere das Raumschiff
  Serial.begin(115200);
  servocontroller.init(); 
  routine.init();
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
  routine.tick();
  
  // Haben Sie eine gültige Hyperraumgenehmigung?
  if (digitalRead(PIN_SWITCH) != 0) {
    return;
  }
  if (digitalRead(PIN_TRIGGER) != 0) {
    routine.modus = 1;
    routine.cnt = 0;
  }
  if (triggerValid) {
    //Serial.println("> Ausloesen?");     
    while(discmonitor.currentPosition != triggerPosition) {}; //Warte auf richtiges Segment
    unsigned long triggerTime = discmonitor.lastImpulseMicros + triggerDelay; //Summe Startzeit des richtigen Segmentes + empfolene Verzögerung = endgültige Auslösezeit
    while(micros() < triggerTime) {}; //Warte auf richtige Zeit
	
	// Bittet das Mutterschiff um Erlaubnis die Expedition zu starten
	//Serial.print("Geschwindigkeit:  ");
	//Serial.println(discmonitor.getSpeed());  

  if (routine.goLittleBall(discmonitor.getSpeed())){
		Serial.println("> Erlaubnis erteilt!");     
    
		// Schickt eine kühne Stahlkugel auf eine Reise ins Unbekannte.
		servocontroller.drop(); 
		//Serial.println("> Ausgeloest!");  
		delay(300);
		servocontroller.reset();
		//Serial.println("> Servo zurueckgefahren!");  
  }
		triggerValid = false;
		lastCheckPosition = discmonitor.currentPosition;

    
  }
  else {
    if (discmonitor.currentPosition != lastCheckPosition) {
      //Serial.print("> Pruefe - Jetzt in Segment ");
      //Serial.println(discmonitor.currentPosition);
      
      triggerValid = discmonitor.getRecommendedTriggerPos(&triggerPosition, &triggerDelay); //Pointer: Die Funktion schreibt ihre Erbenisse in unsere Variablen
      int nextPos = discmonitor.currentPosition + 1;
      if (nextPos > 11) nextPos -= 12; //Normalisieren
      
      if (triggerValid && triggerPosition == nextPos) { //Heißt: Wir erwarten noch eine Flanke, bis der Timer läuft und dann ausgelöst wird
        triggerValid = true;
        //Serial.println("> Ausloesen vorbereitet!");
      }
      else {
        //Serial.println("> Ergebnis verworfen - Prüfe später erneut!");     
        triggerValid = false;   
      }
      lastCheckPosition = discmonitor.currentPosition;
      
    }
    
  }
  
}
