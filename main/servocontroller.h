#pragma once
#include <Servo.h> 

// Definition der Pins
#include "pindef.h"

#define SERVO_DROPPOS 50
#define SERVO_RESETPOS 20

class ServoController
{
  public:
	  void init();
    void drop(); // Löst eine Kugel aus
    void reset(); // Fährt den Server in die Ausgangsposition zurück

  protected:
    Servo servo;
};
