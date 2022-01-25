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
#include "../../../arch/arm/cpu/armv7/sc59x/adsp594.h"

#define pRCU_STAT 0x3108c004

void board_boot_order(u32 *spl_boot_list)
{
	static char * bmodes[] = {
		"JTAG/BOOTROM",
		"QSPI Master",
		"QSPI Slave",
		"UART",
		"LP0 Slave",
		"OSPI",
	};

	char * bmodeString = "unknown";

	u32 bmode = (readl(pRCU_STAT) & BITM_RCU_STAT_BMODE) >> BITP_RCU_STAT_BMODE;

	if(bmode >= 0 && bmode <= sizeof(bmodes)/sizeof(bmodes[0])){
		bmodeString = bmodes[bmode];
	}

	printf("ADI Boot Mode: %x (%s)\n", bmode, bmodeString);

	switch(bmode){
		case 0:
			printf("SPL execution has completed.  Please load U-Boot Proper via JTAG");
			while(1);
			break;
		case 1:
			spl_boot_list[0] = BOOT_DEVICE_SPI;
			break;
		case 5:
			printf("Loading U-Boot Proper from OSPI not yet supported.  This is a future task.\n");
			break;
		default:
			spl_boot_list[0] = BOOT_DEVICE_BOOTROM;
			break;
	}
}

int dram_init_banksize(void)
{
	initcode_shared();
	return 0;
}

int spl_start_uboot(void)
{
	return 1;
}

u32 bootrom_stash __attribute__((section(".data")));

int board_return_to_bootrom(struct spl_image_info *spl_image,
			    struct spl_boot_device *bootdev)
{
	asm volatile ("bl back_to_bootrom;");
	return 0;
}

