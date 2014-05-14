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
scpi_result_t SCPI_SetLightsOnOff(scpi_t * context);
scpi_result_t SCPI_SetDelayReset(scpi_t* context);
scpi_result_t SCPI_SetDelayData(scpi_t* context);
scpi_result_t SCPI_SetDelayClock(scpi_t* context);
scpi_result_t SCPI_GetDelayReset(scpi_t* context);
scpi_result_t SCPI_GetDelayData(scpi_t* context);
scpi_result_t SCPI_GetDelayClock(scpi_t* context);
scpi_result_t SCPI_GetEnableReset(scpi_t* context);
scpi_result_t SCPI_GetEnableData(scpi_t* context);
scpi_result_t SCPI_GetEnableClock(scpi_t* context);
scpi_result_t SCPI_EnableReset(scpi_t* context);
scpi_result_t SCPI_EnableData(scpi_t* context);
scpi_result_t SCPI_EnableClock(scpi_t* context);
scpi_result_t SCPI_GetAttenuation(scpi_t* context);
scpi_result_t SCPI_SetAttenuation(scpi_t* context);

#endif /* SCPI_COMMANDS_MODULE_H_ */