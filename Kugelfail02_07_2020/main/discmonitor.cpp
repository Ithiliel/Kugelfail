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

  float rotspeed = 1000000.0 / (12*segmentTimes[currentPosition]);
  float ratio = 0.0;
  if (oldtime != 0)
    ratio = (float)segmentTimes[currentPosition] / (float)oldtime;
  
  /*Serial.print("Segment ");
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


// Schätzt die Zeit, die das Segment segment in rev_ahead Umdrehung andauern wird
// Rückgabewert null heißt: Keine Schätzung möglich
unsigned long DiscMonitor::getExpectedSegmentTime(int segment, int rev_ahead) {
  /*Serial.print("              Dumme Fallzeitschaetzung Position ");
  Serial.print(segment);
  Serial.print(" Runde ");
  Serial.print(rev_ahead);
  Serial.print(" ==> ");
  Serial.println(segmentTimes[segment]);*/
  // Hier sollte jetzt mit Hilfe eine Schätzung der Beschleunigung abgeschätzt werden, wie schnell die Scheibe dann noch ist
  return segmentTimes[segment];
}

unsigned long DiscMonitor::getDropDelay() {
  float speed = getSpeed();
    //Serial.print("DiscMonitor::getDropDelay() - speed: "); 
    //Serial.println(speed); 

  /*if (speed > 2) {
    //Serial.println("Droptime schnell!"); 
    return 412500; //Schnell 432500 -- --412500
    //     412500 - Gut - Stand 2.7. 13:00
    //      +2 letzte korrektur
    //      +5 
  }
    
  if (speed > 1) 
    return 422500; //Mittel
    //     422500 - aus der Parabel
    
  return 392500;
  //return 370000 + 50000*speed - 10000 * speed*speed;*/

  return 361810.777 + 71654.135 *speed - 20551.378 *speed*speed;
}


//Ermittelt die Nummer des Segmentes, nach dessen Anfang eine Verzögerung von triggerdelay gestartet werden sollte, nach deren Ablauf ausgelöst werden sollte.
//Der Rückgabewert sagt aus, ob das Ergebniss gültig ist
bool DiscMonitor::getRecommendedTriggerPos(int *segment, unsigned long *triggerdelay) {
  //Serial.print("    Triggerzeitberechnung:");
  
  int pos = HOLEPOS;
  int rot = 1;
  long tdelay = DROPTARGETPOS * getExpectedSegmentTime(pos, 1) / 100;
  
  Serial.print("tdelay: ");  
  Serial.println(tdelay);   
  long droptime = getDropDelay();

  
  Serial.print("droptime: ");  
  Serial.println(droptime);   

  #if 1 //Hier auf 1 ändern, um Durchschnittszeit zu verwenden
  
  unsigned long rottime = 0;
  for (int i=0; i<12; i++) 
    rottime += segmentTimes[i];

   
  //rottime = Dauer der letzten Umdrehung
  Serial.print("1rottime: ");  
  Serial.println(rottime);   
  

  float avrgSegmentTime = (float)rottime / 12.0;
  Serial.print("2avrgSegmentTime: ");  
  Serial.println(avrgSegmentTime);
  float rotSegments = (float)droptime / avrgSegmentTime - 0.5; //Anzahl der Segmente, die der Kugelfall dauert
  Serial.print("3rotSegments: ");  
  Serial.println(rotSegments);
  int numSegments = ceil(rotSegments); //Aufrunden
  Serial.print("4numSegments: ");  
  Serial.println(numSegments);
  pos -= numSegments;
  Serial.print("5pos: ");  
  Serial.println(pos);
  while (pos < 0) pos += 12; //Normalisieren
  
  Serial.print("6pos: ");  
  Serial.println(pos);
  rot = ceil(numSegments / 12);
  
  Serial.print("7rot: ");  
  Serial.println(rot);
  tdelay = numSegments * avrgSegmentTime;
  
  Serial.print("8tdelay: ");  
  Serial.println(tdelay);   
  #else   

  while (droptime > tdelay) {
   // Serial.print("        Segment ");
   // Serial.print(pos);
   // Serial.print(" Runde ");
   // Serial.print(rot);
   // Serial.print(" Ist-Verzögerung ");
   // Serial.print(tdelay);
   // Serial.print(" Soll-Verzögerung ");
   // Serial.println(droptime);
    pos -= 1;
    if (pos < 0)
      pos = 11;
    if (pos == HOLEPOS) //Wir sind einmal rum
      rot++;
    unsigned long segmentTime = getExpectedSegmentTime(pos, rot);
    if (segmentTime == 0) {
        //Serial.println("    Abbruch - Schaetzung ungueltig!");
        return false;
    }
    tdelay += segmentTime;
  }
  #endif
 
  *segment = pos;
  
  Serial.print("9*segment: ");  
  Serial.println(*segment);
  *triggerdelay = tdelay - droptime;
  Serial.print("10*triggerdelay: ");  
  Serial.println(*triggerdelay);

  /*Serial.print("    Ergebniss: Segment ");
  Serial.print(pos);
  Serial.print(" Runde ");
  Serial.print(rot);
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
