/*
 * module_scpi_mapping.h
 *
 * Created: 10/12/2013 15:36:04
 *  Author: stephan
 */ 


#ifndef MODULE_SCPI_MAPPING_H_
#define MODULE_SCPI_MAPPING_H_

#define MODULE_SCPI_COMMANDS	{.pattern = "CONFigure:DELay:Reset:VALue", .callback = SCPI_SetDelayReset,},\
{.pattern = "CONFigure:DELay:Data:VALue", .callback = SCPI_SetDelayData,},\
{.pattern = "CONFigure:DELay:Clock:VALue", .callback = SCPI_SetDelayClock,},\
{.pattern = "CONFigure:DELay:Reset:VALue?", .callback = SCPI_GetDelayReset,},\
{.pattern = "CONFigure:DELay:Data:VALue?", .callback = SCPI_GetDelayData,},\
{.pattern = "CONFigure:DELay:Clock:VALue?", .callback = SCPI_GetDelayClock,},\
{.pattern = "CONFigure:DELay:Reset:ENable", .callback = SCPI_EnableReset,},\
{.pattern = "CONFigure:DELay:Data:ENable", .callback = SCPI_EnableData,},\
{.pattern = "CONFigure:DELay:Clock:ENable", .callback = SCPI_EnableClock,},\
{.pattern = "CONFigure:DELay:Reset:ENable?", .callback = SCPI_GetEnableReset,},\
{.pattern = "CONFigure:DELay:Data:ENable?", .callback = SCPI_GetEnableData,},\
{.pattern = "CONFigure:DELay:Clock:ENable?", .callback = SCPI_GetEnableClock,},\
{.pattern = "CONFigure:RF:ATTenuation?", .callback = SCPI_GetAttenuation,},\
{.pattern = "CONFigure:RF:ATTenuation", .callback = SCPI_SetAttenuation,},\


#endif /* MODULE_SCPI_MAPPING_H_ */