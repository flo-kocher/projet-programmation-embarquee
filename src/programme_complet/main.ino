#include <Servo.h>
#include <LiquidCrystal.h>

#define PERIODE_AFFICHAGE 1000000

// 1s entre chaque affichage dans le Serial monitor
#define PERIODE_AFFICHAGE 1000000
// 1024 valeur => 511
#define MILIEU_POTENTIOMETRE 511
#define CONTROLH_PIN_1 9
#define CONTROLH_PIN_2 13 // était en 3
#define CONTROLH_PIN_7 12 // était en 2
#define POTENTIOMETRE_PIN A0

#define X_PIN A1
#define Y_PIN A2

#define LCD_RS_PIN 10 // était en 12
#define LCD_ENABLE_PIN 11
#define LCD_D4_PIN 5
#define LCD_D5_PIN 4
#define LCD_D6_PIN 3
#define LCD_D7_PIN 2

#define CONTROL_PIN 6
#define INFO_PIN 7

#define SERVO_PIN 8

Servo myServo;
int potVal;
int angle;
static unsigned long ulmicroseconds = 0;
static unsigned long ulPrecMicroseconds = 0;

static unsigned long ulmicroseconds = 0;
static unsigned long ulPrecMicroseconds = 0;

void set_horaire()
{
    digitalWrite(CONTROLH_PIN_7, LOW);
    digitalWrite(CONTROLH_PIN_2, HIGH);
}

void set_anti_horaire()
{
    digitalWrite(CONTROLH_PIN_7, HIGH);
    digitalWrite(CONTROLH_PIN_2, LOW);
}

// Initialize LCD Screen
LiquidCrystal lcd(LCD_RS_PIN, LCD_ENABLE_PIN, LCD_D4_PIN, LCD_D5_PIN, LCD_D6_PIN, 2LCD_D7_PIN);

// Initialize control button pin
// const int controlPin = 6;
int controlPinState = 0;
int prevControlPinState = 0;
// if controlMode == true : automatic mode
// if controlMode == false : manual mode
bool controlMode = false;

// Initialize info button pin
// const int infoPin = 7;
int infoPinState = 0;
int prevInfoPinState = 0;
// if infoMode == 0 : show info 1
// if infoMode == 1 : show info 2
// if infoMode == 2 : show info 3
int infoMode = 0;

// Function used to print on the first line of the LCD Screen
// the current control mode
void printControlPinLine()
{
    lcd.setCursor(0, 0);
    if (controlMode)
        lcd.print("AUTO");
    else
        lcd.print("MAN");

    return;
}

// Function used to print on the second line of the LCD Screen
// the current info mode
void printInfoPinLine()
{
    lcd.setCursor(0, 1);
    switch (infoMode)
    {
    case 0:
        lcd.print("Motor speed");
        break;
    case 1:
        lcd.print("Servomotor angle");
        break;
    case 2:
        lcd.print("Angle X, Angle Y");
        break;
    }

    return;
}

void setup()
{
    // put your setup code here, to run once:
    myServo.attach(SERVO_PIN);

    // Serial.begin(115200);
    // Set up the number of columns and rows on the LCD
    lcd.begin(16, 2);
    // Initialize the control pin as an INPUT
    pinMode(CONTROL_PIN, INPUT);
    // Initialize the info pin as an INPUT
    pinMode(INFO_PIN, INPUT);

    // Starting prints of the LCD
    lcd.print("Projet");
    lcd.setCursor(0, 1);
    lcd.print("super cool");

    // initialize the inputs and outputs
    pinMode(CONTROLH_PIN_7, OUTPUT);
    pinMode(CONTROLH_PIN_2, OUTPUT);
    pinMode(CONTROLH_PIN_1, OUTPUT);

    pinMode(POTENTIOMETRE_PIN, INPUT);
    pinMode(X_PIN, INPUT);

    // pull the enable pin LOW to start
    digitalWrite(CONTROLH_PIN_1, LOW);
    Serial.begin(115200);
}

void loop()
{

    // Motor
    int value = 0;

    if (!controlMode)
    {
        // Lecture de la valeur du potentiomètre
        value = analogRead(POTENTIOMETRE_PIN);
    }
    else if (controlMode)
    {
        value = map(analogRead(X_PIN), 210, 310, 0, 1023);

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
    int motorSpeed = (abs(value - MILIEU_POTENTIOMETRE)) / 2;

    if (value <= MILIEU_POTENTIOMETRE)
    {
        // horaire (511->0)
        set_horaire();
    }
    else
    {
        // anti-horaire (512->1023)
        motorSpeed--;
        set_anti_horaire();
    }

    if (controlMode)
    {
        value = map(analogRead(Y_PIN), 210, 310, 0, 1023);

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

    // affichage toutes les secondes sur le moniteur série
    angle = map(value, 0, 1023, 0, 180);
    ulmicroseconds = micros();

    if (ulmicroseconds - ulPrecMicroseconds >= PERIODE_AFFICHAGE)
    {
        if (!controlMode)
            Serial.print("Valeur du potentiomètre : ");
        else if (controlMode)
            Serial.print("Valeur de l'accéléromètre : ");
        Serial.println(value);
        Serial.print("speed : ");
        Serial.println(motorSpeed);
        Serial.print("angle servomoteur : ");
        Serial.println(angle);

        ulPrecMicroseconds = ulmicroseconds;
    }

    // On envoie la vitesse du moteur vers le pont en H
    analogWrite(CONTROLH_PIN_1, motorSpeed);

    myServo.write(angle);

    // delay(15); peut être encore utile

    // Read the control button input pin
    controlPinState = digitalRead(CONTROL_PIN);
    // See if there was a change of state
    if (controlPinState != prevControlPinState)
    {
        // If so, see if the current state is in HIGH
        if (controlPinState == HIGH)
        {
            // Clear the entire LCD
            lcd.clear();
            // Switch the state of the controlMode value
            controlMode = !controlMode;
            // Print the 2 lines of LCD
            printControlPinLine();
            printInfoPinLine();
        }
        // Delay to avoid bouncing
        delay(50);
    }
    // Save the current state as the last state
    prevControlPinState = controlPinState;

    // Read the info button input pin
    infoPinState = digitalRead(INFO_PIN);
    // See if there was a change of state
    if (infoPinState != prevInfoPinState)
    {
        // If so, see if the current state is in HIGH
        if (infoPinState == HIGH)
        {
            // Clear the entire LCD
            lcd.clear();
            // Increment the infoMode value
            infoMode++;
            infoMode %= 3;
            // Print the 2 lines of LCD
            printControlPinLine();
            printInfoPinLine();
        }
        // Delay to avoid bouncing
        delay(50);
    }
    // Save the current state as the last state
    prevInfoPinState = infoPinState;
}
