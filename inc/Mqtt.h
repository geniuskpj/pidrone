#ifndef MQTT_H
#define MQTT_H
#include <mosquitto.h>
#include "Rpi.h"
#include <string.h>
#include <cstdlib>
#include "Led.h"
struct mosquitto *mosq=NULL;
char *mqbuf=(char *)malloc(70*sizeof(char));
int mq_send(const char *topic,const char *msg);
void message_callback(struct mosquitto *mosq,void *obj,const struct mosquitto_message *message)
{
	bool match =0;
	char *msg;
	char *s1;
	char *s2;
	int getid;
	float getvalue;
	
	mosquitto_topic_matches_sub("pidrone/CMD/PI",message->topic,&match);
	if (match)
	{
		msg=(char *)message->payload;
		//printf("got message '%.*s' for topic '%s'\r\n",message->payloadlen,(char*) message->payload,message->topic);
		printf("pi command= %i\r\n",atoi(msg));
		rpi.setvalue=atoi(msg);	
		if(rpi.setvalue==0)
		{
		setAngle(dc,0);
		setAngle(dc2,0);
		setAngle(dc3,0);
		setAngle(dc4,0);
		setAngle(cm1,0);
		setAngle(cm2,0);
		setAngle(dcgm,0);
		setAngle(dcgh,0);
		setAngle(dcgp,0);       
	
	pca9685PWMReset(pwid);
		setThrottle(bl,0);
		setThrottle(bl2,0);
		setAngle(bz,255);
		}	
	}
	
		mosquitto_topic_matches_sub("pidrone/CMD/DC",message->topic,&match);
	if (match)
	{
		msg=(char *)message->payload;
		s1=strtok(msg,",");
		s2=strtok(NULL,"\r");
		getid=atoi(s1);
		getvalue=atof(s2);
		//~ printf("getid= %i getvalue=%.1f\r\n",getid,getvalue);
		//~ printf("getid= %i getvalue=%.1f\r\n",dc.id,getvalue);	
		//~ if(getid==dc.id)

		if(getid ==1)
		{
			dcgm.setvalue=getvalue;
			printf("gymbal mode id= %i setvalue=%.1f\r\n",dcgm.id,dcgm.setvalue);
		}
				else if(getid ==2)
		{
			dcgp.setvalue=getvalue;
			printf("gymbal pitch id= %i setvalue=%.1f\r\n",dcgp.id,dcgp.setvalue);
		}	
				else if(getid ==3)
		{
			dcgh.setvalue=getvalue;
			printf("gymbal head id= %i setvalue=%.1f\r\n",dcgh.id,dcgh.setvalue);
		}
		else if(getid==4)
		{
			dc.setvalue=getvalue;
			printf("dc id= %i setvalue=%.1f\r\n",dc.id,dc.setvalue);	
		}
				else if(getid==5)
		{
			dc2.setvalue=getvalue;
			printf("dc2 id= %i setvalue=%.1f\r\n",dc2.id,dc2.setvalue);	
		}
				else if(getid==6)
		{
			dc3.setvalue=getvalue;
			printf("dc3 id= %i setvalue=%.1f\r\n",dc3.id,dc3.setvalue);	
		}
				else if(getid==7)
		{
			dc4.setvalue=getvalue;
			printf("dc4 id= %i setvalue=%.1f\r\n",dc4.id,dc4.setvalue);	
		}		
				
				else if(getid==10)
		{
			cm1.setvalue=getvalue;
			printf("cm1 id= %i setvalue=%.1f\r\n",cm1.id,cm1.setvalue);	
		}
				else if(getid==11)
		{
			cm2.setvalue=getvalue;
			printf("cm2 id= %i setvalue=%.1f\r\n",cm2.id,cm2.setvalue);	
		}
			
	}
	
			mosquitto_topic_matches_sub("pidrone/CMD/BL",message->topic,&match);
	if (match)
	{
		msg=(char *)message->payload;
		s1=strtok(msg,",");
		s2=strtok(NULL,"\r");
		getid=atoi(s1);
		getvalue=atof(s2);
		if(getid==8)
		{
			bl.setvalue=(int)getvalue;
			printf("bldc1 id %i command=%i\r\n",bl.id,bl.setvalue);	
		}
				if(getid==9)
		{
			bl2.setvalue=(int)getvalue;
			printf("bldc2 %i command=%i\r\n",bl2.id,bl2.setvalue);	
		}
	}
	
	mosquitto_topic_matches_sub("pidrone/MV",message->topic,&match);
	if (match)
	{
		//~ rpi.setvalue=4;
		msg=(char *)message->payload;
		s1=strtok(msg,",");
		roll.r=atof(s1)*15;
		s1=strtok(NULL,",");
		pitch.r=atof(s1)*15;
		s1=strtok(NULL,",");
		yaw.r=yaw.r+atof(s1);
		if(yaw.r>180.0)
		{
			yaw.r=180.0;
		}
		if(yaw.r<-180.0)
		{
			yaw.r=-180.0;
		}
		s1=strtok(NULL,",");
		bl.setvalue=atof(s1)*2000;
		bl2.setvalue=bl.setvalue;
		s1=strtok(NULL,",");
		dcgp.setvalue=atof(s1)*90;
		s1=strtok(NULL,"");
		dcgh.setvalue=atof(s1)*90;
		printf("input : roll %f, pitch %f, yaw %f, bl %i, gp %i, gh %i\r\n",roll.r,pitch.r,0,bl.setvalue,(int)dcgp.setvalue,(int)dcgh.setvalue);	
	}
	
		mosquitto_topic_matches_sub("pidrone/CMD/FG/get",message->topic,&match);
	if (match)
	{
		msg=(char *)message->payload;
		s1=strtok(msg,"");
		if(atoi(s1)==0)
		{
			printf("dc1 max=%f,min=%f,tmax=%f,tmin=%f\r\n",dc.max,dc.min,dc.tmax,dc.tmin);
			sprintf(mqbuf,"%u,%f,%f,%f,%f\r\n",dc.id,dc.max,dc.min,dc.tmax,dc.tmin);	
			mq_send("pidrone/FG",mqbuf);
						printf("dc2 max=%f,min=%f,tmax=%f,tmin=%f\r\n",dc2.max,dc2.min,dc2.tmax,dc2.tmin);
			sprintf(mqbuf,"%u,%f,%f,%f,%f\r\n",dc2.id,dc2.max,dc2.min,dc2.tmax,dc2.tmin);	
			mq_send("pidrone/FG",mqbuf);
						printf("dc3 max=%f,min=%f,tmax=%f,tmin=%f\r\n",dc3.max,dc3.min,dc3.tmax,dc3.tmin);
			sprintf(mqbuf,"%u,%f,%f,%f,%f\r\n",dc3.id,dc3.max,dc3.min,dc3.tmax,dc3.tmin);	
			mq_send("pidrone/FG",mqbuf);
						printf("dc4 max=%f,min=%f,tmax=%f,tmin=%f\r\n",dc4.max,dc4.min,dc4.tmax,dc4.tmin);
			sprintf(mqbuf,"%u,%f,%f,%f,%f\r\n",dc4.id,dc4.max,dc4.min,dc4.tmax,dc4.tmin);	
			mq_send("pidrone/FG",mqbuf);
	
				printf("bl1 max=%i,min=%i,tmax=%f,tmin=%f\r\n",bl.max,bl.min,bl.tmax,bl.tmin);
			sprintf(mqbuf,"%u,%i,%i,%f,%f\r\n",bl.id,bl.max,bl.min,bl.tmax,bl.tmin);	
			mq_send("pidrone/FG",mqbuf);
						printf("bl2 max=%i,min=%i,tmax=%f,tmin=%f\r\n",bl2.max,bl2.min,bl2.tmax,bl2.tmin);
			sprintf(mqbuf,"%u,%i,%i,%f,%f\r\n",bl2.id,bl2.max,bl2.min,bl2.tmax,bl2.tmin);	
			mq_send("pidrone/FG",mqbuf);
						printf("cm1 max=%f,min=%f,tmax=%f,tmin=%f\r\n",cm1.max,cm1.min,cm1.tmax,cm1.tmin);
			sprintf(mqbuf,"%u,%f,%f,%f,%f\r\n",cm1.id,cm1.max,cm1.min,cm1.tmax,cm1.tmin);	
			mq_send("pidrone/FG",mqbuf);
									printf("cm2 max=%f,min=%f,tmax=%f,tmin=%f\r\n",cm2.max,cm2.min,cm2.tmax,cm2.tmin);
			sprintf(mqbuf,"%u,%f,%f,%f,%f\r\n",cm2.id,cm2.max,cm2.min,cm2.tmax,cm2.tmin);	
			mq_send("pidrone/FG",mqbuf);
			
		}
	}
	
			mosquitto_topic_matches_sub("pidrone/CMD/FG/set",message->topic,&match);
	if (match)
	{
		msg=(char *)message->payload;
		s1=strtok(msg,",");
		getid=atoi(s1);
		if(getid==4)
		{
			s1=strtok(NULL,",");
			dc.max=atof(s1);
			s1=strtok(NULL,",");
			dc.min=atof(s1);
			s1=strtok(NULL,",");
			dc.tmax=atof(s1);
			s1=strtok(NULL,"");
			dc.tmin=atof(s1);
		}
		
				if(getid==5)
		{
			s1=strtok(NULL,",");
			dc2.max=atof(s1);
			s1=strtok(NULL,",");
			dc2.min=atof(s1);
			s1=strtok(NULL,",");
			dc2.tmax=atof(s1);
			s1=strtok(NULL,"");
			dc2.tmin=atof(s1);
		}
		
				if(getid==6)
		{
			s1=strtok(NULL,",");
			dc3.max=atof(s1);
			s1=strtok(NULL,",");
			dc3.min=atof(s1);
			s1=strtok(NULL,",");
			dc3.tmax=atof(s1);
			s1=strtok(NULL,"");
			dc3.tmin=atof(s1);
		}
		
				if(getid==7)
		{
			s1=strtok(NULL,",");
			dc4.max=atof(s1);
			s1=strtok(NULL,",");
			dc4.min=atof(s1);
			s1=strtok(NULL,",");
			dc4.tmax=atof(s1);
			s1=strtok(NULL,"");
			dc4.tmin=atof(s1);
		}
		
				if(getid==8)
		{
			s1=strtok(NULL,",");
			bl.max=atoi(s1);
			s1=strtok(NULL,",");
			bl.min=atoi(s1);
			s1=strtok(NULL,",");
			bl.tmax=atof(s1);
			s1=strtok(NULL,"");
			bl.tmin=atof(s1);
		}
		
				if(getid==9)
		{
			s1=strtok(NULL,",");
			bl2.max=atoi(s1);
			s1=strtok(NULL,",");
			bl2.min=atoi(s1);
			s1=strtok(NULL,",");
			bl2.tmax=atof(s1);
			s1=strtok(NULL,"");
			bl2.tmin=atof(s1);
		}
		
				if(getid==10)
		{
			s1=strtok(NULL,",");
			cm1.max=atof(s1);
			s1=strtok(NULL,",");
			cm1.min=atof(s1);
			s1=strtok(NULL,",");
			cm1.tmax=atof(s1);
			s1=strtok(NULL,"");
			cm1.tmin=atof(s1);
		}
						if(getid==11)
		{
			s1=strtok(NULL,",");
			cm2.max=atof(s1);
			s1=strtok(NULL,",");
			cm2.min=atof(s1);
			s1=strtok(NULL,",");
			cm2.tmax=atof(s1);
			s1=strtok(NULL,"");
			cm2.tmin=atof(s1);
		}
		
		

	}
	
				
				
				mosquitto_topic_matches_sub("pidrone/CMD/LED",message->topic,&match);
	if (match)
	{
		msg=(char *)message->payload;
		led.setvalue=strdup(msg);
		printf("led command=%s\r\n",msg);	
		//~ led.setvalue=msg;

		//~ memcpy(&led.setvalue[0],&msg[0],1);
		//setBuffer(led.setvalue,displayBuffer,length);
		letter=0;
		y=0;

	}
	
}

void mq_init()
{
		mosquitto_lib_init();
	
	mosq=mosquitto_new(NULL,true,NULL);
	mosquitto_message_callback_set(mosq,message_callback);
	if(mosquitto_connect(mosq,"58.224.86.126",1883,60))
	{
			printf("mqtt connect error\r\n");;
	}
	mosquitto_subscribe(mosq,NULL,"pidrone/CMD/#",0);
}

void mq_start()
{
					int rc=mosquitto_loop_start(mosq);
				while(rc)
				{
					printf("connection error!\r\n");
					usleep(20000);
					mosquitto_reconnect(mosq);
				}
}

int mq_send(const char *topic,const char *msg)
{
	return mosquitto_publish(mosq,NULL,topic,strlen(msg),msg,0,0);
}

void mq_close()
{
	mosquitto_destroy(mosq);
	mosquitto_lib_cleanup();
}
#endif
