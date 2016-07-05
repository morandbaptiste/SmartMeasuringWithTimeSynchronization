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
	
	void OnReceive(Channel* c);
	void PushData();
	bool HasNewData();
};


#endif /* RELAY_H_ */