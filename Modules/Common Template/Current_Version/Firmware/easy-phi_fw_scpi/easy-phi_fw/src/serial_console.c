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



size_t SCPI_Write(scpi_t * context, const char * data, size_t len) {
	(void) context;
	return SCPI_RES_OK;
    
	//return fwrite(data, 1, len, stdout);
}

scpi_result_t SCPI_Flush(scpi_t * context) {
	return SCPI_RES_OK;
}

int SCPI_Error(scpi_t * context, int_fast16_t err) {
	(void) context;

	printf("**ERROR: %d, \"%s\"\r\n", (int32_t) err, SCPI_ErrorTranslate(err));
	return 0;
}

scpi_result_t SCPI_Control(scpi_t * context, scpi_ctrl_name_t ctrl, scpi_reg_val_t val) {
	if (SCPI_CTRL_SRQ == ctrl) {
		printf("**SRQ: 0x%X (%d)\r\n", val, val);
		} else {
		printf("**CTRL %02x: 0x%X (%d)\r\n", ctrl, val, val);
	}
	return SCPI_RES_OK;
}

scpi_result_t SCPI_Test(scpi_t * context) {
	printf("**Test\r\n");
	return SCPI_RES_OK;
}

scpi_result_t SCPI_Reset(scpi_t * context) {
	printf("**Reset\r\n");
	return SCPI_RES_OK;
}

scpi_result_t SCPI_SystemCommTcpipControlQ(scpi_t * context) {
	return SCPI_RES_ERR;
}


#define LINE_BUFFER_SIZE 50
char current_input_line[LINE_BUFFER_SIZE];
uint16_t current_input_line_pointer = 0;


void console_init(void)
{
	SCPI_Init(&scpi_context);
}

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
    SCPI_Input(&scpi_context, current_input_line+current_input_line_pointer, 1);	
    //SCPI_Input(&scpi_context, current_input_line[current_input_line_pointer], 1);

    //SCPI_Input(&scpi_context, smbuffer, strlen(smbuffer));	
	current_input_line_pointer++;
	
	if (current_input_line_pointer = 50)
	{
         current_input_line_pointer = 0;
	}
	    
	/*
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
	 
	*/ 
}
