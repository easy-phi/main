/*
 * scpi_commands.h
 *
 * Created: 24/10/2013 11:22:19
 *  Author: stephan
 */ 


#ifndef SCPI_COMMANDS_H_
#define SCPI_COMMANDS_H_

#include "SCPI/libscpi/scpi.h"

scpi_result_t SCPI_GetModuleType(scpi_t * context);
scpi_result_t SCPI_GetModuleName(scpi_t * context);
scpi_result_t SCPI_GetModuleDescription(scpi_t * context);
scpi_result_t SCPI_GetModuleSN(scpi_t * context);
scpi_result_t SCPI_GetDissipatedPower(scpi_t * context);
scpi_result_t SCPI_GetMaxCurrent(scpi_t * context);
scpi_result_t SCPI_GetNomCurrent(scpi_t * context);
scpi_result_t SCPI_LedStatus(scpi_t * context);
scpi_result_t SCPI_ButtonStatus(scpi_t * context);
scpi_result_t SCPI_EEPROM_Read(scpi_t * context);
scpi_result_t SCPI_EEPROM_Write(scpi_t * context);
scpi_result_t SCPI_GetSDCardStatus(scpi_t * context);
scpi_result_t SCPI_SetPower(scpi_t * context);
scpi_result_t SCPI_SetUserLed(scpi_t * context);

#endif /* SCPI_COMMANDS_H_ */