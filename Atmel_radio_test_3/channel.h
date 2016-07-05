/*
 * channel.h
 *
 * Created: 04/07/2016 13:25:21
 *  Author: Silve
 */ 


#ifndef CHANNEL_H_
#define CHANNEL_H_

#include <stdint.h>

class ChannelCallback;

class Channel
{
protected:
	ChannelCallback* call;
	
public:
    Channel() : call(0) {}
	virtual ~Channel() {}
		
	virtual void putc(uint8_t)=0;
	virtual uint8_t getc()=0;
	
	virtual void SetCallback(ChannelCallback* c)
	{
		call = c;
	}
};

class ChannelCallback
{
protected:

public:
	virtual ~ChannelCallback(){}
	
	// When data is received
	virtual void OnReceive(Channel* c){}
	// When data is sent
	virtual void OnTransmit(Channel* c){}
	// When a special event for channel occurs
	virtual void OnSpecial(Channel* c, int e){}
	// When an error occurs
	virtual void OnError(Channel* c, int e){}
};



#endif /* CHANNEL_H_ */