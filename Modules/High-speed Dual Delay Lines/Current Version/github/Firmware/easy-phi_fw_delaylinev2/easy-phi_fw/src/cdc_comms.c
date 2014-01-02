/*
 * cdc_comms.c
 *
 * Created: 04/08/2013 12:08:21
 * Author: limpkin
 * Here are all the callbacks from conf_usb.h
 */ 
#include <asf.h>
#include <string.h>
#include "conf_usb.h"
#include "cdc_comms.h"

char cdc_output_buffer[CDC_OUTPUT_BUFFER_SIZE];
volatile Bool is_new_received_cdc_char = false;
volatile Bool is_terminal_open = false;


iram_size_t cdc_send_line(char* line_to_send)
{
	if(is_terminal_open)
		return udi_cdc_write_buf((const void*) line_to_send, strlen(line_to_send));
	else
		return strlen(line_to_send);
}

void main_cdc_set_dtr(uint8_t port, bool b_enable)
{
	if (b_enable) 
		is_terminal_open = true;
	else
		is_terminal_open = false;
}

void uart_rx_notify(uint8_t port)
{
	UNUSED(port);
	is_new_received_cdc_char = true;
}

Bool is_new_cdc_char_received()
{
	irqflags_t flags;
	
	if(is_new_received_cdc_char == false)
		return false;

	flags = cpu_irq_save();
	is_new_received_cdc_char = false;
	cpu_irq_restore(flags);	
	
	return true;
}

void main_suspend_action(void){}
void main_resume_action(void){}
void main_sof_action(void){}
