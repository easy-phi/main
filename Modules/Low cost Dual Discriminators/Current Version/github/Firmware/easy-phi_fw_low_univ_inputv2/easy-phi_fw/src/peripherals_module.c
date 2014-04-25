/*
 * peripherals_module.c
 *
 * Created: 15/11/2013 14:47:11
 *  Author: stephan
 */ 
#include "peripherals_module.h"
#include "defines.h"

#define EN_10MHZ_GPIO					(PIO_PD5_IDX)
#define EN_10MHZ_PIO_ID					ID_PIOD
#define EN_10MHZ_ACTIVE_LEVEL			IOPORT_PIN_LEVEL_LOW
#define EN_10MHZ_INACTIVE_LEVEL			IOPORT_PIN_LEVEL_HIGH
#define FPGA_INT_PIO_ID					ID_PIOC
#define GPIO_FPGA_INT					(PIO_PC16_IDX)

/* Chip select. */
#define FPGA_CHIP_SEL 0
#define DAC_CHIP_SEL  1
/* Clock polarity. */
#define SPI_CLK_POLARITY 1
/* Clock phase. */
#define SPI_CLK_PHASE 1
/* Minimum delay for CS high */
#define DLYBCS		100
/* Delay before SPCK. */
#define SPI_DLYBS	0
/* Delay between consecutive transfers. */
#define SPI_DLYBCT	0
/* SPI clock setting (Hz). */
static uint32_t gs_ul_spi_clock = 3000000;

/* Current discriminator settings */
uint16_t current_channel1_threshold = 0;
uint16_t current_channel2_threshold = 0;
uint16_t current_channel1_offset = 0;
uint16_t current_channel2_offset = 0;
uint16_t is_channel1_dc_coupled = FALSE;
uint16_t is_channel2_dc_coupled = FALSE;
uint16_t is_channel1_offsetted = FALSE;
uint16_t is_channel2_offsetted = FALSE;
uint32_t last_counter1_meas = 0;
uint32_t last_counter2_meas = 0;

/* switch lights off bool */
uint16_t lights_off = FALSE;


void switch_on_off_lights(uint16_t bool_lights)
{
	lights_off = bool_lights;
	// Here we should send something to the FPGA....
}

uint16_t fpga_send_spi_16bits(uint16_t data)
{
	uint16_t return_val;
	uint16_t pcs;
	
	spi_write(SPI0, data, FPGA_CHIP_SEL, 0);
	/* Wait to receive data */
	while((spi_read_status(SPI0) & SPI_SR_RDRF) == 0);;
	spi_read(SPI0, &return_val, &pcs);
	
	return return_val;
}

// Check if we have a FPGA counter reading available
RET_TYPE get_fpga_read_available_flag(void)
{
	if(ioport_get_pin_level(GPIO_FPGA_INT))
		return RETURN_OK;
	else
		return RETURN_NOK;
}

// Read counters values
void read_counters(uint32_t* counter1_pt, uint32_t* counter2_pt)
{
	uint32_t temp_counter1_val = 0;
	uint32_t temp_counter2_val = 0;
	uint16_t temp_val;
	
	/* Clear flag */
	fpga_send_spi_16bits(0x4000);
	/* Ask counter 1 LSB */
	fpga_send_spi_16bits(0);
	/* Read counter 1 LSB, ask MSB */
	temp_val = fpga_send_spi_16bits(1);
	/* Ask counter 2 LSB, read counter 1 MSB */
	temp_counter1_val = (uint32_t)fpga_send_spi_16bits(2);
	temp_counter1_val = (temp_counter1_val << 16) & 0x01FF0000;
	temp_counter1_val += temp_val;
	/* Ask counter 2 MSB, read LSB */
	temp_val = fpga_send_spi_16bits(3);
	/* Clear flag & read counter 2 LSB */
	temp_counter2_val = (uint32_t)fpga_send_spi_16bits(4);
	temp_counter2_val = (temp_counter2_val << 16) & 0x01FF0000;
	temp_counter2_val += temp_val;
	
	if(temp_counter1_val < last_counter1_meas)
	{
		*counter1_pt = 0x02000000 - last_counter1_meas;
		*counter1_pt += temp_counter1_val;
	}
	else
	{
		*counter1_pt = temp_counter1_val - last_counter1_meas;
	}
	
	if(temp_counter2_val < last_counter2_meas)
	{
		*counter2_pt = 0x02000000 - last_counter2_meas;
		*counter2_pt += temp_counter2_val;
	}
	else
	{
		*counter2_pt = temp_counter2_val - last_counter2_meas;
	}
	
	last_counter1_meas = temp_counter1_val;
	last_counter2_meas = temp_counter2_val;
}

