#pragma once

// Definition der Pins
#include "pindef.h"


class Routine
{
  public:
    void startFast(); // Fallsequenz fuer schnellen Modus starten
	  void startMedium(); // Fallsequenz fuer mittleren Modus starten
    void startSlow(); // Fallsequenz fuer langsamen Modus starten
};
