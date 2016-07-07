/*
 * relay.cpp
 *
 * Created: 05/07/2016 14:47:02
 *  Author: Silve
 */ 

#include "relay.h"
#include "mbed.h"
#include "clp_api.h"

Relay::Relay(Channel* out)
  : output(out)
{
	
}

Relay::~Relay()
{
	
}
	
void Relay::OnReceive(Channel* c)
{
	if(output)
	{
		output->putc(c->getc());
	}
}

RelayBuffered::RelayBuffered(Channel* out, uint8_t end)
  : Relay(out), buffer_end(0), buffer_pos(0), newdata(false), endchar(end)
{
	memset(buffer,0,RELAY_BUFFER_SIZE);
}
	
RelayBuffered::~RelayBuffered()
{
		
}
	
void RelayBuffered::OnReceive(Channel* c)
{
	if(c)
	{
		uint8_t ch = c->getc();
			
		buffer[buffer_end++] = ch;
			
		if(ch == endchar)
		{
			newdata = true;
		}
			
		if(buffer_end >= RELAY_BUFFER_SIZE)
		{
			buffer_end = 0;
		}
	}
}
	
void RelayBuffered::PushData()
{
	if(output)
	{
		while(newdata)
		{
			uint8_t ch = buffer[buffer_pos++];
				
			if(buffer_pos >= RELAY_BUFFER_SIZE)
			{
				buffer_pos = 0;
			}
				
			if(buffer_pos == buffer_end)
			{
				newdata = false;
			}
			
			output->putc(ch);
		}
	}
}
	
bool RelayBuffered::HasNewData()
{
	return newdata;
}

RelayBufferedTask::RelayBufferedTask(Channel* out, xSemaphoreHandle synchro, uint8_t end)
  : RelayBuffered(out, end), synchroRelay(synchro)
{

}
	
RelayBufferedTask::~RelayBufferedTask()
{
		
}

void RelayBufferedTask::OnReceive(Channel* c)
{
	if(c)
	{
		uint8_t ch = c->getc();
			
		buffer[buffer_end++] = ch;
			
		if(ch == endchar)
		{
			newdata = true;
			if(synchroRelay!=NULL)
			{
				xSemaphoreGiveFromISR(synchroRelay,NULL);
			}
		}
			
		if(buffer_end >= RELAY_BUFFER_SIZE)
		{
			buffer_end = 0;
		}
	}
}

RelayBufferedTaskCLP::RelayBufferedTaskCLP(Channel* out, xSemaphoreHandle synchroMeter, xSemaphoreHandle synchroTime)
  : RelayBufferedTask(out, synchroMeter, '\x03'), synchroTimeSync(synchroTime)
{

}
	
RelayBufferedTaskCLP::~RelayBufferedTaskCLP()
{
		
}

void RelayBufferedTaskCLP::OnReceive(Channel* c)
{
	if(c)
	{
		uint8_t ch = c->getc();
			
		if(ch == CLP_ESCAPE_STX)
		{
		  
		  buffer_end = 0;
		  buffer_pos = 0;
		  Clock clk;
		  readClock(&clk);
		  timeProt.saveTime[0].second=clk.second;
		  timeProt.saveTime[0].halfmillis=clk.halfmillis;
		  timeProt.saveTime[0].sign=true;
		}
			
		buffer[buffer_end++] = ch;
		
		if(ch == endchar)
		{
			uint8_t packet[CLP_UTILITY_MAX_PACKET_SIZE];
			size_t size = CLP_ParsePacketEscape((uint8_t*)buffer, packet);
			//hmi.printf("ISR rece");
			if(CLP_IsPacketType(packet, CLP_EXTENSION_COMMAND_TIME_SYNC))
			{
				//hmi.printf("T");
				newdata = false;
				memcpy(timeProt.receive,packet,RELAY_BUFFER_SIZE);
				timeProt.length = size;
				if(synchroTimeSync!=NULL)
				{
				  xSemaphoreGiveFromISR(synchroTimeSync,NULL);
				}
			}
			else
			{
				hmi.printf("R");
				newdata = true;
				if(synchroRelay!=NULL)
				{
					xSemaphoreGiveFromISR(synchroRelay,NULL);
				}
			}
		}
			
		if(buffer_end >= RELAY_BUFFER_SIZE)
		{
			buffer_end = 0;
		}
	}
}