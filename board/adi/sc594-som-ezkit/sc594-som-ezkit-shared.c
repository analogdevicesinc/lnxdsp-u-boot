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
#include <asm/mach-adi/common/sc5xx.h>
#include <asm/mach-adi/common/dwmmc.h>
#include <linux/delay.h>
#include <watchdog.h>
#include "soft_switch.h"
#include <asm/spl.h>

struct switch_config switch_config_array_current_state[NUM_SWITCH];

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

int adi_initialize_soft_switches()
{
#ifdef CONFIG_SOFT_SWITCH
	memcpy((char*)&switch_config_array_current_state[0], 
			(char*)&switch_config_array[0], sizeof(struct switch_config));
	memcpy((char*)&switch_config_array_current_state[1],
			(char*)&switch_config_array[1], sizeof(struct switch_config));

	static const unsigned short pins_i2c2[] = P_I2C2;
	peripheral_request_list(pins_i2c2, "i2c2");

	setup_soft_switches(switch_config_array_current_state, NUM_SWITCH);
#endif
}

int adi_enable_ethernet_softconfig()
{
	u8 currentVal;

	currentVal = switch_config_array_current_state[0].value1;
	currentVal &= ~GIGE_RESET(1);
	currentVal &= ~ETH1_RESET(1);
	currentVal &= ~ETH1_EN(1);
	currentVal |= PortB_Address22_EthEnable;
	switch_config_array_current_state[0].value1 = currentVal;

	setup_soft_switches(switch_config_array_current_state, NUM_SWITCH);
}

int adi_disable_ethernet_softconfig()
{
	u8 currentVal;

	currentVal = switch_config_array_current_state[0].value1;
	currentVal &= ~GIGE_RESET(1);
	currentVal &= ~ETH1_RESET(1);
	currentVal &= ~ETH1_EN(1);
	currentVal |= PortB_Address22_EthDisable;
	switch_config_array_current_state[0].value1 = currentVal;

	setup_soft_switches(switch_config_array_current_state, NUM_SWITCH);
}

#ifdef CONFIG_OF_BOARD_SETUP
int ft_board_setup(void *blob, struct bd_info *bd)
{
	return 0;
}
#endif
