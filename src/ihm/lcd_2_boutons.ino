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

int seconds = 0;

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

const int controlPin = 6;
int controlPinState = 0;
int prevControlPinState = 0;
// if controlMode == True then control in automatic mode
// else control in manual mode
bool controlMode = false;

const int infoPin = 7;
int infoPinState = 0;
int prevInfoPinState = 0;
int infoMode = 0;

void setup()
{
  lcd.begin(16, 2); // Set up the number of columns and rows on the LCD.
  pinMode(controlPin, INPUT);
  pinMode(infoPin, INPUT);
  
  // Print a message to the LCD.
  lcd.print("MAN");
  lcd.setCursor(0,1);
  lcd.print("de l'écran");
}

void loop()
{

  // soit trouver le moyen de faire un truc comme ça mais qui
  // clear que 1 ligne quand on veut changer de info ou control
  // ou alors dès que on appuie sur  des 2 boutons, on clear le 
  // lcd et on affiche les deux lignes en regardant à quelle valeur
  // sont chacunes des valeurs controlMode et infoMode
  
  controlPinState = digitalRead(controlPin);
  if(controlPinState == HIGH) {
    lcd.setCursor(0,0);
    lcd.clear();
    if(controlMode)
      lcd.print("AUTO");
    else
      lcd.print("MAN");
    controlMode = !controlMode;
    //prevControlPinState = controlPinState;
  }
  
  infoPinState = digitalRead(infoPin);
  if(infoPinState == HIGH) {
    lcd.setCursor(0,1);
    switch(infoMode){
      case 0:
      lcd.print("1");
      break;
      case 1:
      lcd.print("2");
      break;
      case 2:
      lcd.print("3");
      break;
    }
    infoMode++;
    infoMode %= 3;
    //prevInfoPinState = infoPinState;
  }
}
        


