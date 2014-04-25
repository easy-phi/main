/*
 * scpi_commands_module.c
 *
 * Created: 10/12/2013 15:26:24
 *  Author: stephan
 */ 
#include "scpi_commands_module.h"
#include "peripherals_template.h"
#include "peripherals_module.h"


scpi_result_t SCPI_SetLightsOnOff(scpi_t * context)
{
	int32_t param;
	
	// read first parameter if present
	if (!SCPI_ParamInt(context, &param, true)) {
		// do something, if parameter not present
		return SCPI_RES_ERR;
	}
	
	if(param == 0)
	switch_on_off_lights(FALSE);
	else
	switch_on_off_lights(TRUE);
	
	return SCPI_RES_OK;
}