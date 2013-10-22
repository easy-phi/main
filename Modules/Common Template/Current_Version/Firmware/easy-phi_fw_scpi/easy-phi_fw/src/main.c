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
#include <asf.h>
#include "conf_usb.h"
#include "ui.h"

static volatile bool main_b_msc_enable = false;
static bool main_b_cdc_enable = false;


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

/*! \brief Main function. Execution starts here.
 */
int main(void)
{
	irq_initialize_vectors();
	cpu_irq_enable();

	// Initialize the sleep manager
	sleepmgr_init();

	sysclk_init();
	board_init();
	ui_init();
	ui_powerdown();
	
	/** SD/MMC power enable pin definition */
	#define VDD_MCI_GPIO			(PIO_PB24_IDX)
	#define VDD_MCI_PIO_ID			ID_PIOB
	#define VDD_MCI_ACTIVE_LEVEL    IOPORT_PIN_LEVEL_LOW
	#define VDD_MCI_INACTIVE_LEVEL  IOPORT_PIN_LEVEL_HIGH
	/* Enable power to the sd card slot */
	pmc_enable_periph_clk(VDD_MCI_PIO_ID);
	ioport_set_pin_dir(VDD_MCI_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(VDD_MCI_GPIO, VDD_MCI_ACTIVE_LEVEL);
	delay_ms(200);

	/* Initialize SD MMC stack */
	sd_mmc_init();

	// Start USB stack to authorize VBus monitoring
	udc_start();
    console_init();
	// The main loop manages only the power mode
	// because the USB management is done by interrupt
	while (true) 
	{
		console_process();

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