#include "mbed.h"
#include "channelserial.h"
#include "channelradio.h"
#include "relay.h"

DigitalOut myled(LED1);

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

class SerialCallback : public ChannelCallback
{
public:
	uint8_t buffer[500];
	size_t buffer_end;

	virtual ~SerialCallback(){}
	
	void OnReceive(Channel* c)
	{
		buffer[buffer_end++] = c->getc();
		c->putc(buffer[buffer_end-1]);
	}
};

Channel* pc = 0;

#define MASTER

int main()
{
	/*Channel* */pc = new ChannelSerialDebug;
	Channel* s1 = new ChannelSerialSercom1;
	Channel* s3 = new ChannelSerialSercom3;
	ChannelCallback* call = new SerialCallback;
	ChannelCallback* relaytos3 = new Relay(s3);
	ChannelCallback* relaytos1 = new Relay(s1);
	ChannelCallback* relaytopc = new Relay(pc);
	RelayBuffered* relaybtopc = new RelayBuffered(pc,'\x03');
	RelayBuffered* relaybtos1 = new RelayBuffered(s1,'\x03');
	
	ChannelRadio* radio = ChannelRadio::GetChannel();
	cMxRadio* r = radio->GetRadio();
	// Radio Config
	r->begin(11);
	
	RelayBuffered* relaybtoradio = new RelayBuffered(radio, '\x03');
	
	pc->SetCallback(relaybtoradio);
#ifdef MASTER
	radio->SetCallback(relaybtopc);
#else
	radio->SetCallback(relaybtos1);
#endif
	s1->SetCallback(relaybtoradio);
	
    PrintBuffer(pc,"RESET\r\n");
//	PrintBuffer(s1,"RESET\r\n");
//	PrintBuffer(s3,"RESET\r\n");
	
    while(1)
	{
        myled = 1;
        wait(0.2);
        myled = 0;
        wait(0.2);
		
//      PrintBuffer(pc,"TEST\r\n");
//		PrintBuffer(s1,"TEST\r\n");
//		PrintBuffer(s3,"TEST\r\n");

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
	
	delete relaytopc;
	delete relaytos1;
	delete relaytos3;
	delete call;
	delete s3;
	delete s1;
	delete pc;
}
