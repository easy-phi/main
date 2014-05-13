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
#include <string.h>
#include <math.h>
#include "main.h"


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

scpi_result_t SCPI_GetFreqEnable(scpi_t* context)
{
	printf("%u\x0D", get_freqgen_enable());
	return SCPI_RES_OK;
}

scpi_result_t SCPI_SetFreqEnable(scpi_t* context)
{
	uint32_t param;
	
	// read first parameter if present
	if (!SCPI_ParamInt(context, &param, true)) {
		// do something, if parameter not present
		return SCPI_RES_ERR;
	}
	
	if(param == 0)
		disable_freq_gen();
	else
		enable_freq_gen();
	
	return SCPI_RES_OK;
}

scpi_result_t SCPI_GenPulse(scpi_t* context)
{
	generate_pulse();
	return SCPI_RES_OK;
}

scpi_result_t SCPI_GetQMAOutput(scpi_t* context)
{
	if (get_qma_out_mode() == MODE_IN)
	{
		printf("IN\x0D");
	}
	else if (get_qma_out_mode() == MODE_GEN)
	{
		printf("GEN\x0D");
	}
	else
	{
		printf("STATE\x0D");
	}	
}

scpi_result_t SCPI_SetQMAOutput(scpi_t* context)
{
	const char* param;
	size_t param_len;

	if(!SCPI_ParamString(context, &param, &param_len, true))
		return SCPI_RES_ERR;
	
	if(param_len == 2)
	{
		if(strncmp(param, "IN", 2) == 0)
		{
			set_qma_out_mode(MODE_IN);
			return SCPI_RES_OK;
		}
		else
		{
			return SCPI_RES_ERR;
		}
	}
	else if(param_len == 3)
	{
		if(strncmp(param, "GEN", 3) == 0)
		{
			set_qma_out_mode(MODE_GEN);
			return SCPI_RES_OK;
		}
		else
		{
			return SCPI_RES_ERR;
		}
	}
	else if(param_len == 5)
	{
		if(strncmp(param, "STATE", 5) == 0)
		{
			set_qma_out_mode(MODE_STATE);
			return SCPI_RES_OK;
		}
		else
		{
			return SCPI_RES_ERR;
		}
	}
	else
	{
		return SCPI_RES_OK;
	}	
}

scpi_result_t SCPI_GetQMState(scpi_t* context)
{
	printf("%u\x0D", get_qma_state());
	return SCPI_RES_OK;
}

scpi_result_t SCPI_SetQMState(scpi_t* context)
{
	uint32_t param;
	
	// read first parameter if present
	if (!SCPI_ParamInt(context, &param, true)) {
		// do something, if parameter not present
		return SCPI_RES_ERR;
	}
	
	if(param == 0)
		set_qma_state(0);
	else
		set_qma_state(1);
	
	return SCPI_RES_OK;	
}
