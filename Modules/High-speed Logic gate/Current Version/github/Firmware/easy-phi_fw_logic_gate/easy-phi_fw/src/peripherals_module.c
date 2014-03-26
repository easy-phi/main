/*
 * peripherals_module.c
 *
 * Created: 15/11/2013 14:47:11
 *  Author: stephan
 */ 

#include "peripherals_template.h"
#include "peripherals_module.h"
#include "defines.h"
#define IN_CH1_LED1_GPIO				(PIO_PA6_IDX)
#define IN_CH1_LED1_PIO_ID				ID_PIOA
#define IN_CH1_LED2_GPIO				(PIO_PA3_IDX)
#define IN_CH1_LED2_PIO_ID				ID_PIOA
#define IN_CH1_LED3_GPIO				(PIO_PA4_IDX)
#define IN_CH1_LED3_PIO_ID				ID_PIOA
#define IN_CH2_LED1_GPIO				(PIO_PB13_IDX)
#define IN_CH2_LED1_PIO_ID				ID_PIOB
#define IN_CH2_LED2_GPIO				(PIO_PB18_IDX)
#define IN_CH2_LED2_PIO_ID				ID_PIOB
#define IN_CH2_LED3_GPIO				(PIO_PB17_IDX)
#define IN_CH2_LED3_PIO_ID				ID_PIOB
#define OUT_CH1_LED1_GPIO				(PIO_PB21_IDX)
#define OUT_CH1_LED1_PIO_ID				ID_PIOB
#define OUT_CH1_LED2_GPIO				(PIO_PC12_IDX)
#define OUT_CH1_LED2_PIO_ID				ID_PIOC
#define OUT_CH1_LED3_GPIO				(PIO_PC11_IDX)
#define OUT_CH1_LED3_PIO_ID				ID_PIOC
#define OUT_CH2_LED1_GPIO				(PIO_PC15_IDX)
#define OUT_CH2_LED1_PIO_ID				ID_PIOC
#define OUT_CH2_LED2_GPIO				(PIO_PC17_IDX)
#define OUT_CH2_LED2_PIO_ID				ID_PIOC
#define OUT_CH2_LED3_GPIO				(PIO_PC16_IDX)
#define OUT_CH2_LED3_PIO_ID				ID_PIOC
#define OUT_CH3_LED1_GPIO				(PIO_PB1_IDX)
#define OUT_CH3_LED1_PIO_ID				ID_PIOB
#define OUT_CH3_LED2_GPIO				(PIO_PC4_IDX)
#define OUT_CH3_LED2_PIO_ID				ID_PIOC
#define OUT_CH3_LED3_GPIO				(PIO_PB2_IDX)
#define OUT_CH3_LED3_PIO_ID				ID_PIOB
#define OUT_CH4_LED1_GPIO				(PIO_PB4_IDX)
#define OUT_CH4_LED1_PIO_ID				ID_PIOB
#define OUT_CH4_LED2_GPIO				(PIO_PB6_IDX)
#define OUT_CH4_LED2_PIO_ID				ID_PIOB
#define OUT_CH4_LED3_GPIO				(PIO_PB5_IDX)
#define OUT_CH4_LED3_PIO_ID				ID_PIOB
#define CH1_IN_PULSE_GPIO				(PIO_PA2_IDX)
#define CH1_IN_PULSE_PIO_ID				ID_PIOA
#define CH2_IN_PULSE_GPIO				(PIO_PB19_IDX)
#define CH2_IN_PULSE_PIO_ID				ID_PIOB
#define CH1_OUT_PULSE_GPIO				(PIO_PC13_IDX)
#define CH1_OUT_PULSE_PIO_ID			ID_PIOC
#define CH2_OUT_PULSE_GPIO				(PIO_PC18_IDX)
#define CH2_OUT_PULSE_PIO_ID			ID_PIOC
#define CH3_OUT_PULSE_GPIO				(PIO_PC10_IDX)
#define CH3_OUT_PULSE_PIO_ID			ID_PIOC
#define CH4_OUT_PULSE_GPIO				(PIO_PC20_IDX)
#define CH4_OUT_PULSE_PIO_ID			ID_PIOC
#define CH1_IN_STATE_GPIO				(PIO_PB12_IDX)
#define CH1_IN_STATE_PIO_ID				ID_PIOB
#define CH2_IN_STATE_GPIO				(PIO_PB20_IDX)
#define CH2_IN_STATE_PIO_ID				ID_PIOB
#define CH1_OUT_STATE_GPIO				(PIO_PC14_IDX)
#define CH1_OUT_STATE_PIO_ID			ID_PIOC
#define CH2_OUT_STATE_GPIO				(PIO_PC19_IDX)
#define CH2_OUT_STATE_PIO_ID			ID_PIOC
#define CH3_OUT_STATE_GPIO				(PIO_PB12_IDX)
#define CH3_OUT_STATE_PIO_ID			ID_PIOB
#define CH4_OUT_STATE_GPIO				(PIO_PC21_IDX)
#define CH4_OUT_STATE_PIO_ID			ID_PIOC
#define CH_PULSE_RES_GPIO				(PIO_PC22_IDX)
#define CH_PULSE_RES_PIO_ID				ID_PIOC
#define XPOINT_SIN0_GPIO				(PIO_PA26_IDX)
#define XPOINT_SIN0_PIO_ID				ID_PIOA
#define XPOINT_SIN1_GPIO				(PIO_PA25_IDX)
#define XPOINT_SIN1_PIO_ID				ID_PIOA
#define XPOINT_SOUT0_GPIO				(PIO_PA29_IDX)
#define XPOINT_SOUT0_PIO_ID				ID_PIOA
#define XPOINT_SOUT1_GPIO				(PIO_PB0_IDX)
#define XPOINT_SOUT1_PIO_ID				ID_PIOB
#define XPOINT_LOAD_GPIO				(PIO_PA27_IDX)
#define XPOINT_LOAD_PIO_ID				ID_PIOA
#define XPOINT_CONFIG_GPIO				(PIO_PA28_IDX)
#define XPOINT_CONFIG_PIO_ID			ID_PIOA

