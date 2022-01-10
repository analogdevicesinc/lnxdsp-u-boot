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
#include <asm/arch/sc59x_64.h>
#include <asm/arch-sc59x-64/dwmmc.h>
#include <linux/delay.h>
#include <watchdog.h>
#include "soft_switch.h"
#include <asm/armv8/mmu.h>
#include <asm/spl.h>
#include "../../../arch/arm/cpu/armv8/sc59x-64/adsp598.h"


int adi_mmc_init()
{
	int ret = 0;

	static const unsigned short pins[] = P_EMSI0;
	if (!peripheral_request_list(pins, "emsi0")){
		return -1;
	}

	return ret;
}