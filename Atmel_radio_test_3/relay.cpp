/*
 * relay.cpp
 *
 * Created: 05/07/2016 14:47:02
 *  Author: Silve
 */ 

#include "relay.h"
#include "mbed.h"

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