void send_relays_data_to_fpga(void)
{
	uint16_t temp_val = 0;
	
	if(is_channel1_dc_coupled == TRUE)
		temp_val |= 0x0001;
	if(is_channel2_dc_coupled == TRUE)
		temp_val |= 0x0002;
	if(is_channel1_offsetted == TRUE)
		temp_val |= 0x0004;
	if(is_channel2_offsetted == TRUE)
		temp_val |= 0x0008;
		
	fpga_send_spi_16bits(260);
	Nop();
	fpga_send_spi_16bits(temp_val);
	asm("NOP");asm("NOP");asm("NOP");asm("NOP");
}

void set_channel1_coupling(uint16_t is_dc_coupled)
{
	is_channel1_dc_coupled = is_dc_coupled;
	send_relays_data_to_fpga();
}

uint16_t get_channel1_coupling(void)
{
	return is_channel1_dc_coupled;
}

void set_channel2_coupling(uint16_t is_dc_coupled)
{
	is_channel2_dc_coupled = is_dc_coupled;
	send_relays_data_to_fpga();
}

uint16_t get_channel2_coupling(void)
{
	return is_channel2_dc_coupled;
}

void set_channel1_offset_relay(uint16_t is_offsetted)
{
	is_channel1_offsetted = is_offsetted;
	send_relays_data_to_fpga();
}

uint16_t get_channel1_offset_relay(void)
{
	return is_channel1_offsetted;
}

void set_channel2_offset_relay(uint16_t is_offsetted)
{
	is_channel2_offsetted = is_offsetted;
	send_relays_data_to_fpga();
}

uint16_t get_channel2_offset_relay(void)
{
	return is_channel2_offsetted;
}

void set_threshold_channel1(uint16_t threshold)
{
	volatile uint16_t return_val;
	
	current_channel1_threshold = threshold;
	spi_write(SPI0, (0x1000 | threshold), DAC_CHIP_SEL, 0);
	while((spi_read_status(SPI0) & SPI_SR_RDRF) == 0);;
	spi_read(SPI0, &return_val, DAC_CHIP_SEL);
}

uint16_t get_channel1_threshold(void)
{
	return current_channel1_threshold;
}

void set_threshold_channel2(uint16_t threshold)
{
	volatile uint16_t return_val;
	
	current_channel2_threshold = threshold;
	spi_write(SPI0, (0xD000 | threshold), DAC_CHIP_SEL, 0);
	while((spi_read_status(SPI0) & SPI_SR_RDRF) == 0);;
	spi_read(SPI0, &return_val, DAC_CHIP_SEL);
}

uint16_t get_channel2_threshold(void)
{
	return current_channel2_threshold;
}

void set_offset_channel1(uint16_t offset)
{
	volatile uint16_t return_val;
	
	current_channel1_offset = offset;
	spi_write(SPI0, (0x5000 | offset), DAC_CHIP_SEL, 0);
	while((spi_read_status(SPI0) & SPI_SR_RDRF) == 0);;
	spi_read(SPI0, &return_val, DAC_CHIP_SEL);
}

uint16_t get_offset_channel1(void)
{
	return current_channel1_offset;
}

