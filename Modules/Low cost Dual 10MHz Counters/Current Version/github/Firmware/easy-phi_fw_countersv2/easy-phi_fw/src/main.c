/**
 * \file
 *
 * \brief Main functions for USB composite example
 *
 * Copyright (c) 2011-2013 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */
/************************************************************************
 * MODULE PARTICULAR COMMANDS                                           
 *																	
 * - CONFigure:COUnter:ENable  (Enable counter)
 * - CONFigure:COUnter:DISable (Disable counter)
 * - CONFigure:COUnter:PUSh    (Push counter measurements)
 * - CONFigure:COUnter:PULl    (Pull counter measurements)
 * - CONFigure:COUnter:SETIntegrationTime	(Set integration time)
 * - CONFigure:COUnter:SETTTLInput			(Set QMA out to the TTL input)
 * - CONFigure:COUnter:SETFreqGen			(Set QMA out to the frequency generator)
*************************************************************************/
#include <asf.h>
#include "peripherals_template.h"
#include "peripherals_module.h"
#include "conf_usb.h"
#include "defines.h"
#include "init.h"

#define VALUES_BUF_LENGTH	22
static volatile bool main_b_msc_enable = false;
static bool main_b_cdc_enable = false;
volatile uint16_t second_flag = FALSE;
/* Current counter value integrated */
uint64_t ch0_counter_integrated = 0;
uint64_t ch1_counter_integrated = 0;
/* Buffer containing our latest values */
uint64_t ch0_counters_values_buffer[VALUES_BUF_LENGTH];
uint64_t ch1_counters_values_buffer[VALUES_BUF_LENGTH];
/* Where to store the next values in our buffer */
uint16_t ch0_values_buffer_next_pointer = 0;
uint16_t ch1_values_buffer_next_pointer = 0;
/* Current integration time counters */
uint16_t ch0_integration_time_counter = 0;
uint16_t ch1_integration_time_counter = 0;
/* Goal for our integration time */
uint16_t ch0_integration_time_goal = 1;
uint16_t ch1_integration_time_goal = 1;
/* booleans and flags */
uint16_t meas_push_activated = FALSE;
volatile uint16_t integrator_flag = FALSE;


bool main_msc_enable(void)
{
	main_b_msc_enable = true;
	return true;
}
void main_msc_disable(void)
{
	main_b_msc_enable = false;
}
bool main_cdc_enable(uint8_t port)
{
	main_b_cdc_enable = true;
	return true;
}
void main_cdc_disable(uint8_t port)
{
	main_b_cdc_enable = false;
}

/* Second interrupt */
void set_second_flag(void)
{
	second_flag = TRUE;
}

/* Flag indicating an integration timer tick */
void set_integrator_timer_flag(void)
{
	integrator_flag = TRUE;
}

/* Activate measurements push to console */
void activate_measurement_push(void)
{
	meas_push_activated = TRUE;
}

/* Disable measurements push to console */
void deactivate_measurement_push(void)
{
	meas_push_activated = FALSE;
}

/* Set Counter A integration time */
void set_countera_integration_time(uint16_t integration_time)
{
	ch0_integration_time_goal = integration_time;
	ch0_integration_time_counter = 0;
}

/* Set Counter B integration time */
void set_counterb_integration_time(uint16_t integration_time)
{
	ch1_integration_time_goal = integration_time;
	ch1_integration_time_counter = 0;
}

/* Get Counter A integration time */
uint16_t get_countera_integration_time(void)
{
	return ch0_integration_time_goal;
}

/* Get Counter B integration time */
uint16_t get_counterb_integration_time(void)
{
	return ch1_integration_time_goal;
}

/* Set counters integration time */
void set_counters_integration_time(uint16_t integration_time)
{
	set_countera_integration_time(integration_time);
	set_counterb_integration_time(integration_time);
}

/* Routine to shift buffer contents to the left */
void shift_counter_buffer_contents(uint64_t* buffer)
{
	uint16_t i;
	
	// shift contents to the left!
	for(i = 0; i < (VALUES_BUF_LENGTH - 1); i++)
	{
		buffer[i] = buffer[i+1];
	}
}

