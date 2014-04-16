/*
 * peripherals_module.c
 *
 * Created: 15/11/2013 14:47:11
 *  Author: stephan
 */ 
#include "peripherals_module.h"
#include "defines.h"

#define CH1_DELAY_EN_GPIO				(PIO_PB0_IDX)
#define CH1_DELAY_EN_PIO_ID				ID_PIOB
#define CH2_DELAY_EN_GPIO				(PIO_PA29_IDX)
#define CH2_DELAY_EN_PIO_ID				ID_PIOA
#define CH1_PULSE_GPIO					(PIO_PC15_IDX)
#define CH1_PULSE_PIO_ID				ID_PIOC
#define CH1_PULSE_RES_GPIO				(PIO_PC16_IDX)
#define CH1_PULSE_RES_PIO_ID			ID_PIOC
#define CH2_PULSE_GPIO					(PIO_PB2_IDX)
#define CH2_PULSE_PIO_ID				ID_PIOB
#define CH2_PULSE_RES_GPIO				(PIO_PB4_IDX)
#define CH2_PULSE_RES_PIO_ID			ID_PIOB
#define IN_CH1_LED1_GPIO				(PIO_PC11_IDX)
#define IN_CH1_LED1_PIO_ID				ID_PIOC
#define IN_CH1_LED2_GPIO				(PIO_PC13_IDX)
#define IN_CH1_LED2_PIO_ID				ID_PIOC
#define IN_CH1_LED3_GPIO				(PIO_PC12_IDX)
#define IN_CH1_LED3_PIO_ID				ID_PIOC
#define OUT_CH1_LED1_GPIO				(PIO_PC17_IDX)
#define OUT_CH1_LED1_PIO_ID				ID_PIOC
#define OUT_CH1_LED2_GPIO				(PIO_PC19_IDX)
#define OUT_CH1_LED2_PIO_ID				ID_PIOC
#define OUT_CH1_LED3_GPIO				(PIO_PC18_IDX)
#define OUT_CH1_LED3_PIO_ID				ID_PIOC
#define IN_CH2_LED1_GPIO				(PIO_PC4_IDX)
#define IN_CH2_LED1_PIO_ID				ID_PIOC
#define IN_CH2_LED2_GPIO				(PIO_PB3_IDX)
#define IN_CH2_LED2_PIO_ID				ID_PIOB
#define IN_CH2_LED3_GPIO				(PIO_PC10_IDX)
#define IN_CH2_LED3_PIO_ID				ID_PIOC
#define OUT_CH2_LED1_GPIO				(PIO_PB5_IDX)
#define OUT_CH2_LED1_PIO_ID				ID_PIOB
#define OUT_CH2_LED2_GPIO				(PIO_PB7_IDX)
#define OUT_CH2_LED2_PIO_ID				ID_PIOB
#define OUT_CH2_LED3_GPIO				(PIO_PB6_IDX)
#define OUT_CH2_LED3_PIO_ID				ID_PIOB

/* Chip select. */
#define SPI_CHIP_SEL 0
/* Clock polarity. */
#define SPI_CLK_POLARITY 0
/* Clock phase. */
#define SPI_CLK_PHASE 1
/* Delay before SPCK. */
#define SPI_DLYBS 0
/* Delay between consecutive transfers. */
#define SPI_DLYBCT 0

/* SPI clock setting (Hz). */
static uint32_t gs_ul_spi_clock = 100000;
/* Current delays */
uint16_t current_channel1_delay = 0;
uint16_t current_channel2_delay = 0;
/* Delays statuses */
uint16_t is_channel1_delay_en = FALSE;
uint16_t is_channel2_delay_en = FALSE;
/* Color for leds */
uint16_t ch1_in_color = GREEN;
uint16_t ch1_out_color = BLUE;
uint16_t ch2_in_color = GREEN;
uint16_t ch2_out_color = BLUE;


