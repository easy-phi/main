/*
 * peripherals_template.h
 *
 * Created: 21/10/2013 14:09:05
 *  Author: stephan
 */ 


#ifndef PERIPHERALS_TEMPLATE_H_
#define PERIPHERALS_TEMPLATE_H_

#include "defines.h"

void set_user_led_colour(uint16_t red_val, uint16_t green_val, uint16_t blue_val);
RET_TYPE store_bytes_at_addr(uint16_t addr, uint8_t* buffer, uint16_t nb_bytes);
RET_TYPE read_bytes_at_addr(uint16_t addr, uint8_t* buffer, uint16_t nb_bytes);
RET_TYPE get_sync_signal_status(void);
RET_TYPE get_user_button_status(void);
RET_TYPE get_ok_12v_status(void);
RET_TYPE get_12v_status(void);
void disable_12v(void);
void enable_12v(void);
void init_pwm(void);
void init_i2c(void);


#endif /* PERIPHERALS_TEMPLATE_H_ */