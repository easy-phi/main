/*
 * peripherals_module.h
 *
 * Created: 15/11/2013 14:47:36
 *  Author: stephan
 */ 


#ifndef PERIPHERALS_MODULE_H_
#define PERIPHERALS_MODULE_H_

#include <asf.h>
#include "defines.h"

void switch_on_off_lights(uint16_t bool_lights);
uint16_t fpga_send_spi_16bits(uint16_t data);
void init_module_peripherals_bp(void);
void init_module_peripherals_ap(void);
void deinit_module_peripherals(void);

void read_counters(uint32_t* counter1_pt, uint32_t* counter2_pt);
void set_channel1_offset_relay(uint16_t is_offsetted);
void set_channel2_offset_relay(uint16_t is_offsetted);
void set_channel2_coupling(uint16_t is_dc_coupled);
void set_channel1_coupling(uint16_t is_dc_coupled);
void set_threshold_channel1(uint16_t threshold);
void set_threshold_channel2(uint16_t threshold);
RET_TYPE get_fpga_read_available_flag(void);
void set_offset_channel1(uint16_t offset);
void set_offset_channel2(uint16_t offset);
uint16_t get_channel1_offset_relay(void);
uint16_t get_channel2_offset_relay(void);
uint16_t get_channel1_threshold(void);
uint16_t get_channel2_threshold(void);
uint16_t get_channel1_coupling(void);
uint16_t get_channel2_coupling(void);
uint16_t get_offset_channel1(void);
uint16_t get_offset_channel2(void);

#endif /* PERIPHERALS_MODULE_H_ */