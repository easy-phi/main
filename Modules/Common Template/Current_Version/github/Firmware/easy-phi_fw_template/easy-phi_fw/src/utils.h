/*
 * utils.h
 *
 * Created: 03/08/2013 21:40:21
 *  Author: limpkin
 */ 


#ifndef UTILS_H_
#define UTILS_H_

#include "defines.h"

/*! \brief Calculate week
 */
uint32_t calculate_week(uint32_t ul_year, uint32_t ul_month, uint32_t ul_day);
uint16_t hm_convert_string_to_uint16(char* string);
void epoch_to_time(uint32_t time, Time* timeptr);

#endif /* UTILS_H_ */