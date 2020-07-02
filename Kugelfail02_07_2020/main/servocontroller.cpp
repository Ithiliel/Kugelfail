#include "servocontroller.h"
#include <Arduino.h>
#include <Servo.h> 
#include "pindef.h"


void ServoController::init(){
  servo.attach(PIN_SERVO); 
  reset();
}

void ServoController::drop(){
  servo.write(SERVO_DROPPOS);
}

void ServoController::reset(){
  servo.write(SERVO_RESETPOS);
}
