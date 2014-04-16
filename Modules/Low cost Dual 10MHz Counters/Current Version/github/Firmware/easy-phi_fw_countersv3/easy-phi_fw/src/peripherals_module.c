/*
 * peripherals_module.c
 *
 * Created: 15/11/2013 14:47:11
 *  Author: stephan
 */ 

#include "peripherals_module.h"
#include "defines.h"

/** Module pins definitions */
#define PIN_COUNTERIN_GPIO				PIO_PC27_IDX
#define PIN_COUNTERIN_FLAGS				(IOPORT_MODE_MUX_B)
#define PIN_COUNTER2_GPIO				PIO_PA4_IDX
#define PIN_COUNTER2_FLAGS				(IOPORT_MODE_MUX_A)
#define PIN_COUNTER1_GPIO				PIO_PB16_IDX
#define PIN_COUNTER1_FLAGS				(IOPORT_MODE_MUX_A)
#define MHZ_10_IN_GPIO					(PIO_PC29_IDX)
#define MHZ_10_IN_FLAGS					(IOPORT_MODE_MUX_B)
#define MHZ_10_IN_PIO_ID				ID_PIOC
#define MUX_IN_FLAGS					(IOPORT_MODE_MUX_B)
#define MUX_IN_GPIO						(PIO_PC29_IDX)
#define MUX_IN_PIO_ID					ID_PIOC
#define MUX_SEL_GPIO					(PIO_PB22_IDX)
#define MUX_SEL_PIO_ID					ID_PIOB
#define TTL_D_NRESET_GPIO				(PIO_PA10_IDX)
#define TTL_D_NRESET_PIO_ID				ID_PIOA
#define TTL_IN_GPIO						(PIO_PC28_IDX)
#define TTL_IN_PIO_ID					ID_PIOC
#define IN_CH1_LED1_GPIO				(PIO_PC5_IDX)
#define IN_CH1_LED1_PIO_ID				ID_PIOC
#define IN_CH1_LED2_GPIO				(PIO_PC7_IDX)
#define IN_CH1_LED2_PIO_ID				ID_PIOC
#define IN_CH1_LED3_GPIO				(PIO_PC9_IDX)
#define IN_CH1_LED3_PIO_ID				ID_PIOC
#define IN_CH2_LED1_GPIO				(PIO_PC21_IDX)
#define IN_CH2_LED1_PIO_ID				ID_PIOC
#define IN_CH2_LED2_GPIO				(PIO_PC22_IDX)
#define IN_CH2_LED2_PIO_ID				ID_PIOC
#define IN_CH2_LED3_GPIO				(PIO_PC23_IDX)
#define IN_CH2_LED3_PIO_ID				ID_PIOC
#define IN_CH3_LED1_GPIO				(PIO_PC24_IDX)
#define IN_CH3_LED1_PIO_ID				ID_PIOC
#define IN_CH3_LED2_GPIO				(PIO_PC25_IDX)
#define IN_CH3_LED2_PIO_ID				ID_PIOC
#define IN_CH3_LED3_GPIO				(PIO_PC26_IDX)
#define IN_CH3_LED3_PIO_ID				ID_PIOC
#define OUT_CH3_LED1_GPIO				(PIO_PB7_IDX)
#define OUT_CH3_LED1_PIO_ID				ID_PIOB
#define OUT_CH3_LED2_GPIO				(PIO_PB6_IDX)
#define OUT_CH3_LED2_PIO_ID				ID_PIOB
#define OUT_CH3_LED3_GPIO				(PIO_PB8_IDX)
#define OUT_CH3_LED3_PIO_ID				ID_PIOB

/* Counter values stored at the last 250ms interrupt */
volatile uint32_t last_counter0_val = 0;
volatile uint32_t last_counter1_val = 0;
/* Counters overflows */
volatile uint32_t tc0_overflow;
volatile uint32_t tc1_overflow;
/* Total counter val returned */
volatile uint64_t tc0_counter_v;
volatile uint64_t tc1_counter_v;
/* PWM setting for signal generation */
pwm_channel_t g_pwm_freq_gen;
pwm_clock_t pwm_clock_setting;
uint32_t current_gen_freq = 50000;
uint16_t is_freq_gen_enabled = TRUE;
/* bool for counters */
uint16_t is_qma_set_to_ttlin = TRUE;
uint16_t is_countera_enabled = FALSE;
uint16_t is_counterb_enabled = FALSE;
/* Color for leds */
uint16_t ch1_in_out_color = GREEN;
uint16_t ch2_in_out_color = GREEN;
uint16_t ch3_out_color = BLUE;
uint16_t ch3_in_color = GREEN;
/* logic for leds blinking */
uint32_t led_last_counter0_val = 0;
uint32_t led_last_counter1_val = 0;


