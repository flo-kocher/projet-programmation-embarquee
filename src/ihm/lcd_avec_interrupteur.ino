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

const int switchPin = 6;
int switchState = 0;
int prevSwitchState = 0;
int reply;

void setup()
{
  lcd.begin(16, 2); // Set up the number of columns and rows on the LCD.
	pinMode(switchPin, INPUT);
  
  // Print a message to the LCD.
  lcd.print("Demandez a la");
  lcd.setCursor(0,1);
  lcd.print("boule magique !");
}

void loop()
{
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting
  // begins with 0):
  //lcd_1.setCursor(0, 1);
  // print the number of seconds since reset:
  //lcd_1.print(seconds);
  //delay(1000); // Wait for 1000 millisecond(s)
  //seconds += 1;
  
  switchState = digitalRead(switchPin);
  if(switchState != prevSwitchState){
    if(switchState == LOW){
      reply = random(8);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("La boule dit :");
      lcd.setCursor(0,1);
      
      switch(reply){
        case 0:
        lcd.print("Oui");
        break;
        case 1:
        lcd.print("Probablement");
        break;
        case 2:
        lcd.print("Certainement");
        break;
        case 3:
        lcd.print("Sans doute");
        break;
        case 4:
        lcd.print("Pas sur");
        break;
        case 5:
        lcd.print("Redemande");
        break;
        case 6:
        lcd.print("J'en doute");
        break;
        case 7:
        lcd.print("Non");
        break;     
      }
    }
  }
  prevSwitchState = switchState;
}
        


