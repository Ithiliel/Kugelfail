#include "discmonitor.h"
#include <Arduino.h>

// Bei jeder Flanke des Photosensors aufrufen
void DiscMonitor::registerPhotoTransition() {
  // ueberpruefen, ob Absolutposition stimmt
  if (absEdgeRegistered && currentPosition != 0) {
    currentPosition = 0;
    Serial.println("WARNUNG: Position passt nicht!"); //Sollte nur ein mal ganz am Anfang auftreten
  }
  unsigned long curtime = micros();
  segmentTimes[currentPosition] = curtime - lastImpulseMicros;
  lastImpulseMicros = curtime;
  
  Serial.print("Segment ");
  Serial.print(currentPosition);
  Serial.print(" gerade verlassen (Dauer ");
  Serial.print(segmentTimes[currentPosition]);
  Serial.println(")");
  
  

  absEdgeRegistered = false;
  currentPosition++;
  if (currentPosition == 12)
    currentPosition = 0;
}

// Aufrufen, wenn innerhalb des aktuellen Segmentes die Absolutpositionierung erkannt wird (Fallende Flanke HALL)
void DiscMonitor::registerAbsolutePosition() {
  absEdgeRegistered = true;
}

// Schätzt die Zeit, die das Segment segment in rev_ahead Umdrehung andauern wird
// Rückgabewert null heißt: Keine Schätzung möglich
unsigned long DiscMonitor::getExpectedSegmentTime(int segment, int rev_ahead) {
  Serial.print("              Dumme Fallzeitschaetzung Position ");
  Serial.print(segment);
  Serial.print(" Runde ");
  Serial.print(rev_ahead);
  Serial.print(" ==> ");
  Serial.println(segmentTimes[segment]);
  // Hier sollte jetzt mit Hilfe eine Schätzung der Beschleunigung abgeschätzt werden, wie schnell die Scheibe dann noch ist
  return segmentTimes[segment];
}

//Ermittelt die Nummer des Segmentes, nach dessen Anfang eine Verzögerung von triggerdelay gestartet werden sollte, nach deren Ablauf ausgelöst werden sollte.
//Der Rückgabewert sagt aus, ob das Ergebniss gültig ist
bool DiscMonitor::getRecommendedTriggerPos(int *segment, unsigned long *triggerdelay) {
  Serial.print("    Triggerzeitberechnung:");
  
  int pos = HOLEPOS;
  int rot = 1;
  long tdelay = DROPTARGETPOS * getExpectedSegmentTime(pos, 1) / 100;
  long droptime = DROPDELAY;

  while (droptime > tdelay) {
    Serial.print("        Segment ");
    Serial.print(pos);
    Serial.print(" Runde ");
    Serial.print(rot);
    Serial.print(" Ist-Verzögerung ");
    Serial.print(tdelay);
    Serial.print(" Soll-Verzögerung ");
    Serial.println(droptime);
    pos -= 1;
    if (pos < 0)
      pos = 11;
    if (pos == HOLEPOS) //Wir sind einmal rum
      rot++;
    unsigned long segmentTime = getExpectedSegmentTime(pos, rot);
    if (segmentTime == 0) {
        Serial.println("    Abbruch - Schaetzung ungueltig!");
        return false;
    }
    tdelay += segmentTime;
  }
 
  *segment = pos;
  *triggerdelay = tdelay - droptime;

  Serial.print("    Ergebniss: Segment ");
  Serial.print(pos);
  Serial.print(" Runde ");
  Serial.print(rot);
  Serial.print(" Trigger-Verzögerung ");
  Serial.println(*triggerdelay);
  return true;
}
