/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * (C) Copyright 2022 - Analog Devices, Inc.
 *
 * Written and/or maintained by Timesys Corporation
 *
 * Contact: Nathan Barrett-Morrison <nathan.morrison@timesys.com>
 * Contact: Greg Malysa <greg.malysa@timesys.com>
 *
 * U-boot - The SC598 EZKITs have pinmux conflicts between OSPI/QSPI/UART0
 *
 * UART0 uses PA6+7
 * OSPI0 uses PA0+1+2+3+4+5+6+7+8+9,PD4
 * SPI2 uses  PA0+1+2+3+4+5
 *
 */

#include <common.h>
#include <netdev.h>
#include <phy.h>
#include <asm/io.h>
#include <asm/gpio.h>
#include <asm/mach-types.h>
#include <asm/arch-adi/sc5xx/sc5xx.h>
#include <linux/delay.h>
#include <watchdog.h>
#include <asm/armv8/mmu.h>
#include <asm/spl.h>
#include <dm/pinctrl.h>
#include <asm/arch-adi/sc5xx/sc59x-shared.h>
#include <asm/arch-adi/sc5xx-64/sc598-som-ezkit-dynamic-qspi-ospi-uart-mux.h>

extern bool uartEnabled;
extern bool uartReadyToEnable;

void enable_ospi_mux(){
	//TODO: Consider storing dev pointers instead of looking it up every time
	struct udevice *dev;
	struct uclass *uc;
	int ret;

	ret = uclass_get(UCLASS_SPI, &uc);
	if (ret){
		return;
	}
	uclass_foreach_dev(dev, uc) {
		if(strcmp("ospi", dev->name) == 0){
			pinctrl_select_state(dev, "default");
		}
	}
}

void disable_ospi_mux(){
	//TODO: Consider storing dev pointers instead of looking it up every time
	struct udevice *dev;
	struct uclass *uc;
	int ret;

	ret = uclass_get(UCLASS_SPI, &uc);
	if (ret){
		return;
	}
	uclass_foreach_dev(dev, uc) {
		if(strcmp("spi2", dev->name) == 0){
			pinctrl_select_state(dev, "default");
		}
	}

	ret = uclass_get(UCLASS_SERIAL, &uc);
	if (ret){
		return;
	}
	uclass_foreach_dev(dev, uc) {
		if(strcmp("serial@0x31003000", dev->name) == 0){;
			pinctrl_select_state(dev, "default");
		}
	}
}

int adi_enable_ospi()
{
	//TODO, set these once instead of calling
	//gpio_hog_lookup_name every time
	struct gpio_desc *ftdi;
	struct gpio_desc *octal;
	struct gpio_desc *uart0;
	struct gpio_desc *spi2flash_cs;
	struct gpio_desc *spi2d2_d3;

	gpio_hog_lookup_name("~ftdi-usb-en", &ftdi);
	gpio_hog_lookup_name("octal-spi-cs-en", &octal);
	gpio_hog_lookup_name("~uart0-en", &uart0);
	gpio_hog_lookup_name("~spi2flash-cs", &spi2flash_cs);
	gpio_hog_lookup_name("~spi2d2-d3-en", &spi2d2_d3);

	if (CONFIG_UART_CONSOLE != 0)
		return 0;

	uartReadyToEnable = 0;

	if(uartEnabled != 0){
		u8 currentVal;

		uartEnabled = 0;

		serial_suspend();

		//PortA on Address 22 -- Disable the FTDI
		//PortB on Address 22 -- Enable Octal SPI CS
		//PortA on Address 20 -- Disable UART0, SPI2D2_D3, SPI2FLASH_CS
		dm_gpio_set_value(ftdi, 1);
		dm_gpio_set_value(octal, 1);
		dm_gpio_set_value(uart0, 1);
		dm_gpio_set_value(spi2flash_cs, 1);
		dm_gpio_set_value(spi2d2_d3, 1);

		enable_ospi_mux();
	}

	return 0;
}

int adi_disable_ospi(bool changeMuxImmediately)
{
	//TODO, set these once instead of calling
	//gpio_hog_lookup_name every time
	struct gpio_desc *ftdi;
	struct gpio_desc *octal;
	struct gpio_desc *uart0;
	struct gpio_desc *spi2flash_cs;
	struct gpio_desc *spi2d2_d3;

	gpio_hog_lookup_name("~ftdi-usb-en", &ftdi);
	gpio_hog_lookup_name("octal-spi-cs-en", &octal);
	gpio_hog_lookup_name("~uart0-en", &uart0);
	gpio_hog_lookup_name("~spi2flash-cs", &spi2flash_cs);
	gpio_hog_lookup_name("~spi2d2-d3-en", &spi2d2_d3);

	if (CONFIG_UART_CONSOLE != 0)
		return 0;

	if(!changeMuxImmediately){
		uartReadyToEnable = 1;
	}else if(uartEnabled != 1){
		u8 currentVal;

		uartReadyToEnable = 0;

		disable_ospi_mux();

		serial_resume();

		//PortA on Address 22 -- Enable the FTDI
		//PortB on Address 22 -- Disable Octal SPI CS
		//PortA on Address 20 -- Enable UART0, SPI2D2_D3, SPI2FLASH_CS
		dm_gpio_set_value(ftdi, 0);
		dm_gpio_set_value(octal, 0);
		dm_gpio_set_value(uart0, 0);
		dm_gpio_set_value(spi2flash_cs, 0);
		dm_gpio_set_value(spi2d2_d3, 0);

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