/* Color for leds */
uint16_t ch1_in_color = GREEN;
uint16_t ch2_in_color = GREEN;
uint16_t ch1_out_color = BLUE;
uint16_t ch2_out_color = BLUE;
uint16_t ch3_out_color = BLUE;
uint16_t ch4_out_color = BLUE;
/* Crosspoint configuration */
uint16_t xpoint_conf[3];


void led_activity_routine(void)
{
	#ifdef TEST_FW
		ioport_toggle_pin_level(IN_CH1_LED1_GPIO);
		ioport_toggle_pin_level(IN_CH1_LED2_GPIO);
		ioport_toggle_pin_level(IN_CH1_LED3_GPIO);
		ioport_toggle_pin_level(IN_CH2_LED1_GPIO);
		ioport_toggle_pin_level(IN_CH2_LED2_GPIO);
		ioport_toggle_pin_level(IN_CH2_LED3_GPIO);
		ioport_toggle_pin_level(OUT_CH1_LED1_GPIO);
		ioport_toggle_pin_level(OUT_CH1_LED2_GPIO);
		ioport_toggle_pin_level(OUT_CH1_LED3_GPIO);
		ioport_toggle_pin_level(OUT_CH2_LED1_GPIO);
		ioport_toggle_pin_level(OUT_CH2_LED2_GPIO);
		ioport_toggle_pin_level(OUT_CH2_LED3_GPIO);
		ioport_toggle_pin_level(OUT_CH3_LED1_GPIO);
		ioport_toggle_pin_level(OUT_CH3_LED2_GPIO);
		ioport_toggle_pin_level(OUT_CH3_LED3_GPIO);
		ioport_toggle_pin_level(OUT_CH4_LED1_GPIO);
		ioport_toggle_pin_level(OUT_CH4_LED2_GPIO);
		ioport_toggle_pin_level(OUT_CH4_LED3_GPIO);
		return;
	#endif
	
	if(get_ok_12v_status() == RETURN_NOK)
	{
		ioport_set_pin_level(IN_CH1_LED1_GPIO, IOPORT_PIN_LEVEL_HIGH);
		ioport_set_pin_level(IN_CH1_LED2_GPIO, IOPORT_PIN_LEVEL_HIGH);
		ioport_set_pin_level(IN_CH1_LED3_GPIO, IOPORT_PIN_LEVEL_HIGH);
		ioport_set_pin_level(IN_CH2_LED1_GPIO, IOPORT_PIN_LEVEL_HIGH);
		ioport_set_pin_level(IN_CH2_LED2_GPIO, IOPORT_PIN_LEVEL_HIGH);
		ioport_set_pin_level(IN_CH2_LED3_GPIO, IOPORT_PIN_LEVEL_HIGH);
		return;
	}
	
	if(ioport_get_pin_level(CH1_IN_PULSE_GPIO))
	{		
		if(ch1_in_color == GREEN)
		{
			ioport_toggle_pin_level(IN_CH1_LED3_GPIO);
		}
		else if(ch1_in_color == BLUE)
		{
			ioport_toggle_pin_level(IN_CH1_LED2_GPIO);
		}
		else if(ch1_in_color == RED)
		{
			ioport_toggle_pin_level(IN_CH1_LED1_GPIO);
		}
	}
	else
	{
		ioport_set_pin_level(IN_CH1_LED1_GPIO, IOPORT_PIN_LEVEL_HIGH);
		ioport_set_pin_level(IN_CH1_LED2_GPIO, IOPORT_PIN_LEVEL_HIGH);
		ioport_set_pin_level(IN_CH1_LED3_GPIO, IOPORT_PIN_LEVEL_HIGH);
	}
	
	if(ioport_get_pin_level(CH2_IN_PULSE_GPIO))
	{		
		if(ch2_in_color == GREEN)
		{
			ioport_toggle_pin_level(IN_CH2_LED3_GPIO);
		}
		else if(ch2_in_color == BLUE)
		{
			ioport_toggle_pin_level(IN_CH2_LED2_GPIO);
		}
		else if(ch2_in_color == RED)
		{
			ioport_toggle_pin_level(IN_CH2_LED1_GPIO);
		}
	}
	else
	{
		ioport_set_pin_level(IN_CH2_LED1_GPIO, IOPORT_PIN_LEVEL_HIGH);
		ioport_set_pin_level(IN_CH2_LED2_GPIO, IOPORT_PIN_LEVEL_HIGH);
		ioport_set_pin_level(IN_CH2_LED3_GPIO, IOPORT_PIN_LEVEL_HIGH);
	}
	
	if(ioport_get_pin_level(CH1_OUT_PULSE_GPIO))
	{		
		if(ch1_out_color == GREEN)
		{
			ioport_toggle_pin_level(OUT_CH1_LED3_GPIO);
		}
		else if(ch1_out_color == BLUE)
		{
			ioport_toggle_pin_level(OUT_CH1_LED2_GPIO);
		}
		else if(ch1_out_color == RED)
		{
			ioport_toggle_pin_level(OUT_CH1_LED1_GPIO);
		}
	}
	else
	{
		ioport_set_pin_level(OUT_CH1_LED1_GPIO, IOPORT_PIN_LEVEL_HIGH);
		ioport_set_pin_level(OUT_CH1_LED2_GPIO, IOPORT_PIN_LEVEL_HIGH);
		ioport_set_pin_level(OUT_CH1_LED3_GPIO, IOPORT_PIN_LEVEL_HIGH);
	}
	
	if(ioport_get_pin_level(CH2_OUT_PULSE_GPIO))
	{
		if(ch2_out_color == GREEN)
		{
			ioport_toggle_pin_level(OUT_CH2_LED3_GPIO);
		}
		else if(ch2_out_color == BLUE)
		{
			ioport_toggle_pin_level(OUT_CH2_LED2_GPIO);
		}
		else if(ch2_out_color == RED)
		{
			ioport_toggle_pin_level(OUT_CH2_LED1_GPIO);
		}
	}
	else
	{
		ioport_set_pin_level(OUT_CH2_LED1_GPIO, IOPORT_PIN_LEVEL_HIGH);
		ioport_set_pin_level(OUT_CH2_LED2_GPIO, IOPORT_PIN_LEVEL_HIGH);
		ioport_set_pin_level(OUT_CH2_LED3_GPIO, IOPORT_PIN_LEVEL_HIGH);
	}
	
	if(ioport_get_pin_level(CH3_OUT_PULSE_GPIO))
	{
		if(ch3_out_color == GREEN)
		{
			ioport_toggle_pin_level(OUT_CH3_LED3_GPIO);
		}
		else if(ch3_out_color == BLUE)
		{
			ioport_toggle_pin_level(OUT_CH3_LED2_GPIO);
		}
		else if(ch3_out_color == RED)
		{
			ioport_toggle_pin_level(OUT_CH3_LED1_GPIO);
		}
	}
	else
	{
		ioport_set_pin_level(OUT_CH3_LED1_GPIO, IOPORT_PIN_LEVEL_HIGH);
		ioport_set_pin_level(OUT_CH3_LED2_GPIO, IOPORT_PIN_LEVEL_HIGH);
		ioport_set_pin_level(OUT_CH3_LED3_GPIO, IOPORT_PIN_LEVEL_HIGH);
	}
	
	if(ioport_get_pin_level(CH4_OUT_PULSE_GPIO))
	{
		if(ch3_out_color == GREEN)
		{
			ioport_toggle_pin_level(OUT_CH4_LED3_GPIO);
		}
		else if(ch3_out_color == BLUE)
		{
			ioport_toggle_pin_level(OUT_CH4_LED2_GPIO);
		}
		else if(ch3_out_color == RED)
		{
			ioport_toggle_pin_level(OUT_CH4_LED1_GPIO);
		}
	}
	else
	{
		ioport_set_pin_level(OUT_CH4_LED1_GPIO, IOPORT_PIN_LEVEL_HIGH);
		ioport_set_pin_level(OUT_CH4_LED2_GPIO, IOPORT_PIN_LEVEL_HIGH);
		ioport_set_pin_level(OUT_CH4_LED3_GPIO, IOPORT_PIN_LEVEL_HIGH);
	}
	
	ioport_set_pin_level(CH_PULSE_RES_GPIO, IOPORT_PIN_LEVEL_HIGH);
	asm("NOP");asm("NOP");asm("NOP");
	ioport_set_pin_level(CH_PULSE_RES_GPIO, IOPORT_PIN_LEVEL_LOW);
}

