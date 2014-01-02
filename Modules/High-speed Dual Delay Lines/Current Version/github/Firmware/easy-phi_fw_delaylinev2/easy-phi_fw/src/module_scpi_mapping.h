/*
 * module_scpi_mapping.h
 *
 * Created: 10/12/2013 15:36:04
 *  Author: stephan
 */ 


#ifndef MODULE_SCPI_MAPPING_H_
#define MODULE_SCPI_MAPPING_H_

#define MODULE_SCPI_COMMANDS	{.pattern = "CONFigure:DELay:A:VALue", .callback = SCPI_SetDelayChannelA,},\
								{.pattern = "CONFigure:DELay:B:VALue", .callback = SCPI_SetDelayChannelB,},\
								{.pattern = "CONFigure:DELay:A:VALue?", .callback = SCPI_GetDelayChannelA,},\
								{.pattern = "CONFigure:DELay:B:VALue?", .callback = SCPI_GetDelayChannelB,},\
								{.pattern = "CONFigure:DELay:A:ENable", .callback = SCPI_EnableChannelA,},\
								{.pattern = "CONFigure:DELay:B:ENable", .callback = SCPI_EnableChannelB,},\
								{.pattern = "CONFigure:DELay:A:ENable?", .callback = SCPI_GetEnableChannelA,},\
								{.pattern = "CONFigure:DELay:B:ENable?", .callback = SCPI_GetEnableChannelB,},\

#endif /* MODULE_SCPI_MAPPING_H_ */