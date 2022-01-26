/*
 * U-boot - Shared Functions between SPL/U-Boot Proper
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
#include "../../../arch/arm/cpu/armv7/sc59x/adsp594.h"

void spi_flash_override_defaults(unsigned int * bus,
				unsigned int * cs,
				unsigned int * speed,
				unsigned int * mode);

void adi_multiplex_ospi();
