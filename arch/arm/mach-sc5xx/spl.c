/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * (C) Copyright 2022 - Analog Devices, Inc.
 *
 * Written and/or maintained by Timesys Corporation
 *
 * Contact: Nathan Barrett-Morrison <nathan.morrison@timesys.com>
 * Contact: Greg Malysa <greg.malysa@timesys.com>
 */

#include <asm-generic/gpio.h>
#include <spl.h>
#include <asm/arch-adi/sc5xx/spl.h>
#include "init/init.h"

static bool adi_start_uboot_proper;

static int adi_sf_default_bus = CONFIG_SF_DEFAULT_BUS;
static int adi_sf_default_cs = CONFIG_SF_DEFAULT_CS;
static int adi_sf_default_speed = CONFIG_SF_DEFAULT_SPEED;

#if defined(CONFIG_ADI_FALCON)
static char *adi_kernel_bootargs;
static u32 initramfs_start;
static u32 initramfs_len;
#endif

u32 bmode;

int spl_start_uboot(void)
{
	return adi_start_uboot_proper;
}

unsigned int spl_spi_get_default_speed(void)
{
#if defined(CONFIG_SC59X) || defined(CONFIG_SC59X_64)
	if (adi_sf_default_bus == 0 && adi_sf_default_cs == 0) {
		adi_sf_default_speed = ADI_OSPI_SF_DEFAULT_SPEED;
	}
#endif

	return adi_sf_default_speed;
}

unsigned int spl_spi_get_default_bus(void)
{
	return adi_sf_default_bus;
}

unsigned int spl_spi_get_default_cs(void)
{
	return adi_sf_default_cs;
}

void board_boot_order(u32 *spl_boot_list)
{
	static char *bmodes[] = {
		"JTAG/BOOTROM",
		"QSPI Master",
		"QSPI Slave",
		"UART",
		"LP0 Slave",
		"OSPI",
#ifdef CONFIG_SC59X_64
		"eMMC"
#endif
	};

	char *bmodeString = "unknown";

	bmode = (readl(pRCU_STAT) & BITM_RCU_STAT_BMODE) >> BITP_RCU_STAT_BMODE;

#ifdef CONFIG_ADI_FALCON
	//Check for push button press to determine if we're falling back into U-Boot Proper
	adi_check_pushbuttons(ADI_PB1_POLARITY, ADI_PB2_POLARITY);
#endif

#if CONFIG_ADI_SPL_FORCE_BMODE != 0
	//In case we want to force boot sequences such as:
	//QSPI -> OSPI
	//QSPI -> eMMC

	//If this is not set, then we will always try to use the BMODE setting
	//for both stages... i.e.
	//QSPI -> QSPI
	if (bmode != 0 && bmode != 3) //(Don't allow skipping JTAG/UART BMODE settings)
		bmode = CONFIG_ADI_SPL_FORCE_BMODE;
#endif

	if (bmode >= 0 && bmode <= ARRAY_SIZE(bmodes))
		bmodeString = bmodes[bmode];
	else
		bmode = 0;

	printf("ADI Boot Mode: 0x%x (%s)\n", bmode, bmodeString);

#if defined(CONFIG_ADI_FALCON)
	//Push button was pressed -- let the boot rom start U-boot Proper
	if (bmode != 0 && spl_start_uboot()) {
		spl_boot_list[0] = BOOT_DEVICE_BOOTROM;
		return;
	}

	switch (bmode) {
		case 0:
			printf("SPL execution has completed.  Please load U-Boot Proper via JTAG");
			while (1);
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
#if defined(CONFIG_MMC_SDHCI_ADI) && defined(CONFIG_SC59X_64)
		case 6:
			adi_kernel_bootargs = ADI_BOOTARGS_MMC;
			spl_boot_list[0] = BOOT_DEVICE_MMC1;
			break;
#endif
		default:
			spl_boot_list[0] = BOOT_DEVICE_BOOTROM;
			break;
	}

#else
	if (bmode == 0) {
		printf("SPL execution has completed.  Please load U-Boot Proper via JTAG");
		while (1)
			;
	}

	// Everything goes back to bootrom where we'll read table parameters and ask it
	// to load something
	spl_boot_list[0] = BOOT_DEVICE_BOOTROM;
#endif
}

int32_t __weak adi_rom_boot_hook(struct ADI_ROM_BOOT_CONFIG *config, int32_t cause) {
	return 0;
}

void (*adi_rom_boot)(void *, uint32_t, int32_t, void *, uint32_t) = ADI_ROM_ADDRESS;

