/*
 * scpi_commands_module.h
 *
 * Created: 10/12/2013 15:26:58
 *  Author: stephan
 */ 


#ifndef SCPI_COMMANDS_MODULE_H_
#define SCPI_COMMANDS_MODULE_H_

#include "SCPI/libscpi/scpi.h"

/* Module commands */
scpi_result_t SCPI_SetDelayChannelA(scpi_t* context);
scpi_result_t SCPI_SetDelayChannelB(scpi_t* context);
scpi_result_t SCPI_GetDelayChannelA(scpi_t* context);
scpi_result_t SCPI_GetDelayChannelB(scpi_t* context);
scpi_result_t SCPI_GetEnableChannelA(scpi_t* context);
scpi_result_t SCPI_GetEnableChannelB(scpi_t* context);
scpi_result_t SCPI_EnableChannelA(scpi_t* context);
scpi_result_t SCPI_EnableChannelB(scpi_t* context);

#endif /* SCPI_COMMANDS_MODULE_H_ */