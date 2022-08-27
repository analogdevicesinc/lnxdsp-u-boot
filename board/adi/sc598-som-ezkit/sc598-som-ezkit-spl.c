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
#include <asm/mach-types.h>
#include <asm/mach-adi/common/sc5xx.h>
#include <linux/delay.h>
#include <linux/stringify.h>
#include <watchdog.h>
#include "soft_switch.h"
#include <asm/armv8/mmu.h>
#include <asm/spl.h>
#include <asm/mach-adi/common/rcu.h>
#include "sc598-som-ezkit-shared.h"

#define pRCU_STAT 0x3108c004

extern void adi_check_pushbuttons(void);

static int adi_sf_default_bus = 2;
static int adi_sf_default_cs = 1;
static int adi_sf_default_speed = CONFIG_SF_DEFAULT_SPEED;
static char * adi_kernel_bootargs;
static u32 bmode;

struct adi_boot_args {
	u32 addr;
	u32 flags;
	u32 cmd;
};

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

	bmode = (readl(pRCU_STAT) & BITM_RCU_STAT_BMODE) >> BITP_RCU_STAT_BMODE;

#ifdef CONFIG_ADI_FALCON
	//Check for push button press to determine if we're falling back into U-Boot Proper
	adi_check_pushbuttons();
#endif

#if CONFIG_ADI_SPL_FORCE_BMODE != 0
	//In case we want to force boot sequences such as:
	//QSPI -> OSPI
	//QSPI -> eMMC

	//If this is not set, then we will always try to use the BMODE setting
	//for both stages... i.e.
	//QSPI -> QSPI
	if(bmode != 0 && bmode != 3) //(Don't allow skipping JTAG/UART BMODE settings)
		bmode = CONFIG_ADI_SPL_FORCE_BMODE;
#endif

#if ADI_HAVE_CARRIER == 1
#ifdef CONFIG_SOFT_SWITCH
	adi_initialize_soft_switches();
#endif
#endif

	if (bmode >= 0 && bmode <= sizeof(bmodes)/sizeof(bmodes[0]))
		bmodeString = bmodes[bmode];
	else
		bmode = 0;

	printf("ADI Boot Mode: %x (%s)\n", bmode, bmodeString);

#ifdef CONFIG_ADI_FALCON

	//Push button was pressed -- let the boot rom start U-boot Proper
	if(bmode != 0 && spl_start_uboot()){
		spl_boot_list[0] = BOOT_DEVICE_BOOTROM;
		return;
	}

	switch(bmode){
		case 0:
			printf("SPL execution has completed.  Please load U-Boot Proper via JTAG");
			while(1);
			break;
		case 1:
			adi_sf_default_bus = 2;
			adi_sf_default_cs = 1;
			adi_kernel_bootargs = ADI_BOOTARGS_SPI;
			spl_boot_list[0] = BOOT_DEVICE_SPI;
			break;
		case 5:
			adi_sf_default_bus = 0;
			adi_sf_default_cs = 0;
			adi_kernel_bootargs = ADI_BOOTARGS_SPI;
			spl_boot_list[0] = BOOT_DEVICE_SPI;
			break;
#ifdef CONFIG_MMC_SDHCI_ADI
		case 6:
			adi_kernel_bootargs = ADI_BOOTARGS_EMMC;
			spl_boot_list[0] = BOOT_DEVICE_MMC1;
			break;
#endif
		default:
			spl_boot_list[0] = BOOT_DEVICE_BOOTROM;
			break;
	}
#else
	if (0 == bmode) {
		printf("SPL execution has completed.  Please load U-Boot Proper via JTAG");
		while(1)
			;
	}

	// Everything goes back to bootrom where we'll read table parameters and ask it
	// to load something
	spl_boot_list[0] = BOOT_DEVICE_BOOTROM;
#endif
}

int dram_init_banksize(void)
{
	adi_initcode_shared();
	return 0;
}

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

void adi_fdt_fixup_mac_addr(void * fdt){

	int i = 0, j, prop;
	char *tmp, *end;
	const char *path;
	unsigned char mac_addr[6];
	int offset;

	if (fdt_path_offset(fdt, "/aliases") < 0)
		return;

	/* Cycle through all aliases */
	for (prop = 0; ; prop++) {
		const char *name;

		/* FDT might have been edited, recompute the offset */
		offset = fdt_first_property_offset(fdt,
			fdt_path_offset(fdt, "/aliases"));
		/* Select property number 'prop' */
		for (j = 0; j < prop; j++)
			offset = fdt_next_property_offset(fdt, offset);

		if (offset < 0)
			break;

		path = fdt_getprop_by_offset(fdt, offset, &name, NULL);
		if (!strncmp(name, "ethernet", 8)) {
			/* Treat plain "ethernet" same as "ethernet0". */
			if (!strcmp(name, "ethernet"))
				i = 0;
			else
				i = trailing_strtol(name);

			if(i == 0)
				tmp = ETH0ADDR;
			else
				tmp = ETH1ADDR;

			for (j = 0; j < 6; j++) {
				mac_addr[j] = tmp ?
					      simple_strtoul(tmp, &end, 16) : 0;
				if (tmp)
					tmp = (*end) ? end + 1 : end;
			}

			do_fixup_by_path(fdt, path, "mac-address",
					 &mac_addr, 6, 0);
			do_fixup_by_path(fdt, path, "local-mac-address",
					 &mac_addr, 6, 1);
		}
	}

}

void adi_fdt_fixup_kernel_bootargs(void * fdt){
	if(bmode == 5){ //OSPI
		char temp_bootargs[2048];
		memcpy(temp_bootargs, adi_kernel_bootargs, strlen(adi_kernel_bootargs)+1);
		cadence_ospi_append_chipinfo(temp_bootargs);
		do_fixup_by_path(fdt, "/chosen", "bootargs", temp_bootargs, strlen(temp_bootargs)+1, 0);
	}else{
		do_fixup_by_path(fdt, "/chosen", "bootargs", adi_kernel_bootargs, strlen(adi_kernel_bootargs)+1, 0);
	}
}

int spl_board_fixup_fdt(void * fdt){
	adi_fdt_fixup_mac_addr(fdt);
	adi_fdt_fixup_kernel_bootargs(fdt);
}

#if defined(CONFIG_SPL_LOAD_FIT)
int board_fit_config_name_match(const char *name)
{
	return 0;
}
#endif

#endif
