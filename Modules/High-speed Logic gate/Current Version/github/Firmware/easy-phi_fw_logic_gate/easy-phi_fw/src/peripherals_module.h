/*
 * peripherals_module.h
 *
 * Created: 15/11/2013 14:47:36
 *  Author: stephan
 */ 


#ifndef PERIPHERALS_MODULE_H_
#define PERIPHERALS_MODULE_H_

#include <asf.h>

void configure_crosspoint(uint16_t in, uint16_t out);
uint16_t get_xpoint_outchannel_conf(uint16_t channel);
void init_module_peripherals_bp(void);
void init_module_peripherals_ap(void);
void deinit_module_peripherals(void);

#endif /* PERIPHERALS_MODULE_H_ */