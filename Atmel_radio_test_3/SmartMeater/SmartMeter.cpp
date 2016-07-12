/*
 * SmartMeter.cpp
 *
 * Created: 06/07/2016 11:30:28
 *  Author: Time Syn Project
 */ 
#include "SmartMeter.h"
#include <FreeRTOS.h>
#include <semphr.h>
#include <timeProtocol.h>
void PrintBuffer(Channel* c, char* buffer, size_t length=0)
{
	if(c && buffer)
	{
		if(length > 0)
		{
			for(size_t i=0; i<length; ++i)
			{
				c->putc(buffer[i]);
			}
		}
		else
		{
			size_t count = 0;
			while(buffer[count]!='\0')
			{
				c->putc(buffer[count++]);
			}
		}
	}
}

//extern xSemaphoreHandle timeProt.synchroTimeReceive;

Channel* pc = NULL;

void smartMeterTask(void){
	
	xSemaphoreHandle synchroSmartMeter;
	vSemaphoreCreateBinary(synchroSmartMeter);
	
	/*Channel* */pc = new ChannelSerialDebug;
	Channel* s1 = new ChannelSerialSercom1;
	ChannelCallback* relaytos1 = new Relay(s1);
	ChannelCallback* relaytopc = new Relay(pc);
	RelayBuffered* relaybtopc = new RelayBufferedTaskCLP(pc,synchroSmartMeter,timeProt.synchroTimeReceive);
	RelayBuffered* relaybtos1 = new RelayBufferedTaskCLP(s1,synchroSmartMeter,timeProt.synchroTimeReceive);
	
	ChannelRadio* radio = ChannelRadio::GetChannel();
	cMxRadio* r = radio->GetRadio();
	// Radio Config
	r->begin(CHANNEL_RADIO);
	
	RelayBuffered* relaybtoradio = new RelayBufferedTaskCLP(radio,synchroSmartMeter,timeProt.synchroTimeReceive);
	
	pc->SetCallback(relaybtoradio);
#ifdef MASTERMODE
	radio->SetCallback(relaybtopc);
#else
	radio->SetCallback(relaybtos1);
#endif
	s1->SetCallback(relaybtoradio);
	//PrintBuffer(pc,"RESET\r\n");
    while(1)
	{
		if(synchroSmartMeter!=NULL)
		{
			if( xSemaphoreTake(synchroSmartMeter,portMAX_DELAY) == pdTRUE )
			{//stay here until sync request
				if(relaybtopc->HasNewData())
				{
					relaybtopc->PushData();
				}
		
				if(relaybtos1->HasNewData())
				{
					relaybtos1->PushData();
				}
		
				if(relaybtoradio->HasNewData())
				{
					relaybtoradio->PushData();
				}
			}
		}
		
		
		
    }
	
	
}