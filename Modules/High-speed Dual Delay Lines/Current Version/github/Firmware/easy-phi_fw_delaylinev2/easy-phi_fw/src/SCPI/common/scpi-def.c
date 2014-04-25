/*-
 * Copyright (c) 2012-2013 Jan Breuer,
 *
 * All Rights Reserved
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE AUTHORS ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file   scpi-def.c
 * @date   Thu Nov 15 10:58:45 UTC 2012
 * 
 * @brief  SCPI parser test
 * 
 * 
 */
#include <asf.h>
#include <string.h>
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SCPI/libscpi/scpi.h"
#include "scpi-def.h"
#include "cdc_comms.h"
#include "scpi_commands.h"
#include "module_scpi_mapping.h"
#include "scpi_commands_module.h"

#define SCPI_INPUT_BUFFER_LENGTH 256
static scpi_reg_val_t scpi_regs[SCPI_REG_COUNT];
static char scpi_input_buffer[SCPI_INPUT_BUFFER_LENGTH];

static const scpi_command_t scpi_commands[] = 
{
    /* IEEE Mandated Commands (SCPI std V1999.0 4.1.1) */
    { .pattern = "*CLS", .callback = SCPI_CoreCls,},
    { .pattern = "*ESE", .callback = SCPI_CoreEse,},
    { .pattern = "*ESE?", .callback = SCPI_CoreEseQ,},
    { .pattern = "*ESR?", .callback = SCPI_CoreEsrQ,},
    { .pattern = "*IDN?", .callback = SCPI_CoreIdnQ,},
    { .pattern = "*OPC", .callback = SCPI_CoreOpc,},
    { .pattern = "*OPC?", .callback = SCPI_CoreOpcQ,},
    { .pattern = "*RST", .callback = SCPI_CoreRst,},
    { .pattern = "*SRE", .callback = SCPI_CoreSre,},
    { .pattern = "*SRE?", .callback = SCPI_CoreSreQ,},
    { .pattern = "*STB?", .callback = SCPI_CoreStbQ,},
    { .pattern = "*TST?", .callback = SCPI_CoreTstQ,},
    { .pattern = "*WAI", .callback = SCPI_CoreWai,},

    /* Required SCPI commands (SCPI std V1999.0 4.2.1) */
    {.pattern = "SYSTem:ERRor?", .callback = SCPI_SystemErrorNextQ,},
    {.pattern = "SYSTem:ERRor:NEXT?", .callback = SCPI_SystemErrorNextQ,},
    {.pattern = "SYSTem:ERRor:COUNt?", .callback = SCPI_SystemErrorCountQ,},
    {.pattern = "SYSTem:VERSion?", .callback = SCPI_SystemVersionQ,},
		
	/* Module standard commands */
	{.pattern = "SYSTem:TYPE?", .callback = SCPI_GetModuleType,},
	{.pattern = "SYSTem:NAME?", .callback = SCPI_GetModuleName,},
	{.pattern = "SYSTem:DESC?", .callback = SCPI_GetModuleDescription,},
	{.pattern = "SYSTem:SN?", .callback = SCPI_GetModuleSN,},		
	{.pattern = "SYSTem:DPOWer?", .callback = SCPI_GetDissipatedPower,},
	{.pattern = "SYSTem:MAXCurrent?", .callback = SCPI_GetMaxCurrent,},
	{.pattern = "SYSTem:NOMCurrent?", .callback = SCPI_GetNomCurrent,},
		
	{.pattern = "STATus:SDCardstatus?", .callback = SCPI_GetSDCardStatus,},
	{.pattern = "STATus:LEDStatus?", .callback = SCPI_LedStatus,},
	{.pattern = "STATus:BUTtonstatus?", .callback = SCPI_ButtonStatus,},
		
    {.pattern = "CONFigure:LED", .callback = SCPI_SetUserLed,},
	{.pattern = "CONFigure:LIGhts", .callback = SCPI_SetLightsOnOff,},
    {.pattern = "CONFigure:POWer", .callback = SCPI_SetPower,},
    {.pattern = "CONFigure:EEPROM", .callback = SCPI_EEPROM_Write,},

	{.pattern = "MEASure:EEPROM?", .callback = SCPI_EEPROM_Read,},
		
	/* Module particular commands */
	MODULE_SCPI_COMMANDS

    {.pattern = "STATus:QUEStionable?", .callback = SCPI_StatusQuestionableEventQ,},
    {.pattern = "STATus:QUEStionable:EVENt?", .callback = SCPI_StatusQuestionableEventQ,},
    //{.pattern = "STATus:QUEStionable:CONDition?", .callback = scpi_stub_callback,},
    {.pattern = "STATus:QUEStionable:ENABle", .callback = SCPI_StatusQuestionableEnable,},
    {.pattern = "STATus:QUEStionable:ENABle?", .callback = SCPI_StatusQuestionableEnableQ,},

    {.pattern = "STATus:PRESet", .callback = SCPI_StatusPreset,},

    /* DMM */
    {.pattern = "MEASure:VOLTage:DC?", .callback = SCPI_StubQ,},
    {.pattern = "CONFigure:VOLTage:DC", .callback = SCPI_StubQ,},
    {.pattern = "MEASure:VOLTage:DC:RATio?", .callback = SCPI_StubQ,},
    {.pattern = "MEASure:VOLTage:AC?", .callback = SCPI_StubQ,},
    {.pattern = "MEASure:CURRent:DC?", .callback = SCPI_StubQ,},
    {.pattern = "MEASure:CURRent:AC?", .callback = SCPI_StubQ,},
    {.pattern = "MEASure:RESistance?", .callback = SCPI_StubQ,},
    {.pattern = "MEASure:FRESistance?", .callback = SCPI_StubQ,},
    {.pattern = "MEASure:FREQuency?", .callback = SCPI_StubQ,},
    {.pattern = "MEASure:PERiod?", .callback = SCPI_StubQ,},

    SCPI_CMD_LIST_END
};

static scpi_interface_t scpi_interface = {
    .error = SCPI_Error,
    .write = SCPI_Write,
    .control = SCPI_Control,
    .flush = SCPI_Flush,
    .reset = SCPI_Reset,
    .test = SCPI_Test,
};

scpi_t scpi_context = {
    .cmdlist = scpi_commands,
    .buffer = {
        .length = SCPI_INPUT_BUFFER_LENGTH,
        .data = scpi_input_buffer,
    },
    .interface = &scpi_interface,
    .registers = scpi_regs,
    .units = scpi_units_def,
    .special_numbers = scpi_special_numbers_def,
};
