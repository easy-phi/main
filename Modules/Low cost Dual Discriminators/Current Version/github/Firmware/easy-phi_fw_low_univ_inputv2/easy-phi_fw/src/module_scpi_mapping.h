/*
 * module_scpi_mapping.h
 *
 * Created: 10/12/2013 15:36:04
 *  Author: stephan
 */ 


#ifndef MODULE_SCPI_MAPPING_H_
#define MODULE_SCPI_MAPPING_H_

#define MODULE_SCPI_COMMANDS	{.pattern = "CONFigure:SPI:TestSend", .callback = SCPI_TestSPISend,},\
								{.pattern = "CONFigure:SPI:TestReceive?", .callback = SCPI_TestSPIReceive,},\
								{.pattern = "CONFigure:CHAnnel:A:OFFset:VALue", .callback = SCPI_SetChannel1Offset,},\
								{.pattern = "CONFigure:CHAnnel:B:OFFset:VALue", .callback = SCPI_SetChannel2Offset,},\
								{.pattern = "CONFigure:CHAnnel:A:OFFset:VALue?", .callback = SCPI_GetChannel1Offset,},\
								{.pattern = "CONFigure:CHAnnel:B:OFFset:VALue?", .callback = SCPI_GetChannel2Offset,},\
								{.pattern = "CONFigure:CHAnnel:A:THReshold", .callback = SCPI_SetChannel1Threshold,},\
								{.pattern = "CONFigure:CHAnnel:B:THReshold", .callback = SCPI_SetChannel2Threshold,},\
								{.pattern = "CONFigure:CHAnnel:A:THReshold?", .callback = SCPI_GetChannel1Threshold,},\
								{.pattern = "CONFigure:CHAnnel:B:THReshold?", .callback = SCPI_GetChannel2Threshold,},\
								{.pattern = "CONFigure:CHAnnel:A:OFFset:ENable", .callback = SCPI_SetChannel1OffsetRelay,},\
								{.pattern = "CONFigure:CHAnnel:B:OFFset:ENable", .callback = SCPI_SetChannel2OffsetRelay,},\
								{.pattern = "CONFigure:CHAnnel:A:OFFset:ENable?", .callback = SCPI_GetChannel1OffsetRelay,},\
								{.pattern = "CONFigure:CHAnnel:B:OFFset:ENable?", .callback = SCPI_GetChannel2OffsetRelay,},\
								{.pattern = "CONFigure:CHAnnel:A:COUpling", .callback = SCPI_SetChannel1Coupling,},\
								{.pattern = "CONFigure:CHAnnel:B:COUpling", .callback = SCPI_SetChannel2Coupling,},\
								{.pattern = "CONFigure:CHAnnel:A:COUpling?", .callback = SCPI_GetChannel1Coupling,},\
								{.pattern = "CONFigure:CHAnnel:B:COUpling?", .callback = SCPI_GetChannel2Coupling,},\
								{.pattern = "CONFigure:COUnter:SETIntegrationTime", .callback = SCPI_SetIntegrationT,},\
								{.pattern = "CONFigure:COUnter:A:Read?", .callback = SCPI_CounterA_Read,},\
								{.pattern = "CONFigure:COUnter:B:Read?", .callback = SCPI_CounterB_Read,},\
								{.pattern = "CONFigure:COUnter:A:INTegrationtime?", .callback = SCPI_GetCounterA_IT,},\
								{.pattern = "CONFigure:COUnter:A:INTegrationtime", .callback = SCPI_CounterA_IT,},\
								{.pattern = "CONFigure:COUnter:B:INTegrationtime?", .callback = SCPI_GetCounterB_IT,},\
								{.pattern = "CONFigure:COUnter:B:INTegrationtime", .callback = SCPI_CounterB_IT,},\

#endif /* MODULE_SCPI_MAPPING_H_ */