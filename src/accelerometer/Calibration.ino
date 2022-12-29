/*****************************************************************************/
//	Function:	 To calibrate the parameters and it will recommend parameter 
//				setting and print to the serial monitor.So the values of the
//				marco definitions in the ADXL335.h should be modified.
//  Hardware:    Grove - 3-Axis Analog Accelerometer
//	Arduino IDE: Arduino-1.0
//	Author:	 Frankie.Chu		
//	Date: 	 Jan 11,2013
//	Version: v1.0
//	by www.seeedstudio.com
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//
/*******************************************************************************/

#include "ADXL335.h"

#define PERIODE_AFFICHAGE       1000000  

int zero_x;
int zero_y;
int zero_z;
int max_x,max_y,max_z;//when 1g
float sensitivity;
ADXL335 accelerometer;

static unsigned long ulmicroseconds = 0;
static unsigned long ulPrecMicroseconds = 0;


void setup()
{
	Serial.begin(9600);
	accelerometer.begin();
}

void loop()
{

  ulmicroseconds = micros();
  if(ulmicroseconds-ulPrecMicroseconds >= PERIODE_AFFICHAGE)
  {
    int x,y,z;
    accelerometer.getXYZ(&x,&y,&z);
    Serial.println("value of X/Y/Z: ");
    Serial.println(x);
    Serial.println(y);
    Serial.println(z);
    Serial.println(map(x, 210, 310, -100, 100));
	  Serial.println(map(y, 210, 310, -100, 100));
	  Serial.println(map(z, 230, 330, -100, 100));
    ulPrecMicroseconds = ulmicroseconds;
  }


}
void calibrate(int* _x,int* _y,int* _z)
{
	int x,y,z;
	int sum_x,sum_y,sum_z;
	accelerometer.getXYZ(&x,&y,&z);
	float ax,ay,az;
	accelerometer.getAcceleration(&ax,&ay,&az);
	if((abs(ax) < 0.06)&&(abs(ay) < 0.06))
	{
		
		*_x = x;
		*_y = y;
		*_z = z;
	}
	else if((abs(ax) < 0.06)&&(abs(az) < 0.06))
	{
		*_x = x;
		*_y = y;
		*_z = z;
	}
	else if((abs(az) < 0.06)&&(abs(ay) < 0.06))
	{
		*_x = x;
		*_y = y;
		*_z = z;
	}
	else Serial.println("calibrate error");
}