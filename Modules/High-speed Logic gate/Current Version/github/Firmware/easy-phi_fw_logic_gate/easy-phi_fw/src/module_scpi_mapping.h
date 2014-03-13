/*
 * module_scpi_mapping.h
 *
 * Created: 10/12/2013 15:36:04
 *  Author: stephan
 */ 


#ifndef MODULE_SCPI_MAPPING_H_
#define MODULE_SCPI_MAPPING_H_

#define MODULE_SCPI_COMMANDS				{.pattern = "CONFigure:OUT1", .callback = SCPI_SetOut1,},\
											{.pattern = "CONFigure:OUT2", .callback = SCPI_SetOut2,},\
											{.pattern = "CONFigure:OUT3", .callback = SCPI_SetOut3,},\
											{.pattern = "CONFigure:OUT4", .callback = SCPI_SetOut4,},\
											{.pattern = "CONFigure:OUT1?", .callback = SCPI_GetOut1,},\
											{.pattern = "CONFigure:OUT2?", .callback = SCPI_GetOut2,},\
											{.pattern = "CONFigure:OUT3?", .callback = SCPI_GetOut3,},\
											{.pattern = "CONFigure:OUT4?", .callback = SCPI_GetOut4,},\

#endif /* MODULE_SCPI_MAPPING_H_ */