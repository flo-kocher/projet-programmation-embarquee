#include <Servo.h>

#define PERIODE_AFFICHAGE       1000000  
#define POT_PIN                 A0

Servo myServo;
int potVal;
int angle;
static unsigned long ulmicroseconds = 0;
static unsigned long ulPrecMicroseconds = 0;

void setup() {
  // put your setup code here, to run once:
  myServo.attach(9);
  
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  potVal = analogRead(potPin);  
  angle=map(potVal, 0, 1023, 0, 180);
 
  //affichage toutes les secondes sur le moniteur série
  ulmicroseconds = micros();
  if(ulmicroseconds-ulPrecMicroseconds >= PERIODE_AFFICHAGE)
  {
    Serial.print("Valeur du potentiomètre : ");
    Serial.println(potVal);
    Serial.print("angle : ");
    Serial.println(angle);

    ulPrecMicroseconds = ulmicroseconds;
  }

  myServo.write(angle);
  delay(15);
}
