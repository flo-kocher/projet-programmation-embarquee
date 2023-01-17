#include <Servo.h>
#include <LiquidCrystal.h>


/*
  Constantes
*/
///////////////////////////////////////////////////////////
/*
  Constantes moteur
*/
//1024 valeur => 511 
#define MILIEU_POTENTIOMETRE    511 

//gère la pwm
#define CONTROLH_PIN_1          9

//controle le sens horaire
#define CONTROLH_PIN_2          13

//controle le sens anti-horaire
#define CONTROLH_PIN_7          12 

//pin analogique du potentiomètre
#define POTENTIOMETRE_PIN       A0   

/*
  Constantes accelerometre
*/
#define X_PIN A1
#define Y_PIN A2

/*
  Constantes LCD
*/
#define LCD_RS_PIN 10 // était en 12
#define LCD_ENABLE_PIN 11
#define LCD_D4_PIN 2
#define LCD_D5_PIN 3
#define LCD_D6_PIN 4
#define LCD_D7_PIN 5

#define CONTROL_PIN 6
#define INFO_PIN 7

/*
  Constante Servo Moteur
*/
#define SERVO_PIN 8

/*
  Constante globale
*/
// 1s entre chaque affichage dans le Serial monitor
#define PERIODE_AFFICHAGE 1000000
///////////////////////////////////////////////////////////

/*
  Variables globales
*/
///////////////////////////////////////////////////////////
/*
  Servo moteur
*/
Servo myServo;
int angle;
int angle_prec;

/*
  ecran lcd
*/
// Initialisation de l'écran LCD
LiquidCrystal lcd(LCD_RS_PIN, LCD_ENABLE_PIN, LCD_D4_PIN, LCD_D5_PIN, LCD_D6_PIN, LCD_D7_PIN);

// Initialisation du bouton permettant de changer de mode
int controlPinState = 0;
int prevControlPinState = 0;
// si controlMode == true : mode automatique
// si controlMode == false : mode manuel
bool controlMode = false;

// Initialisation du bouton permettant de mettre à jour les 
// valeurs du servomoteur et du moteur
int infoPinState = 0;
int prevInfoPinState = 0;
// si infoMode == 0 : afficher valeurs moteur
// si infoMode == 1 : afficher valeurs servomoteur
// si infoMode == 2 : afficher valeurs angles
int infoMode = 0;

/*
  moteur
*/
//variable contenant la vitesse du moteur
long vitesse = 0;

//variable contenant le sens de rotation du moteur
char symb = '+';

//valeur récupérée d'une entrée (pot, accelerometre, …)
int value = 0;

/*
  accelerometre
*/
int angleX = 0;
int angleY = 0;

/*
  General
*/
static unsigned long ulmicroseconds = 0;
static unsigned long ulPrecMicroseconds = 0;

///////////////////////////////////////////////////////////

/*
  Fonctions
*/
///////////////////////////////////////////////////////////

/*
  Moteur
*/
//met le sens horaire du moteur
void set_horaire()
{
    digitalWrite(CONTROLH_PIN_7, LOW);
    digitalWrite(CONTROLH_PIN_2, HIGH);
}

//met le sens anti-horaire du moteur
void set_anti_horaire()
{
    digitalWrite(CONTROLH_PIN_7, HIGH);
    digitalWrite(CONTROLH_PIN_2, LOW);
}


/*
  LCD
*/
// Fonction utilisé pour afficher sur l'écran LCD le mode de contrôle actuel
void printControlPinLine()
{
    lcd.setCursor(0, 0);
    if (controlMode)
        lcd.print("AUTO");
    else
        lcd.print("MAN");

    return;
}

// Fonction utilisé pour afficher sur le LCD les valeurs du moteur et servomoteur actuels
void printInfoPinLine()
{
    lcd.setCursor(6, 0);
    switch (infoMode)
    {
    case 0:     
        lcd.print("- Motor");
        lcd.setCursor(0, 1);
        lcd.print(symb);
        lcd.print(vitesse);
        lcd.print("tr/min");
        break;
    case 1:
        lcd.print("- Servo");
        lcd.setCursor(0, 1);
        lcd.print(angle);
        lcd.print(" degres");
        break;
    case 2:
        if(controlMode)
        {
            lcd.print("Angle X, Y");
            lcd.setCursor(0, 1);
            lcd.print("(");
            lcd.print(angleX);
            lcd.print(",");
            lcd.print(angleY);
            lcd.print(")");
        }
        else
        {
            infoMode++;
            infoMode %= 3;  
        }
        break;
    }

    return;
}
///////////////////////////////////////////////////////////

void setup()
{
    /*
      Servo moteur init
    */
    // put your setup code here, to run once:
    myServo.attach(SERVO_PIN);

    /*
      LCD init
    */
    // Set up le nombre de colonnes et lignes sur le LCD
    lcd.begin(16, 2);
    // Initialisation du control pin en INPUT
    pinMode(CONTROL_PIN, INPUT);
    // Initialisation du info pin en INPUT
    pinMode(INFO_PIN, INPUT);

    // Affichage de départ du LCD
    lcd.print("Projet");
    lcd.setCursor(0, 1);
    lcd.print("super cool");

    /*
      moteur init
    */
    // initialize the inputs and outputs
    pinMode(CONTROLH_PIN_7, OUTPUT);
    pinMode(CONTROLH_PIN_2, OUTPUT);
    pinMode(CONTROLH_PIN_1, OUTPUT);

    pinMode(POTENTIOMETRE_PIN, INPUT);

    /*
      accelerometre
    */
    pinMode(X_PIN, INPUT);
    pinMode(Y_PIN, INPUT);

    // pull the enable pin LOW to start
    digitalWrite(CONTROLH_PIN_1, LOW);
    Serial.begin(115200);
}

