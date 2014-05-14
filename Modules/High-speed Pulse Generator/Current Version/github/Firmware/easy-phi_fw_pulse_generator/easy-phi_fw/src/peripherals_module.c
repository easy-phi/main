/*
 * peripherals_module.c
 *
 * Created: 15/11/2013 14:47:11
 *  Author: stephan
 */ 

#include "peripherals_template.h"
#include "peripherals_module.h"
#include "defines.h"
// LEDS
#define IN_CLK_LED1_GPIO				(PIO_PA6_IDX)
#define IN_CLK_LED1_PIO_ID				ID_PIOA
#define IN_CLK_LED2_GPIO				(PIO_PA3_IDX)
#define IN_CLK_LED2_PIO_ID				ID_PIOA
#define IN_CLK_LED3_GPIO				(PIO_PA4_IDX)
#define IN_CLK_LED3_PIO_ID				ID_PIOA
#define IN_DAT_LED1_GPIO				(PIO_PA2_IDX)
#define IN_DAT_LED1_PIO_ID				ID_PIOA
#define IN_DAT_LED2_GPIO				(PIO_PB13_IDX)
#define IN_DAT_LED2_PIO_ID				ID_PIOB
#define IN_DAT_LED3_GPIO				(PIO_PB12_IDX)
#define IN_DAT_LED3_PIO_ID				ID_PIOB
#define OUT_CH1_CH2_LED1_GPIO			(PIO_PC16_IDX)
#define OUT_CH1_CH2_LED1_PIO_ID			ID_PIOC
#define OUT_CH1_CH2_LED2_GPIO			(PIO_PC14_IDX)
#define OUT_CH1_CH2_LED2_PIO_ID			ID_PIOC
#define OUT_CH1_CH2_LED3_GPIO			(PIO_PC15_IDX)
#define OUT_CH1_CH2_LED3_PIO_ID			ID_PIOC
#define OUT_CH3_LED1_GPIO				(PIO_PC17_IDX)
#define OUT_CH3_LED1_PIO_ID				ID_PIOC
#define OUT_CH3_LED2_GPIO				(PIO_PC19_IDX)
#define OUT_CH3_LED2_PIO_ID				ID_PIOC
#define OUT_CH3_LED3_GPIO				(PIO_PC18_IDX)
#define OUT_CH3_LED3_PIO_ID				ID_PIOC
// Pulse inputs
#define OUT_PULSE_DET_GPIO				(PIO_PB2_IDX)
#define OUT_PULSE_DET_PIO_ID			ID_PIOB
#define OUT_PULSE_STATE_GPIO			(PIO_PB3_IDX)
#define OUT_PULSE_STATE_PIO_ID			ID_PIOB
#define DATA_IN_DET_GPIO				(PIO_PB18_IDX)
#define DATA_IN_DET_PIO_ID				ID_PIOB
#define DATA_IN_STATE_GPIO				(PIO_PB17_IDX)
#define DATA_IN_STATE_PIO_ID			ID_PIOB
#define CLK_IN_DET_GPIO					(PIO_PA16_IDX)
#define CLK_IN_DET_PIO_ID				ID_PIOA
#define CLK_IN_STATE_GPIO				(PIO_PB16_IDX)
#define CLK_IN_STATE_PIO_ID				ID_PIOB
// Reset detections
#define OUT_PULSE_RST_GPIO				(PIO_PB0_IDX)
#define OUT_PULSE_RST_PIO_ID			ID_PIOB
#define DATA_PULSE_RST_GPIO				(PIO_PB19_IDX)
#define DATA_PULSE_RST_PIO_ID			ID_PIOB
#define CLK_PULSE_RST_GPIO				(PIO_PB20_IDX)
#define CLK_PULSE_RST_PIO_ID			ID_PIOB
// Delay enables
#define DATA_DELAY_EN_GPIO				(PIO_PC11_IDX)
#define DATA_DELAY_EN_PIO_ID			ID_PIOC
#define CLOCK_DELAY_EN_GPIO				(PIO_PC12_IDX)
#define CLOCK_DELAY_EN_PIO_ID			ID_PIOC
#define RESET_DELAY_EN_GPIO				(PIO_PC13_IDX)
#define RESET_DELAY_EN_PIO_ID			ID_PIOC
// Delay SLoad
#define DATA_DELAY_SLOAD_GPIO			(PIO_PB21_IDX)
#define DATA_DELAY_SLOAD_PIO_ID			ID_PIOB
#define CLOCK_DELAY_SLOAD_GPIO			(PIO_PA28_IDX)
#define CLOCK_DELAY_SLOAD_PIO_ID		ID_PIOA
#define RESET_DELAY_SLOAD_GPIO			(PIO_PA29_IDX)
#define RESET_DELAY_SLOAD_PIO_ID		ID_PIOA
// Latch RF attenuator
#define RF_ATTEN_SLOAD_GPIO				(PIO_PB1_IDX)
#define RF_ATTEN_SLOAD_PIO_ID			ID_PIOB