/* Routine for LED blinking */
void led_activity_routine(void)
{
	#ifdef TEST_FW
		ioport_set_pin_level(IN_CH1_LED1_GPIO, IOPORT_PIN_LEVEL_LOW);
		ioport_set_pin_level(IN_CH1_LED2_GPIO, IOPORT_PIN_LEVEL_LOW);
		ioport_set_pin_level(IN_CH1_LED3_GPIO, IOPORT_PIN_LEVEL_LOW);
		ioport_set_pin_level(IN_CH2_LED1_GPIO, IOPORT_PIN_LEVEL_LOW);
		ioport_set_pin_level(IN_CH2_LED2_GPIO, IOPORT_PIN_LEVEL_LOW);
		ioport_set_pin_level(IN_CH2_LED3_GPIO, IOPORT_PIN_LEVEL_LOW);
		ioport_set_pin_level(IN_CH3_LED1_GPIO, IOPORT_PIN_LEVEL_LOW);
		ioport_set_pin_level(IN_CH3_LED2_GPIO, IOPORT_PIN_LEVEL_LOW);
		ioport_set_pin_level(IN_CH3_LED3_GPIO, IOPORT_PIN_LEVEL_LOW);
		ioport_set_pin_level(OUT_CH3_LED1_GPIO, IOPORT_PIN_LEVEL_LOW);
		ioport_set_pin_level(OUT_CH3_LED2_GPIO, IOPORT_PIN_LEVEL_LOW);
		ioport_set_pin_level(OUT_CH3_LED3_GPIO, IOPORT_PIN_LEVEL_LOW);
		return;
	#endif
	
	if(led_last_counter0_val != tc_read_cv(TC1, 2))
	{		
		if(ch1_in_out_color == GREEN)
		{
			ioport_toggle_pin_level(IN_CH1_LED2_GPIO);
		}
		else if(ch1_in_out_color == BLUE)
		{
			ioport_toggle_pin_level(IN_CH1_LED3_GPIO);
		}
		led_last_counter0_val = tc_read_cv(TC1, 2);
	}
	else
	{
		ioport_set_pin_level(IN_CH1_LED1_GPIO, IOPORT_PIN_LEVEL_HIGH);
		ioport_set_pin_level(IN_CH1_LED2_GPIO, IOPORT_PIN_LEVEL_HIGH);
		ioport_set_pin_level(IN_CH1_LED3_GPIO, IOPORT_PIN_LEVEL_HIGH);
	}	
	if(led_last_counter1_val != tc_read_cv(TC0, 1))
	{
		if(ch2_in_out_color == GREEN)
		{
			ioport_toggle_pin_level(IN_CH2_LED2_GPIO);
		}
		else if(ch2_in_out_color == BLUE)
		{
			ioport_toggle_pin_level(IN_CH2_LED3_GPIO);
		}
		led_last_counter1_val = tc_read_cv(TC0, 1);
	}
	else
	{
		ioport_set_pin_level(IN_CH2_LED1_GPIO, IOPORT_PIN_LEVEL_HIGH);
		ioport_set_pin_level(IN_CH2_LED2_GPIO, IOPORT_PIN_LEVEL_HIGH);
		ioport_set_pin_level(IN_CH2_LED3_GPIO, IOPORT_PIN_LEVEL_HIGH);
	}
	if(ioport_get_pin_level(TTL_IN_GPIO))
	{
		ioport_set_pin_level(TTL_D_NRESET_GPIO, IOPORT_PIN_LEVEL_LOW);
		asm("NOP");asm("NOP");asm("NOP");
		ioport_set_pin_level(TTL_D_NRESET_GPIO, IOPORT_PIN_LEVEL_HIGH);
		
		if(ch3_in_color == GREEN)
		{
			ioport_toggle_pin_level(IN_CH3_LED2_GPIO);
		}
		else if(ch3_in_color == BLUE)
		{
			ioport_toggle_pin_level(IN_CH3_LED3_GPIO);
		}
		if(is_qma_set_to_ttlin == TRUE)
		{
			if(ch3_out_color == GREEN)
			{
				ioport_toggle_pin_level(OUT_CH3_LED2_GPIO);
			}
			else if(ch3_out_color == BLUE)
			{
				ioport_toggle_pin_level(OUT_CH3_LED3_GPIO);
			}
		}
	}
	else
	{
		ioport_set_pin_level(IN_CH3_LED1_GPIO, IOPORT_PIN_LEVEL_HIGH);
		ioport_set_pin_level(IN_CH3_LED2_GPIO, IOPORT_PIN_LEVEL_HIGH);
		ioport_set_pin_level(IN_CH3_LED3_GPIO, IOPORT_PIN_LEVEL_HIGH);
		if(is_qma_set_to_ttlin == TRUE)
		{
			ioport_set_pin_level(OUT_CH3_LED1_GPIO, IOPORT_PIN_LEVEL_HIGH);
			ioport_set_pin_level(OUT_CH3_LED2_GPIO, IOPORT_PIN_LEVEL_HIGH);
			ioport_set_pin_level(OUT_CH3_LED3_GPIO, IOPORT_PIN_LEVEL_HIGH);
		}
	}
	if(is_qma_set_to_ttlin == FALSE)
	{
		if(is_freq_gen_enabled == TRUE)
		{
			if(ch3_out_color == GREEN)
			{
				ioport_toggle_pin_level(OUT_CH3_LED2_GPIO);
			}
			else if(ch3_out_color == BLUE)
			{
				ioport_toggle_pin_level(OUT_CH3_LED3_GPIO);
			}
		} 
		else
		{
			ioport_set_pin_level(OUT_CH3_LED1_GPIO, IOPORT_PIN_LEVEL_HIGH);
			ioport_set_pin_level(OUT_CH3_LED2_GPIO, IOPORT_PIN_LEVEL_HIGH);
			ioport_set_pin_level(OUT_CH3_LED3_GPIO, IOPORT_PIN_LEVEL_HIGH);
		}
	}
}

