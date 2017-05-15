#ifndef RPI_H
#define RPI_H


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
	STATUS stat;
} DC;

typedef struct Bl
{
	int id;
	int setvalue;
	STATUS stat;
} BL;

typedef struct Led
{
	char *setvalue;
	STATUS stat;
} LED;



typedef struct Pid
{
	float mbar;
	float k;
	float kv;
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
	DC dc;
	BL bl;
	PID pitch={0.004,100,20,0.8,0.04,4,0.006,6280,0.1,0,0,0,0,2,1,0,0};
	PLANT plant={0.1,10,100,0,0,0};
	
void rpinit()
{


	dc.id=1;
	bl.id=1;
	led.setvalue="123456789  ";



}

void setGain(PID& s,float mbarnew,float knew,float kvnew)
{
	s.k=knew;
	s.kv=kvnew;
	s.mbar=mbarnew;
	s.kp=s.mbar*s.kv/s.T;
	s.ki=s.mbar*s.k/s.T;
	s.kd=s.mbar/s.T;

}

void setLim(PID& s,float umaxnew,float uminnew)
{
	s.umax=umaxnew;
	s.umin=uminnew;
}

void getInput(PID& s,float rnew,float ynew)
{
	float eold=s.r-s.y;
	float e=rnew-ynew;
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
	
	
	


#endif