/* Chip select. */
#define SPI_CHIP_SEL		0
#define SPI_ALT_CHIP_SEL	1
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

/* Delays statuses */
uint16_t is_reset_delay_en = FALSE;
uint16_t is_data_delay_en = FALSE;
uint16_t is_clock_delay_en = FALSE;

/* Current delays */
uint16_t current_reset_delay = 0;
uint16_t current_data_delay = 0;
uint16_t current_clock_delay = 0;

/* Color for leds */
uint16_t clk_in_color = GREEN;
uint16_t data_in_color = GREEN;
uint16_t ch_out_color = BLUE;

/* switch lights off bool */
uint16_t lights_off = FALSE;

/* Current atteunation */
uint16_t current_attenution = 127;


void switch_on_off_lights(uint16_t bool_lights)
{
	lights_off = bool_lights;
}

void led_activity_routine(void)
{
	
	if((get_ok_12v_status() == RETURN_NOK) || (lights_off == TRUE))
	{
		ioport_set_pin_level(IN_CLK_LED1_GPIO, IOPORT_PIN_LEVEL_HIGH);
		ioport_set_pin_level(IN_CLK_LED2_GPIO, IOPORT_PIN_LEVEL_HIGH);
		ioport_set_pin_level(IN_CLK_LED3_GPIO, IOPORT_PIN_LEVEL_HIGH);
		ioport_set_pin_level(IN_DAT_LED1_GPIO, IOPORT_PIN_LEVEL_HIGH);
		ioport_set_pin_level(IN_DAT_LED2_GPIO, IOPORT_PIN_LEVEL_HIGH);
		ioport_set_pin_level(IN_DAT_LED3_GPIO, IOPORT_PIN_LEVEL_HIGH);
		ioport_set_pin_level(OUT_CH1_CH2_LED1_GPIO, IOPORT_PIN_LEVEL_HIGH);
		ioport_set_pin_level(OUT_CH1_CH2_LED2_GPIO, IOPORT_PIN_LEVEL_HIGH);
		ioport_set_pin_level(OUT_CH1_CH2_LED3_GPIO, IOPORT_PIN_LEVEL_HIGH);
		ioport_set_pin_level(OUT_CH3_LED1_GPIO, IOPORT_PIN_LEVEL_HIGH);
		ioport_set_pin_level(OUT_CH3_LED2_GPIO, IOPORT_PIN_LEVEL_HIGH);
		ioport_set_pin_level(OUT_CH3_LED3_GPIO, IOPORT_PIN_LEVEL_HIGH);
		return;
	}
	
	if(ioport_get_pin_level(CLK_IN_DET_GPIO))
	{		
		if(clk_in_color == GREEN)
		{
			ioport_toggle_pin_level(IN_CLK_LED3_GPIO);
		}
		else if(clk_in_color == BLUE)
		{
			ioport_toggle_pin_level(IN_CLK_LED2_GPIO);
		}
		else if(clk_in_color == RED)
		{
			ioport_toggle_pin_level(IN_CLK_LED1_GPIO);
		}
	}
	else
	{
		ioport_set_pin_level(IN_CLK_LED1_GPIO, IOPORT_PIN_LEVEL_HIGH);
		ioport_set_pin_level(IN_CLK_LED2_GPIO, IOPORT_PIN_LEVEL_HIGH);
		ioport_set_pin_level(IN_CLK_LED3_GPIO, IOPORT_PIN_LEVEL_HIGH);
	}	
	
	if(ioport_get_pin_level(DATA_IN_DET_GPIO))
	{		
		if(data_in_color == GREEN)
		{
			ioport_toggle_pin_level(IN_DAT_LED3_GPIO);
		}
		else if(data_in_color == BLUE)
		{
			ioport_toggle_pin_level(IN_DAT_LED2_GPIO);
		}
		else if(data_in_color == RED)
		{
			ioport_toggle_pin_level(IN_DAT_LED1_GPIO);
		}
	}
	else
	{
		ioport_set_pin_level(IN_DAT_LED1_GPIO, IOPORT_PIN_LEVEL_HIGH);
		ioport_set_pin_level(IN_DAT_LED2_GPIO, IOPORT_PIN_LEVEL_HIGH);
		ioport_set_pin_level(IN_DAT_LED3_GPIO, IOPORT_PIN_LEVEL_HIGH);
	}
	
	if(!ioport_get_pin_level(OUT_PULSE_DET_GPIO)) // Signal are inverted for routing facility (OG 10.04.2014)
	{		
		if(ch_out_color == GREEN)
		{
			ioport_toggle_pin_level(OUT_CH1_CH2_LED3_GPIO);
			ioport_toggle_pin_level(OUT_CH3_LED3_GPIO);
		}
		else if(ch_out_color == BLUE)
		{
			ioport_toggle_pin_level(OUT_CH1_CH2_LED2_GPIO);
			ioport_toggle_pin_level(OUT_CH3_LED2_GPIO);
		}
		else if(ch_out_color == RED)
		{
			ioport_toggle_pin_level(OUT_CH1_CH2_LED1_GPIO);
			ioport_toggle_pin_level(OUT_CH3_LED1_GPIO);
		}
	}
	else
	{
		ioport_set_pin_level(OUT_CH1_CH2_LED1_GPIO, IOPORT_PIN_LEVEL_HIGH);
		ioport_set_pin_level(OUT_CH1_CH2_LED2_GPIO, IOPORT_PIN_LEVEL_HIGH);
		ioport_set_pin_level(OUT_CH1_CH2_LED3_GPIO, IOPORT_PIN_LEVEL_HIGH);
		ioport_set_pin_level(OUT_CH3_LED1_GPIO, IOPORT_PIN_LEVEL_HIGH);
		ioport_set_pin_level(OUT_CH3_LED2_GPIO, IOPORT_PIN_LEVEL_HIGH);
		ioport_set_pin_level(OUT_CH3_LED3_GPIO, IOPORT_PIN_LEVEL_HIGH);
	}
	
	ioport_set_pin_level(DATA_PULSE_RST_GPIO, IOPORT_PIN_LEVEL_HIGH);
	ioport_set_pin_level(CLK_PULSE_RST_GPIO, IOPORT_PIN_LEVEL_HIGH);
	ioport_set_pin_level(OUT_PULSE_RST_GPIO, IOPORT_PIN_LEVEL_HIGH);
	asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");
	ioport_set_pin_level(DATA_PULSE_RST_GPIO, IOPORT_PIN_LEVEL_LOW);
	ioport_set_pin_level(CLK_PULSE_RST_GPIO, IOPORT_PIN_LEVEL_LOW);
	ioport_set_pin_level(OUT_PULSE_RST_GPIO, IOPORT_PIN_LEVEL_LOW);
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
	pmc_enable_periph_clk(IN_CLK_LED1_PIO_ID);
	ioport_set_pin_dir(IN_CLK_LED1_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(IN_CLK_LED1_GPIO, IOPORT_PIN_LEVEL_HIGH);
	pmc_enable_periph_clk(IN_CLK_LED2_PIO_ID);
	ioport_set_pin_dir(IN_CLK_LED2_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(IN_CLK_LED2_GPIO, IOPORT_PIN_LEVEL_HIGH);
	pmc_enable_periph_clk(IN_CLK_LED3_PIO_ID);
	ioport_set_pin_dir(IN_CLK_LED3_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(IN_CLK_LED3_GPIO, IOPORT_PIN_LEVEL_HIGH);	
	pmc_enable_periph_clk(IN_DAT_LED1_PIO_ID);
	ioport_set_pin_dir(IN_DAT_LED1_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(IN_DAT_LED1_GPIO, IOPORT_PIN_LEVEL_HIGH);
	pmc_enable_periph_clk(IN_DAT_LED2_PIO_ID);
	ioport_set_pin_dir(IN_DAT_LED2_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(IN_DAT_LED2_GPIO, IOPORT_PIN_LEVEL_HIGH);
	pmc_enable_periph_clk(IN_DAT_LED3_PIO_ID);
	ioport_set_pin_dir(IN_DAT_LED3_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(IN_DAT_LED3_GPIO, IOPORT_PIN_LEVEL_HIGH);	
	pmc_enable_periph_clk(OUT_CH1_CH2_LED1_PIO_ID);
	ioport_set_pin_dir(OUT_CH1_CH2_LED1_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(OUT_CH1_CH2_LED1_GPIO, IOPORT_PIN_LEVEL_HIGH);
	pmc_enable_periph_clk(OUT_CH1_CH2_LED2_PIO_ID);
	ioport_set_pin_dir(OUT_CH1_CH2_LED2_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(OUT_CH1_CH2_LED2_GPIO, IOPORT_PIN_LEVEL_HIGH);
	pmc_enable_periph_clk(OUT_CH1_CH2_LED3_PIO_ID);
	ioport_set_pin_dir(OUT_CH1_CH2_LED3_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(OUT_CH1_CH2_LED3_GPIO, IOPORT_PIN_LEVEL_HIGH);	
	pmc_enable_periph_clk(OUT_CH3_LED1_PIO_ID);
	ioport_set_pin_dir(OUT_CH3_LED1_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(OUT_CH3_LED1_GPIO, IOPORT_PIN_LEVEL_HIGH);
	pmc_enable_periph_clk(OUT_CH3_LED2_PIO_ID);
	ioport_set_pin_dir(OUT_CH3_LED2_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(OUT_CH3_LED2_GPIO, IOPORT_PIN_LEVEL_HIGH);
	pmc_enable_periph_clk(OUT_CH3_LED3_PIO_ID);
	ioport_set_pin_dir(OUT_CH3_LED3_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(OUT_CH3_LED3_GPIO, IOPORT_PIN_LEVEL_HIGH);
	
	/* Pulse inputs/state & d reset output */
	pmc_enable_periph_clk(OUT_PULSE_DET_PIO_ID);
	ioport_set_pin_dir(OUT_PULSE_DET_GPIO, IOPORT_DIR_INPUT);
	pmc_enable_periph_clk(DATA_IN_DET_PIO_ID);
	ioport_set_pin_dir(DATA_IN_DET_GPIO, IOPORT_DIR_INPUT);
	pmc_enable_periph_clk(CLK_IN_DET_PIO_ID);
	ioport_set_pin_dir(CLK_IN_DET_GPIO, IOPORT_DIR_INPUT);	
	pmc_enable_periph_clk(OUT_PULSE_STATE_PIO_ID);
	ioport_set_pin_dir(OUT_PULSE_STATE_GPIO, IOPORT_DIR_INPUT);
	pmc_enable_periph_clk(DATA_IN_STATE_PIO_ID);
	ioport_set_pin_dir(DATA_IN_STATE_GPIO, IOPORT_DIR_INPUT);
	pmc_enable_periph_clk(CLK_IN_STATE_PIO_ID);
	ioport_set_pin_dir(CLK_IN_STATE_GPIO, IOPORT_DIR_INPUT);	
	pmc_enable_periph_clk(OUT_PULSE_RST_PIO_ID);
	ioport_set_pin_dir(OUT_PULSE_RST_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(OUT_PULSE_RST_GPIO, IOPORT_PIN_LEVEL_LOW);	
	pmc_enable_periph_clk(DATA_PULSE_RST_PIO_ID);
	ioport_set_pin_dir(DATA_PULSE_RST_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(DATA_PULSE_RST_GPIO, IOPORT_PIN_LEVEL_LOW);	
	pmc_enable_periph_clk(CLK_PULSE_RST_PIO_ID);
	ioport_set_pin_dir(CLK_PULSE_RST_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(CLK_PULSE_RST_GPIO, IOPORT_PIN_LEVEL_LOW);
	
	/* Sload */
	pmc_enable_periph_clk(DATA_DELAY_SLOAD_PIO_ID);
	ioport_set_pin_dir(DATA_DELAY_SLOAD_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(DATA_DELAY_SLOAD_GPIO, IOPORT_PIN_LEVEL_LOW);
	pmc_enable_periph_clk(CLOCK_DELAY_SLOAD_PIO_ID);
	ioport_set_pin_dir(CLOCK_DELAY_SLOAD_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(CLOCK_DELAY_SLOAD_GPIO, IOPORT_PIN_LEVEL_LOW);
	pmc_enable_periph_clk(RESET_DELAY_SLOAD_PIO_ID);
	ioport_set_pin_dir(RESET_DELAY_SLOAD_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(RESET_DELAY_SLOAD_GPIO, IOPORT_PIN_LEVEL_LOW);
	pmc_enable_periph_clk(RF_ATTEN_SLOAD_PIO_ID);
	ioport_set_pin_dir(RF_ATTEN_SLOAD_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(RF_ATTEN_SLOAD_GPIO, IOPORT_PIN_LEVEL_LOW);

	/* Delay enables */
	pmc_enable_periph_clk(DATA_DELAY_EN_PIO_ID);
	ioport_set_pin_dir(DATA_DELAY_EN_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(DATA_DELAY_EN_GPIO, IOPORT_PIN_LEVEL_HIGH);	// default state is HIGH (OG 10.04.2014)
	pmc_enable_periph_clk(CLOCK_DELAY_EN_PIO_ID);
	ioport_set_pin_dir(CLOCK_DELAY_EN_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(CLOCK_DELAY_EN_GPIO, IOPORT_PIN_LEVEL_HIGH);	// default state is HIGH (OG 10.04.2014)
	pmc_enable_periph_clk(RESET_DELAY_EN_PIO_ID);
	ioport_set_pin_dir(RESET_DELAY_EN_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(RESET_DELAY_EN_GPIO, IOPORT_PIN_LEVEL_HIGH);	// default state is HIGH (OG 10.04.2014)
	
		
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
	//gpio_configure_pin(SPI0_NPCS0_GPIO, SPI0_NPCS0_FLAGS); // Controled by software
	
	/* Configure an SPI peripheral. */
	spi_enable_clock(SPI0);
	spi_disable(SPI0);
	spi_reset(SPI0);
	spi_set_lastxfer(SPI0);
	spi_set_master_mode(SPI0);
	spi_disable_mode_fault_detect(SPI0);
	
	/* Set variable chip select */
	spi_set_variable_peripheral_select(SPI0);
	
	/* Configure delay SPI channel */
	spi_set_clock_polarity(SPI0, SPI_CHIP_SEL, SPI_CLK_POLARITY);
	spi_set_clock_phase(SPI0, SPI_CHIP_SEL, SPI_CLK_PHASE);
	spi_set_bits_per_transfer(SPI0, SPI_CHIP_SEL, SPI_CSR_BITS_11_BIT);
	spi_configure_cs_behavior(SPI0, SPI_CHIP_SEL, SPI_CS_RISE_FORCED);
	spi_set_baudrate_div(SPI0, SPI_CHIP_SEL, (sysclk_get_cpu_hz() / gs_ul_spi_clock));
	spi_set_transfer_delay(SPI0, SPI_CHIP_SEL, SPI_DLYBS, SPI_DLYBCT);
	
	/* Configure RF atten SPI channel */
	spi_set_clock_polarity(SPI0, SPI_ALT_CHIP_SEL, SPI_CLK_POLARITY);
	spi_set_clock_phase(SPI0, SPI_ALT_CHIP_SEL, SPI_CLK_PHASE);
	spi_set_bits_per_transfer(SPI0, SPI_ALT_CHIP_SEL, SPI_CSR_BITS_16_BIT);
	spi_configure_cs_behavior(SPI0, SPI_ALT_CHIP_SEL, SPI_CS_RISE_FORCED);
	spi_set_baudrate_div(SPI0, SPI_ALT_CHIP_SEL, (sysclk_get_cpu_hz() / gs_ul_spi_clock));
	spi_set_transfer_delay(SPI0, SPI_ALT_CHIP_SEL, SPI_DLYBS, SPI_DLYBCT);
	
	/* Enable SPI */
	spi_enable(SPI0);
}

uint16_t is_reset_delay_enabled(void)
{
	return is_reset_delay_en;
}

uint16_t is_data_delay_enabled(void)
{
	return is_data_delay_en;
}

uint16_t is_clock_delay_enabled(void)
{
	return is_clock_delay_en;
}

void enable_disable_reset_delay(uint16_t status)
{
	if(status == TRUE)
	{
		is_reset_delay_en = TRUE;
		ioport_set_pin_level(RESET_DELAY_EN_GPIO, IOPORT_PIN_LEVEL_LOW);
	}
	else
	{
		is_reset_delay_en = FALSE;
		ioport_set_pin_level(RESET_DELAY_EN_GPIO, IOPORT_PIN_LEVEL_HIGH);
	}
}

void enable_disable_data_delay(uint16_t status)
{
	if(status == TRUE)
	{
		is_data_delay_en = TRUE;
		ioport_set_pin_level(DATA_DELAY_EN_GPIO, IOPORT_PIN_LEVEL_LOW);
	}
	else
	{
		is_data_delay_en = FALSE;
		ioport_set_pin_level(DATA_DELAY_EN_GPIO, IOPORT_PIN_LEVEL_HIGH);
	}
}

void enable_disable_clock_delay(uint16_t status)
{
	if(status == TRUE)
	{
		is_clock_delay_en = TRUE;
		ioport_set_pin_level(CLOCK_DELAY_EN_GPIO, IOPORT_PIN_LEVEL_LOW);
	}
	else
	{
		is_clock_delay_en = FALSE;
		ioport_set_pin_level(CLOCK_DELAY_EN_GPIO, IOPORT_PIN_LEVEL_HIGH);
	}
}

void setup_delay(uint16_t channel, uint16_t PD1, uint16_t PD0)
{
	volatile uint32_t data;
	
	__attribute__((unused)) uint16_t dummy;
	__attribute__((unused)) uint8_t dummy2;
		
	// Clear receive buffer
	//while((spi_read_status(SPI0) & SPI_SR_RDRF) == 0);
	spi_read(SPI0, &dummy, &dummy2);
		
	data = PD1 << 2;
	data |= 0x03;
	data = __RBIT(data);
	data >>= (32-11);
	spi_write(SPI0, data, SPI_CHIP_SEL, 0);
	/* Wait transfer done. */
	while((spi_read_status(SPI0) & SPI_SR_RDRF) == 0);
	spi_read(SPI0, &dummy, &dummy2);
	
	ioport_set_pin_level(channel, IOPORT_PIN_LEVEL_HIGH);
	for(int i=0;i<30;i++);
	ioport_set_pin_level(channel, IOPORT_PIN_LEVEL_LOW);	
	
	data = PD0 << 2;
	data |= 0x02;
	data = __RBIT(data);
	data >>= (32-11);
	spi_write(SPI0, data, SPI_CHIP_SEL, 0);
	/* Wait transfer done. */
	while((spi_read_status(SPI0) & SPI_SR_RDRF) == 0);
	spi_read(SPI0, &dummy, &dummy2);
	
	ioport_set_pin_level(channel, IOPORT_PIN_LEVEL_HIGH);
	for(int i=0;i<30;i++);
	ioport_set_pin_level(channel, IOPORT_PIN_LEVEL_LOW);
}

void setup_rf_attenuator(uint16_t atten)
{	
	volatile uint32_t data;
	
	__attribute__((unused)) uint16_t dummy;
	__attribute__((unused)) uint8_t dummy2;
	
	current_attenution = atten;
	
	// Clear receive buffer
	//while((spi_read_status(SPI0) & SPI_SR_RDRF) == 0);
	spi_read(SPI0, &dummy, &dummy2);
	
	data = (uint32_t)atten & 0x007F;
	data = __RBIT(data);
	data >>= 16;
	spi_write(SPI0, data, SPI_ALT_CHIP_SEL, 0);
	/* Wait transfer done. */
	while((spi_read_status(SPI0) & SPI_SR_RDRF) == 0);
	spi_read(SPI0, &dummy, &dummy2);
	
	ioport_set_pin_level(RF_ATTEN_SLOAD_GPIO, IOPORT_PIN_LEVEL_HIGH);
	for(int i=0;i<30;i++);
	ioport_set_pin_level(RF_ATTEN_SLOAD_GPIO, IOPORT_PIN_LEVEL_LOW);
}

uint16_t get_current_attenuation(void)
{
	return current_attenution;
}

void SetDelayClock(uint16_t delay)
{
	current_clock_delay = delay;
	delay = (delay +5 ) / 11;
	setup_delay(CLOCK_DELAY_SLOAD_GPIO, delay < 512 ? 0:(delay-511), delay < 512 ? delay : 511);	
}

uint16_t GetDelayClock(void)
{
	return current_clock_delay;
}

void SetDelayReset(uint16_t delay)
{
	current_reset_delay = delay;
	delay = (delay +5 ) / 11;
	setup_delay(RESET_DELAY_SLOAD_GPIO, delay < 512 ? 0:(delay-511), delay < 512 ? delay : 511);
}

uint16_t GetDelayReset(void)
{
	return current_reset_delay;
}

void SetDelayData(uint16_t delay)
{
	current_data_delay = delay;
	delay = (delay +5 ) / 11;
	setup_delay(DATA_DELAY_SLOAD_GPIO, delay < 512 ? 0:(delay-511), delay < 512 ? delay : 511);
}

uint16_t GetDelayData(void)
{
	return current_data_delay;
}

// Here should be all the initialization functions for the module after 12v power
void init_module_peripherals_ap(void)
{
	
}

// Here should be all the deinitialization functions for the module before 12v power removal
void deinit_module_peripherals(void)
{
	
}