// Set QMA out to the TTL IN
void set_out_to_ttl_in(void)
{
	ioport_set_pin_level(MUX_SEL_GPIO, IOPORT_PIN_LEVEL_LOW);
	is_qma_set_to_ttlin = TRUE;
}

// Set QMA out to the microcontroller
void set_out_to_uc(void)
{
	ioport_set_pin_level(MUX_SEL_GPIO, IOPORT_PIN_LEVEL_HIGH);
	is_qma_set_to_ttlin = FALSE;
}

// Is QMA out set to ttlin?
uint16_t is_qma_out_set_to_ttl(void)
{
	return is_qma_set_to_ttlin;
}

// Find if there's been some activity @ the ttl input
RET_TYPE get_ttl_in_status(void)
{
	if(ioport_get_pin_level(TTL_IN_GPIO))
		return RETURN_OK;
	else
		return RETURN_NOK;
}

// Reset the D flipflop located at the ttl input
void reset_dflipflop_ttlin(void)
{
	ioport_set_pin_level(TTL_D_NRESET_GPIO, IOPORT_PIN_LEVEL_LOW);
	asm("NOP");asm("NOP");asm("NOP");
	ioport_set_pin_level(TTL_D_NRESET_GPIO, IOPORT_PIN_LEVEL_HIGH);
}

/**
 * \brief Interrupt handler for the RTC, for second interrupt. Here we get the counters values
 */
void RTC_Handler(void)
{
	uint32_t ul_status = rtc_get_status(RTC);

	/* Second increment interrupt */
	if ((ul_status & RTC_SR_SEC) == RTC_SR_SEC) 
	{
		/* Disable RTC interrupt */
		rtc_disable_interrupt(RTC, RTC_IDR_SECDIS);
		set_second_flag();
		rtc_clear_status(RTC, RTC_SCCR_SECCLR);
		rtc_enable_interrupt(RTC, RTC_IER_SECEN);
	}
}

/**
 * \brief TC0,0 compare interrupt
 */
