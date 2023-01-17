#include <Servo.h>

#define PERIODE_AFFICHAGE       1000000  
#define POT_PIN                 A0
#define SERVO_PIN               8

Servo myServo;
int pot_val;
int angle;
int angle_prec;
static unsigned long ulmicroseconds = 0;
static unsigned long ulPrecMicroseconds = 0;

//Initialisation
void setup() {
	myServo.attach(SERVO_PIN);  
	Serial.begin(9600);
}

void loop() {
	// Lecture de la pin du potentiomètre pour récupérer sa valeur
	pot_val = analogRead(POT_PIN);  
	// Conversion de la valeur du potentiomètre en une mesure d'angle
	angle=map(pot_val, 0, 1023, 0, 180);
	
	// int temp_servo = 0;
	// int value_servo = 0;
	// if (controlMode)
    // {
    //     temp_servo = analogRead(Y_PIN);
    //     value_servo = map(temp_servo, 210, 310, 0, 1023);
    //     angleY = map(temp_servo, 210, 310, 0, 360);
    //     // sécurité pour le moteur
    //     if (value_servo > 1023)
    //     {
    //         value_servo = 1023;
    //     }
    //     else if (value_servo < 0)
    //     {
    //         value_servo = 0;
    //     }
    // }
	//
	// angle=map(value_servo, 0, 1023, 0, 180);

	// Affichage toutes les secondes sur le moniteur série
	ulmicroseconds = micros();
	if(ulmicroseconds-ulPrecMicroseconds >= PERIODE_AFFICHAGE)
	{
		Serial.print("Valeur du potentiomètre : ");
		Serial.println(pot_val);
		Serial.print("Angle : ");
		Serial.println(angle);

		ulPrecMicroseconds = ulmicroseconds;
	}

 	//contraintes des +- 20° par seconde
	if(angle < angle_prec-20)
	{
		angle = angle_prec-20;
	}
	if(angle > angle_prec+20)
	{
		angle = angle_prec+20;
	}
	angle_prec = angle;

	// Transmission de l'angle au servomoteur
	myServo.write(angle);
	delay(15);
}