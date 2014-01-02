/*
 * scpi_commands.c
 *
 * Created: 24/10/2013 11:22:29
 *  Author: stephan
 */
 #include "peripherals_template.h"
 #include "peripherals_module.h"
 #include "SCPI/libscpi/scpi.h"
 #include "scpi_commands.h"
 #include "cdc_comms.h"
 #include <string.h>
 #include <math.h>
 #include "asf.h"
 
 scpi_result_t SCPI_GetModuleType(scpi_t * context)
 {
	 printf("Low cost dual discriminators (v1)\x0D");
	 return SCPI_RES_OK;
 }

scpi_result_t SCPI_GetModuleName(scpi_t * context)
{
	printf("Low cost dual discriminators (v1)\x0D");
	return SCPI_RES_OK;
}

scpi_result_t SCPI_GetModuleDescription(scpi_t * context)
{
	printf("Low cost dual discriminators (v1)\x0D");
	return SCPI_RES_OK;
}

scpi_result_t SCPI_GetModuleSN(scpi_t * context)
{
	printf("0x0000\x0D");
	return SCPI_RES_OK;
}

scpi_result_t SCPI_GetDissipatedPower(scpi_t * context)
{
	printf("840mW\x0D");
	return SCPI_RES_OK;
}

scpi_result_t SCPI_GetMaxCurrent(scpi_t * context)
{
	printf("0mA\x0D");
	return SCPI_RES_OK;
}

scpi_result_t SCPI_GetNomCurrent(scpi_t * context)
{
	printf("0mA\x0D");
	return SCPI_RES_OK;
}

scpi_result_t SCPI_GetSDCardStatus(scpi_t * context)
{
	printf("OK\x0D");
	return SCPI_RES_OK;
}

scpi_result_t SCPI_LedStatus(scpi_t * context)
{
	printf("OK\x0D");
	return SCPI_RES_OK;
}

scpi_result_t SCPI_ButtonStatus(scpi_t * context)
{
	if(get_user_button_status() == RETURN_OK)
	{
		printf("pushed\x0D");
	} 
	else
	{
		printf("released\x0D");
	}
	return SCPI_RES_OK;
}

scpi_result_t SCPI_EEPROM_Read(scpi_t * context)
{
	// TBD...
	return SCPI_RES_OK;
}

scpi_result_t SCPI_EEPROM_Write(scpi_t * context)
{
	// TBD...
	return SCPI_RES_OK;
}

scpi_result_t SCPI_SetPower(scpi_t * context)
{
	const char * param;
	size_t param_len;

	if(!SCPI_ParamString(context, &param, &param_len, true)) 
		return SCPI_RES_ERR;
		
	if(param_len >= 2)
	{
		if(strncmp(param, "ON", 2) == 0)
		{
			enable_12v();
			init_module_peripherals_ap();
			return SCPI_RES_OK;
		}
		else if((param_len > 2) && (strncmp(param, "OFF", 3) == 0))
		{
			deinit_module_peripherals();
			disable_12v();
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
	
	return SCPI_RES_OK;
}

scpi_result_t SCPI_SetUserLed(scpi_t * context)
{
	uint32_t param1, param2, param3;
	
	// read first parameter if present
	if (!SCPI_ParamInt(context, &param1, true)) {
		// do something, if parameter not present
		return SCPI_RES_ERR;
	}

	// read second paraeter if present
	if (!SCPI_ParamInt(context, &param2, true)) {
		// do something, if parameter not present
		return SCPI_RES_ERR;
	}

	// read second paraeter if present
	if (!SCPI_ParamInt(context, &param3, true)) {
		// do something, if parameter not present
		return SCPI_RES_ERR;
	}

	set_user_led_colour((uint16_t)round(param1), (uint16_t)round(param2), (uint16_t)round(param3));
	return SCPI_RES_OK;
}