void led_activity_routine(void)
{
	#ifdef TEST_FW
		ioport_set_pin_level(IN_CH1_LED1_GPIO, IOPORT_PIN_LEVEL_LOW);
		ioport_set_pin_level(IN_CH1_LED2_GPIO, IOPORT_PIN_LEVEL_LOW);
		ioport_set_pin_level(IN_CH1_LED3_GPIO, IOPORT_PIN_LEVEL_LOW);
		ioport_set_pin_level(IN_CH2_LED1_GPIO, IOPORT_PIN_LEVEL_LOW);
		ioport_set_pin_level(IN_CH2_LED2_GPIO, IOPORT_PIN_LEVEL_LOW);
		ioport_set_pin_level(IN_CH2_LED3_GPIO, IOPORT_PIN_LEVEL_LOW);
		ioport_set_pin_level(OUT_CH1_LED1_GPIO, IOPORT_PIN_LEVEL_LOW);
		ioport_set_pin_level(OUT_CH1_LED2_GPIO, IOPORT_PIN_LEVEL_LOW);
		ioport_set_pin_level(OUT_CH1_LED3_GPIO, IOPORT_PIN_LEVEL_LOW);
		ioport_set_pin_level(OUT_CH2_LED1_GPIO, IOPORT_PIN_LEVEL_LOW);
		ioport_set_pin_level(OUT_CH2_LED2_GPIO, IOPORT_PIN_LEVEL_LOW);
		ioport_set_pin_level(OUT_CH2_LED3_GPIO, IOPORT_PIN_LEVEL_LOW);
		return;
	#endif
	
	if(ioport_get_pin_level(CH1_PULSE_GPIO))
	{
		ioport_set_pin_level(CH1_PULSE_RES_GPIO, IOPORT_PIN_LEVEL_HIGH);
		asm("NOP");asm("NOP");asm("NOP");
		ioport_set_pin_level(CH1_PULSE_RES_GPIO, IOPORT_PIN_LEVEL_LOW);
		
		if(ch1_in_color == GREEN)
		{
			ioport_toggle_pin_level(IN_CH1_LED3_GPIO);
		}
		else if(ch1_in_color == BLUE)
		{
			ioport_toggle_pin_level(IN_CH1_LED2_GPIO);
		}
		if(is_channel1_delay_en == TRUE)
		{
			if(ch1_out_color == GREEN)
			{
				ioport_toggle_pin_level(OUT_CH1_LED3_GPIO);
			}
			else if(ch1_out_color == BLUE)
			{
				ioport_toggle_pin_level(OUT_CH1_LED2_GPIO);
			}			
		}
		else
		{
			ioport_set_pin_level(OUT_CH1_LED1_GPIO, IOPORT_PIN_LEVEL_HIGH);
			ioport_set_pin_level(OUT_CH1_LED2_GPIO, IOPORT_PIN_LEVEL_HIGH);
			ioport_set_pin_level(OUT_CH1_LED3_GPIO, IOPORT_PIN_LEVEL_HIGH);			
		}
	}
	else
	{
		ioport_set_pin_level(IN_CH1_LED1_GPIO, IOPORT_PIN_LEVEL_HIGH);
		ioport_set_pin_level(IN_CH1_LED2_GPIO, IOPORT_PIN_LEVEL_HIGH);
		ioport_set_pin_level(IN_CH1_LED3_GPIO, IOPORT_PIN_LEVEL_HIGH);
		ioport_set_pin_level(OUT_CH1_LED1_GPIO, IOPORT_PIN_LEVEL_HIGH);
		ioport_set_pin_level(OUT_CH1_LED2_GPIO, IOPORT_PIN_LEVEL_HIGH);
		ioport_set_pin_level(OUT_CH1_LED3_GPIO, IOPORT_PIN_LEVEL_HIGH);		
	}
	
	if(ioport_get_pin_level(CH2_PULSE_GPIO))
	{
		ioport_set_pin_level(CH2_PULSE_RES_GPIO, IOPORT_PIN_LEVEL_HIGH);
		asm("NOP");asm("NOP");asm("NOP");
		ioport_set_pin_level(CH2_PULSE_RES_GPIO, IOPORT_PIN_LEVEL_LOW);
		
		if(ch2_in_color == GREEN)
		{
			ioport_toggle_pin_level(IN_CH2_LED3_GPIO);
		}
		else if(ch2_in_color == BLUE)
		{
			ioport_toggle_pin_level(IN_CH2_LED2_GPIO);
		}
		if (is_channel2_delay_en == TRUE)
		{
			if(ch2_out_color == GREEN)
			{
				ioport_toggle_pin_level(OUT_CH2_LED3_GPIO);
			}
			else if(ch2_out_color == BLUE)
			{
				ioport_toggle_pin_level(OUT_CH2_LED2_GPIO);
			}
		}
		else
		{
			ioport_set_pin_level(OUT_CH2_LED1_GPIO, IOPORT_PIN_LEVEL_HIGH);
			ioport_set_pin_level(OUT_CH2_LED2_GPIO, IOPORT_PIN_LEVEL_HIGH);
			ioport_set_pin_level(OUT_CH2_LED3_GPIO, IOPORT_PIN_LEVEL_HIGH);			
		}
	}
	else
	{
		ioport_set_pin_level(IN_CH2_LED1_GPIO, IOPORT_PIN_LEVEL_HIGH);
		ioport_set_pin_level(IN_CH2_LED2_GPIO, IOPORT_PIN_LEVEL_HIGH);
		ioport_set_pin_level(IN_CH2_LED3_GPIO, IOPORT_PIN_LEVEL_HIGH);
		ioport_set_pin_level(OUT_CH2_LED1_GPIO, IOPORT_PIN_LEVEL_HIGH);
		ioport_set_pin_level(OUT_CH2_LED2_GPIO, IOPORT_PIN_LEVEL_HIGH);
		ioport_set_pin_level(OUT_CH2_LED3_GPIO, IOPORT_PIN_LEVEL_HIGH);
	}
}

