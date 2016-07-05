/*
 * channelradio.h
 *
 * Created: 04/07/2016 19:52:33
 *  Author: Silve
 */ 


#ifndef CHANNELRADIO_H_
#define CHANNELRADIO_H_

#include "channel.h"
#include "MxRadio.h"

class ChannelRadio : public Channel
{
protected:
	static ChannelRadio* inst;
	
	ChannelRadio();
	cMxRadio radio;
	bool receiving;
	bool transmit;
	
	char buffer[50];
	size_t buffer_end;
	size_t buffer_pos;
	
public:
	static ChannelRadio* GetChannel();
	
	virtual ~ChannelRadio();
	
	virtual void putc(uint8_t);
	virtual uint8_t getc();
	virtual void write(uint8_t* buffer, uint8_t size);

	virtual void OnReceive();
	virtual void OnTransmit();
	virtual void OnError(int e);
	virtual void OnSpecial(int e);

	void SetReceive(bool set);
	bool IsReceive();
	
	void SetTransmit(bool set);
	bool IsTransmit();

	cMxRadio* GetRadio();
};


#endif /* CHANNELRADIO_H_ */