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
void enable_disable_reset_delay(uint16_t status);
void enable_disable_data_delay(uint16_t status);
void enable_disable_clock_delay(uint16_t status);
void setup_delay(uint16_t channel, uint16_t PD1, uint16_t PD0);
void setup_rf_attenuator(uint16_t atten);
uint16_t get_current_attenuation(void);
uint16_t is_reset_delay_enabled(void);
uint16_t is_data_delay_enabled(void);
uint16_t is_clock_delay_enabled(void);
void init_module_peripherals_bp(void);
void init_module_peripherals_ap(void);
void deinit_module_peripherals(void);

void SetDelayClock(uint16_t delay);
void SetDelayData(uint16_t delay);
void SetDelayReset(uint16_t delay);

uint16_t GetDelayClock(void);
uint16_t GetDelayData(void);
uint16_t GetDelayReset(void);

#endif /* PERIPHERALS_MODULE_H_ */