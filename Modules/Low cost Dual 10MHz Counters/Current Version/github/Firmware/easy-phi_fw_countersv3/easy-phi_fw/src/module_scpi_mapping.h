/*
 * module_scpi_mapping.h
 *
 * Created: 10/12/2013 15:36:04
 *  Author: stephan
 */ 

#ifndef MODULE_SCPI_MAPPING_H_
#define MODULE_SCPI_MAPPING_H_

#define MODULE_SCPI_COMMANDS	{.pattern = "CONFigure:COUnter:ENAble", .callback = SCPI_EnableCounter,},\
								{.pattern = "CONFigure:COUnter:DISable", .callback = SCPI_DisableCounter,},\
								{.pattern = "CONFigure:COUnter:PUSh", .callback = SCPI_ActivatePush,},\
								{.pattern = "CONFigure:COUnter:PULl", .callback = SCPI_ActivatePull,},\
								{.pattern = "CONFigure:COUnter:SETIntegrationTime", .callback = SCPI_SetIntegrationT,},\
								{.pattern = "CONFigure:COUnter:SETTTLInput", .callback = SCPI_SetTTLInput,},\
								{.pattern = "CONFigure:COUnter:A:ENable?", .callback = SCPI_GetCounterA_Enable,},\
								{.pattern = "CONFigure:COUnter:A:ENable", .callback = SCPI_CounterA_Enable,},\
								{.pattern = "CONFigure:COUnter:B:ENable?", .callback = SCPI_GetCounterB_Enable,},\
								{.pattern = "CONFigure:COUnter:B:ENable", .callback = SCPI_CounterB_Enable,},\
								{.pattern = "CONFigure:COUnter:A:Read?", .callback = SCPI_CounterA_Read,},\
								{.pattern = "CONFigure:COUnter:B:Read?", .callback = SCPI_CounterB_Read,},\
								{.pattern = "CONFigure:COUnter:A:INTegrationtime?", .callback = SCPI_GetCounterA_IT,},\
								{.pattern = "CONFigure:COUnter:A:INTegrationtime", .callback = SCPI_CounterA_IT,},\
								{.pattern = "CONFigure:COUnter:B:INTegrationtime?", .callback = SCPI_GetCounterB_IT,},\
								{.pattern = "CONFigure:COUnter:B:INTegrationtime", .callback = SCPI_CounterB_IT,},\
								{.pattern = "CONFigure:TTLinputenable?", .callback = SCPI_GetTTLInput,},\
								{.pattern = "CONFigure:TTLinputenable", .callback = SCPI_TTLInput,},\
								{.pattern = "CONFigure:FREQuence?", .callback = SCPI_GetFreq,},\
								{.pattern = "CONFigure:FREQuence", .callback = SCPI_SetFreq,},\
								/* 10/12/2013 SPECS From Raphael 
								* - CONFigure:COUnter:A:ENable boolean (Enable counter A)
								* - CONFigure:COUnter:B:ENable boolean (Enable counter B)
								* - CONFigure:COUnter:A:Read? (Read counter A)
								* - CONFigure:COUnter:B:Read? (Read counter A)
								* - CONFigure:COUnter:A:INTegrationtime integer (Set integration time of counter A in ms)
								* - CONFigure:COUnter:B:INTegrationtime integer (Set integration time of counter B in ms)
								* - CONFigure:TTLinputenable boolean (Set QMA out to the TTL input)
								* - CONFigure:FREQuence integer (Set frequency on QMA out)
								*/		
								

#endif /* MODULE_SCPI_MAPPING_H_ */