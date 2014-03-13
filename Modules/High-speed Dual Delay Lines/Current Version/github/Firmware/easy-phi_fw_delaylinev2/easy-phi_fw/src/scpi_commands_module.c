/*
 * scpi_commands_module.c
 *
 * Created: 10/12/2013 15:26:24
 *  Author: stephan
 */ 
#include "scpi_commands_module.h"
#include "peripherals_template.h"
#include "peripherals_module.h"
#include <math.h>


scpi_result_t SCPI_SetDelayChannelA(scpi_t* context)
{
	uint32_t param;
	
	// read first parameter if present
	if (!SCPI_ParamInt(context, &param, true)) {
		// do something, if parameter not present
		return SCPI_RES_ERR;
	}
	
	setup_channel1_delay((uint16_t)round(param/5));
	return SCPI_RES_OK;
}

scpi_result_t SCPI_SetDelayChannelB(scpi_t* context)
{
	uint32_t param;
	
	// read first parameter if present
	if (!SCPI_ParamInt(context, &param, true)) {
		// do something, if parameter not present
		return SCPI_RES_ERR;
	}
	
	setup_channel2_delay((uint16_t)round(param/5));
	return SCPI_RES_OK;
}

scpi_result_t SCPI_GetDelayChannelA(scpi_t* context)
{
	printf("%u\x0D", get_channel1_delay()*5);
	return SCPI_RES_OK;
}

scpi_result_t SCPI_GetDelayChannelB(scpi_t* context)
{
	printf("%u\x0D", get_channel2_delay()*5);
	return SCPI_RES_OK;
}

scpi_result_t SCPI_EnableChannelA(scpi_t* context)
{
	uint32_t param;
	
	// read first parameter if present
	if (!SCPI_ParamInt(context, &param, true)) {
		// do something, if parameter not present
		return SCPI_RES_ERR;
	}
	
	if(param == 0)
		enable_disable_ch1_delay(FALSE);
	else
		enable_disable_ch1_delay(TRUE);
	
	return SCPI_RES_OK;
}

scpi_result_t SCPI_EnableChannelB(scpi_t* context)
{
	uint32_t param;
	
	// read first parameter if present
	if (!SCPI_ParamInt(context, &param, true)) {
		// do something, if parameter not present
		return SCPI_RES_ERR;
	}
	
	if(param == 0)
		enable_disable_ch2_delay(FALSE);
	else
		enable_disable_ch2_delay(TRUE);
	
	return SCPI_RES_OK;
}

scpi_result_t SCPI_GetEnableChannelA(scpi_t* context)
{
	printf("%u\x0D", is_channel1_delay_enabled());
	return SCPI_RES_OK;
}

scpi_result_t SCPI_GetEnableChannelB(scpi_t* context)
{
	printf("%u\x0D", is_channel2_delay_enabled());
	return SCPI_RES_OK;
}
