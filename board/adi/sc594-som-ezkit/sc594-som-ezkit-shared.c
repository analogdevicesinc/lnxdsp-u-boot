/*
 * U-boot - SPL management
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
#include <asm/arch/sc59x.h>
#include <asm/arch-sc59x/dwmmc.h>
#include <linux/delay.h>
#include <watchdog.h>
#include "soft_switch.h"
#include <asm/spl.h>

void spi_flash_override_defaults(unsigned int * bus,
				unsigned int * cs,
				unsigned int * speed,
				unsigned int * mode)
{
	if(*bus == 0 && *cs == 0){
		*speed = ADI_OSPI_SF_DEFAULT_SPEED;
	}else{
		*speed = CONFIG_SF_DEFAULT_SPEED;
	}
}

void adi_multiplex_ospi(){
#ifdef CONFIG_CADENCE_QSPI
	static const unsigned short pins_ospi0[] = P_OSPI0;
	if (peripheral_request_list(pins_ospi0, "ospi0")){
		printf("Unable to pinmux OSPI0\r\n");
	}
#endif
}

void adi_setup_soft_switches(){
#ifdef CONFIG_SOFT_SWITCH
	static const unsigned short pins_i2c2[] = P_I2C2;
	if (peripheral_request_list(pins_i2c2, "i2c2")){
		printf("Unable to pinmux I2C2\r\n");
	}
	setup_soft_switches(switch_config_array_ethernet_enabled, NUM_SWITCH);
#endif
}