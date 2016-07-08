/*
 * iddle.c
 *
 * Created: 08/07/2016 11:28:31
 *  Author: Time Syn Project
 */ 
#include <wdt_config.h>
void vApplicationIdleHook( void ){
	
	wdt_clear();
}