/*
 * utask.c
 *
 * Created: 13/05/2016 16:34:52
 *  Author: MORAND Baptiste
 */ 
#include "utask.h"

void kernelConfig(void){	
	// tasks creations
	timeProt.synchroTimeReceive=NULL;
	vSemaphoreCreateBinary(timeProt.synchroTimeReceive);
	xTaskCreate((pdTASK_CODE)HMITask,(const signed char*)"HMITask",configMINIMAL_STACK_SIZE,NULL, tskIDLE_PRIORITY +1, NULL);
	xTaskCreate((pdTASK_CODE)smartMeterTask,(const signed char*)"SmartMeterTask",configMINIMAL_STACK_SIZE,NULL, tskIDLE_PRIORITY +1, NULL);
	xTaskCreate((pdTASK_CODE)timeProtocolTask,(const signed char*)"time",configMINIMAL_STACK_SIZE+600,NULL, tskIDLE_PRIORITY +3, NULL);
}
