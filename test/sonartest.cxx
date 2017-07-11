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


#include <RTIMULib.h>
#include "Sonar.h"
#include "Led.h"

#include <mosquitto.h>
#include "gps.h"
#include "Mqtt.h"


using namespace std;

void  INThandler(int sig);



int main(int argc, char **argv)
{
	
	uint64_t rateTimer;
	uint64_t rateTimer_s;
	uint64_t sonarTimer;
	uint64_t tok,tokold;
	uint64_t toktime;
	int tokcnt=0;
	uint64_t displayTimer;
	uint64_t now;
	RTIMU_DATA imuData;

	uint32_t ControlPeriod=10000;//10ms
	uint32_t ControlPeriod_s=1000000;//1000ms
	uint32_t DisplayPeriod=100000;//100ms
	uint16_t fd;
	uint16_t sonarid=0x71;
	uint16_t sonarid2=0x72;
	uint16_t sonarid3=0x73;
	uint16_t sonarid4=0x74;
	char *imuresult;
	bool sonarflag=true;

        
    //interrupt for exit
	signal(SIGINT, INThandler);


	//init Sonar
	
	SONAR sonar=SONAR(sonarid);
	//~ SONAR sonar2=SONAR(sonarid2);
	//~ SONAR sonar3=SONAR(sonarid3);
	//~ SONAR sonar4=SONAR(sonarid4);
	
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
data.latitude=0;
data.longitude=0;
data.altitude=0;

	//init mqtt
	mq_init();
	
	//init pwm
	pwid= pca9685Setup(pin_base,0x40,pwmfreq);
	if(pwid <0)
	{
		printf("error insetup\n");
	}
	pca9685PWMReset(pwid);


	pinMode(LED1,OUTPUT);
	pinMode(LED2,OUTPUT);


			//throttle
		setThrottle(bl,0);
		
		//set angle
		setAngle(dc,0);
		setAngle(dc2,0);
		setAngle(dc3,0);
		setAngle(dc4,0);		
		//setAngle(dcgm,0);
		//setAngle(dcgh,0);
		//setAngle(dcgp,0);
		//setAngle(bz,255);
	delay(2000);

	//init controller
	//~ printf("original kp %f, ki %f, kd %f\r\n",pitch.kp,pitch.ki,pitch.kd);
	setLim(pitch,dc.min,dc.max);
	setLim(roll,dc.min,dc.max);
	setGain(pitch,0.004,100,20);
	setGain(roll,0.004,100,20);
	
	//~ printf("new kp %f, ki %f, kd %f\r\n",pitch.kp,pitch.ki,pitch.kd);
	
	float ref=0;
	int cnt=0;

	
        //put all the characters of the scrolling text in a contiguous block
	
		setBuffer(led.setvalue,displayBuffer,length);
		
		for(int j=0;j<8; j++)
		{
			printf("%u\r",j,0,displayBuffer[j][0]);
			
		}




	
		
	            //sonar.SetID(newID);





	//~ rateTimer = displayTimer = RTMath::currentUSecsSinceEpoch();
	rateTimer = displayTimer = micros();
	rateTimer_s = rateTimer;

	//mqtt start
	mq_start();



	while(1)
	{
		


		//imu
		usleep(imu->IMUGetPollInterval()*1000);
		

		
		tokold=micros();
		while(imu->IMURead())
		{
			
		}
		imuData = imu->getIMUData();


		



		imuresult=(char *)RTMath::displayDegrees("",imuData.fusionPose);
		imuresult=strtok(imuresult,":");
		imuresult=strtok(NULL,",");
		roll.y=atof(imuresult);// roll
		imuresult=strtok(NULL,":");

		imuresult=strtok(NULL,",");
		pitch.y=atof(imuresult); //pitch
		
		
		if(cnt==10	)
		{
			roll.r=0;
			pitch.r=0;
			roll.yo=roll.y;
			pitch.yo=pitch.y;

		}
		
		

		
		//PID simulation
		//~ if(cnt<100)
		//~ {
			//~ if(cnt>20) ref=1;
			
			//~ doSimul(plant,pitch.u);
			//~ getInput(pitch,ref,plant.y);
			//~ printf("input %i =%f\r\n",cnt,pitch.u);
			//~ sprintf(mqbuf,"%f",plant.y);
			//~ mq_send("pidrone/PI/SIM",mqbuf);
			//~ fflush(stdout);
			//~ cnt++;
		//~ }
		if(rpi.setvalue==1)
		{
		getInput(pitch,pitch.r,pitch.y);
		getInput(roll,roll.r,roll.y);
		//~ printf("pitch=%f,u=%f,up=%f,ui=%f,ud=%f,r=%f\r\n",pitch.y,pitch.u,pitch.up,pitch.ui,pitch.ud,pitch.r);
		printf("roll=%f,u=%f,up=%f,ui=%f,ud=%f,r=%f\r\n",roll.y,roll.u,roll.up,roll.ui,roll.ud,roll.r);
		dc.setvalue=pitch.u;
		dc2.setvalue=roll.u;
		dc3.setvalue=-pitch.u;
		dc4.setvalue=-roll.u;
		}


			//throttle
		setThrottle(bl,bl.setvalue);
		
		//set angle
		setAngle(dc,dc.setvalue);
		setAngle(dc2,dc2.setvalue);
		setAngle(dc3,dc3.setvalue);
		setAngle(dc4,dc4.setvalue);
		setAngle(dcgm,dcgm.setvalue);
		setAngle(dcgh,dcgh.setvalue);
		setAngle(dcgp,dcgp.setvalue);

		
		
			
		//sonar
		if(!sonarflag)
		{
		sonar.RequestData();
		sonarTimer=micros();
		sonarflag=true;
		}
	
	
		//led scroll
		//~ ledScroll(displayBuffer,length,letter,y);

		
					

	
				//~ now =RTMath::currentUSecsSinceEpoch();			
		now =micros();
		// slow timer
				if((now-rateTimer_s)>=ControlPeriod_s)
		{
			//get gps data
			gps_location(&data);
			lBeep();
			
			
			

			rateTimer_s= now;
		}
		
		
		

		
				
		//display 
							if((now-displayTimer)>DisplayPeriod)
		{


			//~ printf("in main %lf, %lf \r\n",data.latitude, data.longitude);		
	  		//~ printf("Recievd sonar value : %u cm\n",sonar.distance);
			//~ 
			//~ printf("Recievd: %s\r\n",RTMath::displayDegrees("",imuData.fusionPose));
			
			sprintf(mqbuf,"%i,%i,%i\r",sonar.id,sonar.distance,sonar_status);
			mq_send("pidrone/SONAR",mqbuf);
			
			sprintf(mqbuf,"%lf,%lf,%lf,%i\r",data.latitude, data.longitude,data.altitude,gps_status);	
			mq_send("pidrone/GPS",mqbuf);
			sprintf(mqbuf,"%s,%i\r",RTMath::displayDegrees("",imuData.fusionPose),imu_status);
			mq_send("pidrone/IMU",mqbuf);
			
			
			
			toktime=toktime/tokcnt;
			tokcnt=0;
			//~ toktime=0;
			sprintf(mqbuf,"measured time is %llds.\r\n",toktime);
			mq_send("pidrone/PI",mqbuf);
			
			
			fflush(stdout);
			displayTimer= now;
		}

		tok=micros();
		toktime=toktime+tok-tokold;
		tokcnt++;
	
		// wait until control period
				while((now-rateTimer)<ControlPeriod)
		{
			
			//~ usleep(40000); //5ms for under 60cm 20ms for under 100cm 40ms for 200cm
			
			if (((now-sonarTimer)>=60000)&&sonarflag)
			{
			sonar.distance=sonar.GetValues();
			sonarflag=false;
			}
		
			
			
			
			//~ now=RTMath::currentUSecsSinceEpoch();
			now =micros();

		}
		rateTimer= now;
			
		if(onbit)
		{
		digitalWrite(LED1,1);
		digitalWrite(LED2,0);
		onbit=0;
		//~ printf("onbit\r\n");
		}
		else
		{
		
			digitalWrite(LED1,0);
			digitalWrite(LED2,1);
			onbit=1;
			//~ printf("offbit\r\n");
		}
		cnt++;
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
	
	
	
	
		setAngle(dc,0);
		setAngle(dcgm,0);
		setAngle(dcgh,0);
		setAngle(dcgp,0);       
	
	pca9685PWMReset(pwid);
		setThrottle(bl,0);
		setAngle(bz,255);
	
	signal(sig, SIG_IGN);

        close(file);
        exit(0);
}