void loop()
{
    /*
      accelerometre
    */
    int temp_servo = 0;

    //stocke la valeur de l'axe X temporairement
    int temp_moteur = 0;

    /*
      Moteur
    */
    ///////////////////////////////////////////////////////
      

    //lecture pour le moteur
    if (!controlMode)
    {
        // Lecture de la valeur du potentiomètre
        value = analogRead(POTENTIOMETRE_PIN);
    }
    else if (controlMode)
    {
        //lecture de la valeur de l'accelerometre
        temp_moteur = analogRead(X_PIN);

        //conversion en valeur d'un potentiometre
        value = map(temp_moteur, 210, 310, 0, 1023);

        //conversion en valeur d'un angle
        angleX = map(temp_moteur, 210, 310, 0, 360);

        // sécurité pour le moteur
        if (value > 1023)
        {
            value = 1023;
        }
        else if (value < 0)
        {
            value = 0;
        }
    }

    // Explications motorSpeed
    // - On utilise abs() pour avoir constamment une valeur positive
    //
    // - On fait la différence de la valeur du potentiometre par MILIEU_POTENTIOMETRE
    //   pour avoir d'un côté la plage anti horaire et de l'autre la plage horaire
    //
    // - Enfin on divise par 2 pour avoir une valeur comprise entre 0 et 255 pour
    //   être dans la plage du moteur.
    int motorSpeed = ((abs(value - MILIEU_POTENTIOMETRE)) / 2);

    if (value <= MILIEU_POTENTIOMETRE)
    {
        symb = '+';
        // horaire (511->0)
        set_horaire();
    }
    else
    {
        motorSpeed--;
        symb = '-';
        // anti-horaire (512->1023)
        set_anti_horaire();
    }
    vitesse = map(motorSpeed, 0, 255, 0, 12000);

    // On envoie la vitesse du moteur vers le pont en H
    analogWrite(CONTROLH_PIN_1, motorSpeed);
    ///////////////////////////////////////////////////////

    /*
      Servo moteur
    */
    ///////////////////////////////////////////////////////
    int value_servo = value;
    //Pour le servo moteur 
    if (controlMode)
    {
        temp_servo = analogRead(Y_PIN);
        value_servo = map(temp_servo, 210, 310, 0, 1023);
        angleY = map(temp_servo, 210, 310, 0, 360);
        // sécurité pour le moteur
        if (value_servo > 1023)
        {
            value_servo = 1023;
        }
        else if (value_servo < 0)
        {
            value_servo = 0;
        }
    }

    // affichage toutes les secondes sur le moniteur série
    angle = map(value_servo, 0, 1023, 0, 180);
    ///////////////////////////////////////////////////////



    /*
      LCD
    */
    ///////////////////////////////////////////////////////
    // Lecture de la valeur du control pin
    controlPinState = digitalRead(CONTROL_PIN);
    // On regarde s'il y a eu un changement d'état
    if (controlPinState != prevControlPinState)
    {
        // Si oui, regarder si ce changement d'état est HIGH
        if (controlPinState == HIGH)
        {            
            // On change le mode de contrôle
            controlMode = !controlMode;          
        }
    }
    // On enregistre l'état actuel comme étant l'ancien
    prevControlPinState = controlPinState;

    // Lecture de la valeur de l'info pin
    infoPinState = digitalRead(INFO_PIN);
    // On regarde s'il y a eu un changement d'état
    if (infoPinState != prevInfoPinState)
    {
        // Si oui, regarder si ce changement d'état est HIGH
        if (infoPinState == HIGH)
        {
            // Incrémentation de la valeur infoMode
            infoMode++;
            infoMode %= 3;           
        }
    }
    // Le nouvel état devient l'ancien état
    prevInfoPinState = infoPinState;
    ///////////////////////////////////////////////////////

    /*
      Affichage moniteur série et lcd
    */
    ///////////////////////////////////////////////////////
    ulmicroseconds = micros();

    if (ulmicroseconds - ulPrecMicroseconds >= PERIODE_AFFICHAGE)
    {
        if (!controlMode)
            Serial.print("Valeur du potentiomètre : ");
        else if (controlMode)
            Serial.print("Valeur de l'accéléromètre : ");
        Serial.print(value);
        Serial.print(" - ");
        Serial.println(value_servo);

        //debug moteur
        Serial.print("speed : ");
        Serial.print(motorSpeed);
        Serial.print(" - ");
        Serial.print(symb);
        Serial.print(vitesse);
        Serial.println(" tr/min");

        //debug servo
        Serial.print("angle servomoteur : ");
        Serial.println(angle);

        //debug accelerometre
        Serial.print("acc : X = ");
        Serial.print(temp_moteur);
        Serial.print(" Y = ");
        Serial.println(temp_servo);
        

        //lcd
        lcd.clear();
        // On met à jour les 2 lignes du LCD
        printControlPinLine();
        printInfoPinLine();

        //servo moteur
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

        myServo.write(angle);

        //maj du temps
        ulPrecMicroseconds = ulmicroseconds;
    }
    ///////////////////////////////////////////////////////
}
