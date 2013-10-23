/*
 * defines.h
 *
 * Created: 30/07/2013 17:01:15
 *  Author: stephan
 */ 


#ifndef DEFINES_H_
#define DEFINES_H_

// Defines controleur I2C
#define I2C_START		0x08
#define	I2C_RSTART		0x10
#define I2C_SLA_ACK		0x18
#define I2C_SLA_NACK	0x20
#define I2C_SLAR_ACK	0x40
#define I2C_SLAR_NACK	0x48
#define	I2C_DATA_ACK	0x28
#define	I2C_DATA_NACK	0x30
#define	I2C_DATAR_ACK	0x50
#define	I2C_DATAR_NACK	0x58
#define	I2C_ARB_ERROR	0x38

// Defines erreurs I2C
#define	I2C_START_ERROR	0x05
#define	I2C_SLA_ERROR	0x06
#define	I2C_DATA_ERROR	0x07
#define	I2C_RSTART_ERR	0x08
#define	I2C_SLAR_ERROR	0x09

#define FALSE			0
#define TRUE			(!FALSE)

// Define averaging
#define AVG_SIZE		16
#define AVG_DIV_SHIFT	4

// Defines channels
#define CHANNEL_CURRENT	0
#define CHANNEL_5V_MON	1
#define CHANNEL_12V		2
#define CHANNEL_3V		3
#define CHANNEL_5V		4

#endif /* DEFINES_H_ */