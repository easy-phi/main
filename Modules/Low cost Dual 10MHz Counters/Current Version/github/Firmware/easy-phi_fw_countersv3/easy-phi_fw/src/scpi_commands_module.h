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
scpi_result_t SCPI_SetIntegrationT(scpi_t* context);
scpi_result_t SCPI_EnableCounter(scpi_t* context);
scpi_result_t SCPI_DisableCounter(scpi_t* context);
scpi_result_t SCPI_ActivatePush(scpi_t* context);
scpi_result_t SCPI_ActivatePull(scpi_t* context);
scpi_result_t SCPI_SetTTLInput(scpi_t* context);
/* New commands */
scpi_result_t SCPI_CounterA_Enable(scpi_t* context);
scpi_result_t SCPI_CounterB_Enable(scpi_t* context);
scpi_result_t SCPI_CounterA_Read(scpi_t* context);
scpi_result_t SCPI_CounterB_Read(scpi_t* context);
scpi_result_t SCPI_CounterA_IT(scpi_t* context);
scpi_result_t SCPI_CounterB_IT(scpi_t* context);
scpi_result_t SCPI_TTLInput(scpi_t* context);
scpi_result_t SCPI_SetFreq(scpi_t* context);
scpi_result_t SCPI_GetCounterA_Enable(scpi_t* context);
scpi_result_t SCPI_GetCounterB_Enable(scpi_t* context);
scpi_result_t SCPI_GetCounterA_IT(scpi_t* context);
scpi_result_t SCPI_GetCounterB_IT(scpi_t* context);
scpi_result_t SCPI_GetTTLInput(scpi_t* context);
scpi_result_t SCPI_GetFreq(scpi_t* context);
scpi_result_t SCPI_GetFreqEnable(scpi_t* context);
scpi_result_t SCPI_SetFreqEnable(scpi_t* context);
scpi_result_t SCPI_GenPulse(scpi_t* context);
scpi_result_t SCPI_GetQMAOutput(scpi_t* context);
scpi_result_t SCPI_SetQMAOutput(scpi_t* context);
scpi_result_t SCPI_GetQMState(scpi_t* context);
scpi_result_t SCPI_SetQMState(scpi_t* context);
scpi_result_t SCPI_SetLightsOnOff(scpi_t * context);

#endif /* SCPI_COMMANDS_MODULE_H_ */