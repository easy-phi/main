/*
 * serial_console.c
 *
 * Created: 09/06/2013 18:29:35
 *  Author: limpkin
 */ 
#include <asf.h>
#include <string.h>
#include "utils.h"
#include "SCPI/libscpi/scpi.h"
#include "SCPI/common/scpi-def.h"
#include "cdc_comms.h"
#include "filesystem_operations.h"
#include "peripherals_template.h"

#define LINE_BUFFER_SIZE 200
uint16_t current_input_line_pointer = 0;
char current_input_line[LINE_BUFFER_SIZE];


size_t SCPI_Write(scpi_t * context, const char * data, size_t len) 
{
	(void) context;
	printf("%.*s", len, data);
	return SCPI_RES_OK;    
}

scpi_result_t SCPI_Flush(scpi_t * context) 
{
	return SCPI_RES_OK;
}

int SCPI_Error(scpi_t * context, int_fast16_t err) 
{
	uint16_t i = 0;
	
	#define ONLY_OUTPUT_READ_ERRORS
	#ifdef ONLY_OUTPUT_READ_ERRORS
		if (context->cmd_error == 1)
		{
			// If it's an unknown command 
			while(i < (context->buffer.position))
			{
				if (context->buffer.data[i] == '?')
				{
					printf("**ERROR: %d, \"%s\"\r\n", (int32_t) err, SCPI_ErrorTranslate(err));
				}
				i++;
			}	
		}
	#endif
	//(void) context;
	//printf("**ERROR: %d, \"%s\"\r\n", (int32_t) err, SCPI_ErrorTranslate(err));
	return 0;
}

scpi_result_t SCPI_Control(scpi_t * context, scpi_ctrl_name_t ctrl, scpi_reg_val_t val) 
{
	if (SCPI_CTRL_SRQ == ctrl) 
	{
		printf("**SRQ: 0x%X (%d)\r\n", val, val);
	}
	else 
	{
		printf("**CTRL %02x: 0x%X (%d)\r\n", ctrl, val, val);
	}
	return SCPI_RES_OK;
}

scpi_result_t SCPI_Test(scpi_t * context) 
{
	printf("**Test\r\n");
	return SCPI_RES_OK;
}

scpi_result_t SCPI_Reset(scpi_t * context) 
{
	printf("**Reset\r\n");
	return SCPI_RES_OK;
}

void console_init(void)
{
	SCPI_Init(&scpi_context);
}

void console_process(void)
{
	iram_size_t data_remaining = 0;
	uint8_t temp_buffer[10];
	
	if(udi_cdc_get_nb_received_data() == 0)
		return;
	
	do 
	{
		data_remaining = udi_cdc_read_buf(current_input_line+current_input_line_pointer, 1);
		//udi_cdc_putc(current_input_line[current_input_line_pointer]);
		SCPI_Input(&scpi_context, current_input_line+current_input_line_pointer, 1);
		current_input_line_pointer++;
		
		if((current_input_line[current_input_line_pointer-1] == 0x0D) || (current_input_line[current_input_line_pointer-1] == 0x0A))
		{
			current_input_line_pointer = 0;
			// should we do another parsing rather than scpi?
		}
	} 
	while((data_remaining > 0) && (current_input_line_pointer < LINE_BUFFER_SIZE));
	
	if(current_input_line_pointer == LINE_BUFFER_SIZE)
	{
		// We should return a message or smth...
		current_input_line_pointer = 0;
	}
	
// 	if(current_input_line[current_input_line_pointer-1] == 'x')
// 	{
// 		mount_chanfat_filesystem();
// 		list_folder_contents("/", true);
//  }
}
