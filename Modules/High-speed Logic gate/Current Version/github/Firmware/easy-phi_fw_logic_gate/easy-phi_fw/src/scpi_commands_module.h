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
scpi_result_t SCPI_SetOut1(scpi_t* context);
scpi_result_t SCPI_SetOut2(scpi_t* context);
scpi_result_t SCPI_SetOut3(scpi_t* context);
scpi_result_t SCPI_SetOut4(scpi_t* context);
scpi_result_t SCPI_GetOut1(scpi_t* context);
scpi_result_t SCPI_GetOut2(scpi_t* context);
scpi_result_t SCPI_GetOut3(scpi_t* context);
scpi_result_t SCPI_GetOut4(scpi_t* context);

#endif /* SCPI_COMMANDS_MODULE_H_ */