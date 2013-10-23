/*
 * rtc.h
 *
 * Created: 03/05/2012 19:13:16
 *  Author: limpkin
 */ 


#ifndef RTC_H_
#define RTC_H_

unsigned char write_nb_chars(unsigned char addr, unsigned char nb_chars, unsigned char* chars);
unsigned char read_nb_chars(unsigned char addr, unsigned char nb_chars, unsigned char* chars);
unsigned char get_next_eeprom_byte(unsigned char is_last_byte);
unsigned char initiate_eeprom_read(unsigned char addr);
unsigned char default_get_temperature(unsigned int* result_ptr);
void wait_for_twr(unsigned char addr);
void init_i2c_interface();

#endif /* RTC_H_ */