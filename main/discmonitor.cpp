#include "discmonitor.h"
#include <Arduino.h>
#include <math.h>
 

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

  /*float rotspeed = 1000000.0 / (12*segmentTimes[currentPosition]);
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
  Serial.println(")");*/
  
  

  lastHallState = currentHallState;
  currentPosition++;
  if (currentPosition == 12)
    currentPosition = 0;
}

unsigned long DiscMonitor::getDropDelay() {
  return 420000;
}


//Ermittelt die Nummer des Segmentes, nach dessen Anfang eine Verzögerung von triggerdelay gestartet werden sollte, nach deren Ablauf ausgelöst werden sollte.
//Der Rückgabewert sagt aus, ob das Ergebniss gültig ist
bool DiscMonitor::getRecommendedTriggerPos(int *segment, unsigned long *triggerdelay) {
  //Serial.print("    Triggerzeitberechnung:");
  
  int pos = HOLEPOS;
  long droptime = getDropDelay();

  unsigned long rottime = 0;
  for (int i=0; i<12; i++) 
    rottime += segmentTimes[i];
    
  //rottime = Dauer der letzten Umdrehung

  float avrgSegmentTime = (float)rottime / 12.0;
  float rotSegments = (float)droptime / avrgSegmentTime - 0.5; //Anzahl der Segmente, die der Kugelfall dauert
  int numSegments = ceil(rotSegments); // Aufrunden
  float segFraction = (float)numSegments - rotSegments; // Nachkommaanteil aus der Division
  pos -= numSegments;
  while (pos < 0) pos += 12; //Normalisieren
  
  *segment = pos;
  *triggerdelay = segFraction * (float)avrgSegmentTime;

  /*Serial.print("    Ergebniss: Segment ");
  Serial.print(pos);
  Serial.print(" Trigger-Verzögerung ");
  Serial.println(*triggerdelay);*/
  return true;
}

// gibt die aktuelle Geschwindigkeit zurueck, gemittelt über 12 Segmente; bei vermutetem Fehler (Abbremsen etc.) wird 0 zurückgegeben
float DiscMonitor::getSpeed(){
	float sum = 0;
	// Wenn die Abweichung der letzten beiden gleichfarbigen Segmente zu groß ist wird 0 zurückgegeben
 int lastKnownTimePos = currentPosition-1;
 if (lastKnownTimePos < 0) lastKnownTimePos += 12;
 int lastKnownTimePos2 = currentPosition-3;
 if (lastKnownTimePos2 < 0) lastKnownTimePos2 += 12;
 //Serial.println(abs(segmentTimes[lastKnownTimePos]-segmentTimes[lastKnownTimePos2])/segmentTimes[lastKnownTimePos]);
 if (abs((float)segmentTimes[lastKnownTimePos]-(float)segmentTimes[lastKnownTimePos2]) > 0.15*segmentTimes[lastKnownTimePos]){
		return 0.0;
	}
	
	// Summe der Zeit der letzten 12 Segmente
	for(int i = 0; i<12 ; i++){
		sum+=segmentTimes[i];
	}
	
	return 1000000.0 / sum;
}
