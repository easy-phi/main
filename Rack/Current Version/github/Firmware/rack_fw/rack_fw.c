/* Simple example for Teensy USB Development Board
 * http://www.pjrc.com/teensy/
 * Copyright (c) 2008 PJRC.COM, LLC
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include "utils.h"
#include "defines.h"
#include <util/delay.h>
#include "usb_serial.h"
#include "i2c.h"

#define CPU_PRESCALE(n) (CLKPR = 0x80, CLKPR = (n))									// Define CPU prescaling
#define DEFAULT_MUX_SETTINGS	((1 << REFS1) | (1 << REFS0) | (1 << ADLAR))		// Default MUX setting for the ADC: 2.56V reference, left adjusted
uint8_t mux_channels[] = {0, 1, 4, 5, 6};											// Channels to digitize
#define MAX_MUX_CHANNELS sizeof(mux_channels)										// Number of channels
uint8_t channels_results[MAX_MUX_CHANNELS][AVG_SIZE];								// Buffer in which to store the ADC results
volatile unsigned char conv_round_finished;											// Know if we finished a conv round
uint8_t current_result_buffer_pointer;												// Pointer for averaging
uint16_t overcurrent_states;														// Var containing the overcurrent states
uint8_t current_channel;															// Current channel we're digitizing


void check_overcurrents(void)
{
	// returned as in the schematics
	if(PINB & 0x10)	overcurrent_states &= ~(1 << 1); else overcurrent_states |= (1 << 0);
	if(PINB & 0x01)	overcurrent_states &= ~(1 << 11);else overcurrent_states |= (1 << 10);
	if(PINC & 0x80)	overcurrent_states &= ~(1 << 6); else overcurrent_states |= (1 << 5);
	if(PINC & 0x40)	overcurrent_states &= ~(1 << 7); else overcurrent_states |= (1 << 6);
	if(PIND & 0x80)	overcurrent_states &= ~(1 << 2); else overcurrent_states |= (1 << 1);
	if(PIND & 0x40)	overcurrent_states &= ~(1 << 3); else overcurrent_states |= (1 << 2);
	if(PIND & 0x20)	overcurrent_states &= ~(1 << 8); else overcurrent_states |= (1 << 7);
	if(PIND & 0x10)	overcurrent_states &= ~(1 << 4); else overcurrent_states |= (1 << 3);
	if(PIND & 0x08)	overcurrent_states &= ~(1 << 9); else overcurrent_states |= (1 << 8);
	if(PIND & 0x04)	overcurrent_states &= ~(1 << 10);else overcurrent_states |= (1 << 9);
	if(PINE & 0x40)	overcurrent_states &= ~(1 << 12);else overcurrent_states |= (1 << 11);
	if(PINF & 0x80)	overcurrent_states &= ~(1 << 5); else overcurrent_states |= (1 << 4);		
}

/*!	\fn		ISR(SIG_ADC)
*	\brief	Interrupt called when a conversion is finished
*/
ISR(ADC_vect)
{
	ADCL;
	channels_results[current_channel++][current_result_buffer_pointer] = ADCH;
	
	if(current_channel == MAX_MUX_CHANNELS)
	{
		current_channel = 0;
		current_result_buffer_pointer++;
		if(current_result_buffer_pointer == AVG_SIZE)
		{
			current_result_buffer_pointer = 0;
			conv_round_finished = TRUE;
		}
	}
		
	ADMUX = DEFAULT_MUX_SETTINGS|mux_channels[current_channel];
	ADCSRA |= (1 << ADSC);
}

uint8_t compute_channel_avg(uint8_t* table)
{
	unsigned char i;
	uint16_t temp_uint16 = 0;
	
	for(i = 0; i < AVG_SIZE; i++)
	{
		temp_uint16 += table[i];
	}
	
	temp_uint16 = temp_uint16 >> AVG_DIV_SHIFT;
	return (uint8_t)temp_uint16;
}

