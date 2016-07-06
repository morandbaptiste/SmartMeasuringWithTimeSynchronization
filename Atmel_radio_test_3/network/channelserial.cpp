/*
 * channelserial.cpp
 *
 * Created: 04/07/2016 13:27:59
 *  Author: Silve
 */ 
#include "channelserial.h"

ChannelSerial::ChannelSerial(PinName tx, PinName rx)
  : s(tx, rx), n_rx_int(0)
{
	
}

ChannelSerial::~ChannelSerial()
{
	
}

extern Channel* pc;
extern void PrintBuffer(Channel* c, char* buffer, size_t length);

void ChannelSerial::putc(uint8_t c)
{
/*	n_rx_int++;
	if(c == '\xFF')
	{
		char buf[14];
		memset(buf,0,14);
		sprintf(buf,"Num Int: %d\r\n",n_rx_int);
		PrintBuffer(pc,buf,14);
		n_rx_int=0;
	}*/
	s.putc(c);
}

uint8_t ChannelSerial::getc()
{
	return s.getc();
}

void ChannelSerial::SetCallback(ChannelCallback* c)
{
	Channel::SetCallback(c);
	
	s.attach(this,&ChannelSerial::OnReceive,Serial::RxIrq);
//	s.attach(this,&ChannelSerial::OnTransmit,Serial::TxIrq);
}

void ChannelSerial::OnReceive()
{
	if(call)
	{
		call->OnReceive(this);
	}
}

void ChannelSerial::OnTransmit()
{
	if(call)
	{
		call->OnTransmit(this);
	}
}