/*
 * U-boot - SPL management
 *
 * Copyright (c) 2022 Analog Devices Inc.
 *
 * Licensed under the GPL-2 or later.
 */

#include "../common/sc59x/sc59x-shared-spl.h"
#include <asm/armv8/mmu.h>

u64 bootrom_stash[32] __attribute__((section(".data")));

// cf. include/adi/cortex-a55/defSC59x_rom_jumptable.h
void (*adi_rom_boot)(void *addr, uint32_t flags, int32_t blocks, void *pHook,
uint32_t cmd) = 0x000000E4;

int board_return_to_bootrom(struct spl_image_info *spl_image,
			    struct spl_boot_device *bootdev)
{
	// Table 43-14 in SC598 hardware reference manual
	static const struct adi_boot_args rom_boot_args[] = {
		// JTAG/no boot
		[0] = {0, 0, 0},
		// SPI master, used for qspi as well
		[1] = {0x60020000, 0x00040000, 0x00000207},
		// SPI slave
		[2] = {0, 0, 0x00000212},
		// UART slave
		[3] = {0, 0, 0x00000013},
		// Linkport slave
		[4] = {0, 0, 0x00000014},
		// OSPI master
		[5] = {0x60020000, 0, 0x00000008},
		// eMMC
		[6] = {0x200, 0, 0x00082009},
		// reserved, also no boot
		[7] = {0, 0, 0}
	};

#if CONFIG_ADI_SPL_FORCE_BMODE != 0
	// see above
	if(bmode != 0 && bmode != 3)
		bmode = CONFIG_ADI_SPL_FORCE_BMODE;
#endif

	if (bmode >= (sizeof(rom_boot_args) / sizeof(rom_boot_args[0])))
		bmode = 0;

	adi_rom_boot(rom_boot_args[bmode].addr,
		rom_boot_args[bmode].flags,
		0, NULL,
		rom_boot_args[bmode].cmd);
	return 0;
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

#ifdef CONFIG_ADI_FALCON
void spl_board_prepare_ethernet(void){
	// select RGMII, little endian for both ports
	writel((readl(REG_PADS0_PCFG0) | 0xc), REG_PADS0_PCFG0);
	writel(readl(REG_PADS0_PCFG0) & ~(1 << 19), REG_PADS0_PCFG0);
	writel(readl(REG_PADS0_PCFG0) & ~(1 << 20), REG_PADS0_PCFG0);
}

void spl_board_prepare_for_linux(void)
{
	spl_board_prepare_ethernet();
	adi_disable_ospi(1);
}

#if defined(CONFIG_SPL_LOAD_FIT)
int board_fit_config_name_match(const char *name)
{
	return 0;
}
#endif

#endif
