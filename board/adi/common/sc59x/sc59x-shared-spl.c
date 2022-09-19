/*
 * U-boot - SPL management
 *
 * Copyright (c) 2022 Analog Devices Inc.
 *
 * Licensed under the GPL-2 or later.
 */

#include "sc59x-shared-spl.h"
#include <asm/io.h>

#ifdef CONFIG_SC59X
static int adi_sf_default_bus = 0;
static int adi_sf_default_cs = 0;
#endif

#ifdef CONFIG_SC59X_64
static int adi_sf_default_bus = 2;
static int adi_sf_default_cs = 1;
#endif

static int adi_sf_default_speed = CONFIG_SF_DEFAULT_SPEED;
static char * adi_kernel_bootargs;
u32 bmode;

void board_boot_order(u32 *spl_boot_list)
{
	static char * bmodes[] = {
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

	if (bmode >= 0 && bmode <= sizeof(bmodes)/sizeof(bmodes[0]))
		bmodeString = bmodes[bmode];
	else
		bmode = 0;

	printf("ADI Boot Mode: %x (%s)\n", bmode, bmodeString);

#if defined(CONFIG_ADI_FALCON) || defined(CONFIG_SC59X)

#ifdef CONFIG_SC59X_64
	//Push button was pressed -- let the boot rom start U-boot Proper
	if(bmode != 0 && spl_start_uboot()){
		spl_boot_list[0] = BOOT_DEVICE_BOOTROM;
		return;
	}
#endif

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
#if defined(CONFIG_MMC_SDHCI_ADI) && defined(CONFIG_SC59X_64)
		case 6:
			adi_kernel_bootargs = ADI_BOOTARGS_EMMC;
			spl_boot_list[0] = BOOT_DEVICE_MMC1;
			break;
#endif
		default:
			spl_boot_list[0] = BOOT_DEVICE_BOOTROM;
			break;
	}

#elif defined(CONFIG_SC59X_64)
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
	writel(0x1, 0x30643108); /* SCB6 A55 M0 Ib.fn Mod */
	isb();

	/* configure DDR prefetch behavior, per ADI */
	writel(0x1, 0x31076000);

	/* configure smart mode, per ADI */
	writel(0x1307, 0x31076004);

	/* configure spu0 */
	for (i = 0; i < 214; ++i) {
		writel(0, 0x3108BA00 + 4*i);
	}

	/* configure spu0 wp */
	for (i = 0; i < 214; ++i) {
		writel(0, 0x3108B400 + i*4);
	}

	/* configure smpus permissively */
	for (i = 0; i < ARRAY_SIZE(smpus); ++i) {
		writel(0x500, smpus[i]);
	}
#endif

	adi_initcode_shared();
	return 0;
}

unsigned int spl_spi_get_default_speed()
{
	if(adi_sf_default_bus == 0 && adi_sf_default_cs == 0){
		adi_sf_default_speed = ADI_OSPI_SF_DEFAULT_SPEED;
	}

	return adi_sf_default_speed;
}

unsigned int spl_spi_get_default_bus()
{
	return adi_sf_default_bus;
}

unsigned int spl_spi_get_default_cs()
{
	return adi_sf_default_cs;
}

#ifdef CONFIG_ADI_FALCON
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
	return 0;
}
#endif
