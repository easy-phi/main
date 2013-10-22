/*
 * serial_console.c
 *
 * Created: 09/06/2013 18:29:35
 *  Author: limpkin
 */ 
#include <asf.h>
#include <string.h>
#include "utils.h"
#include "cdc_comms.h"
#include "filesystem_operations.h"

#define LINE_BUFFER_SIZE 50
char current_input_line[LINE_BUFFER_SIZE];
uint16_t current_input_line_pointer = 0;


void console_process(void)
{
	iram_size_t data_remaining = 0;
	//uint32_t temp_uint32;
	char temp_char;
	
	if(is_new_cdc_char_received() == false)
		return;
	
	data_remaining = udi_cdc_read_buf(current_input_line+current_input_line_pointer, 1);
	udi_cdc_putc(current_input_line[current_input_line_pointer]);
	//usart_putchar(USART0, current_input_line[current_input_line_pointer]);
	current_input_line_pointer++;
	
	while(data_remaining && (current_input_line_pointer < LINE_BUFFER_SIZE))
	{
		data_remaining = udi_cdc_read_buf(current_input_line+current_input_line_pointer, 1);
		udi_cdc_putc(current_input_line[current_input_line_pointer]);
		//usart_putchar(USART0, current_input_line[current_input_line_pointer]);
		current_input_line_pointer++;
	}
	
	if(current_input_line[current_input_line_pointer-1] == 'x')
	{
		mount_chanfat_filesystem();
		list_folder_contents("/", true);
 	}
}
