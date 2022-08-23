/*
 * U-boot - Functions which are shared between u-boot proper + u-boot SPL
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
#include <asm/arch-sc59x-64/dwmmc.h>
#include <linux/delay.h>
#include <watchdog.h>
#include "soft_switch.h"
#include <asm/armv8/mmu.h>
#include <asm/spl.h>
#include "sc598-som-ezkit-shared.h"

struct switch_config switch_config_array_current_state[NUM_SWITCH];

int adi_mmc_init()
{
	int ret = 0;
	static const unsigned short pins[] = P_EMSI0;
	if (peripheral_request_list(pins, "emsi0")){
		printf("Error requesting P_EMSI0\r\n");
	}

	return ret;
}

int adi_initialize_soft_switches()
{
	memcpy((char*)&switch_config_array_current_state[0], 
			(char*)&switch_config_array[0], sizeof(struct switch_config));
	memcpy((char*)&switch_config_array_current_state[1],
			(char*)&switch_config_array[1], sizeof(struct switch_config));

	static const unsigned short pins_i2c2[] = P_I2C2;
	if (peripheral_request_list(pins_i2c2, "i2c2")){
		printf("Error requesting P_I2C2\r\n");
	}

	setup_soft_switches(switch_config_array_current_state, NUM_SWITCH);
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
	switch_config_array_current_state[0].pullup1 = currentVal;

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
	switch_config_array_current_state[0].pullup1 = currentVal;

	setup_soft_switches(switch_config_array_current_state, NUM_SWITCH);
}

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

#ifdef CONFIG_OF_BOARD_SETUP
int ft_board_setup(void *blob, struct bd_info *bd)
{
	return 0;
}
#endif
