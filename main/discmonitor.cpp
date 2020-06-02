#include "discmonitor.h"
#include <Arduino.h>

// Bei jeder Flanke des Photosensors aufrufen
void DiscMonitor::registerPhotoTransition(bool currentHallState) {
  // prüfe, ob Absolutposition stimmt, wenn Hallwert wechselt
  if (!lastHallState && currentHallState && currentPosition != 0) {
    currentPosition = 0;
    Serial.println("WARNUNG: Position passt nicht!"); //Sollte nur ein mal ganz am Anfang auftreten
  }
  unsigned long curtime = micros();
  unsigned long oldtime = segmentTimes[currentPosition];
  segmentTimes[currentPosition] = curtime - lastImpulseMicros;
  lastImpulseMicros = curtime;

  float rotspeed = 1000000.0 / (12*segmentTimes[currentPosition]);
  float ratio = 0.0;
  if (oldtime != 0)
    ratio = (float)segmentTimes[currentPosition] / (float)oldtime;
  
  Serial.print("Segment ");
  Serial.print(currentPosition);
  Serial.print(" gerade verlassen (Dauer ");
  Serial.print(segmentTimes[currentPosition]);
  Serial.print(", entspricht grob ");
  Serial.print(rotspeed, 3);
  Serial.print(" U/s, Vehaeltniss ");
  Serial.print(ratio, 3);
  Serial.println(")");
  
  

  lastHallState = currentHallState;
  currentPosition++;
  if (currentPosition == 12)
    currentPosition = 0;
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
