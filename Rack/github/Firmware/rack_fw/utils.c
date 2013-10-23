/*
 * utils.c
 *
 * Created: 03/05/2012 18:59:28
 *  Author: limpkin
 */ 


/*!	\fn		string_to_char(char* string, unsigned char delimiter)
*	\brief	Convert a string to a its char value
*	\param	string		The string
*	\param	delimiter	The delimiter
*	\return char value
*/
unsigned char string_to_char(char* string, unsigned char delimiter)
{
	unsigned char result = 0;
	unsigned char i;
	
	for(i = 0; i < 3; i++)
	{
		if(string[i] == delimiter)
		{
			return result;
		}
		else if((i == 2) && (result > 25))
		{
			return 0xFF;
		}
		else if((i == 2) && (result == 25) && (string[i] > 0x35))
		{
			return 0xFF;
		}
		else if((string[i] < 0x30) || (string[i] > 0x39))
		{
			return result;
		}
		else
		{
			result = result * 10;
			result += (string[i] - 0x30);
		}
	}
	
	return result;
}

/*!	\fn		numchar_to_char(unsigned char c)
*	\brief	Convert a char value (0 to 9) to be displayed
*	\param	c	The char
*	\return Displayable char
*/
char numchar_to_char(unsigned char c)
{
	return c + 0x30;
}

/*!	\fn		hexachar_to_string(unsigned char c, char* string)
*	\brief	Convert a char to a string that we can display
*	\param	c	The char
*	\param	string	Pointer to the string in which we will write
*/
void hexachar_to_string(unsigned char c, char* string)
{
	unsigned char temp = c & 0x0F;

	if(temp > 9)
		string[1] = temp + 0x37;
	else
		string[1] = temp + 0x30;

	temp = (c >> 4) & 0x0F;

	if(temp > 9)
		string[0] = temp + 0x37;
	else
		string[0] = temp + 0x30;

	string[2] = 0x00;
}

/*!	\fn		hexaint_to_string(unsigned int c, char* string)
*	\brief	Convert a int to a string that we can display
*	\param	c	The int
*	\param	string	Pointer to the string in which we will write
*/
void hexaint_to_string(unsigned int c, char* string)
{
	unsigned char temp = (unsigned char)(c >> 8);
	hexachar_to_string(temp, string);
	temp = (unsigned char)(c & 0x00FF);	
	hexachar_to_string(temp, string + 2);
}

/*!	\fn		chr_strlen(char* string)	
*	\brief	Ported strlen function
*	\param	string	The string
*	\return The length
*/
unsigned char chr_strlen(char* string)						// A light version of strlen()
{
	unsigned char i = 0;

	while(*string++)
		i++;

	return i;
}

/*!	\fn		int_strlen(char* string)	
*	\brief	Ported strlen function
*	\param	string	The string
*	\return The length
*/
unsigned int int_strlen(char* string)						// A light version of strlen()
{
	unsigned int i = 0;

	while(*string++)
		i++;

	return i;
}

/*!	\fn		clear_string(char* string, int nb_char)
*	\brief	Clear a string
*	\param	string	The string
*	\param	nb_char	Length
*/
void clear_string(char* string, int nb_char)
{
	unsigned int i;

	for(i = 0; i < nb_char; i++)
		string[i] = 0;
}

/*!	\fn		str_cpy(char* source, char* dest, int nb_char)
*	\brief	Ported strcpy() with the args inverted
*	\param	source	Pointer to the source
*	\param	dest	Pointer to the destination
*	\param	nb_char	Number of chars to copy
*/
void str_ncpy(char* source, char* dest, int nb_char)
{
	unsigned int i;

	for(i = 0; i < nb_char; i++)
		dest[i] = source[i];
}

/*!	\fn		char_to_string(unsigned char value, char* string)
*	\brief	Convert a char to a string that we can display (numerical form)
*	\param	value	The char
*	\param	string	Pointer to the string in which we will write
*/
void char_to_string(unsigned char value, char* string)
{
	unsigned char index = 0;
	unsigned char div_rest;
	unsigned char temp;
	unsigned char i;

	if(value == 0)
	{
		string[index] = numchar_to_char(0);
		string[index + 1] = 0x00;
		return;
	}

	while(value != 0)
	{
		div_rest = (unsigned char)(value % 10);
		value = value / 10;
		string[index++] = numchar_to_char(div_rest);
	}

	for(i = 0; i < (index >> 1); i++)
	{
		temp = string[i];
		string[i] = string[index - i - 1];
		string[index - i - 1] = temp;
	}

	string[index] = 0x00;
}

/*!	\fn		int_to_string(unsigned int value, char* string)
*	\brief	Convert a int to a string that we can display (numerical form)
*	\param	value	The int
*	\param	string	Pointer to the string in which we will write
*/
void int_to_string(unsigned int value, char* string)
{
	unsigned char index = 0;
	unsigned char div_rest;
	unsigned char temp;
	unsigned char i;

	if(value == 0)
	{
		string[index] = numchar_to_char(0);
		string[index + 1] = 0x00;
		return;
	}

	while(value != 0)
	{
		div_rest = (unsigned char)(value % 10);
		value = value / 10;
		string[index++] = numchar_to_char(div_rest);
	}

	for(i = 0; i < (index >> 1); i++)
	{
		temp = string[i];
		string[i] = string[index - i - 1];
		string[index - i - 1] = temp;
	}

	string[index] = 0x00;
}