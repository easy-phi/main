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

#define MAX_DELAY 11242

scpi_result_t SCPI_SetDelayReset(scpi_t* context)
{
	int32_t param;
	
	// read first parameter if present
	if (!SCPI_ParamInt(context, &param, true)) {
		// do something, if parameter not present
		return SCPI_RES_ERR;
	}
	
	if ((param < 0) || (param > MAX_DELAY))
	{
		return SCPI_RES_ERR;
	}
	
	SetDelayReset((uint16_t)param);
	
	return SCPI_RES_OK;
}

scpi_result_t SCPI_SetDelayData(scpi_t* context)
{
	int32_t param;
	
	// read first parameter if present
	if (!SCPI_ParamInt(context, &param, true)) {
		// do something, if parameter not present
		return SCPI_RES_ERR;
	}
	
	if ((param < 0) || (param > MAX_DELAY))
	{
		return SCPI_RES_ERR;
	}
	
	SetDelayData((uint16_t)param);
	
	return SCPI_RES_OK;
}

scpi_result_t SCPI_SetDelayClock(scpi_t* context)
{
	int32_t param;
	
	// read first parameter if present
	if (!SCPI_ParamInt(context, &param, true)) {
		// do something, if parameter not present
		return SCPI_RES_ERR;
	}
	
	if ((param < 0) || (param > MAX_DELAY))
	{
		return SCPI_RES_ERR;
	}
	
	SetDelayClock((uint16_t)param);
		
	return SCPI_RES_OK;
}

scpi_result_t SCPI_GetDelayReset(scpi_t* context)
{
	printf("%u\x0D", GetDelayReset());
	return SCPI_RES_OK;
}

scpi_result_t SCPI_GetDelayData(scpi_t* context)
{
	printf("%u\x0D", GetDelayData());
	return SCPI_RES_OK;
}

scpi_result_t SCPI_GetDelayClock(scpi_t* context)
{
	printf("%u\x0D", GetDelayClock());
	return SCPI_RES_OK;
}

scpi_result_t SCPI_EnableReset(scpi_t* context)
{
	int32_t param;
	
	// read first parameter if present
	if (!SCPI_ParamInt(context, &param, true)) {
		// do something, if parameter not present
		return SCPI_RES_ERR;
	}
	
	if(param == 0)
		enable_disable_reset_delay(FALSE);
	else
		enable_disable_reset_delay(TRUE);
	
	return SCPI_RES_OK;
}

scpi_result_t SCPI_EnableData(scpi_t* context)
{
	int32_t param;
	
	// read first parameter if present
	if (!SCPI_ParamInt(context, &param, true)) {
		// do something, if parameter not present
		return SCPI_RES_ERR;
	}
	
	if(param == 0)
	enable_disable_data_delay(FALSE);
	else
	enable_disable_data_delay(TRUE);
	
	return SCPI_RES_OK;
}

scpi_result_t SCPI_EnableClock(scpi_t* context)
{
	int32_t param;
	
	// read first parameter if present
	if (!SCPI_ParamInt(context, &param, true)) {
		// do something, if parameter not present
		return SCPI_RES_ERR;
	}
	
	if(param == 0)
	enable_disable_clock_delay(FALSE);
	else
	enable_disable_clock_delay(TRUE);
	
	return SCPI_RES_OK;
}

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

scpi_result_t SCPI_GetEnableReset(scpi_t* context)
{
	printf("%u\x0D", is_reset_delay_enabled());
	return SCPI_RES_OK;
}

scpi_result_t SCPI_GetEnableData(scpi_t* context)
{
	printf("%u\x0D", is_data_delay_enabled());
	return SCPI_RES_OK;
}

scpi_result_t SCPI_GetEnableClock(scpi_t* context)
{
	printf("%u\x0D", is_clock_delay_enabled());
	return SCPI_RES_OK;
}

scpi_result_t SCPI_GetAttenuation(scpi_t* context)
{
	double return_val = (double)get_current_attenuation();
	return_val /= 4;
	printf("%lf\x0D", return_val);
	return SCPI_RES_OK;	
}

scpi_result_t SCPI_SetAttenuation(scpi_t* context)
{
	double param;
	
	// read first parameter if present
	if (!SCPI_ParamDouble(context, &param, true)) {
		// do something, if parameter not present
		return SCPI_RES_ERR;
	}
	
	setup_rf_attenuator((uint16_t)round(param*4));
	return SCPI_RES_OK;
}

