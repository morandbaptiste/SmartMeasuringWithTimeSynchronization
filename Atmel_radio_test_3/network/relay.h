/*
 * relay.h
 *
 * Created: 05/07/2016 14:47:16
 *  Author: Silve
 */ 


#ifndef RELAY_H_
#define RELAY_H_

#include "channel.h"
#include <stddef.h>
#include <FreeRTOS.h>
#include <semphr.h>

#define RELAY_BUFFER_SIZE 300

class Relay : public ChannelCallback
{
protected:
	Channel* output;

public:
	Relay(Channel* out);
	virtual ~Relay();
	
	virtual void OnReceive(Channel* c);
};

class RelayBuffered : public Relay
{
protected:
	char buffer[RELAY_BUFFER_SIZE];
	size_t buffer_end;
	size_t buffer_pos;
	bool newdata;
	char endchar;

public:
	RelayBuffered(Channel* out, uint8_t end='\0');
	
	virtual ~RelayBuffered();
	
	virtual void OnReceive(Channel* c);
	virtual void PushData();
	virtual bool HasNewData();
	
};

class RelayBufferedTask : public RelayBuffered
{
protected:
    xSemaphoreHandle synchroRelay;

public:
	RelayBufferedTask(Channel* out, xSemaphoreHandle synchro, uint8_t end='\0');
	
	virtual ~RelayBufferedTask();
	
	void OnReceive(Channel* c);
	
};


#endif /* RELAY_H_ */