/*
 * rtc.c
 *
 * Created: 03/05/2012 19:13:23
 *  Author: limpkin
 */ 

#include <avr/io.h>
#include "defines.h"
#include <util/delay.h>

/*!	\fn		start_condition()	
*	\brief	Generate start condition
*/
#define start_condition()		(TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN))
/*!	\fn		stop_condition()	
*	\brief	Generate stop condition
*/
#define	stop_condition()		(TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO))
/*!	\fn		clear_twint_flag()	
*	\brief	Clear TWINT flag
*/
#define	clear_twint_flag()		(TWCR = (1<<TWINT) | (1<<TWEN))
/*!	\fn		acknowledge_data()	
*	\brief	Acknowledge received data, ask for a new one
*/
#define	acknowledge_data()		(TWCR = (1<<TWINT) | (1<<TWEN) | (1 << TWEA))


/*!	\fn		wait_for_twint_flag()	
*	\brief	Wait for TWINT flag, indicating that current task is finished
*/
static inline void wait_for_twint_flag()
{
	while(!(TWCR & (1<<TWINT)));
}

/*!	\fn		is_start_sending_ok()	
*	\brief	Know if we successfully generated the start condition
*	\return	TRUE if everything is alright
*/
static inline unsigned char is_start_sending_ok()
{
	if((TWSR & 0xF8) == I2C_START)
		return TRUE;
	else
		return FALSE;
}

/*!	\fn		is_restart_sending_ok()	
*	\brief	Know if we successfully generated the restart condition
*	\return	TRUE if everything is alright
*/
static inline unsigned char is_restart_sending_ok()
{
	if((TWSR & 0xF8) == I2C_RSTART)
		return TRUE;
	else
		return FALSE;
}

/*!	\fn		is_slaw_sending_ok()	
*	\brief	Know if we successfully sent the addr byte
*	\return	TRUE if everything is alright
*/
static inline unsigned char is_slaw_sending_ok()
{
	if((TWSR & 0xF8) == I2C_SLA_ACK)
		return TRUE;
	else
		return FALSE;
}

/*!	\fn		is_slar_sending_ok()	
*	\brief	Know if we successfully sent the addr byte (reading mode)
*	\return	TRUE if everything is alright
*/
static inline unsigned char is_slar_sending_ok()
{
	if((TWSR & 0xF8) == I2C_SLAR_ACK)
		return TRUE;
	else
		return FALSE;
}

/*!	\fn		is_data_sending_ok()	
*	\brief	Know if we successfully sent the data byte
*	\return	TRUE if everything is alright
*/
static inline unsigned char is_data_sending_ok()
{
	if((TWSR & 0xF8) == I2C_DATA_ACK)
		return TRUE;
	else
		return FALSE;
}

/*!	\fn		wait_for_twr(unsigned char addr)
*	\brief	Wait for data to be stored in memory after a write
*	\param	addr	The address
*/
void wait_for_twr(unsigned char addr)
{	
	_delay_ms(6);
	/*unsigned char value = 1;

	while(value)
	{
		start_condition();
		wait_for_twint_flag();
		TWDR = addr;
		clear_twint_flag();
		wait_for_twint_flag();
		if(is_slaw_sending_ok())
			value = 0;
		stop_condition();
	}*/
}

unsigned char default_get_temperature(unsigned int* result_ptr)
{
	unsigned int temp_int = 0;
	
	start_condition();
	wait_for_twint_flag();
	if(!is_start_sending_ok())
	{
		stop_condition();
		return I2C_START_ERROR;
	}
	
	TWDR = 0x91;
	clear_twint_flag();
	wait_for_twint_flag();
	if(!is_slar_sending_ok())
	{
		stop_condition();
		return I2C_SLAR_ERROR;
	}
	
	acknowledge_data();
	wait_for_twint_flag();
	temp_int = TWDR;
	temp_int =  temp_int << 8;
	
	clear_twint_flag();
	wait_for_twint_flag();
	temp_int += TWDR;
	
	stop_condition();
	
	*result_ptr = temp_int;	
	return 0;
}

