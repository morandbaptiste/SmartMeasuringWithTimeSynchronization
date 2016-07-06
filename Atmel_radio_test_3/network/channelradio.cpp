/*
 * channelradio.cpp
 *
 * Created: 04/07/2016 19:52:15
 *  Author: Silve
 */ 

#include "channelradio.h"
#include <FreeRTOS.h>
#include <task.h>


//extern Channel* pc;
//extern void PrintBuffer(Channel* c, char* buffer, size_t length);

void RadioOnEvent(uint8_t e)
{
	ChannelRadio* r = ChannelRadio::GetChannel();
	
//	char buffer[20];
//	memset(buffer,0,20);
//	sprintf(buffer,"RADIO EVENT: %d,%d\r\n",int(e),r->IsReceive());
//	PrintBuffer(pc,buffer,18);
//	wait_ms(1);
	
	if((e & TRX_IRQ_RX_START) == TRX_IRQ_RX_START)
	{
		r->SetReceive(true);
	}
	if((e & TRX_IRQ_TRX_END) == TRX_IRQ_TRX_END)
	{
		// Received data for correct address
		if(r->IsReceive()) //&& ((e & TRX_IRQ_AMI) == TRX_IRQ_AMI))
		{
			r->OnReceive();
			r->SetReceive(false);
		}
		// Transmitted data
		else if(!r->IsReceive())
		{
			r->OnTransmit();
			r->SetTransmit(false);
		}
	}
}

void RadioOnError(radio_error_t e)
{
	ChannelRadio::GetChannel()->OnError(e);
}

ChannelRadio* ChannelRadio::inst = 0;

ChannelRadio* ChannelRadio::GetChannel()
{
	if(inst==0)
	{
		inst = new ChannelRadio;
	}
	
	return inst;
}

cMxRadio* ChannelRadio::GetRadio()
{
	return &radio;
}

ChannelRadio::ChannelRadio()
  : radio(PB30,PC19,PC18,PB31,PB15,PA20,PB00), receiving(false), transmit(false), buffer_end(0), buffer_pos(0)
{
	radio.attachIrq(&RadioOnEvent);
	radio.attachError(&RadioOnError);
}

ChannelRadio::~ChannelRadio()
{
	
}

void ChannelRadio::putc(uint8_t c)
{
	transmit = true;
	
	radio.beginTransmission();
	radio.write(c);
	radio.endTransmission();
	
	vTaskDelay(2/portTICK_RATE_MS);
//	while(transmit){}
}

uint8_t ChannelRadio::getc()
{	
	uint8_t c = buffer[buffer_pos++];
	
	if(buffer_pos >= buffer_end)
	{
		buffer_pos = buffer_end;
	}
	else if(buffer_pos >= 50)
	{
		buffer_pos = 0;
	}
	
	return c;
}

void ChannelRadio::OnReceive()
{
	while(radio.available())
	{
		buffer[buffer_end++] = radio.read();
		
		if(buffer_end >= 50)
		{
			buffer_end = 0;
		}
	}
	
	radio.flush();
	
	if(call)
	{
		call->OnReceive(this);
	}
}

void ChannelRadio::OnTransmit()
{
	if(call)
	{
		call->OnTransmit(this);
	}
}

void ChannelRadio::OnError(int e)
{
	if(call)
	{
		call->OnError(this,e);
	}
}

void ChannelRadio::OnSpecial(int e)
{
	if(call)
	{
		call->OnSpecial(this,e);
	}
}

void ChannelRadio::SetReceive(bool set)
{
	receiving = set;
}

bool ChannelRadio::IsReceive()
{
	return receiving;
}

void ChannelRadio::SetTransmit(bool set)
{
	transmit = set;
}

bool ChannelRadio::IsTransmit()
{
	return transmit;
}

void ChannelRadio::write(uint8_t* buffer, uint8_t size)
{
	if(buffer && (size >= 0))
	{
		radio.beginTransmission();
		radio.write(buffer, size);
		radio.endTransmission();
	}
}