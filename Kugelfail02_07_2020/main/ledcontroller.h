#pragma once
#include <Servo.h> 

// Definition der Pins
#include "pindef.h"

#define BLINKTIME 262144 // =2**18 Mikrosekunden, nicht leichtfertig ändern - muss eine Zweierpotenz sein, um eine teure Division zu sparen!

enum LEDState { off, on, blinking };

class LEDController
{
  public:
	  void init();
    void tick(); // Muss regelmäßig aufgerifen werden (Prüft. ob blinkende LED umgeschaltet werden sollen)
    void setstates(LEDState state_green, LEDState state_yellow);

  protected:
    LEDState state_green = off;
    LEDState state_yellow = off;
    unsigned long lastToggleTime = 0;
};
