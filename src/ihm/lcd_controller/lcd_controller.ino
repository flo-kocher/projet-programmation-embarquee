// Lien Tinkercad vers le schéma Arduino permettant de tester le code ci-dessous :
// https://www.tinkercad.com/things/l8eG69AYHF5?sharecode=dKoCBTEMIGsRdDm6rqgJd6K3AFFeXGwr5NfiHCSPzuQ

// C++ code
//
/*
  LiquidCrystal Library - Hello World

   Demonstrates the use of a 16x2 LCD display.
  The LiquidCrystal library works with all LCD
  displays that are compatible with the  Hitachi
  HD44780 driver. There are many of them out
  there, and you  can usually tell them by the
  16-pin interface.

  This sketch prints "Hello World!" to the LCD
  and shows the time.

  The circuit:
  * LCD RS pin to digital pin 12
  * LCD Enable pin to digital pin 11
  * LCD D4 pin to digital pin 5
  * LCD D5 pin to digital pin 4
  * LCD D6 pin to digital pin 3
  * LCD D7 pin to digital pin 2
  * LCD R/W pin to ground
  * LCD VSS pin to ground
  * LCD VCC pin to 5V
  * 10K resistor:
  * ends to +5V and ground
  * wiper to LCD VO pin (pin 3)

  Library originally added 18 Apr 2008  by David
  A. Mellis
  library modified 5 Jul 2009  by Limor Fried
  (http://www.ladyada.net)
  example added 9 Jul 2009  by Tom Igoe
  modified 22 Nov 2010  by Tom Igoe

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/LiquidCrystal
*/

#include <LiquidCrystal.h>

#define LCD_RS_PIN 10
#define LCD_ENABLE_PIN 11
#define LCD_D4_PIN 2
#define LCD_D5_PIN 3
#define LCD_D6_PIN 4
#define LCD_D7_PIN 5

#define CONTROL_PIN 6
#define INFO_PIN 7

// Initialize LCD Screen
LiquidCrystal lcd(LCD_RS_PIN, LCD_ENABLE_PIN, LCD_D4_PIN, LCD_D5_PIN, LCD_D6_PIN, LCD_D7_PIN);

// Initialize control button pin states
int controlPinState = 0;
int prevControlPinState = 0;
// if controlMode == true : automatic mode
// if controlMode == false : manual mode
bool controlMode = false;

// Initialize info button pin states
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
  // Serial.begin(115200);
  //  Set up the number of columns and rows on the LCD
  lcd.begin(16, 2);
  // Initialize the control pin as an INPUT
  pinMode(CONTROL_PIN, INPUT);
  // Initialize the info pin as an INPUT
  pinMode(INFO_PIN, INPUT);

  // Starting prints of the LCD
  lcd.print("Projet");
  lcd.setCursor(0, 1);
  lcd.print("super cool");
}

void loop()
{
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
