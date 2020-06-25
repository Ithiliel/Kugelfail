#include <Servo.h> 
 
// HALL: 3 
// PHOTO:2 
// TRIGGER: 4 
 
#define PIN_PHOTO 2

Servo servo; // Objekterzeugung (Konstruktor-Aufruf) 

void interruptPhoto()
{
    Serial.print("Photosensor ist jetzt ");
    Serial.println(digitalRead(PIN_PHOTO));
}

// the setup function runs once when you press reset or power the board 
void setup() { 
// initialize digital pin LED_BUILTIN as an output. 
pinMode(LED_BUILTIN, OUTPUT); 
pinMode(3, INPUT); 
pinMode(4, INPUT); 
Serial.begin(115200); 
attachInterrupt(digitalPinToInterrupt(PIN_PHOTO), interruptPhoto, CHANGE);
servo.attach(9); 
} 
 
// the loop function runs over and over again forever 
void loop() { 
// Nur wenn der Trigger gedrückt ist wird der Servo ausgelöst
while (digitalRead(4)==0){} 
Serial.println("Servo auf");
servo.write(50); 
delay(1000); 
Serial.println("Servo zu");
servo.write(20); 
}
