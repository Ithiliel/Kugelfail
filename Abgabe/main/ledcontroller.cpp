#include "ledcontroller.h"
#include <Arduino.h> 
#include "pindef.h"


void LEDController::init(){
  pinMode(PIN_LED_YELLOW, OUTPUT); 
  pinMode(PIN_LED_GREEN, OUTPUT); 
}

void LEDController::tick(){
  unsigned long curtime = micros();
  bool blink_state = (curtime & BLINKTIME) != 0;
  if (state_yellow == blinking) digitalWrite(PIN_LED_YELLOW, blink_state);
  if (state_green == blinking) digitalWrite(PIN_LED_GREEN, blink_state);
  
}

void LEDController::setstates(LEDState new_state_green, LEDState new_state_yellow){
  state_green = new_state_green;
  state_yellow = new_state_yellow;
  digitalWrite(PIN_LED_YELLOW, new_state_yellow == on);
  digitalWrite(PIN_LED_GREEN, new_state_green == on);
}
