/*
 * scpi_commands_module.c
 *
 * Created: 10/12/2013 15:26:24
 *  Author: stephan
 */ 
#include "scpi_commands_module.h"
#include "peripherals_template.h"
#include "peripherals_module.h"
#include <string.h>
#include <math.h>

scpi_result_t SCPI_TestSPISend(scpi_t* context)
{
	uint32_t param;
	
	// read first parameter if present
	if (!SCPI_ParamInt(context, &param, true)) {
		// do something, if parameter not present
		return SCPI_RES_ERR;
	}
	
	fpga_send_spi_16bits((uint16_t)round(param));	
	return SCPI_RES_OK;	
}

scpi_result_t SCPI_TestSPIReceive(scpi_t* context)
{
	printf("%u\x0D", fpga_send_spi_16bits(0));
	return SCPI_RES_OK;	
}

scpi_result_t SCPI_SetChannel1Offset(scpi_t* context)
{
	int32_t param;
	
	// read first parameter if present
	if (!SCPI_ParamInt(context, &param, true)) {
		// do something, if parameter not present
		return SCPI_RES_ERR;
	}
	
	/* Offset is only between -2.5v and 2.5v */
	if((param < -2500) || (param > 2500))
	{
		return SCPI_RES_ERR;
	}
	
	set_offset_channel1((uint16_t)round((param + 2500)/1.221));
	return SCPI_RES_OK;
}

scpi_result_t SCPI_SetChannel2Offset(scpi_t* context)
{
	int32_t param;
	
	// read first parameter if present
	if (!SCPI_ParamInt(context, &param, true)) {
		// do something, if parameter not present
		return SCPI_RES_ERR;
	}
	
	/* Offset is only between -2.5v and 2.5v */
	if((param < -2500) || (param > 2500))
	{
		return SCPI_RES_ERR;
	}
	
	set_offset_channel2((uint16_t)round((param + 2500)/1.221));
	return SCPI_RES_OK;
}

scpi_result_t SCPI_SetChannel1Threshold(scpi_t* context)
{
	int32_t param;
	
	// read first parameter if present
	if (!SCPI_ParamInt(context, &param, true)) {
		// do something, if parameter not present
		return SCPI_RES_ERR;
	}
	
	/* Threshold is only between -5v and 5v */
	if((param < -5000) || (param > 5000))
	{
		return SCPI_RES_ERR;
	}
	
	set_threshold_channel1((uint16_t)round((param + 5000)/2.442));
	return SCPI_RES_OK;
}

scpi_result_t SCPI_SetChannel2Threshold(scpi_t* context)
{
	int32_t param;
	
	// read first parameter if present
	if (!SCPI_ParamInt(context, &param, true)) {
		// do something, if parameter not present
		return SCPI_RES_ERR;
	}
	
	/* Threshold is only between -5v and 5v */
	if((param < -5000) || (param > 5000))
	{
		return SCPI_RES_ERR;
	}
	
	set_threshold_channel2((uint16_t)round((param + 5000)/2.442));
	return SCPI_RES_OK;
}

scpi_result_t SCPI_SetChannel1OffsetRelay(scpi_t* context)
{
	uint32_t param;
	
	// read first parameter if present
	if (!SCPI_ParamInt(context, &param, true)) {
		// do something, if parameter not present
		return SCPI_RES_ERR;
	}
	
	if(param == 0)
		set_channel1_offset_relay(FALSE);
	else
		set_channel1_offset_relay(TRUE);
	
	return SCPI_RES_OK;
}

scpi_result_t SCPI_SetChannel2OffsetRelay(scpi_t* context)
{
	uint32_t param;
	
	// read first parameter if present
	if (!SCPI_ParamInt(context, &param, true)) {
		// do something, if parameter not present
		return SCPI_RES_ERR;
	}
	
	if(param == 0)
		set_channel2_offset_relay(FALSE);
	else
		set_channel2_offset_relay(TRUE);
	
	return SCPI_RES_OK;
}

scpi_result_t SCPI_SetChannel1Coupling(scpi_t* context)
{
	const char* param;
	size_t param_len;

	if(!SCPI_ParamString(context, &param, &param_len, true))
		return SCPI_RES_ERR;
	
	if(param_len == 2)
	{
		if(strncmp(param, "AC", 2) == 0)
		{
			set_channel1_coupling(FALSE);
			return SCPI_RES_OK;
		}
		else if(strncmp(param, "DC", 2) == 0)
		{
			set_channel1_coupling(TRUE);
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

scpi_result_t SCPI_SetChannel2Coupling(scpi_t* context)
{
	const char* param;
	size_t param_len;

	if(!SCPI_ParamString(context, &param, &param_len, true))
	return SCPI_RES_ERR;
	
	if(param_len == 2)
	{
		if(strncmp(param, "AC", 2) == 0)
		{
			set_channel2_coupling(FALSE);
			return SCPI_RES_OK;
		}
		else if(strncmp(param, "DC", 2) == 0)
		{
			set_channel2_coupling(TRUE);
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

scpi_result_t SCPI_GetChannel1Threshold(scpi_t* context)
{
	int16_t val = (int16_t)get_channel1_threshold();
	val = round(val*2.442 - 5000);
	printf("%i\x0D", val);
	return SCPI_RES_OK;
}

scpi_result_t SCPI_GetChannel2Threshold(scpi_t* context)
{
	int16_t val = (int16_t)get_channel2_threshold();
	val = round(val*2.442 - 5000);
	printf("%i\x0D", val);
	return SCPI_RES_OK;
}

scpi_result_t SCPI_GetChannel1Coupling(scpi_t* context)
{
	if(get_channel1_coupling() == FALSE)
	{
		printf("AC\x0D");
	} 
	else
	{
		printf("DC\x0D");
	}
	return SCPI_RES_OK;
}

scpi_result_t SCPI_GetChannel2Coupling(scpi_t* context)
{
	if(get_channel2_coupling() == FALSE)
	{
		printf("AC\x0D");
	}
	else
	{
		printf("DC\x0D");
	}
	return SCPI_RES_OK;
}

scpi_result_t SCPI_GetChannel1OffsetRelay(scpi_t* context)
{
	printf("%u\x0D", get_channel1_offset_relay());
	return SCPI_RES_OK;
}

scpi_result_t SCPI_GetChannel2OffsetRelay(scpi_t* context)
{
	printf("%u\x0D", get_channel2_offset_relay());
	return SCPI_RES_OK;
}

scpi_result_t SCPI_GetChannel1Offset(scpi_t* context)
{
	int16_t val = (int16_t)get_offset_channel1();
	val = round(val*1.221 - 2500);
	printf("%i\x0D", val);
	return SCPI_RES_OK;
}

scpi_result_t SCPI_GetChannel2Offset(scpi_t* context)
{
	int16_t val = (int16_t)get_offset_channel2();
	val = round(val*1.221 - 2500);
	printf("%i\x0D", val);
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