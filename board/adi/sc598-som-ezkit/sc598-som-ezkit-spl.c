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
#include <asm/arch/sc59x_64.h>
#include <asm/arch-sc59x-64/dwmmc.h>
#include <linux/delay.h>
#include <watchdog.h>
#include "soft_switch.h"
#include <asm/armv8/mmu.h>
#include <asm/spl.h>
#include "../../../arch/arm/cpu/armv8/sc59x-64/adsp598.h"
#include "sc598-som-ezkit-shared.h"

#define pRCU_STAT 0x3108c004

static int adi_sf_default_bus = 2;
static int adi_sf_default_cs = 1;
static int adi_sf_default_speed = CONFIG_SF_DEFAULT_SPEED;

void board_boot_order(u32 *spl_boot_list)
{
	static char * bmodes[] = {
		"JTAG/BOOTROM",
		"QSPI Master",
		"QSPI Slave",
		"UART",
		"LP0 Slave",
		"OSPI",
		"eMMC"
	};

	char * bmodeString = "unknown";

	u32 bmode = (readl(pRCU_STAT) & BITM_RCU_STAT_BMODE) >> BITP_RCU_STAT_BMODE;

	if(bmode >= 0 && bmode <= sizeof(bmodes)/sizeof(bmodes[0])){
		bmodeString = bmodes[bmode];
	}

#if ADI_HAVE_CARRIER == 1
#ifdef CONFIG_SOFT_SWITCH
	adi_initialize_soft_switches();
#endif
#endif

	printf("ADI Boot Mode: %x (%s)\n", bmode, bmodeString);

	switch(bmode){
		case 0:
			printf("SPL execution has completed.  Please load U-Boot Proper via JTAG");
			while(1);
			break;
		case 1:
			adi_sf_default_bus = 2;
			adi_sf_default_cs = 1;
			spl_boot_list[0] = BOOT_DEVICE_SPI;
			break;
		case 5:
			adi_sf_default_bus = 0;
			adi_sf_default_cs = 0;
			spl_boot_list[0] = BOOT_DEVICE_SPI;
			break;
#ifdef CONFIG_MMC_SDHCI_ADI
		case 6:
			adi_mmc_init();
			spl_boot_list[0] = BOOT_DEVICE_MMC1;
			break;
#endif
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

u64 bootrom_stash[32] __attribute__((section(".data")));

int board_return_to_bootrom(struct spl_image_info *spl_image,
			    struct spl_boot_device *bootdev)
{
	asm volatile ("bl back_to_bootrom;");
	return 0;
}

int spl_start_uboot(void)
{
	return 1;
}

unsigned long spl_mmc_get_uboot_raw_sector(struct mmc *mmc,
						  unsigned long raw_sect)
{
	unsigned int mmc_sector_offs = 0;

#if CONFIG_IS_ENABLED(OF_CONTROL) && !CONFIG_IS_ENABLED(OF_PLATDATA)
    mmc_sector_offs = fdtdec_get_config_int(gd->fdt_blob,
                                         "u-boot,spl-mmc-sector-offset",
                                         mmc_sector_offs);
#endif

	return mmc_sector_offs;
}


unsigned int spl_spi_get_default_bus()
{
	return adi_sf_default_bus;
}

unsigned int spl_spi_get_default_cs()
{
	return adi_sf_default_cs;
}

unsigned int spl_spi_get_default_speed()
{
	if(adi_sf_default_bus == 0 && adi_sf_default_cs == 0){
		adi_sf_default_speed = ADI_OSPI_SF_DEFAULT_SPEED;
	}

	return adi_sf_default_speed;
}
