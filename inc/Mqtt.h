#ifndef MQTT_H
#define MQTT_H
#include <mosquitto.h>
#include "Rpi.h"
#include <string.h>
#include <cstdlib>
#include "Led.h"
struct mosquitto *mosq=NULL;
char *mqbuf=(char *)malloc(70*sizeof(char));
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
		if(getid==1)
		{
			dc.setvalue=getvalue;
			printf("dcid= %i setvalue=%.1f\r\n",dc.id,dc.setvalue);	
		}
		else if(getid ==dcgm.id)
		{
			dcgm.setvalue=getvalue;
			printf("gymbal mode id= %i setvalue=%.1f\r\n",dcgm.id,dcgm.setvalue);
		}
				else if(getid ==dcgp.id)
		{
			dcgp.setvalue=getvalue;
			printf("gymbal pitch id= %i setvalue=%.1f\r\n",dcgp.id,dcgp.setvalue);
		}	
				else if(getid ==dcgh.id)
		{
			dcgh.setvalue=getvalue;
			printf("gymbal head id= %i setvalue=%.1f\r\n",dcgh.id,dcgh.setvalue);
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
		if(getid==bl.id)
		{
			bl.setvalue=(int)getvalue;
			printf("bldc %i command=%i\r\n",bl.id,bl.setvalue);	
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
	if(mosquitto_connect(mosq,"143.248.204.35",1883,60))
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
