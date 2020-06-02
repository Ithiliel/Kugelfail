// HALL: 3 
// PHOTO:9 
 
int oldhall; 
int oldphoto; 
 
// the setup function runs once when you press reset or power the board 
void setup() { 
// initialize digital pin LED_BUILTIN as an output. 
pinMode(LED_BUILTIN, OUTPUT); 
pinMode(3, INPUT); 
pinMode(2, INPUT); 
Serial.begin(115200); 
} 
 
// the loop function runs over and over again forever 
void loop() { 
int hall = digitalRead(3); // Liest den Inputpin  
int photo = digitalRead(2); 
if (oldhall != hall || oldphoto != photo){ 
Serial.print(micros()); 
Serial.print(";"); 
Serial.print(hall); 
Serial.print(";"); 
Serial.println(photo); 
oldhall = hall; 
oldphoto = photo; 
} 
}
