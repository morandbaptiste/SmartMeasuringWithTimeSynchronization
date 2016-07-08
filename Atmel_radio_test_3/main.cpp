#include "mbed.h"
#include <utask.h>
#include <hmi.h>
#include <conf_timeProtocol.h>
#include <ppsGPS.h>
#include <wdt_config.h>




int main()
{
	system_interrupt_disable_global();
	
	//configureHMI 
	configureHMI();
	//wdt cofnig
	wdt_config();
    //RTC configure
   configureInternalClock();
    //configure external intrerrupt for pps
    configurationPPS();
    //create your task
    kernelConfig();
    //demask global interrupt
    system_interrupt_enable_global();
    hmi.printf("\r\n\r\n**********************************************************************\r\n");
    hmi.printf("tick time: %lu ms\r\n",portTICK_RATE_MS);   
    #ifdef MASTERMODE
        hmi.printf("Master clock\r\n");
    #else
        hmi.printf("Slave Clock \r\n");
    #endif
    hmi.printf("end config application begin\r\n");
    hmi.printf("\r\n**********************************************************************\r\n\r\n");
    //start the scheduler
    vTaskStartScheduler();
    while(1){//trap
		hmi.printf("trap!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\r\n");
    }
}