/* buffer pop() */
RET_TYPE read_last_contera_val(uint64_t* return_data)
{
	if(ch0_values_buffer_next_pointer > 0)
	{
		ch0_values_buffer_next_pointer--;
		*return_data = ch0_counters_values_buffer[0];
		shift_counter_buffer_contents(ch0_counters_values_buffer);
		return RETURN_OK;
	}
	else
	{
		return RETURN_NOK;
	}
}

/* buffer pop() */
RET_TYPE read_last_conterb_val(uint64_t* return_data)
{
	if(ch1_values_buffer_next_pointer > 0)
	{
		ch1_values_buffer_next_pointer--;
		*return_data = ch1_counters_values_buffer[0];
		shift_counter_buffer_contents(ch1_counters_values_buffer);
		return RETURN_OK;
	}
	else
	{
		return RETURN_NOK;
	}
}

/* buffer push() */
void add_new_countera_measurement_to_queue(uint64_t measurement)
{
	// If the buffer is full, remove oldest element
	if(ch0_values_buffer_next_pointer == VALUES_BUF_LENGTH)
	{
		shift_counter_buffer_contents(ch0_counters_values_buffer);
		ch0_counters_values_buffer[VALUES_BUF_LENGTH-1] = measurement;
	}
	else
	{
		ch0_counters_values_buffer[ch0_values_buffer_next_pointer++] = measurement;
	}	
}

/* buffer push() */
void add_new_counterb_measurement_to_queue(uint64_t measurement)
{
	// If the buffer is full, remove oldest element
	if(ch1_values_buffer_next_pointer == VALUES_BUF_LENGTH)
	{
		shift_counter_buffer_contents(ch1_counters_values_buffer);
		ch1_counters_values_buffer[VALUES_BUF_LENGTH-1] = measurement;
	}
	else
	{
		ch1_counters_values_buffer[ch1_values_buffer_next_pointer++] = measurement;
	}
}

/*! \brief Main function. Execution starts here.
 */
int main(void)
{
	irq_initialize_vectors();
	cpu_irq_enable();
	wdt_disable(WDT);

	// Initialize the sleep manager
	sleepmgr_init();

	// Board initialization
	sysclk_init();
	init_board();
	init_pwm();
	init_i2c();

	// Module initialization	
	init_module_peripherals_bp();

	/* Initialize SD MMC stack */
	delay_ms(200);
	sd_mmc_init();

	// Start USB stack to authorize VBus monitoring
	udc_start();
	
	// Init SCPI parser
    console_init();	
		
	//set_user_led_colour(0, 300, 0);
	while (true) 
	{
		console_process();		

		if(integrator_flag == TRUE)
		{
			integrator_flag = FALSE;
			
			ch0_integration_time_counter++;
			ch1_integration_time_counter++;
			ch0_counter_integrated += get_counter0_value();
			ch1_counter_integrated += get_counter1_value();
			
			if(ch0_integration_time_counter == ch0_integration_time_goal)
			{
				ch0_integration_time_counter = 0;
				if(meas_push_activated)
				{
					printf("Counter0 value: %llu\x0D", ch0_counter_integrated);
				}
				add_new_countera_measurement_to_queue(ch0_counter_integrated);
				ch0_counter_integrated = 0;
			}
			
			if(ch1_integration_time_counter == ch1_integration_time_goal)
			{
				ch1_integration_time_counter = 0;
				if(meas_push_activated)
				{
					printf("Counter1 value: %llu\x0D", ch1_counter_integrated);
				}
				add_new_counterb_measurement_to_queue(ch1_counter_integrated);
				ch1_counter_integrated = 0;
			}
		}
		
		if(second_flag == TRUE)
		{
			second_flag = FALSE;	
		}	

		if (main_b_msc_enable) 
		{
			if (!udi_msc_process_trans()) 
			{
				//sleepmgr_enter_sleep();
			}
		}
		else
		{
			//sleepmgr_enter_sleep();
		}
	}
}