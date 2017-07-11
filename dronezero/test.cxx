/*
 * test.cxx
 * 
 * Copyright 2017  <pi@raspberrypi>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * `
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */
 
 #define LED1 4 //gpio23
 #define LED2 5 // gpio24


#include <iostream>
#include <wiringPi.h>
#include <RTIMULib.h>
using namespace std;

int main(int argc, char **argv)
{
	
	uint64_t rateTimer;
	//uint64_t displayTimer;
	uint64_t now;
	uint32_t ControlPeriod=1000;//100ms
	//uint32_t DisplayPeriod=10000;//100ms
	
	if(wiringPiSetup()==-1)
		return 1;
	
	//init
	
	pinMode(LED1,OUTPUT);
	pinMode(LED2,OUTPUT);
	
	RTIMUSettings *settings = new RTIMUSettings("RTIMULib");
	
	
	rateTimer = micros();
	while(1)
	{					while((now-rateTimer)<ControlPeriod)
			{
				now=micros();

			}
			rateTimer= now;
		digitalWrite(LED1,1);
		digitalWrite(LED2,0);
							while((now-rateTimer)<ControlPeriod)
			{
				now=micros();

			}
			rateTimer= now;
		//~ delay(1);
		digitalWrite(LED1,0);
		digitalWrite(LED2,1);
		//~ delay(1);
	
	}



		
	cout << "hello" << endl;
	
	return 0;
}

