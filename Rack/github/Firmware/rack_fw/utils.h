/*
 * utils.h
 *
 * Created: 03/05/2012 18:59:04
 *  Author: limpkin
 */ 


#ifndef UTILS_H_
#define UTILS_H_

unsigned char string_to_char(char* string, unsigned char delimiter);
void hexachar_to_string(unsigned char c, char* string);
void char_to_string(unsigned char value, char* string);
void hexaint_to_string(unsigned int c, char* string);
void int_to_string(unsigned int value, char* string);
void str_ncpy(char* source, char* dest, int nb_char);
void clear_string(char* string, int nb_char);
unsigned char chr_strlen(char* string);
unsigned int int_strlen(char* string);
char numchar_to_char(unsigned char c);

#endif /* UTILS_H_ */