/**
 * \brief TC0,1 overflow interrupt
 */
void TC1_Handler(void)
{
	tc_get_status(TC0, 1);
	led_activity_routine();
}

uint16_t get_xpoint_outchannel_conf(uint16_t channel)
{
	if(channel > 3)
		return 0;
	else
		return xpoint_conf[channel];
}

void configure_crosspoint(uint16_t in, uint16_t out)
{
	if((in > 3) || (out > 3))
		return;
		
	xpoint_conf[out] = in;
		
	if(in & 0x0001)
		ioport_set_pin_level(XPOINT_SIN0_GPIO, IOPORT_PIN_LEVEL_HIGH);
	else
		ioport_set_pin_level(XPOINT_SIN0_GPIO, IOPORT_PIN_LEVEL_LOW);
	if(in & 0x0002)
		ioport_set_pin_level(XPOINT_SIN1_GPIO, IOPORT_PIN_LEVEL_HIGH);
	else
		ioport_set_pin_level(XPOINT_SIN1_GPIO, IOPORT_PIN_LEVEL_LOW);
		
	if(out & 0x0001)
		ioport_set_pin_level(XPOINT_SOUT0_GPIO, IOPORT_PIN_LEVEL_HIGH);
	else
		ioport_set_pin_level(XPOINT_SOUT0_GPIO, IOPORT_PIN_LEVEL_LOW);
	if(out & 0x0002)
		ioport_set_pin_level(XPOINT_SOUT1_GPIO, IOPORT_PIN_LEVEL_HIGH);
	else
		ioport_set_pin_level(XPOINT_SOUT1_GPIO, IOPORT_PIN_LEVEL_LOW);
	
	asm("NOP");asm("NOP");
	ioport_set_pin_level(XPOINT_LOAD_GPIO, IOPORT_PIN_LEVEL_HIGH);
	asm("NOP");asm("NOP");
	ioport_set_pin_level(XPOINT_LOAD_GPIO, IOPORT_PIN_LEVEL_LOW);
	asm("NOP");asm("NOP");
	ioport_set_pin_level(XPOINT_CONFIG_GPIO, IOPORT_PIN_LEVEL_HIGH);
	asm("NOP");asm("NOP");
	ioport_set_pin_level(XPOINT_CONFIG_GPIO, IOPORT_PIN_LEVEL_LOW);
	asm("NOP");asm("NOP");
}

