/*
 * scpi_commands_module.c
 *
 * Created: 10/12/2013 15:26:24
 *  Author: stephan
*/
#include "scpi_commands_module.h"
#include "peripherals_template.h"
#include "peripherals_module.h"
#include "conf_usb.h"
#include <math.h>
#include "main.h"

scpi_result_t SCPI_SetIntegrationT(scpi_t* context)
{
	uint32_t param1;
	
	// read first parameter if present
	if (!SCPI_ParamInt(context, &param1, true)) {
		// do something, if parameter not present
		return SCPI_RES_ERR;
	}
	
	set_counters_integration_time((uint16_t)round(param1));
	return SCPI_RES_OK;
}

scpi_result_t SCPI_EnableCounter(scpi_t* context)
{
	enable_counters();
	return SCPI_RES_OK;
}

scpi_result_t SCPI_DisableCounter(scpi_t* context)
{
	disable_counters();
	return SCPI_RES_OK;
}

scpi_result_t SCPI_ActivatePush(scpi_t* context)
{
	activate_measurement_push();
	return SCPI_RES_OK;
}

scpi_result_t SCPI_ActivatePull(scpi_t* context)
{
	deactivate_measurement_push();
	return SCPI_RES_OK;
}

scpi_result_t SCPI_SetTTLInput(scpi_t* context)
{
	set_out_to_ttl_in();
	return SCPI_RES_OK;
}

scpi_result_t SCPI_CounterA_Enable(scpi_t* context)
{
	uint32_t param;
	
	// read first parameter if present
	if (!SCPI_ParamInt(context, &param, true)) {
		// do something, if parameter not present
		return SCPI_RES_ERR;
	}
	
	if(param == 0)
		disable_countera();
	else	
		enable_countera();
		
	return SCPI_RES_OK;
}

scpi_result_t SCPI_CounterB_Enable(scpi_t* context)
{
	uint32_t param;
	
	// read first parameter if present
	if (!SCPI_ParamInt(context, &param, true)) {
		// do something, if parameter not present
		return SCPI_RES_ERR;
	}
	
	if(param == 0)
		disable_counterb();
	else
		enable_counterb();
	
	return SCPI_RES_OK;
}

scpi_result_t SCPI_CounterA_Read(scpi_t* context)
{
	uint64_t counter_val;
	
	if(read_last_contera_val(&counter_val) == RETURN_OK)
	{
		printf("%llu\x0D", counter_val);
	}
	else
	{
		printf("No new val\x0D");
	}
	return SCPI_RES_OK;
}

scpi_result_t SCPI_CounterB_Read(scpi_t* context)
{
	uint64_t counter_val;
	
	if(read_last_conterb_val(&counter_val) == RETURN_OK)
	{
		printf("%llu\x0D", counter_val);
	}
	else
	{
		printf("No new val\x0D");
	}
	return SCPI_RES_OK;
}

scpi_result_t SCPI_CounterA_IT(scpi_t* context)
{
	uint32_t param1;
	
	// read first parameter if present
	if (!SCPI_ParamInt(context, &param1, true)) {
		// do something, if parameter not present
		return SCPI_RES_ERR;
	}
	
	set_countera_integration_time((uint16_t)round(param1));
	return SCPI_RES_OK;
}

scpi_result_t SCPI_CounterB_IT(scpi_t* context)
{
	uint32_t param1;
	
	// read first parameter if present
	if (!SCPI_ParamInt(context, &param1, true)) {
		// do something, if parameter not present
		return SCPI_RES_ERR;
	}
	
	set_counterb_integration_time((uint16_t)round(param1));
	return SCPI_RES_OK;
}

scpi_result_t SCPI_TTLInput(scpi_t* context)
{
	uint32_t param;
	
	// read first parameter if present
	if (!SCPI_ParamInt(context, &param, true)) {
		// do something, if parameter not present
		return SCPI_RES_ERR;
	}
	
	if(param == 0)
		set_out_to_uc();
	else
		set_out_to_ttl_in();
	
	return SCPI_RES_OK;
}

scpi_result_t SCPI_SetFreq(scpi_t* context)
{
	uint32_t param;
	
	// read first parameter if present
	if (!SCPI_ParamInt(context, &param, true)) {
		// do something, if parameter not present
		return SCPI_RES_ERR;
	}
	
	setup_freqgen_freq(param);
	return SCPI_RES_OK;
}

scpi_result_t SCPI_GetCounterA_Enable(scpi_t* context)
{
	printf("%u\x0D", get_countera_en_status());
	return SCPI_RES_OK;
}

scpi_result_t SCPI_GetCounterB_Enable(scpi_t* context)
{
	printf("%u\x0D", get_counterb_en_status());
	return SCPI_RES_OK;
}

scpi_result_t SCPI_GetCounterA_IT(scpi_t* context)
{
	printf("%u\x0D", get_countera_integration_time());
	return SCPI_RES_OK;
}

scpi_result_t SCPI_GetCounterB_IT(scpi_t* context)
{
	printf("%u\x0D", get_counterb_integration_time());
	return SCPI_RES_OK;
}

scpi_result_t SCPI_GetTTLInput(scpi_t* context)
{
	printf("%u\x0D", is_qma_out_set_to_ttl());
	return SCPI_RES_OK;
}

scpi_result_t SCPI_GetFreq(scpi_t* context)
{
	printf("%u\x0D", get_freqgen_freq());
	return SCPI_RES_OK;
}
