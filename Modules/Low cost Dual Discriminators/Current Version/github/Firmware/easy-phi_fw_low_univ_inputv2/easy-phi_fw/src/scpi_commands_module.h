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
scpi_result_t SCPI_TestSPISend(scpi_t* context);
scpi_result_t SCPI_TestSPIReceive(scpi_t* context);

/* Discriminator related */
scpi_result_t SCPI_SetChannel1Offset(scpi_t* context);
scpi_result_t SCPI_SetChannel2Offset(scpi_t* context);
scpi_result_t SCPI_SetChannel1Threshold(scpi_t* context);
scpi_result_t SCPI_SetChannel2Threshold(scpi_t* context);
scpi_result_t SCPI_SetChannel1OffsetRelay(scpi_t* context);
scpi_result_t SCPI_SetChannel2OffsetRelay(scpi_t* context);
scpi_result_t SCPI_GetChannel1OffsetRelay(scpi_t* context);
scpi_result_t SCPI_GetChannel2OffsetRelay(scpi_t* context);
scpi_result_t SCPI_SetChannel1Coupling(scpi_t* context);
scpi_result_t SCPI_SetChannel2Coupling(scpi_t* context);
scpi_result_t SCPI_GetChannel1Threshold(scpi_t* context);
scpi_result_t SCPI_GetChannel2Threshold(scpi_t* context);
scpi_result_t SCPI_GetChannel1Coupling(scpi_t* context);
scpi_result_t SCPI_GetChannel2Coupling(scpi_t* context);
scpi_result_t SCPI_GetChannel1Offset(scpi_t* context);
scpi_result_t SCPI_GetChannel2Offset(scpi_t* context);
/* Counters */
scpi_result_t SCPI_SetIntegrationT(scpi_t* context);
scpi_result_t SCPI_CounterA_Read(scpi_t* context);
scpi_result_t SCPI_CounterB_Read(scpi_t* context);
scpi_result_t SCPI_CounterA_IT(scpi_t* context);
scpi_result_t SCPI_CounterB_IT(scpi_t* context);
scpi_result_t SCPI_GetCounterA_IT(scpi_t* context);
scpi_result_t SCPI_GetCounterB_IT(scpi_t* context);

#endif /* SCPI_COMMANDS_MODULE_H_ */