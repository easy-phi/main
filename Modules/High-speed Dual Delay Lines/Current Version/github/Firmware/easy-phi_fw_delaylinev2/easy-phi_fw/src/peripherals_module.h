/*
 * peripherals_module.h
 *
 * Created: 15/11/2013 14:47:36
 *  Author: stephan
 */ 


#ifndef PERIPHERALS_MODULE_H_
#define PERIPHERALS_MODULE_H_

#include <asf.h>

void switch_on_off_lights(uint16_t bool_lights);
void enable_disable_ch1_delay(uint16_t status);
void enable_disable_ch2_delay(uint16_t status);
void setup_channel1_delay(uint16_t delay);
void setup_channel2_delay(uint16_t delay);
uint16_t is_channel1_delay_enabled(void);
uint16_t is_channel2_delay_enabled(void);
void init_module_peripherals_bp(void);
void init_module_peripherals_ap(void);
void deinit_module_peripherals(void);
uint16_t get_channel1_delay(void);
uint16_t get_channel2_delay(void);
void send_updated_delays(void);

#endif /* PERIPHERALS_MODULE_H_ */