void set_offset_channel2(uint16_t offset)
{
	volatile uint16_t return_val;
	
	current_channel2_offset = offset;
	spi_write(SPI0, (0x9000 | offset), DAC_CHIP_SEL, 0);
	while((spi_read_status(SPI0) & SPI_SR_RDRF) == 0);;
	spi_read(SPI0, &return_val, DAC_CHIP_SEL);
}

uint16_t get_offset_channel2(void)
{
	return current_channel2_offset;
}

// Here should be all the initialization functions for the module before 12v power
void init_module_peripherals_bp(void)
{
	
}

// Here should be all the initialization functions for the module after 12v power
void init_module_peripherals_ap(void)
{
	gpio_configure_pin(SPI0_MISO_GPIO, SPI0_MISO_FLAGS);
	gpio_configure_pin(SPI0_MOSI_GPIO, SPI0_MOSI_FLAGS);
	gpio_configure_pin(SPI0_SPCK_GPIO, SPI0_SPCK_FLAGS);
	gpio_configure_pin(SPI0_NPCS0_GPIO, SPI0_NPCS0_FLAGS);
	gpio_configure_pin(SPI0_NPCS1_PA29_GPIO, SPI0_NPCS1_PA29_FLAGS);
	
	/* Configure an SPI peripheral. */
	spi_enable_clock(SPI0);
	spi_disable(SPI0);
	spi_reset(SPI0);
	spi_set_lastxfer(SPI0);
	spi_set_master_mode(SPI0);
	spi_disable_mode_fault_detect(SPI0);
	
	/* Set variable chip select */
	spi_set_variable_peripheral_select(SPI0);
	
	/* Configure FPGA SPI channel */
	spi_set_clock_polarity(SPI0, FPGA_CHIP_SEL, SPI_CLK_POLARITY);
	spi_set_clock_phase(SPI0, FPGA_CHIP_SEL, SPI_CLK_PHASE);
	spi_set_bits_per_transfer(SPI0, FPGA_CHIP_SEL, SPI_CSR_BITS_16_BIT);
	spi_configure_cs_behavior(SPI0, FPGA_CHIP_SEL, SPI_CS_RISE_FORCED);
	spi_set_baudrate_div(SPI0, FPGA_CHIP_SEL, (sysclk_get_cpu_hz() / gs_ul_spi_clock));
	spi_set_transfer_delay(SPI0, FPGA_CHIP_SEL, SPI_DLYBS, SPI_DLYBCT);
	
	/* Configure DAC SPI channel */
	spi_set_clock_polarity(SPI0, DAC_CHIP_SEL, SPI_CLK_POLARITY);
	spi_set_clock_phase(SPI0, DAC_CHIP_SEL, SPI_CLK_PHASE);
	spi_set_bits_per_transfer(SPI0, DAC_CHIP_SEL, SPI_CSR_BITS_16_BIT);
	spi_configure_cs_behavior(SPI0, DAC_CHIP_SEL, SPI_CS_RISE_FORCED);
	spi_set_baudrate_div(SPI0, DAC_CHIP_SEL, (sysclk_get_cpu_hz() / gs_ul_spi_clock));
	spi_set_transfer_delay(SPI0, DAC_CHIP_SEL, SPI_DLYBS, SPI_DLYBCT);
	spi_set_delay_between_chip_select(SPI0, DLYBCS);
	
	/* Enable SPI */
	spi_enable(SPI0);		
		
	/* Enable 10MHz clock */
	pmc_enable_periph_clk(EN_10MHZ_PIO_ID);
	ioport_set_pin_dir(EN_10MHZ_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(EN_10MHZ_GPIO, EN_10MHZ_ACTIVE_LEVEL);	
	
	/* FPGA INT input */	
	pmc_enable_periph_clk(FPGA_INT_PIO_ID);
	ioport_set_pin_dir(GPIO_FPGA_INT, IOPORT_DIR_INPUT);	
	ioport_set_pin_mode(GPIO_FPGA_INT, IOPORT_MODE_GLITCH_FILTER);
}

// Here should be all the deinitialization functions for the module before 12v power removal
void deinit_module_peripherals(void)
{
	
}
