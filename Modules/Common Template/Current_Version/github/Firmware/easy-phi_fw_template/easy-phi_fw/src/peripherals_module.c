/*
 * peripherals_module.c
 *
 * Created: 15/11/2013 14:47:11
 *  Author: stephan
 */ 

#include "peripherals_module.h"
#include "defines.h"

/* switch lights off bool */
uint16_t lights_off = FALSE;


void switch_on_off_lights(uint16_t bool_lights)
{
	lights_off = bool_lights;
}

// Here should be all the initialization functions for the module before 12v power
void init_module_peripherals_bp(void)
{
	
}

// Here should be all the initialization functions for the module after 12v power
void init_module_peripherals_ap(void)
{
	
}

// Here should be all the deinitialization functions for the module before 12v power removal
void deinit_module_peripherals(void)
{
	
}