int board_return_to_bootrom(struct spl_image_info *spl_image,
			    struct spl_boot_device *bootdev)
{
#if CONFIG_ADI_SPL_FORCE_BMODE != 0
	// see above
	if (bmode != 0 && bmode != 3)
		bmode = CONFIG_ADI_SPL_FORCE_BMODE;
#endif

	if (bmode >= (ARRAY_SIZE(adi_rom_boot_args)))
		bmode = 0;

	adi_rom_boot((void *)adi_rom_boot_args[bmode].addr,
		     adi_rom_boot_args[bmode].flags,
		0, &adi_rom_boot_hook,
		adi_rom_boot_args[bmode].cmd);
	return 0;
};

int dram_init_banksize(void)
{
	return 0;
}

void board_init_f(ulong dummy)
{
	int ret;

#ifdef CONFIG_SC59X_64
	void __iomem *smpus[] = {
		0x31007800, //SMPU0
		0x31083800, //SMPU2
		0x31084800, //SMPU3
		0x31085800, //SMPU4
		0x31086800, //SMPU5
		0x31087800, //SMPU6
		0x310A0800, //SMPU9
		0x310A1800, //SMPU11
		0x31012800, //SMPU12
	};
	size_t i;

	/* Alter outstanding transactions property of A55*/
	writel(0x3, 0x30643108); /* SCB6 A55 M0 Ib.fn Mod */
	isb();

	/* disable DDR prefetch behavior */
	writel(0x0, 0x31076000);

	/* configure smart mode, per ADI */
	writel(0x1307, 0x31076004);

	/* configure spu0 */
	for (i = 0; i < 214; ++i) {
		writel(0, 0x3108BA00 + 4 * i);
	}

	/* configure spu0 wp */
	for (i = 0; i < 214; ++i) {
		writel(0, 0x3108B400 + i * 4);
	}

	/* configure smpus permissively */
	for (i = 0; i < ARRAY_SIZE(smpus); ++i) {
		writel(0x500, smpus[i]);
	}
#endif

	adi_initcode_shared();

	ret = spl_early_init();
	if (ret)
		panic("spl_early_init() failed\n");

	preloader_console_init();

#ifdef CONFIG_SET_SHARC_IDLE
	ret = set_sharc_cores_idle();
	if (ret)
		printf("Warn: failed to set SHARC cores idle (%d)\n", ret);
#endif
}

#ifdef CONFIG_ADI_FALCON
void adi_check_pushbuttons(bool active1, bool active2)
{
	struct gpio_desc *pb0;
	struct gpio_desc *pb1;
	struct gpio_desc *pbEn;

	gpio_hog_lookup_name("pushbutton0", &pb0);
	gpio_hog_lookup_name("pushbutton1", &pb1);
	gpio_hog_lookup_name("pushbutton-en", &pbEn);

	//Assert push button enablement on GPIO expander
	if (pbEn)
		dm_gpio_set_value(pbEn, 1);

	if (!pb0 || !pb1)
		return;

	if ((dm_gpio_get_value(pb0) == active1) || (dm_gpio_get_value(pb1) == active2)) {
		printf("Pushbutton helding during boot -- entering U-Boot Proper");
		adi_start_uboot_proper = 1;
		//Wait until they're released, in case these pins conflict with peripherals (OSPI, etc)
		while ((dm_gpio_get_value(pb0) == active1));
		while ((dm_gpio_get_value(pb1) == active2));
	} else {
		adi_start_uboot_proper = 0;
	}
}

void adi_fdt_fixup_mac_addr(void *fdt)
{
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

			if (i == 0)
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

void adi_fdt_fixup_kernel_bootargs(void *fdt)
{
	if (bmode == 5) { //OSPI
		char temp_bootargs[2048];

		memcpy(temp_bootargs, adi_kernel_bootargs, strlen(adi_kernel_bootargs) + 1);
		cadence_ospi_append_chipinfo(temp_bootargs);
		do_fixup_by_path(fdt, "/chosen", "bootargs", temp_bootargs, strlen(temp_bootargs) + 1, 0);
	} else {
		do_fixup_by_path(fdt, "/chosen", "bootargs", adi_kernel_bootargs, strlen(adi_kernel_bootargs) + 1, 0);
	}
}

#if defined(CONFIG_ADI_FALCON)
void adi_store_initramfs_addr(u32 start, uint32_t len)
{
	initramfs_start = start;
	initramfs_len = len;
}
#endif

int spl_board_fixup_fdt(void *fdt)
{
	adi_fdt_fixup_mac_addr(fdt);
	adi_fdt_fixup_kernel_bootargs(fdt);

#if defined(CONFIG_ADI_FALCON)
	fdt_initrd(fdt, initramfs_start, initramfs_start + initramfs_len);
#endif

	return 0;
}
#endif
