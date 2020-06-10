#pragma once

// Definition der Pins
#include "pindef.h"
#include "ledcontroller.h"


class Routine
{
  public:
  void init();
	bool goLittleBall(float speed); // Gibt zurück, ob die Kugel fallengelassen werden soll
  void tick();
	
	volatile int modus = 0; // aktueller Modus (0:schnell, 1:mitel, 2:langsam, 3:fertig)
	volatile int cnt = 0; //Zähler

  protected:
  LEDController ledcontroller;
};
