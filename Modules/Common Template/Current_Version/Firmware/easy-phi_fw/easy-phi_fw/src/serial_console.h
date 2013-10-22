/*
 * serial_console.h
 *
 * Created: 09/06/2013 18:29:48
 *  Author: limpkin
 */ 


#ifndef SERIAL_CONSOLE_H_
#define SERIAL_CONSOLE_H_

#include <asf.h>

/*! \brief Configures communication line
 *
 * \param cfg      line configuration
 */
void uart_config(uint8_t port, usb_cdc_line_coding_t * cfg);

/*! \brief console process
*/
void console_process(void);

#endif /* SERIAL_CONSOLE_H_ */