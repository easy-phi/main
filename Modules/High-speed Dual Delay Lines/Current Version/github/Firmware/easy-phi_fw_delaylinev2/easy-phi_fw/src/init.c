/*
 * init.c
 *
 * Created: 21/10/2013 10:52:30
 *  Author: stephan
 */
 #include <asf.h>
 #include "init.h"
 
 
 void init_board(void)
 {
	/* Configure USB_ID (UOTGID) pin */
	gpio_configure_pin(USB_ID_GPIO, USB_ID_FLAGS);
	/* Configure USB_VBOF (UOTGVBOF) pin */
	gpio_configure_pin(USB_VBOF_GPIO, USB_VBOF_FLAGS);
	 
	/* Configure HSMCI pins */
	gpio_configure_pin(PIN_HSMCI_MCCDA_GPIO, PIN_HSMCI_MCCDA_FLAGS);
	gpio_configure_pin(PIN_HSMCI_MCCK_GPIO, PIN_HSMCI_MCCK_FLAGS);
	gpio_configure_pin(PIN_HSMCI_MCDA0_GPIO, PIN_HSMCI_MCDA0_FLAGS);
	gpio_configure_pin(PIN_HSMCI_MCDA1_GPIO, PIN_HSMCI_MCDA1_FLAGS);
	gpio_configure_pin(PIN_HSMCI_MCDA2_GPIO, PIN_HSMCI_MCDA2_FLAGS);
	gpio_configure_pin(PIN_HSMCI_MCDA3_GPIO, PIN_HSMCI_MCDA3_FLAGS);
	/* Configure SD/MMC card detect pin */
	gpio_configure_pin(SD_MMC_0_CD_GPIO, SD_MMC_0_CD_FLAGS);
	
	/* Configure EEPROM pins **/	
	pmc_enable_periph_clk(ID_TWI0);
	gpio_configure_pin(TWI0_DATA_GPIO, TWI0_DATA_FLAGS);
	gpio_configure_pin(TWI0_CLK_GPIO, TWI0_CLK_FLAGS);
	 
	/* Enable power to the SD card slot */
	pmc_enable_periph_clk(VDD_MCI_PIO_ID);
	ioport_set_pin_dir(VDD_MCI_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(VDD_MCI_GPIO, VDD_MCI_ACTIVE_LEVEL);
	 
	/* Configure PWM LED1&2&3 pins */
	ioport_set_pin_mode(PIN_LED1_GPIO, PIN_LED1_FLAGS);
	ioport_disable_pin(PIN_LED1_GPIO);
	ioport_set_pin_mode(PIN_LED2_GPIO, PIN_LED2_FLAGS);
	ioport_disable_pin(PIN_LED2_GPIO);
	ioport_set_pin_mode(PIN_LED3_GPIO, PIN_LED3_FLAGS);
	ioport_disable_pin(PIN_LED3_GPIO);
	 	 
	/* User button */
	pmc_enable_periph_clk(USR_BUTTON_PIO_ID);
	ioport_set_pin_dir(GPIO_USR_BUTTON, IOPORT_DIR_INPUT);
	ioport_set_pin_mode(GPIO_USR_BUTTON, GPIO_USR_BUTTON_FLAGS);
	 
	/* Sync signals */
	pmc_enable_periph_clk(SYNC_PIO_ID);
	ioport_set_pin_dir(GPIO_SYNC, IOPORT_DIR_INPUT);
	ioport_set_pin_mode(GPIO_SYNC, GPIO_SYNC_FLAGS);
	pmc_enable_periph_clk(SYNC_READ_PIO_ID);
	ioport_set_pin_dir(SYNC_READ_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(SYNC_READ_GPIO, SYNC_READ_ACTIVE_LEVEL);
	pmc_enable_periph_clk(SYNC_WRITE_PIO_ID);
	ioport_set_pin_dir(SYNC_WRITE_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(SYNC_WRITE_GPIO, SYNC_WRITE_INACTIVE_LEVEL);
	
	/* 12v detect */
	pmc_enable_periph_clk(DET_12V_PIO_ID);
	ioport_set_pin_dir(GPIO_DET_12V, IOPORT_DIR_INPUT);
	ioport_set_pin_mode(GPIO_DET_12V, GPIO_DET_12V_FLAGS);
	
	/* 12v power ok */
	pmc_enable_periph_clk(OK_12V_PIO_ID);
	ioport_set_pin_dir(GPIO_OK_12V, IOPORT_DIR_INPUT);
	ioport_set_pin_mode(GPIO_OK_12V, GPIO_OK_12V_FLAGS);
	
	/* 12v enable */	
	pmc_enable_periph_clk(ENABLE_12V_PIO_ID);
	ioport_set_pin_dir(ENABLE_12V_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(ENABLE_12V_GPIO, ENABLE_12V_INACTIVE_LEVEL);
}
