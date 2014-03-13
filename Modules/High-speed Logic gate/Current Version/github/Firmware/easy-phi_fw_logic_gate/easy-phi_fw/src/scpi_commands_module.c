/*
 * scpi_commands_module.c
 *
 * Created: 10/12/2013 15:26:24
 *  Author: stephan
 */ 
#include "scpi_commands_module.h"
#include "peripherals_template.h"
#include "peripherals_module.h"
#include "SCPI/libscpi/scpi.h"
#include <string.h>

scpi_result_t SCPI_SetOut1(scpi_t* context)
{
	const char* param;
	size_t param_len;

	if(!SCPI_ParamString(context, &param, &param_len, true))
		return SCPI_RES_ERR;
	
	if(param_len == 2)
	{
		if(strncmp(param, "OR", 2) == 0)
		{
			configure_crosspoint(1, 0);
			return SCPI_RES_OK;
		}
		else
		{
			return SCPI_RES_ERR;
		}
	}
	
	if(param_len == 3)
	{
		if(strncmp(param, "AND", 3) == 0)
		{
			configure_crosspoint(3, 0);
			return SCPI_RES_OK;
		}
		else if(strncmp(param, "IN1", 3) == 0)
		{
			configure_crosspoint(2, 0);
			return SCPI_RES_OK;
		}
		else if(strncmp(param, "IN2", 3) == 0)
		{
			configure_crosspoint(0, 0);
			return SCPI_RES_OK;
		}
		else
		{
			return SCPI_RES_ERR;
		}
	}
	else
	{
		return SCPI_RES_ERR;
	}
}

scpi_result_t SCPI_SetOut2(scpi_t* context)
{
	const char* param;
	size_t param_len;

	if(!SCPI_ParamString(context, &param, &param_len, true))
	return SCPI_RES_ERR;
	
	if(param_len == 2)
	{
		if(strncmp(param, "OR", 2) == 0)
		{
			configure_crosspoint(1, 1);
			return SCPI_RES_OK;
		}
		else
		{
			return SCPI_RES_ERR;
		}
	}
	
	if(param_len == 3)
	{
		if(strncmp(param, "AND", 3) == 0)
		{
			configure_crosspoint(3, 1);
			return SCPI_RES_OK;
		}
		else if(strncmp(param, "IN1", 3) == 0)
		{
			configure_crosspoint(2, 1);
			return SCPI_RES_OK;
		}
		else if(strncmp(param, "IN2", 3) == 0)
		{
			configure_crosspoint(0, 1);
			return SCPI_RES_OK;
		}
		else
		{
			return SCPI_RES_ERR;
		}
	}
	else
	{
		return SCPI_RES_ERR;
	}
}

scpi_result_t SCPI_SetOut3(scpi_t* context)
{
	const char* param;
	size_t param_len;

	if(!SCPI_ParamString(context, &param, &param_len, true))
	return SCPI_RES_ERR;
	
	if(param_len == 2)
	{
		if(strncmp(param, "OR", 2) == 0)
		{
			configure_crosspoint(1, 2);
			return SCPI_RES_OK;
		}
		else
		{
			return SCPI_RES_ERR;
		}
	}
	
	if(param_len == 3)
	{
		if(strncmp(param, "AND", 3) == 0)
		{
			configure_crosspoint(3, 2);
			return SCPI_RES_OK;
		}
		else if(strncmp(param, "IN1", 3) == 0)
		{
			configure_crosspoint(2, 2);
			return SCPI_RES_OK;
		}
		else if(strncmp(param, "IN2", 3) == 0)
		{
			configure_crosspoint(0, 2);
			return SCPI_RES_OK;
		}
		else
		{
			return SCPI_RES_ERR;
		}
	}
	else
	{
		return SCPI_RES_ERR;
	}
}

scpi_result_t SCPI_SetOut4(scpi_t* context)
{
	const char* param;
	size_t param_len;

	if(!SCPI_ParamString(context, &param, &param_len, true))
	return SCPI_RES_ERR;
	
	if(param_len == 2)
	{
		if(strncmp(param, "OR", 2) == 0)
		{
			configure_crosspoint(1, 3);
			return SCPI_RES_OK;
		}
		else
		{
			return SCPI_RES_ERR;
		}
	}
	
	if(param_len == 3)
	{
		if(strncmp(param, "AND", 3) == 0)
		{
			configure_crosspoint(3, 3);
			return SCPI_RES_OK;
		}
		else if(strncmp(param, "IN1", 3) == 0)
		{
			configure_crosspoint(2, 3);
			return SCPI_RES_OK;
		}
		else if(strncmp(param, "IN2", 3) == 0)
		{
			configure_crosspoint(0, 3);
			return SCPI_RES_OK;
		}
		else
		{
			return SCPI_RES_ERR;
		}
	}
	else
	{
		return SCPI_RES_ERR;
	}
}

scpi_result_t SCPI_GetOut1(scpi_t* context)
{
	uint16_t val = get_xpoint_outchannel_conf(0);
	
	if(val < 4)
	{
		if(val == 0)
		{
			printf("IN2\x0D");
		}
		else if(val == 1)
		{
			printf("OR\x0D");
		}
		else if(val == 2)
		{
			printf("IN1\x0D");
		}
		else if(val == 3)
		{
			printf("AND\x0D");
		}
			
		return SCPI_RES_OK;
	}
	else
	{
		return SCPI_RES_ERR;
	}		
}

scpi_result_t SCPI_GetOut2(scpi_t* context)
{
	uint16_t val = get_xpoint_outchannel_conf(1);
	
	if(val < 4)
	{
		if(val == 0)
		{
			printf("IN2\x0D");
		}
		else if(val == 1)
		{
			printf("OR\x0D");
		}
		else if(val == 2)
		{
			printf("IN1\x0D");
		}
		else if(val == 3)
		{
			printf("AND\x0D");
		}
		
		return SCPI_RES_OK;
	}
	else
	{
		return SCPI_RES_ERR;
	}
}

scpi_result_t SCPI_GetOut3(scpi_t* context)
{
	uint16_t val = get_xpoint_outchannel_conf(2);
	
	if(val < 4)
	{
		if(val == 0)
		{
			printf("IN2\x0D");
		}
		else if(val == 1)
		{
			printf("OR\x0D");
		}
		else if(val == 2)
		{
			printf("IN1\x0D");
		}
		else if(val == 3)
		{
			printf("AND\x0D");
		}
		
		return SCPI_RES_OK;
	}
	else
	{
		return SCPI_RES_ERR;
	}
}

scpi_result_t SCPI_GetOut4(scpi_t* context)
{
	uint16_t val = get_xpoint_outchannel_conf(3);
	
	if(val < 4)
	{
		if(val == 0)
		{
			printf("IN2\x0D");
		}
		else if(val == 1)
		{
			printf("OR\x0D");
		}
		else if(val == 2)
		{
			printf("IN1\x0D");
		}
		else if(val == 3)
		{
			printf("AND\x0D");
		}
		
		return SCPI_RES_OK;
	}
	else
	{
		return SCPI_RES_ERR;
	}
}