void init_platform(void)
{
	overcurrent_states = 0;		// Init
	MCUCR |=(1<<JTD);			// Disable JTAG
	MCUCR |=(1<<JTD);			// Disable JTAG
	DDRB |= 0x06;				// 2 LEDs are on PB1 & PB2
	DDRB &= ~(0x11);			// DIG1 on PB4 & DIG11 on PB0
	PORTB &= ~(0x11);			// DIG1 on PB4 & DIG11 on PB0	
	DDRC &= ~(0xC0);			// DIG6 on PC7 & DIG7 on PC6
	PORTC &= ~(0xC0);			// DIG6 on PC7 & DIG7 on PC6	
	DDRD &= ~(0xFC);			// DIG2 on PD7, DIG3 on PD6, DIG8 on PD5, DIG4 on PD4, DIG9 on PD3, DIG10 on PD2
	PORTD &= ~(0xFC);			// DIG2 on PD7, DIG3 on PD6, DIG8 on PD5, DIG4 on PD4, DIG9 on PD3, DIG10 on PD2	
	DDRE &= ~(0x40);			// DIG12 on PE6
	PORTE &= ~(0x40);			// DIG12 on PE6
	DDRF &= ~(0x80);			// DIG5 on PF7
	PORTF &= ~(0x80);			// DIG5 on PF7
	
	DDRF &= ~(0x73);			// Analog inputs on PF0 PF1 PF4 PF5 PF6
	PORTF &= ~(0x73);			// Analog inputs on PF0 PF1 PF4 PF5 PF6
	DIDR0 |= (0x73);			// Disable input buffers
	ADMUX = DEFAULT_MUX_SETTINGS|mux_channels[current_channel];										// Ref: AVcc with Cap connected on Aref, result left justified
	ADCSRB = 0x00;																					// Default settings
	ADCSRA = (1 << ADEN) | (1 << ADSC) | (1 << ADIE) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);	// Enable ADC, enable EOC interrupt, freq adc = Fxtal / 128 = 156KHz
}

void output_status(void)
{
	unsigned int temp_int;
	char mess_to_send[64];
	char temp_buffer[32];
	
	hexaint_to_string(overcurrent_states, mess_to_send);
	str_ncpy("|", mess_to_send+chr_strlen(mess_to_send), 2);
	temp_int = compute_channel_avg(channels_results[0])*2 - 50;		// remove 0 output voltage (500mV)
	temp_int = (temp_int*100) >> 7;									// divide by 128 (instead of 133) to get number of 100mA
	int_to_string(temp_int, temp_buffer);
	str_ncpy(temp_buffer, mess_to_send+chr_strlen(mess_to_send), chr_strlen(temp_buffer)+1);
	str_ncpy("|", mess_to_send+chr_strlen(mess_to_send), 2);
	temp_int = compute_channel_avg(channels_results[1])*4;
	int_to_string(temp_int, temp_buffer);
	str_ncpy(temp_buffer, mess_to_send+chr_strlen(mess_to_send), chr_strlen(temp_buffer)+1);
	str_ncpy("|", mess_to_send+chr_strlen(mess_to_send), 2);
	temp_int = compute_channel_avg(channels_results[2])*8;
	int_to_string(temp_int, temp_buffer);
	str_ncpy(temp_buffer, mess_to_send+chr_strlen(mess_to_send), chr_strlen(temp_buffer)+1);
	str_ncpy("|", mess_to_send+chr_strlen(mess_to_send), 2);
	temp_int = compute_channel_avg(channels_results[3])*2;
	int_to_string(temp_int, temp_buffer);
	str_ncpy(temp_buffer, mess_to_send+chr_strlen(mess_to_send), chr_strlen(temp_buffer)+1);
	str_ncpy("|", mess_to_send+chr_strlen(mess_to_send), 2);
	temp_int = compute_channel_avg(channels_results[4])*4;
	int_to_string(temp_int, temp_buffer);
	str_ncpy(temp_buffer, mess_to_send+chr_strlen(mess_to_send), chr_strlen(temp_buffer)+1);

	str_ncpy("|", mess_to_send+chr_strlen(mess_to_send), 2);
	if(default_get_temperature(&temp_int) == 0)
	{
		temp_int = temp_int >> 7;
		int_to_string(temp_int, temp_buffer);
		str_ncpy(temp_buffer, mess_to_send+chr_strlen(mess_to_send), chr_strlen(temp_buffer)+1);
	}
	
	usb_serial_write(mess_to_send, int_strlen(mess_to_send));
}

int main(void)
{	
	CPU_PRESCALE(0);
	init_i2c_interface();
	init_platform();
	usb_init();
	
	//while (!usb_configured()) /* wait */ ;
	//_delay_ms(1000);
		
	while (1) 
	{
		if(conv_round_finished)
		{
			conv_round_finished = FALSE;
			/* Check if the 3.3V is between 3.1V and 3.5V */
			if((compute_channel_avg(channels_results[CHANNEL_3V]) < 155) || (compute_channel_avg(channels_results[CHANNEL_3V]) > 175))
			PORTB &= ~(0x02);
			else
			PORTB |= 0x02;
			
			/* Check if the 5V is between 4.8V and 5.2V */
			if((compute_channel_avg(channels_results[CHANNEL_5V]) < 120) || (compute_channel_avg(channels_results[CHANNEL_5V]) > 130))
			PORTB &= ~(0x04);
			else
			PORTB |= 0x04;
		}		
		
		int n = usb_serial_getchar();
		if (n >= 0) 
		{
			check_overcurrents();
			output_status();
		}
	}
}

// wait for the user to run their terminal emulator program
// which sets DTR to indicate it is ready to receive.
//while (!(usb_serial_get_control() & USB_SERIAL_DTR)) /* wait */ ;
// user no longer connected
//if (!usb_configured() || !(usb_serial_get_control() & USB_SERIAL_DTR)) {