/*!	\fn		initiate_write(unsigned char addr)
*	\brief	Initiate a write process in the eeprom
*	\param	addr	The address of the EEPROM
*	\return	0x00 if everything is alright, the pb code otherwise
*/
unsigned char initiate_write(unsigned char addr)
{	
	start_condition();
	wait_for_twint_flag();
	if(!is_start_sending_ok())
	{
		stop_condition();
		return I2C_START_ERROR;
	}

	if(addr < 0x20)		// this is cheating!!! this way, we can't read or write in eeprom adresses from 0x00 to 0x20 as this addr range is replaced with sram
		TWDR = 0xDE;
	else
		TWDR = 0xAE;
	
	clear_twint_flag();
	wait_for_twint_flag();
	if(!is_slaw_sending_ok())
	{
		stop_condition();
		return I2C_SLA_ERROR;
	}

	TWDR = addr;
	clear_twint_flag();
	wait_for_twint_flag();
	if(!is_data_sending_ok())
	{
		stop_condition();
		return I2C_DATA_ERROR;
	}

	return 0x00;
}

/*!	\fn		write_nb_chars(unsigned char addr, unsigned char nb_chars, unsigned char* chars)
*	\brief	Write a number of bytes in the memory
*	\param	addr		The address of the EEPROM
*	\param	nb_chars	Number of chars
*	\param	chars		Pointer to the string we have to send
*	\return	0x00 if everything is alright, the pb code otherwise
*/
unsigned char write_nb_chars(unsigned char addr, unsigned char nb_chars, unsigned char* chars)
{
	unsigned char i;
	unsigned char value;
	
	value = initiate_write(addr);

	if(value != 0)
		return value;

	for(i = 0; i < nb_chars; i++)
	{
		TWDR = chars[i];
		clear_twint_flag();
		wait_for_twint_flag();
		if(!is_data_sending_ok())
		{
			stop_condition();
			return I2C_DATA_ERROR;
		}
	}

	stop_condition();
	return 0x00;
}

/*!	\fn		initiate_eeprom_read(unsigned char addr)
*	\brief	Initiate a read process in the eeprom
*	\param	addr	The address of the EEPROM
*	\return	0x00 if everything is alright, the pb code otherwise
*/
unsigned char initiate_eeprom_read(unsigned char addr)
{
	unsigned char value;
	
	value = initiate_write(addr);

	if(value)
		return value;
	
	start_condition();
	wait_for_twint_flag();
	if(!is_restart_sending_ok())
	{
		stop_condition();
		return I2C_RSTART_ERR;
	}

	if(addr < 0x20)		// this is cheating!!! this way, we can't read or write in eeprom adresses from 0x00 to 0x20 as this addr range is replaced with sram
		TWDR = 0xDE | 0x01;
	else
		TWDR = 0xAE | 0x01;
		
	clear_twint_flag();
	wait_for_twint_flag();
	if(!is_slar_sending_ok())
	{
		stop_condition();
		return I2C_SLAR_ERROR;
	}

	return 0x00;
}

/*!	\fn		get_next_eeprom_byte(unsigned char is_last_byte)
*	\brief	Get the next byte in the eeprom
*	\param	is_last_byte	Boolean to know if it is the last byte we want to read
*	\return	The byte
*/
unsigned char get_next_eeprom_byte(unsigned char is_last_byte)
{
	unsigned char return_value;

	if(is_last_byte)
	{
		wait_for_twint_flag();
		return_value = TWDR;
		clear_twint_flag();
		wait_for_twint_flag();
		stop_condition();
	}
	else
	{
		wait_for_twint_flag();
		return_value = TWDR;
		acknowledge_data();
	}

	return return_value;
}

/*!	\fn		read_nb_chars(unsigned char addr, unsigned char nb_chars, unsigned char* chars)
*	\brief	Read a number of bytes in the memory
*	\param	addr		The address of the EEPROM
*	\param	nb_chars	Number of chars
*	\param	chars		Pointer to the string we will store the data
*	\return	0x00 if everything is alright, the pb code otherwise
*/
unsigned char read_nb_chars(unsigned char addr, unsigned char nb_chars, unsigned char* chars)
{
	unsigned int i;

	initiate_eeprom_read(addr);

	if(nb_chars == 1)
		clear_twint_flag();
	else
		acknowledge_data();

	for(i = 0; i < nb_chars - 1; i++)
	{
		wait_for_twint_flag();
		chars[i] = TWDR;
		acknowledge_data();
	}

	wait_for_twint_flag();
	chars[i] = TWDR;
	stop_condition();

	return 0x00;
}

void init_i2c_interface()
{
	PORTD |= (1 << 0) | (1 << 1);
	DDRD |= (1 << 0) | (1 << 1);			// PD0 and PD1 as output
	TWBR = 17;								// I²C freq = 16Mhz / (16 + 2*TWBR*4^TWPS)
	TWSR &= ~((1 << TWPS1) | (1 << TWPS0));	// Set TWPS to 0
}