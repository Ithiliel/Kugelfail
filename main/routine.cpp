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
	
	// TODO speed
	if (modus == 0){ // Modus schnell
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
		
	} else if (modus == 1){ // Modus mittel
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
		
		
	} else if (modus == 2){ // Modus langsam
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
		
	} else if (modus == 3){ // Modus fertig
	
		return false;
		
	} else{ // irgendwas ist schiefgegangen
		Serial.print("Ups, unbekannter Modus");
		return false;
	}

}