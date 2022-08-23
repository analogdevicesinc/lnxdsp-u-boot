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
#include <asm/mach-adi/common/sc5xx.h>
#include <asm/arch-sc59x/dwmmc.h>
#include <linux/delay.h>
#include <watchdog.h>
#include "soft_switch.h"
#include <asm/spl.h>
#include <asm/mach-adi/common/rcu.h>
#include "sc594-som-ezkit-shared.h"

#define pRCU_STAT 0x3108c004

static int adi_sf_default_bus = 0;
static int adi_sf_default_cs = 0;
static int adi_sf_default_speed = CONFIG_SF_DEFAULT_SPEED;
static bool adi_start_uboot_proper = 1;
static char * adi_kernel_bootargs;
static u32 bmode;

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

	bmode = (readl(pRCU_STAT) & BITM_RCU_STAT_BMODE) >> BITP_RCU_STAT_BMODE;

#ifdef CONFIG_ADI_FALCON
	adi_check_pushbuttons();
#endif

#if CONFIG_ADI_SPL_FORCE_BMODE != 0
	//In case we want to force boot sequences such as:
	//QSPI -> OSPI

	//If this is not set, then we will always try to use the BMODE setting
	//for both stages... i.e.
	//QSPI -> QSPI
	if(bmode != 0 && bmode != 3) //(Don't allow skipping JTAG/UART BMODE settings)
		bmode = CONFIG_ADI_SPL_FORCE_BMODE;
#endif

	if(bmode >= 0 && bmode <= sizeof(bmodes)/sizeof(bmodes[0])){
		bmodeString = bmodes[bmode];
	}

	printf("ADI Boot Mode: %x (%s)\n", bmode, bmodeString);

//#if ADI_HAVE_CARRIER == 1
#ifdef CONFIG_SOFT_SWITCH
	adi_initialize_soft_switches();
#endif
//#endif

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
			adi_multiplex_ospi();
			spl_boot_list[0] = BOOT_DEVICE_SPI;
			break;
		default:
			spl_boot_list[0] = BOOT_DEVICE_BOOTROM;
			break;
	}
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

int dram_init_banksize(void)
{
	adi_initcode_shared();
	return 0;
}

int spl_start_uboot(void)
{
	return adi_start_uboot_proper;
}

u32 bootrom_stash __attribute__((section(".data")));

int board_return_to_bootrom(struct spl_image_info *spl_image,
			    struct spl_boot_device *bootdev)
{
	asm volatile ("bl back_to_bootrom;");
	return 0;
}

#ifdef CONFIG_ADI_FALCON
void adi_check_pushbuttons(){
	gpio_request(GPIO_PD0, "pushbutton0");
	gpio_direction_input(GPIO_PD0);

	gpio_request(GPIO_PH0, "pushbutton1");
	gpio_direction_input(GPIO_PH0);

	if(gpio_get_value(GPIO_PD0) || gpio_get_value(GPIO_PH0)){
		adi_start_uboot_proper = 1;

		//Wait until they're released, in case these pins conflict with peripherals (OSPI, etc)
		while(gpio_get_value(GPIO_PD0));
		while(gpio_get_value(GPIO_PH0));
	}else{
		adi_start_uboot_proper = 0;
	}

	gpio_free(GPIO_PD0);
	gpio_free(GPIO_PH0);
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
	return 0;
}
#endif
