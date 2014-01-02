/*
 * cdc_comms.h
 *
 * Created: 04/08/2013 12:08:37
 *  Author: limpkin
 */ 


#ifndef CDC_COMMS_H_
#define CDC_COMMS_H_

#include "asf.h"

#define CDC_OUTPUT_BUFFER_SIZE	100
extern char cdc_output_buffer[CDC_OUTPUT_BUFFER_SIZE];
iram_size_t cdc_send_line(char* line_to_send);

#define printf(...)		snprintf(cdc_output_buffer, 100, __VA_ARGS__); cdc_send_line(cdc_output_buffer)
#define puts(...)		cdc_send_line(__VA_ARGS__)

Bool is_new_cdc_char_received(void);

/*! \brief Called by CDC interface
 * Callback running when CDC device have received data
 */
void uart_rx_notify(uint8_t port);

#endif /* CDC_COMMS_H_ */