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
#include <asm/mach-adi/common/sc5xx.h>
#include <asm/mach-adi/common/dwmmc.h>
#include <linux/delay.h>
#include <watchdog.h>

void spi_flash_override_defaults(unsigned int * bus,
				unsigned int * cs,
				unsigned int * speed,
				unsigned int * mode);

void adi_multiplex_ospi();

int adi_initialize_soft_switches();
int adi_enable_ethernet_softconfig();
int adi_disable_ethernet_softconfig();

extern struct switch_config switch_config_array_current_state[NUM_SWITCH];