/**
 * \brief TC0,1 overflow interrupt
 */
void TC1_Handler(void)
{
	tc_get_status(TC0, 1);
	led_activity_routine();
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

	pmc_enable_periph_clk(OUT_CH1_LED1_PIO_ID);
	ioport_set_pin_dir(OUT_CH1_LED1_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(OUT_CH1_LED1_GPIO, IOPORT_PIN_LEVEL_HIGH);
	pmc_enable_periph_clk(OUT_CH1_LED2_PIO_ID);
	ioport_set_pin_dir(OUT_CH1_LED2_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(OUT_CH1_LED2_GPIO, IOPORT_PIN_LEVEL_HIGH);
	pmc_enable_periph_clk(OUT_CH1_LED3_PIO_ID);
	ioport_set_pin_dir(OUT_CH1_LED3_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(OUT_CH1_LED3_GPIO, IOPORT_PIN_LEVEL_HIGH);

	pmc_enable_periph_clk(IN_CH2_LED1_PIO_ID);
	ioport_set_pin_dir(IN_CH2_LED1_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(IN_CH2_LED1_GPIO, IOPORT_PIN_LEVEL_HIGH);
	pmc_enable_periph_clk(IN_CH2_LED2_PIO_ID);
	ioport_set_pin_dir(IN_CH2_LED2_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(IN_CH2_LED2_GPIO, IOPORT_PIN_LEVEL_HIGH);
	pmc_enable_periph_clk(IN_CH2_LED3_PIO_ID);
	ioport_set_pin_dir(IN_CH2_LED3_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(IN_CH2_LED3_GPIO, IOPORT_PIN_LEVEL_HIGH);

	pmc_enable_periph_clk(OUT_CH2_LED1_PIO_ID);
	ioport_set_pin_dir(OUT_CH2_LED1_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(OUT_CH2_LED1_GPIO, IOPORT_PIN_LEVEL_HIGH);
	pmc_enable_periph_clk(OUT_CH2_LED2_PIO_ID);
	ioport_set_pin_dir(OUT_CH2_LED2_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(OUT_CH2_LED2_GPIO, IOPORT_PIN_LEVEL_HIGH);
	pmc_enable_periph_clk(OUT_CH2_LED3_PIO_ID);
	ioport_set_pin_dir(OUT_CH2_LED3_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(OUT_CH2_LED3_GPIO, IOPORT_PIN_LEVEL_HIGH);
	
	/* Pulse inputs & d reset output */	
	pmc_enable_periph_clk(CH1_PULSE_PIO_ID);
	ioport_set_pin_dir(CH1_PULSE_GPIO, IOPORT_DIR_INPUT);
	pmc_enable_periph_clk(CH1_PULSE_RES_PIO_ID);
	ioport_set_pin_dir(CH1_PULSE_RES_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(CH1_PULSE_RES_GPIO, IOPORT_PIN_LEVEL_LOW);
	
	pmc_enable_periph_clk(CH2_PULSE_PIO_ID);
	ioport_set_pin_dir(CH2_PULSE_GPIO, IOPORT_DIR_INPUT);
	pmc_enable_periph_clk(CH2_PULSE_RES_PIO_ID);
	ioport_set_pin_dir(CH2_PULSE_RES_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(CH2_PULSE_RES_GPIO, IOPORT_PIN_LEVEL_LOW);
	
	/* Delays enable */
	pmc_enable_periph_clk(CH1_DELAY_EN_PIO_ID);
	ioport_set_pin_dir(CH1_DELAY_EN_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(CH1_DELAY_EN_GPIO, IOPORT_PIN_LEVEL_HIGH);
	pmc_enable_periph_clk(CH2_DELAY_EN_PIO_ID);
	ioport_set_pin_dir(CH2_DELAY_EN_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(CH2_DELAY_EN_GPIO, IOPORT_PIN_LEVEL_HIGH);
	
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
	
	/* SPI interface */
	gpio_configure_pin(SPI0_MISO_GPIO, SPI0_MISO_FLAGS);
	gpio_configure_pin(SPI0_MOSI_GPIO, SPI0_MOSI_FLAGS);
	gpio_configure_pin(SPI0_SPCK_GPIO, SPI0_SPCK_FLAGS);
	gpio_configure_pin(SPI0_NPCS0_GPIO, SPI0_NPCS0_FLAGS);
	
	/* Configure an SPI peripheral. */
	spi_enable_clock(SPI0);
	spi_disable(SPI0);
	spi_reset(SPI0);
	spi_set_lastxfer(SPI0);
	spi_set_master_mode(SPI0);
	spi_disable_mode_fault_detect(SPI0);
	spi_set_peripheral_chip_select_value(SPI0, SPI_CHIP_SEL);
	spi_set_clock_polarity(SPI0, SPI_CHIP_SEL, SPI_CLK_POLARITY);
	spi_set_clock_phase(SPI0, SPI_CHIP_SEL, SPI_CLK_PHASE);
	spi_set_bits_per_transfer(SPI0, SPI_CHIP_SEL, SPI_CSR_BITS_10_BIT);
	spi_configure_cs_behavior(SPI0, SPI_CHIP_SEL, SPI_CS_RISE_NO_TX);
	spi_set_baudrate_div(SPI0, SPI_CHIP_SEL, (sysclk_get_cpu_hz() / gs_ul_spi_clock));
	spi_set_transfer_delay(SPI0, SPI_CHIP_SEL, SPI_DLYBS, SPI_DLYBCT);
	spi_enable(SPI0);
}

uint16_t is_channel1_delay_enabled(void)
{
	return is_channel1_delay_en;
}

uint16_t is_channel2_delay_enabled(void)
{
	return is_channel2_delay_en;
}

void enable_disable_ch1_delay(uint16_t status)
{
	if(status == TRUE)
	{
		is_channel1_delay_en = TRUE;
		ioport_set_pin_level(CH1_DELAY_EN_GPIO, IOPORT_PIN_LEVEL_LOW);
	}
	else
	{
		is_channel1_delay_en = FALSE;
		ioport_set_pin_level(CH1_DELAY_EN_GPIO, IOPORT_PIN_LEVEL_HIGH);
	}
}

void enable_disable_ch2_delay(uint16_t status)
{
	if(status == TRUE)
	{
		is_channel2_delay_en = TRUE;
		ioport_set_pin_level(CH2_DELAY_EN_GPIO, IOPORT_PIN_LEVEL_LOW);
	}
	else
	{
		is_channel2_delay_en = FALSE;
		ioport_set_pin_level(CH2_DELAY_EN_GPIO, IOPORT_PIN_LEVEL_HIGH);
	}
}

uint16_t inverse_10_bits_order(uint16_t data)
{
	uint32_t x = (uint32_t)data;
	
	x = (x & 0x55555555) <<  1 | (x & 0xAAAAAAAA) >>  1;
	x = (x & 0x33333333) <<  2 | (x & 0xCCCCCCCC) >>  2;
	x = (x & 0x0F0F0F0F) <<  4 | (x & 0xF0F0F0F0) >>  4;
	x = (x & 0x00FF00FF) <<  8 | (x & 0xFF00FF00) >>  8;
	x = (x & 0x0000FFFF) << 16 | (x & 0xFFFF0000) >> 16;
	
	x = (x >> 22);
	return (uint16_t)x;
}

void send_updated_delays(void)
{
	spi_write(SPI0, current_channel1_delay, 0, 0);
	/* Wait transfer done. */
	while((spi_read_status(SPI0) & SPI_SR_TDRE) == 0);;
	spi_write(SPI0, current_channel2_delay, 0, 0);
	/* Wait transfer done. */
	while((spi_read_status(SPI0) & SPI_SR_TDRE) == 0);;	
}

void setup_channel1_delay(uint16_t delay)
{
	delay = inverse_10_bits_order(delay);
	current_channel1_delay = delay;
	send_updated_delays();
}

void setup_channel2_delay(uint16_t delay)
{
	delay = inverse_10_bits_order(delay);
	current_channel2_delay = delay;
	send_updated_delays();
}

uint16_t get_channel1_delay(void)
{
	return (inverse_10_bits_order(current_channel1_delay) & 0x03FF);
}

uint16_t get_channel2_delay(void)
{
	return (inverse_10_bits_order(current_channel2_delay) & 0x03FF);
}

// Here should be all the initialization functions for the module after 12v power
void init_module_peripherals_ap(void)
{
	
}

// Here should be all the deinitialization functions for the module before 12v power removal
void deinit_module_peripherals(void)
{
	
}

