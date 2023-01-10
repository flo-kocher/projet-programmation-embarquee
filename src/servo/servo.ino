#include <Servo.h>

#define PERIODE_AFFICHAGE       1000000  
#define POT_PIN                 A0
#define SERVO_PIN               9

Servo myServo;
int potVal;
int angle;
static unsigned long ulmicroseconds = 0;
static unsigned long ulPrecMicroseconds = 0;

//Initialisation
void setup() {
	myServo.attach(SERVO_PIN);  
	Serial.begin(9600);
}

void loop() {
	// Lecture de la pin du potentiomètre pour récupérer sa valeur
	potVal = analogRead(POT_PIN);  
	// Conversion de la valeur du potentiomètre en une mesure d'angle
	angle=map(potVal, 0, 1023, 0, 180);
	
	// Affichage toutes les secondes sur le moniteur série
	ulmicroseconds = micros();
	if(ulmicroseconds-ulPrecMicroseconds >= PERIODE_AFFICHAGE)
	{
		Serial.print("Valeur du potentiomètre : ");
		Serial.println(potVal);
		Serial.print("Angle : ");
		Serial.println(angle);

		ulPrecMicroseconds = ulmicroseconds;
	}

	// Transmission de l'angle au servomoteur
	myServo.write(angle);
	delay(15);
}