/*
 * peripherals_template.c
 *
 * Created: 21/10/2013 14:09:13
 *  Author: stephan
 */ 
#include <asf.h>
#include "peripherals_template.h"
#define EEPROM_ADDR  0x50

/** PWM channel instances for LEDs */
pwm_channel_t g_pwm_channel_led1;
pwm_channel_t g_pwm_channel_led2;
pwm_channel_t g_pwm_channel_led3;

/** I2C vars */
twi_options_t i2c_options;
twi_packet_t i2c_packet;


void init_i2c(void)
{	
	i2c_options.master_clk = sysclk_get_cpu_hz();
	i2c_options.speed = 400000;
	i2c_options.chip = EEPROM_ADDR;
	i2c_options.smbus = 0;
	twi_enable_master_mode(TWI0);
	twi_master_init(TWI0, &i2c_options);	
}

RET_TYPE store_bytes_at_addr(uint16_t addr, uint8_t* buffer, uint16_t nb_bytes)
{
	//!!!! TAKE CARE OF THE PAGE (not done yet)
	i2c_packet.addr[0] = addr >> 8;
	i2c_packet.addr[1] = addr;
	i2c_packet.addr_length = 2;
	i2c_packet.buffer = buffer;
	i2c_packet.length = nb_bytes;
	i2c_packet.chip = EEPROM_ADDR;
	
	while(twi_probe(TWI0, EEPROM_ADDR) != TWI_SUCCESS);
	return (RET_TYPE)twi_master_write(TWI0, &i2c_packet);
}

RET_TYPE read_bytes_at_addr(uint16_t addr, uint8_t* buffer, uint16_t nb_bytes)
{
	//!!!! TAKE CARE OF THE PAGE (not done yet)
	i2c_packet.addr[0] = addr >> 8;
	i2c_packet.addr[1] = addr;
	i2c_packet.addr_length = 2;
	i2c_packet.buffer = buffer;
	i2c_packet.length = nb_bytes;
	i2c_packet.chip = EEPROM_ADDR;
	
	while(twi_probe(TWI0, EEPROM_ADDR) != TWI_SUCCESS);
	return (RET_TYPE)twi_master_read(TWI0, &i2c_packet);
}

