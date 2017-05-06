/*
 * sonartest.cxx
 * 
 * Copyright 2017  <pi@raspberrypi>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
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
#include <wiringPiI2C.h>
#include <RTIMULib.h>
#include "Sonar.h"
using namespace std;

int main(int argc, char **argv)
{
	
	int onbit =0;
	uint64_t rateTimer;
	uint64_t displayTimer;
	uint64_t now;
	RTIMU_DATA imuData;
	uint32_t ControlPeriod=100000;
	uint32_t DisplayPeriod=100000;
	uint16_t sonarcenti=0;
	uint16_t fd;
	uint16_t sonarid=0x71;
	
	
	

	
	//init
	
	//~ fd=wiringPiI2CSetup(0X71);
	SONAR sonar=SONAR(sonarid);
		
	if(wiringPiSetup()==-1)
		return 1;
		
	pinMode(LED1,OUTPUT);
	pinMode(LED2,OUTPUT);
	
	RTIMUSettings *settings = new RTIMUSettings("RTIMULib");
	RTIMU *imu = RTIMU::createIMU(settings);
	
	if ((imu==NULL) || (imu->IMUType()==RTIMU_TYPE_NULL))
	{
		printf("No IMU found\n");
		exit(1);
	}
	imu->IMUInit();
	imu->setSlerpPower(0.02);
	imu->setGyroEnable(true);
	imu->setAccelEnable(true);
	imu->setCompassEnable(true);
	
	rateTimer = displayTimer = RTMath::currentUSecsSinceEpoch();
	
				//~ wiringPiI2CWriteReg8(fd,0,0xA0);
				//~ usleep(20000);
				//~ wiringPiI2CWriteReg8(fd,0,0xAA);
				//~ usleep(20000);
				//~ wiringPiI2CWriteReg8(fd,0,0xA5);
				//~ usleep(20000);
				//~ wiringPiI2CWriteReg8(fd,0,0xE6);
	
	while(1)
	{
		usleep(imu->IMUGetPollInterval()*1000);
		while(imu->IMURead())
		{
			imuData = imu->getIMUData();
			now =RTMath::currentUSecsSinceEpoch();	
		}
			
			//sonar
			sonar.RequestData();
			//~ wiringPiI2CWriteReg8(fd,0,81);

			usleep(65000); //5ms for under 60cm 20ms for under 200cm
			sonarcenti=sonar.GetValues();

			
			

		

					if((now-displayTimer)>DisplayPeriod)
			{

				printf("Recievd sonar value : %u cm\n",sonarcenti);
				
				//printf("Recievd: %s\r\n",RTMath::displayDegrees("",imuData.fusionPose));
				fflush(stdout);
				displayTimer= now;
			}
		
					while((now-rateTimer)<ControlPeriod)
			{
				now=RTMath::currentUSecsSinceEpoch();

			}
			rateTimer= now;
			
			if(onbit)
			{
			digitalWrite(LED1,1);
			digitalWrite(LED2,0);
			onbit=0;
			}
			else
			{
			
				digitalWrite(LED1,0);
				digitalWrite(LED2,1);
				onbit=1;
			}
	}



		
	cout << "hello" << endl;
	
	return 0;
}



