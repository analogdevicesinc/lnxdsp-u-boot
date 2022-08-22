/*
 * U-boot - The SC598 EZKITs have pinmux conflicts between OSPI/QSPI/UART0
 *
 * UART0 uses PA6+7
 * OSPI0 uses PA0+1+2+3+4+5+6+7+8+9,PD4
 * SPI2 uses  PA0+1+2+3+4+5
 *
 * Copyright (c) 2022 Analog Devices Inc.
 *
 * Licensed under the GPL-2 or later.
 */

#include <common.h>
#include <netdev.h>
#include <phy.h>
#include <asm/io.h>
#include <asm/gpio.h>
#include <asm/mach-types.h>
#include <asm/arch/portmux.h>
#include <asm/arch/sc59x_64.h>
#include <asm/arch-sc59x-64/dwmmc.h>
#include <linux/delay.h>
#include <watchdog.h>
#include "soft_switch.h"
#include <asm/armv8/mmu.h>
#include <asm/spl.h>
#include "sc598-som-ezkit-shared.h"
#include <adi/59x-64/sc598-som-ezkit-dynamic-qspi-ospi-uart-mux.h>

static const unsigned short pins_ospi0[] = P_OSPI0;
static const unsigned short pins_qspi2[] = QSPI_PINS_SWITCH(2);
static const uint16_t *uart_pins = { P_UART0_TX, P_UART0_RX, 0 };

extern bool uartEnabled;
extern bool uartReadyToEnable;

int adi_enable_ospi()
{
	if (CONFIG_UART_CONSOLE != 0)
		return 0;

	uartReadyToEnable = 0;

	if(uartEnabled != 0){
		u8 currentVal;

		uartEnabled = 0;

		serial_suspend();

		//PortA on Address 22 -- Disable the FTDI
		currentVal = switch_config_array_current_state[0].value0;
		currentVal &= ~FTDI_USB(1);
		currentVal |= DISABLE_FTDI_USB;
		switch_config_array_current_state[0].value0 = currentVal;
		switch_config_array_current_state[0].pullup0 = currentVal;

		//PortB on Address 22 -- Enable Octal SPI CS
		currentVal = switch_config_array_current_state[0].value1;
		currentVal &= ~OCTAL_SPI_CS(1);
		currentVal |= ENABLE_OCTAL_SPI_CS;
		switch_config_array_current_state[0].value1 = currentVal;
		switch_config_array_current_state[0].pullup1 = currentVal;

		//PortA on Address 20 -- Disable UART0, SPI2D2_D3, SPI2FLASH_CS
		currentVal = switch_config_array_current_state[1].value0;
		currentVal &= ~UART0(1);
		currentVal &= ~SPI2FLASH_CS(1);
		currentVal &= ~SPI2D2_D3(1);
		currentVal |= DISABLE_UART0 | DISABLE_SPI2FLASH_CS | DISABLE_SPI2D2_D3;
		switch_config_array_current_state[1].value0 = currentVal;
		switch_config_array_current_state[1].pullup0 = currentVal;

		setup_soft_switches(switch_config_array_current_state, NUM_SWITCH);

		//If OSPI is being used, then these pins cannot be muxed
		peripheral_free_list(uart_pins);
		peripheral_free_list(pins_qspi2);
		peripheral_free_list(pins_ospi0);
		peripheral_request_list(pins_ospi0, "ospi0");
	}

	return 0;
}

int adi_disable_ospi(bool changeMuxImmediately)
{
	if (CONFIG_UART_CONSOLE != 0)
		return 0;

	if(!changeMuxImmediately){
		uartReadyToEnable = 1;
	}else if(uartEnabled != 1){
		u8 currentVal;

		uartReadyToEnable = 0;

		//If OSPI is being used, then these pins cannot be muxed
		peripheral_free_list(uart_pins);
		peripheral_free_list(pins_qspi2);
		peripheral_free_list(pins_ospi0);
		peripheral_request_list(uart_pins, "adi-uart4");
		peripheral_request_list(pins_qspi2, "adi-qspi2");

		serial_resume();

		//PortA on Address 22 -- Enable the FTDI
		currentVal = switch_config_array_current_state[0].value0;
		currentVal &= ~FTDI_USB(1);
		currentVal |= ENABLE_FTDI_USB;
		switch_config_array_current_state[0].value0 = currentVal;
		switch_config_array_current_state[0].pullup0 = currentVal;

		//PortB on Address 22 -- Disable Octal SPI CS
		currentVal = switch_config_array_current_state[0].value1;
		currentVal &= ~OCTAL_SPI_CS(1);
		currentVal |= DISABLE_OCTAL_SPI_CS;
		switch_config_array_current_state[0].value1 = currentVal;
		switch_config_array_current_state[0].pullup1 = currentVal;

		//PortA on Address 20 -- Enable UART0, SPI2D2_D3, SPI2FLASH_CS
		currentVal = switch_config_array_current_state[1].value0;
		currentVal &= ~UART0(1);
		currentVal &= ~SPI2FLASH_CS(1);
		currentVal &= ~SPI2D2_D3(1);
		currentVal |= ENABLE_UART0 | ENABLE_SPI2FLASH_CS | ENABLE_SPI2D2_D3;
		switch_config_array_current_state[1].value0 = currentVal;
		switch_config_array_current_state[1].pullup0 = currentVal;

		setup_soft_switches(switch_config_array_current_state, NUM_SWITCH);

		uartEnabled = 1;
	}
}

#ifdef CONFIG_SPL_BUILD
void spl_board_prepare_for_boot(void)
{
	if(spl_spi_get_default_bus() == 0 && spl_spi_get_default_cs() == 0){
		adi_disable_ospi(1);
	}
}
#endif