void init_pwm(void)
{
	/* Enable PWM peripheral clock */
	pmc_enable_periph_clk(ID_PWM);

	/* Disable PWM channels for LEDs */
	pwm_channel_disable(PWM, PIN_LED1_CHANNEL);
	pwm_channel_disable(PWM, PIN_LED2_CHANNEL);
	pwm_channel_disable(PWM, PIN_LED3_CHANNEL);

	/* Set PWM clock A as LED_PWM_FREQUENCY*PERIOD_VALUE */
	pwm_clock_t clock_setting = {
		.ul_clka = LED_PWM_FREQUENCY * LED_PWM_PERIOD_VALUE,
		.ul_clkb = sysclk_get_cpu_hz(),
		.ul_mck = sysclk_get_cpu_hz()
	};
	pwm_init(PWM, &clock_setting);

	/* Initialize PWM channel for LED1 */
	/* Period is left-aligned */
	g_pwm_channel_led1.alignment = PWM_ALIGN_LEFT;
	/* Output waveform starts at a low level */
	g_pwm_channel_led1.polarity = PWM_HIGH;
	/* Use PWM clock A as source clock */
	g_pwm_channel_led1.ul_prescaler = PWM_CMR_CPRE_CLKA;
	/* Period value of output waveform */
	g_pwm_channel_led1.ul_period = LED_PWM_PERIOD_VALUE;
	/* Duty cycle value of output waveform */
	g_pwm_channel_led1.ul_duty = LED_INIT_DUTY_VALUE;
	g_pwm_channel_led1.channel = PIN_LED1_CHANNEL;
	pwm_channel_init(PWM, &g_pwm_channel_led1);

	/* Enable channel counter event interrupt */
	pwm_channel_disable_interrupt(PWM, PIN_LED1_CHANNEL, 0);
	//pwm_channel_enable_interrupt(PWM, PIN_LED1_CHANNEL, 0);

	/* Initialize PWM channel for LED2 */
	/* Period is center-aligned */
	g_pwm_channel_led2.alignment = PWM_ALIGN_LEFT;
	/* Output waveform starts at a high level */
	g_pwm_channel_led2.polarity = PWM_HIGH;
	/* Use PWM clock A as source clock */
	g_pwm_channel_led2.ul_prescaler = PWM_CMR_CPRE_CLKA;
	/* Period value of output waveform */
	g_pwm_channel_led2.ul_period = LED_PWM_PERIOD_VALUE;
	/* Duty cycle value of output waveform */
	g_pwm_channel_led2.ul_duty = LED_INIT_DUTY_VALUE;
	g_pwm_channel_led2.channel = PIN_LED2_CHANNEL;
	pwm_channel_init(PWM, &g_pwm_channel_led2);

	/* Disable channel counter event interrupt */
	pwm_channel_disable_interrupt(PWM, PIN_LED2_CHANNEL, 0);

	/* Initialize PWM channel for LED3 */
	/* Period is center-aligned */
	g_pwm_channel_led3.alignment = PWM_ALIGN_LEFT;
	/* Output waveform starts at a high level */
	g_pwm_channel_led3.polarity = PWM_LOW;
	/* Use PWM clock A as source clock */
	g_pwm_channel_led3.ul_prescaler = PWM_CMR_CPRE_CLKA;
	/* Period value of output waveform */
	g_pwm_channel_led3.ul_period = LED_PWM_PERIOD_VALUE;
	/* Duty cycle value of output waveform */
	g_pwm_channel_led3.ul_duty = LED_INIT_DUTY_VALUE;
	g_pwm_channel_led3.channel = PIN_LED3_CHANNEL;
	pwm_channel_init(PWM, &g_pwm_channel_led3);

	/* Disable channel counter event interrupt */
	pwm_channel_disable_interrupt(PWM, PIN_LED3_CHANNEL, 0);
	
	/* Enable PWM channels for LEDs */
	pwm_channel_enable(PWM, PIN_LED1_CHANNEL);
	pwm_channel_enable(PWM, PIN_LED2_CHANNEL);
	pwm_channel_enable(PWM, PIN_LED3_CHANNEL);
}

// Set user led colour
void set_user_led_colour(uint16_t red_val, uint16_t green_val, uint16_t blue_val)
{
	pwm_channel_update_duty(PWM, &g_pwm_channel_led1, red_val);
	pwm_channel_update_duty(PWM, &g_pwm_channel_led2, blue_val);
	pwm_channel_update_duty(PWM, &g_pwm_channel_led3, green_val);
}

// To see if the user pressed the front panel button
RET_TYPE get_user_button_status(void)
{
	if(ioport_get_pin_level(GPIO_USR_BUTTON))
		return RETURN_NOK;
	else
		return RETURN_OK;
}

// Sync signal status
RET_TYPE get_sync_signal_status(void)
{
	if(ioport_get_pin_level(GPIO_SYNC))
		return RETURN_OK;
	else
		return RETURN_NOK;
}

// See if 12v is present on the connector
RET_TYPE get_12v_status(void)
{
	if(ioport_get_pin_level(GPIO_DET_12V))
		return RETURN_OK;
	else
		return RETURN_NOK;
}

// Is 12v on the board ok
RET_TYPE get_ok_12v_status(void)
{
	if(ioport_get_pin_level(GPIO_OK_12V))
		return RETURN_OK;
	else
		return RETURN_NOK;
}

// Enable 12v
void enable_12v(void)
{
	ioport_set_pin_level(ENABLE_12V_GPIO, ENABLE_12V_ACTIVE_LEVEL);
}

// Disable 12v
void disable_12v(void)
{
	ioport_set_pin_level(ENABLE_12V_GPIO, ENABLE_12V_INACTIVE_LEVEL);
}