void TC0_Handler(void)
{
	volatile uint32_t temp_uint32t, temp_uint32t2;
	
	tc_get_status(TC0, 0);
	set_integrator_timer_flag();
	
	temp_uint32t = tc_read_cv(TC1, 2);
	temp_uint32t2 = tc_read_cv(TC0, 1);
		
	if(tc0_overflow > 0)
		tc0_counter_v = tc0_overflow + last_counter0_val + temp_uint32t + 1;
	else
		tc0_counter_v  = temp_uint32t - last_counter0_val;
		
	if(tc1_overflow > 0)
		tc1_counter_v = tc1_overflow + last_counter1_val + temp_uint32t2 + 1;
	else
		tc1_counter_v  = temp_uint32t2 - last_counter1_val;
		
	last_counter0_val = temp_uint32t;
	last_counter1_val = temp_uint32t2;
	tc0_overflow = 0;
	tc1_overflow = 0;
}

/**
 * \brief TC0,1 timer interrupt
 */
void TC1_Handler(void)
{
	tc_get_status(TC0, 1);
	tc1_overflow = 0xFFFFFFFF - last_counter1_val;
	last_counter1_val = 0;
}

/**
 * \brief TC1,2 overflow interrupt
 */
void TC5_Handler(void)
{
	tc_get_status(TC1, 2);
	tc0_overflow = 0xFFFFFFFF - last_counter0_val;
	last_counter0_val = 0;
}

/**
 * \brief TC0,2 timer interrupt (for leds)
 */
void TC2_Handler(void)
{
	tc_get_status(TC0, 2);
	led_activity_routine();
}

/**
 * \brief Get counter0 value, updated every second
 */
uint64_t get_counter0_value(void)
{
	return tc0_counter_v;
}

/**
 * \brief Get counter1 value, updated every second
 */
uint64_t get_counter1_value(void)
{
	return tc1_counter_v;
}

/**
 * \brief	Setup the RTC for second interrupts
 */
void configure_rtc(void)
{
	/* Default RTC configuration, 24-hour mode using external XTAL */
	rtc_set_hour_mode(RTC, 0);
	supc_switch_sclk_to_32kxtal(SUPC, 0);

	/* Configure RTC second interrupt */
	NVIC_DisableIRQ(RTC_IRQn);
	NVIC_ClearPendingIRQ(RTC_IRQn);
	//NVIC_SetPriority(RTC_IRQn, 0);
	NVIC_EnableIRQ(RTC_IRQn);
	rtc_enable_interrupt(RTC, RTC_IER_SECEN);
}

/**
 * \brief	Enable counter A
 */
void enable_countera(void)
{
	if(is_countera_enabled == FALSE)
	{
		// INPUT1 TCLK5, TC1 channel 1 > XC2, enable counter overflow interrupt
		pmc_enable_periph_clk(ID_TC5);
		tc_init(TC1, 2, (TC_CMR_TCCLKS_XC2|TC_CMR_WAVE));
		tc_set_block_mode(TC1, TC_BMR_TC2XC2S_TCLK2);
		tc_enable_interrupt(TC1, 2, TC_IER_COVFS);
		tc_start(TC1, 2);
		NVIC_DisableIRQ(TC5_IRQn);
		NVIC_ClearPendingIRQ(TC5_IRQn);
		//NVIC_SetPriority(TC5_IRQn, 0);
		NVIC_EnableIRQ(TC5_IRQn);
		is_countera_enabled = TRUE;		
	}
}

/**
 * \brief	Enable counter B
 */
void enable_counterb(void)
{
	if(is_counterb_enabled == FALSE)
	{
		// INPUT2 TCLK1, TC0 channel 2 > XC1, enable counter overflow interrupt
		pmc_enable_periph_clk(ID_TC1);
		tc_init(TC0, 1, (TC_CMR_TCCLKS_XC1|TC_CMR_WAVE));
		tc_set_block_mode(TC0, TC_BMR_TC1XC1S_TCLK1);
		tc_enable_interrupt(TC0, 1, TC_IER_COVFS);
		tc_start(TC0, 1);
		NVIC_DisableIRQ(TC1_IRQn);
		NVIC_ClearPendingIRQ(TC1_IRQn);
		//NVIC_SetPriority(TC1_IRQn, 0);
		NVIC_EnableIRQ(TC1_IRQn);	
		is_counterb_enabled = TRUE;
	}
}

/**
 * \brief	Enable counters
 */
