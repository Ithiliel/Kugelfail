#include <Servo.h> 
 
// HALL: 3 
// PHOTO:2 
// TRIGGER: 4 
 
Servo servo; // Objekterzeugung (Konstruktor-Aufruf) 
 
int oldhall; 
int oldphoto; 
 
// the setup function runs once when you press reset or power the board 
void setup() { 
// initialize digital pin LED_BUILTIN as an output. 
pinMode(LED_BUILTIN, OUTPUT); 
pinMode(3, INPUT); 
pinMode(4, INPUT); 
Serial.begin(115200); 
servo.attach(9); 
} 
 
// the loop function runs over and over again forever 
void loop() { 
servo.write(20); 
while (digitalRead(4)==0){} 
servo.write(50); 
delay(1000); 
}