// Here should be all the initialization functions for the module before 12v power
void init_module_peripherals_bp(void)
{
	/* LEDs IO */
	pmc_enable_periph_clk(IN_CH1_LED1_PIO_ID);
	ioport_set_pin_dir(IN_CH1_LED1_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(IN_CH1_LED1_GPIO, IOPORT_PIN_LEVEL_HIGH);
	pmc_enable_periph_clk(IN_CH1_LED2_PIO_ID);
	ioport_set_pin_dir(IN_CH1_LED2_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(IN_CH1_LED2_GPIO, IOPORT_PIN_LEVEL_HIGH);
	pmc_enable_periph_clk(IN_CH1_LED3_PIO_ID);
	ioport_set_pin_dir(IN_CH1_LED3_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(IN_CH1_LED3_GPIO, IOPORT_PIN_LEVEL_HIGH);

	pmc_enable_periph_clk(IN_CH2_LED1_PIO_ID);
	ioport_set_pin_dir(IN_CH2_LED1_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(IN_CH2_LED1_GPIO, IOPORT_PIN_LEVEL_HIGH);
	pmc_enable_periph_clk(IN_CH2_LED2_PIO_ID);
	ioport_set_pin_dir(IN_CH2_LED2_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(IN_CH2_LED2_GPIO, IOPORT_PIN_LEVEL_HIGH);
	pmc_enable_periph_clk(IN_CH2_LED3_PIO_ID);
	ioport_set_pin_dir(IN_CH2_LED3_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(IN_CH2_LED3_GPIO, IOPORT_PIN_LEVEL_HIGH);

	pmc_enable_periph_clk(OUT_CH1_LED1_PIO_ID);
	ioport_set_pin_dir(OUT_CH1_LED1_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(OUT_CH1_LED1_GPIO, IOPORT_PIN_LEVEL_HIGH);
	pmc_enable_periph_clk(OUT_CH1_LED2_PIO_ID);
	ioport_set_pin_dir(OUT_CH1_LED2_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(OUT_CH1_LED2_GPIO, IOPORT_PIN_LEVEL_HIGH);
	pmc_enable_periph_clk(OUT_CH1_LED3_PIO_ID);
	ioport_set_pin_dir(OUT_CH1_LED3_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(OUT_CH1_LED3_GPIO, IOPORT_PIN_LEVEL_HIGH);

	pmc_enable_periph_clk(OUT_CH2_LED1_PIO_ID);
	ioport_set_pin_dir(OUT_CH2_LED1_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(OUT_CH2_LED1_GPIO, IOPORT_PIN_LEVEL_HIGH);
	pmc_enable_periph_clk(OUT_CH2_LED2_PIO_ID);
	ioport_set_pin_dir(OUT_CH2_LED2_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(OUT_CH2_LED2_GPIO, IOPORT_PIN_LEVEL_HIGH);
	pmc_enable_periph_clk(OUT_CH2_LED3_PIO_ID);
	ioport_set_pin_dir(OUT_CH2_LED3_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(OUT_CH2_LED3_GPIO, IOPORT_PIN_LEVEL_HIGH);

	pmc_enable_periph_clk(OUT_CH3_LED1_PIO_ID);
	ioport_set_pin_dir(OUT_CH3_LED1_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(OUT_CH3_LED1_GPIO, IOPORT_PIN_LEVEL_HIGH);
	pmc_enable_periph_clk(OUT_CH3_LED2_PIO_ID);
	ioport_set_pin_dir(OUT_CH3_LED2_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(OUT_CH3_LED2_GPIO, IOPORT_PIN_LEVEL_HIGH);
	pmc_enable_periph_clk(OUT_CH3_LED3_PIO_ID);
	ioport_set_pin_dir(OUT_CH3_LED3_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(OUT_CH3_LED3_GPIO, IOPORT_PIN_LEVEL_HIGH);

	pmc_enable_periph_clk(OUT_CH4_LED1_PIO_ID);
	ioport_set_pin_dir(OUT_CH4_LED1_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(OUT_CH4_LED1_GPIO, IOPORT_PIN_LEVEL_HIGH);
	pmc_enable_periph_clk(OUT_CH4_LED2_PIO_ID);
	ioport_set_pin_dir(OUT_CH4_LED2_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(OUT_CH4_LED2_GPIO, IOPORT_PIN_LEVEL_HIGH);
	pmc_enable_periph_clk(OUT_CH4_LED3_PIO_ID);
	ioport_set_pin_dir(OUT_CH4_LED3_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(OUT_CH4_LED3_GPIO, IOPORT_PIN_LEVEL_HIGH);
	
	/* Pulse inputs/state & d reset output */
	pmc_enable_periph_clk(CH1_IN_PULSE_PIO_ID);
	ioport_set_pin_dir(CH1_IN_PULSE_GPIO, IOPORT_DIR_INPUT);
	pmc_enable_periph_clk(CH2_IN_PULSE_PIO_ID);
	ioport_set_pin_dir(CH2_IN_PULSE_GPIO, IOPORT_DIR_INPUT);
	pmc_enable_periph_clk(CH1_OUT_PULSE_PIO_ID);
	ioport_set_pin_dir(CH1_OUT_PULSE_GPIO, IOPORT_DIR_INPUT);
	pmc_enable_periph_clk(CH2_OUT_PULSE_PIO_ID);
	ioport_set_pin_dir(CH2_OUT_PULSE_GPIO, IOPORT_DIR_INPUT);
	pmc_enable_periph_clk(CH3_OUT_PULSE_PIO_ID);
	ioport_set_pin_dir(CH3_OUT_PULSE_GPIO, IOPORT_DIR_INPUT);
	pmc_enable_periph_clk(CH4_OUT_PULSE_PIO_ID);
	ioport_set_pin_dir(CH4_OUT_PULSE_GPIO, IOPORT_DIR_INPUT);

	pmc_enable_periph_clk(CH1_IN_STATE_PIO_ID);
	ioport_set_pin_dir(CH1_IN_STATE_GPIO, IOPORT_DIR_INPUT);
	pmc_enable_periph_clk(CH2_IN_STATE_PIO_ID);
	ioport_set_pin_dir(CH2_IN_STATE_GPIO, IOPORT_DIR_INPUT);
	pmc_enable_periph_clk(CH1_OUT_STATE_PIO_ID);
	ioport_set_pin_dir(CH1_OUT_STATE_GPIO, IOPORT_DIR_INPUT);
	pmc_enable_periph_clk(CH2_OUT_STATE_PIO_ID);
	ioport_set_pin_dir(CH2_OUT_STATE_GPIO, IOPORT_DIR_INPUT);
	pmc_enable_periph_clk(CH3_OUT_STATE_PIO_ID);
	ioport_set_pin_dir(CH3_OUT_STATE_GPIO, IOPORT_DIR_INPUT);
	pmc_enable_periph_clk(CH4_OUT_STATE_PIO_ID);
	ioport_set_pin_dir(CH4_OUT_STATE_GPIO, IOPORT_DIR_INPUT);

	pmc_enable_periph_clk(CH_PULSE_RES_PIO_ID);
	ioport_set_pin_dir(CH_PULSE_RES_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(CH_PULSE_RES_GPIO, IOPORT_PIN_LEVEL_LOW);
	
	/* Crosspoint config */
	pmc_enable_periph_clk(XPOINT_SIN0_PIO_ID);
	ioport_set_pin_dir(XPOINT_SIN0_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(XPOINT_SIN0_GPIO, IOPORT_PIN_LEVEL_LOW);
	pmc_enable_periph_clk(XPOINT_SIN1_PIO_ID);
	ioport_set_pin_dir(XPOINT_SIN1_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(XPOINT_SIN1_GPIO, IOPORT_PIN_LEVEL_LOW);
	pmc_enable_periph_clk(XPOINT_SOUT0_PIO_ID);
	ioport_set_pin_dir(XPOINT_SOUT0_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(XPOINT_SOUT0_GPIO, IOPORT_PIN_LEVEL_LOW);
	pmc_enable_periph_clk(XPOINT_SOUT1_PIO_ID);
	ioport_set_pin_dir(XPOINT_SOUT1_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(XPOINT_SOUT1_GPIO, IOPORT_PIN_LEVEL_LOW);
	pmc_enable_periph_clk(XPOINT_LOAD_PIO_ID);
	ioport_set_pin_dir(XPOINT_LOAD_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(XPOINT_LOAD_GPIO, IOPORT_PIN_LEVEL_LOW);
	pmc_enable_periph_clk(XPOINT_CONFIG_PIO_ID);
	ioport_set_pin_dir(XPOINT_CONFIG_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(XPOINT_CONFIG_GPIO, IOPORT_PIN_LEVEL_LOW);
	
	// Init LED interrupt,
	uint32_t ul_div;
	uint32_t ul_tcclks;
	/* Get system clock. */
	uint32_t ul_sysclk = sysclk_get_cpu_hz();
	/* Configure PMC. */
	pmc_enable_periph_clk(ID_TC1);
	/* Configure TC for a TC_FREQ frequency and trigger on RC compare. */
	tc_find_mck_divisor(20, ul_sysclk, &ul_div, &ul_tcclks, ul_sysclk);
	tc_init(TC0, 1, ul_tcclks | TC_CMR_CPCTRG);
	tc_write_rc(TC0, 1, (ul_sysclk / ul_div) / 20);
	/* Configure and enable interrupt on RC compare. */
	tc_start(TC0, 1);
	NVIC_DisableIRQ(TC1_IRQn);
	NVIC_ClearPendingIRQ(TC1_IRQn);
	//NVIC_SetPriority(TC1_IRQn, 0);
	NVIC_EnableIRQ((IRQn_Type)ID_TC1);
	tc_enable_interrupt(TC0, 1, TC_IER_CPCS);
}

// Here should be all the initialization functions for the module after 12v power
void init_module_peripherals_ap(void)
{
	
}

// Here should be all the deinitialization functions for the module before 12v power removal
void deinit_module_peripherals(void)
{
	
}