void enable_counters(void)
{
	enable_countera();
	enable_counterb();
}

/**
 * \brief	Disable counter A
 */
void disable_countera(void)
{
	tc_stop(TC1, 2);
	is_countera_enabled = FALSE;
}

/**
 * \brief	Disable counter B
 */
void disable_counterb(void)
{
	tc_stop(TC0, 1);
	is_counterb_enabled = FALSE;
}

/**
 * \brief	Disable counters
 */
void disable_counters(void)
{
	disable_countera();
	disable_counterb();
}

/**
 * \brief	Is counter a enabled?
 */
uint16_t get_countera_en_status(void)
{
	return is_countera_enabled;
}

/**
 * \brief	Is counter b enabled?
 */
uint16_t get_counterb_en_status(void)
{
	return is_counterb_enabled;
}

// Here should be all the initialization functions for the module before 12v power
void init_module_peripherals_bp(void)
{	
	// Initialize RTC
	configure_rtc();
	
	// MUX
	pmc_enable_periph_clk(MUX_IN_PIO_ID);
	ioport_set_pin_mode(MUX_IN_GPIO, MUX_IN_FLAGS);
	ioport_disable_pin(MUX_IN_GPIO);
	ioport_disable_pin(PIO_PB14_IDX);
	pmc_enable_periph_clk(MUX_SEL_PIO_ID);
	ioport_set_pin_dir(MUX_SEL_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(MUX_SEL_GPIO, IOPORT_PIN_LEVEL_LOW);
	
	// INPUTS RELATED
	pmc_enable_periph_clk(TTL_D_NRESET_PIO_ID);
	ioport_set_pin_dir(TTL_D_NRESET_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(TTL_D_NRESET_GPIO, IOPORT_PIN_LEVEL_HIGH);
	pmc_enable_periph_clk(TTL_IN_PIO_ID);
	ioport_set_pin_dir(TTL_IN_GPIO, IOPORT_DIR_INPUT);
	
	// LEDS
	pmc_enable_periph_clk(OUT_CH3_LED1_PIO_ID);
	ioport_set_pin_dir(OUT_CH3_LED1_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(OUT_CH3_LED1_GPIO, IOPORT_PIN_LEVEL_HIGH);
	pmc_enable_periph_clk(OUT_CH3_LED2_PIO_ID);
	ioport_set_pin_dir(OUT_CH3_LED2_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(OUT_CH3_LED2_GPIO, IOPORT_PIN_LEVEL_HIGH);
	pmc_enable_periph_clk(OUT_CH3_LED3_PIO_ID);
	ioport_set_pin_dir(OUT_CH3_LED3_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(OUT_CH3_LED3_GPIO, IOPORT_PIN_LEVEL_HIGH);
	
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
	
	pmc_enable_periph_clk(IN_CH3_LED1_PIO_ID);
	ioport_set_pin_dir(IN_CH3_LED1_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(IN_CH3_LED1_GPIO, IOPORT_PIN_LEVEL_HIGH);
	pmc_enable_periph_clk(IN_CH3_LED2_PIO_ID);
	ioport_set_pin_dir(IN_CH3_LED2_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(IN_CH3_LED2_GPIO, IOPORT_PIN_LEVEL_HIGH);
	pmc_enable_periph_clk(IN_CH3_LED3_PIO_ID);
	ioport_set_pin_dir(IN_CH3_LED3_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(IN_CH3_LED3_GPIO, IOPORT_PIN_LEVEL_HIGH);
	
	// Init frequency inputs
	ioport_set_pin_mode(PIN_COUNTER2_GPIO, PIN_COUNTER2_FLAGS);
	ioport_disable_pin(PIN_COUNTER2_GPIO);
	ioport_set_pin_mode(PIN_COUNTER1_GPIO, PIN_COUNTER1_FLAGS);
	ioport_disable_pin(PIN_COUNTER1_GPIO);
	ioport_set_pin_mode(PIN_COUNTERIN_GPIO, PIN_COUNTERIN_FLAGS);
	ioport_disable_pin(PIN_COUNTERIN_GPIO);
	ioport_set_pin_mode(MHZ_10_IN_GPIO, MHZ_10_IN_FLAGS);
	ioport_disable_pin(MHZ_10_IN_GPIO);
	
	// Init 250ms interrupt, clocked by the external 10MHz		
	// TCLK0, TC0 channel 0 > XC0, enable compare interrupt
	pmc_enable_periph_clk(ID_TC0);
	tc_init(TC0, 0, (TC_CMR_TCCLKS_XC0|TC_CMR_WAVE|TC_CMR_CPCTRG));
	tc_write_rc(TC0, 0, (10000000/4));
	tc_set_block_mode(TC0, TC_BMR_TC0XC0S_TCLK0);
	tc_enable_interrupt(TC0, 0, TC_IER_CPCS);
	tc_start(TC0, 0);
	NVIC_DisableIRQ(TC0_IRQn);
	NVIC_ClearPendingIRQ(TC0_IRQn);
	//NVIC_SetPriority(TC0_IRQn, 0);
	NVIC_EnableIRQ(TC0_IRQn);
	
	// Init LED interrupt,
	uint32_t ul_div;
	uint32_t ul_tcclks;
	/* Get system clock. */
	uint32_t ul_sysclk = sysclk_get_cpu_hz();
	/* Configure PMC. */
	pmc_enable_periph_clk(ID_TC2);
	/* Configure TC for a TC_FREQ frequency and trigger on RC compare. */
	tc_find_mck_divisor(20, ul_sysclk, &ul_div, &ul_tcclks, ul_sysclk);
	tc_init(TC0, 2, ul_tcclks | TC_CMR_CPCTRG);
	tc_write_rc(TC0, 2, (ul_sysclk / ul_div) / 20);
	/* Configure and enable interrupt on RC compare. */
	tc_start(TC0, 2);
	NVIC_DisableIRQ(TC2_IRQn);
	NVIC_ClearPendingIRQ(TC2_IRQn);
	//NVIC_SetPriority(TC2_IRQn, 0);
	NVIC_EnableIRQ((IRQn_Type)ID_TC2);
	tc_enable_interrupt(TC0, 2, TC_IER_CPCS);	
	
	// Init frequency generator
	current_gen_freq = 50000;
	/* Configure PMC. */
	pmc_enable_periph_clk(ID_TC7);
	/* Configure TC for a TC_FREQ frequency and trigger on RC compare. */
	tc_find_mck_divisor(current_gen_freq, ul_sysclk, &ul_div, &ul_tcclks, ul_sysclk);
	tc_init(TC2, 1, ul_tcclks | TC_CMR_WAVSEL_UP_RC | TC_CMR_WAVE | TC_CMR_BCPB_CLEAR | TC_CMR_BCPC_SET | TC_CMR_EEVT_XC0);
	tc_write_rb(TC2, 1, 2);
	tc_write_rc(TC2, 1, (ul_sysclk / ul_div) / current_gen_freq);
	/* Configure and enable interrupt on RC compare. */
	tc_start(TC2, 1);
	NVIC_DisableIRQ(TC7_IRQn);
	NVIC_ClearPendingIRQ(TC7_IRQn);	
}

// Setup frequency
void setup_freqgen_freq(uint32_t frequency)
{
	current_gen_freq = frequency;	
	
	// Init LED interrupt,
	uint32_t ul_div;
	uint32_t ul_tcclks;
	/* Get system clock. */
	uint32_t ul_sysclk = sysclk_get_cpu_hz();
	tc_stop(TC2, 1);
	/* Configure TC for a TC_FREQ frequency and trigger on RC compare. */
	tc_find_mck_divisor(current_gen_freq, ul_sysclk, &ul_div, &ul_tcclks, ul_sysclk);
	tc_init(TC2, 1, ul_tcclks | TC_CMR_WAVSEL_UP_RC | TC_CMR_WAVE | TC_CMR_BCPB_CLEAR | TC_CMR_BCPC_SET | TC_CMR_EEVT_XC0);
	tc_write_rb(TC2, 1, 2);
	tc_write_rc(TC2, 1, (ul_sysclk / ul_div) / current_gen_freq);
	/* Configure and enable interrupt on RC compare. */
	tc_start(TC2, 1);
}

// Get current frequency
uint32_t get_freqgen_freq(void)
{
	return current_gen_freq;
}

// Here should be all the initialization functions for the module after 12v power
void init_module_peripherals_ap(void)
{
	
}

// Here should be all the deinitialization functions for the module before 12v power removal
void deinit_module_peripherals(void)
{
	
}

