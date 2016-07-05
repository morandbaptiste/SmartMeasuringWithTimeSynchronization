/*
 * channelserial.h
 *
 * Created: 04/07/2016 13:28:17
 *  Author: Silve
 */ 


#ifndef CHANNELSERIAL_H_
#define CHANNELSERIAL_H_

#include "mbed.h"
#include "channel.h"

#define ChannelSerialDebug ChannelSerial(USBTX, USBRX);
#define ChannelSerialSercom1 ChannelSerial(PA16, PA17);
#define ChannelSerialSercom3 ChannelSerial(PA22, PA23);

class ChannelSerial : public Channel
{
protected:
	Serial s;
	
	// When data is received
	void OnReceive();
	// When data is sent
	void OnTransmit();
	
	size_t n_rx_int;
	
public:
	ChannelSerial(PinName tx, PinName rx);
	virtual ~ChannelSerial();
	
	virtual void putc(uint8_t c);
	virtual uint8_t getc();
	
	virtual void SetCallback(ChannelCallback* c);
};

#endif /* CHANNELSERIAL_H_ */