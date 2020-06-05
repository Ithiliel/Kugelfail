#include "routine.h"
#include <Arduino.h>


bool goLittleBall(float speed){
	// Test, ob die Erkennung von Fehlzuständen klappt. Wenn ja, löschen
	if (speed != 0){
		return true;
	else
		return false;
	}
	// hier Test-Ende
	
	// TODO LED
	// Modus schnell
	if (modus == 0){ 
		if (speed > 1.75 && < 2.5){ // wenn Geschwindigkeit == schnell
	
			if (cnt > 4){ // dieser Fall solllte nicht auftreten, etwas ist schiefgegangen
				Serial.print(cnt);
				Serial.println(" - Fehler in Modus Schnell");
				return false;
			}
			
			if (cnt = 4){ // letzte Kugel
				cnt = 0;
				modus = 1;
				return true;
			} else { // nicht letzte Kugel; alternierend fallen lassen und frei lassen
				if (cnt%2 == 0){
					cnt++;
					return true;
				} else {
					cnt++;
					return false;
				}
			}			
		} else { // falsche Geschwindigkeit
			return false;
		}
		
	// Modus mittel	
	} else if (modus == 1){ 
		if (speed > 1 && speed < 1.75){ // Geschwindigkeit == mittel
		
			if (cnt = 8){ // letzte Kugel
				cnt = 0;
				modus = 2;
				return true;
			} else if (cnt < 8){ // nicht letzte Kugel
				if (cnt==0 || cnt==2 || cnt==3 || cnt==6 || cnt==7){ // Fallen lassen!
					cnt++;
					return true;
				} else { // nicht fallen lassen
					cnt++;
					return false;
				}			
				
			} else {// dieser Fall solllte nicht auftreten, etwas ist schiefgegangen
				Serial.print(cnt);
				Serial.println(" - Fehler in Modus mittel");
				return false;
			}
		} else { // falsche Geschwindigkeit
			return false;
		}
		
	// Modus langsam	
	} else if (modus == 2){ 
		if (speed > 0.25 && speed < 1){
		
			if (cnt = 2){ // letzte Kugel
				cnt = 0;
				modus = 3;
				return true;
			} else {
				if (cnt < 2){
					cnt ++;
					return true;
				} else { // dieser Fall solllte nicht auftreten, etwas ist schiefgegangen
					Serial.print(cnt);
					Serial.println(" - Fehler in Modus langsam");
					return false;
				}
			}
		} else { // falsche Geschwindigkeit
			return false;
		}
		
	// Modus fertig
	} else if (modus == 3){ 
	
		return false;
		
	} else{ // irgendwas ist schiefgegangen
		Serial.print("Ups, unbekannter Modus");
		return false;
	}

}