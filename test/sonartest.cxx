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
 */ 
 

 #define LED1 4 //gpio23
 #define LED2 5 // gpio24


#include "Rpi.h"
#include <iostream>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <wiringSerial.h>
#include <RTIMULib.h>
#include "Sonar.h"
#include "Led.h"
#include <stdio.h>
#include <stdlib.h>
#include <mosquitto.h>
#include "gps.h"
#include "Mqtt.h"
#include <pca9685.h>
using namespace std;

void  INThandler(int sig);

	int pin_base=300;
	int max_pwm=4096;
	int pwmfreq=50;
int calcTicks(float Ms,int hertz)
{
	float cycle=1000.0f /hertz;
	return (int)(max_pwm*Ms/cycle+0.5f);
}
float map(float input, float min,float max)
{
	return (input*max)+(1-input)*min;
}


int main(int argc, char **argv)
{
	
	int onbit =0;
	uint64_t rateTimer;
	uint64_t rateTimer_s;
	uint64_t displayTimer;
	uint64_t now;
	RTIMU_DATA imuData;

	uint32_t ControlPeriod=100000;//100ms
	uint32_t ControlPeriod_s=1000000;//1000ms
	uint32_t DisplayPeriod=100000;//100ms
	uint16_t fd;
	uint16_t sonarid=0x71;
	

	
        //uint16_t newID=0X71;
        

    //interrupt for exit
	signal(SIGINT, INThandler);

	pinMode(LED1,OUTPUT);
	pinMode(LED2,OUTPUT);
	//init Sonar
	
	SONAR sonar=SONAR(sonarid);

	//init rpi
	rpinit();
		
	if(wiringPiSetup()==-1)
		return 1;
		

	

	//init imu
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

		

	//init led
	ledinit();

	//init GPS
	gps_init();
	loc_t data;


	//init mqtt
	mq_init();
	
	//init pwm
	int dcid= pca9685Setup(pin_base,0x40,pwmfreq);
	if(dcid <0)
	{
		printf("error insetup\n");
	}
	pca9685PWMReset(dcid);
	float mills=1.5;
	int tick= calcTicks(mills,pwmfreq);
	pwmWrite(pin_base+16,tick);
	
	
	
        //put all the characters of the scrolling text in a contiguous block
	
		setBuffer(led.setvalue,displayBuffer,length);
		
			for(int j=0;j<8; j++){
				printf("%u\r",j,0,displayBuffer[j][0]);
				
			}








	
				
	            //sonar.SetID(newID);





	rateTimer = displayTimer = RTMath::currentUSecsSinceEpoch();
	rateTimer_s = RTMath::currentUSecsSinceEpoch();

			//mqtt start
			mq_start();
		
			//~ printf("original kp %f, ki %f, kd %f\r\n",pitch.kp,pitch.ki,pitch.kd);
			setLim(pitch,100,-100);
			setGain(pitch,0.004,0,20);
			//~ printf("new kp %f, ki %f, kd %f\r\n",pitch.kp,pitch.ki,pitch.kd);
			
			float ref=0;
			int cnt=0;

	while(1)
	{








		//imu
		usleep(imu->IMUGetPollInterval()*1000);
		while(imu->IMURead())
		{
			imuData = imu->getIMUData();
			now =RTMath::currentUSecsSinceEpoch();	
		}
		
				//PID simulation
		if(cnt<100)
		{
			if(cnt>20) ref=1;
			
			//doSimul(plant,pitch.u);
			getInput(pitch,ref,plant.y);
			//~ printf("input %i =%f\r\n",cnt,pitch.u);
			//~ sprintf(mqbuf,"%f",plant.y);
			//~ mq_send("pidrone/PI/SIM",mqbuf);
			//~ fflush(stdout);
			cnt++;
		}
		
		
			
			//sonar
			sonar.RequestData();
			usleep(20000); //5ms for under 60cm 20ms for under 200cm
			sonar.distance=sonar.GetValues();
		
		
			//led scroll
			ledScroll(displayBuffer,length,letter,y);

		

			// slow timer
					if((now-rateTimer_s)>ControlPeriod_s)
			{

				//gps
				gps_location(&data);

				rateTimer_s= now;
			}
			
	
			
			//display 
								if((now-displayTimer)>DisplayPeriod)
			{


				//printf("in main %lf, %lf \r\n",data.latitude, data.longitude);		
		  		//printf("Recievd sonar value : %u cm\n",sonarcenti);
				
				//printf("Recievd: %s\r\n",RTMath::displayDegrees("",imuData.fusionPose));
				
				sprintf(mqbuf,"%i,%i,%i\r",sonar.id,sonar.distance,sonar_status);
				mq_send("pidrone/SONAR",mqbuf);
				
				sprintf(mqbuf,"%lf,%lf,%lf,%i\r",data.latitude, data.longitude,data.altitude,gps_status);	
				mq_send("pidrone/GPS",mqbuf);
				sprintf(mqbuf,"%s,%i\r",RTMath::displayDegrees("",imuData.fusionPose),imu_status);
				mq_send("pidrone/IMU",mqbuf);
				fflush(stdout);
				displayTimer= now;
			}
		
			// wait until control period
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

void  INThandler(int sig)
{
       // Closing file and turning off Matrix

	unsigned short int clear[] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

	displayImage(clear,res, daddress, file);


        printf("Closing file and turning off \r\n");
        daddress = 0x20;
        for(daddress = 0xef; daddress >= 0xe0; daddress--) {
                res = i2c_smbus_write_byte(file, daddress);
        }
	gps_off();

	
	signal(sig, SIG_IGN);

        close(file);
        exit(0);
}




