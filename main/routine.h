#pragma once

// Definition der Pins
#include "pindef.h"


class Routine
{
  public:
	bool goLittleBall(float speed); // Gibt zurück, ob die Kugel fallengelassen werden soll
	
	volatile int modus = 0; // aktueller Modus (0:schnell, 1:mitel, 2:langsam, 3:fertig)
	volatile int cnt = 0; //Zähler
};
