/*
 * utask.c
 *
 * Created: 13/05/2016 16:34:52
 *  Author: MORAND Baptiste
 */ 
#include "utask.h"

void kernelConfig(void){	
	// tasks creations
	xTaskCreate((pdTASK_CODE)smartMeterTask,(const signed char*)"SmartMeterTask",configMINIMAL_STACK_SIZE,NULL, tskIDLE_PRIORITY +1, NULL);
}
