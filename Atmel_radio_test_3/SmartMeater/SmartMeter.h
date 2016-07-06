/*
 * SmartMeter.h
 *
 * Created: 06/07/2016 11:30:43
 *  Author: Time Syn Project
 */ 


#ifndef SMARTMETER_H_
#define SMARTMETER_H_

#include <channelserial.h>
#include <channelradio.h>
#include <relay.h>
#include <FreeRTOS.h>
#include <task.h>
#include <conf_timeProtocol.h>

void smartMeterTask(void);

#endif /* SMARTMETER_H_ */