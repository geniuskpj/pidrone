#ifndef RPI_H
#define RPI_H

#include <pca9685.h>
#include <wiringPi.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

typedef enum
{
 OK,
 ERROR
} STATUS;



typedef struct Rpi
{
	int setvalue;
	STATUS stat;
} RPI;

typedef struct Dc
{
	int id;
	float setvalue;
	float max;
	float min;
	float tmax;
	float tmin;
	STATUS stat;
} DC;

typedef struct Bl
{
	int id;
	int setvalue;
	int max;
	int min;
	float tmax;
	float tmin;
	STATUS stat;
} BL;

typedef struct Led
{
	char *setvalue;
	STATUS stat;
} LED;



typedef struct Pid
{
	float kp;
	float ki;
	float kd;
	float wi;
	float wd;
	float T;
	float up;
	float ui;
	float ud;
	float u;
	float umax;
	float umin;
	float r;
	float y;
	float yo;
} PID;
	
typedef struct Plant
{
	float T;
	float a;
	float b;
	float u;
	float y;
	float yold;
} PLANT;










	STATUS gps_status,imu_status,sonar_status;
	LED led;
	RPI rpi={0,OK};
	DC dcgm={0,-90,90,-90,2.5,0.5,OK};  //mode of gymbal
	DC dcgp={1,0,90,-90,2.5,0.5,OK};  // gymbal pitch
	DC dcgh={2,0,90,-90,2.5,0.5,OK}; //gymbal heading
	
	
	DC dc={4,0,90,-90,2.5,0.5,OK};
	DC dc2={5,0,90,-90,2.5,0.5,OK};
	DC dc3={6,0,90,-90,2.5,0.5,OK};
	DC dc4={7,0,90,-90,2.5,0.5,OK};

	
	DC bz={15,0,255,0,50,0,OK};
	//BL bl={8,0,5000,0,1.86,1.06,OK};
	BL bl={8,0,5000,0,1.86,1.16,OK};
	PID pitch={8,4,4,6,628,0.01,0,0,0,0,2,1,0,0,0};
	PID roll={8,4,4,6,628,0.01,0,0,0,0,2,1,0,0,0};
	//PID yaw={0.004,100,20,0.8,0.04,4,0.006,6280,0.1,0,0,0,0,2,1,0,0};
	PLANT plant={0.1,10,100,0,0,0};
	int pwid;
	int pin_base=300;
	int max_pwm=4096;
	int pwmfreq=50;
	int onbit=0;
	int llcount=0;
	int lcount=6;
	
void rpinit()
{



	//~ led.setvalue="123456789  ";
	led.setvalue=">  ";



}

void setGain(PID& s,float pnew,float inew,float dnew)
{
	
	//~ s.kp=s.mbar*s.kv/s.T;
	//~ s.ki=s.mbar*s.k/s.T;
	//~ s.kd=s.mbar/s.T;
	s.kp=pnew;
	s.ki=inew;
	s.kd=dnew;

}

void setLim(PID& s,float uminnew,float umaxnew)
{
	s.umax=umaxnew;
	s.umin=uminnew;
}

void getInput(PID& s,float rnew,float ynew)
{
	float eold=s.r-(s.y-s.yo);
	float e=rnew-(ynew-s.yo);
	s.r=rnew;
	s.y=ynew;	
	s.up=s.kp*e;
	s.ui=s.ui/(1+s.wi*s.T)+s.ki*s.wi*s.T*e/(1+s.wi*s.T);
	s.ud=s.ud/(1+s.wd*s.T)+s.kd*s.wd*(e-eold)/(1+s.wd*s.T);
	s.u=s.up+s.ui+s.ud;
	if(s.u>s.umax)
	{
		s.u=s.umax;
	}
	else if(s.u<s.umin)
	{
		s.u=s.umin;
	}
	
}

void doSimul(PLANT& s,float unew)
{
	s.u=unew;
	s.y=((2+s.a*s.T)*s.y-s.yold+s.b*s.T*s.T*s.u)/(1+s.a*s.T+s.b*s.T*s.T);
}
	
	int calcTicks(float Ms,int hertz)
{
	float cycle=1000.0f /hertz;
	return (int)(max_pwm*Ms/cycle+0.5f);
}
float map(float input, float min,float max,float pmin,float pmax)
{
	float rinput=input-min;
	return pmin+(pmax-pmin)/(max-min)*rinput;
}

void setThrottle(BL& s,int input)
{
	float mils=map((float)input,s.min,s.max,s.tmin,s.tmax);
	int tick=calcTicks(mils,pwmfreq);// 50Hz
	int id=s.id+pin_base; // id 0-15, 16 all pin
	pwmWrite(id,tick); 
	//~ printf("id=%i,tick=%i\r\n",id,tick);
}

void setAngle(DC& s,float input)
{
	float mils=map(input,s.min,s.max,s.tmin,s.tmax);
	int tick=calcTicks(mils,pwmfreq);// 50Hz
	int id=s.id+pin_base; // id 0-15, 16 all pin
	pwmWrite(id,tick);
	//~ printf("id=%i,tick=%i\r\n",id,tick); 
}

void lBeep()
{
	if(lcount>0)
	{
		if(llcount<lcount)
		{
			if (llcount%2==0) 
			{
				setAngle(bz,0);
				//~ printf("buz on\r\n");
			}                                                                                                                                                                                           
			else
			{
				setAngle(bz,255);
				//~ printf("buz off\r\n");
			}
			llcount++;
			
		}
		else
		{
			llcount=0;
			lcount=0;
		}
		
	}
		
}	
	


#endif
