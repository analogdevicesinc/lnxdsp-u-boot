/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * (C) Copyright 2024 - Analog Devices, Inc.
 *
 * Contact: Nathan Barrett-Morrison <nathan.morrison@timesys.com>
 * Contact: Greg Malysa <greg.malysa@timesys.com>
 *
 */

#ifndef __CONFIG_SC589_H
#define __CONFIG_SC589_H

// #include <asm/arch-adi/sc5xx/config.h> // TODO Fix
#include <linux/sizes.h>

#include <configs/sc5xx-common.h>

/*
 * Processor Settings
 */
#define CONFIG_LOADADDR	0xC3000000
#define CONFIG_DTBLOADADDR	"0xC4000000"
#define CONFIG_SYS_BOOTM_LEN 0x1800000

/*
 * Memory Settings
 */
#define MEM_MT41K128M16JT
#define MEM_DMC0
#define MEM_DMC1

#define CFG_SYS_SDRAM_BASE	0xC2000000
#define CFG_SYS_SDRAM_SIZE	0xe000000
#define CONFIG_SYS_LOAD_ADDR	0x0
#define HAS_CUSTOM_SYS_INIT_SP_ADDR
#define CUSTOM_SYS_INIT_SP_ADDR (CFG_SYS_SDRAM_BASE + 0x3f000)

#define CONFIG_SPL_MAX_SIZE SZ_128K

#ifdef CONFIG_SPL_BUILD
	#define CONFIG_SKIP_LOWLEVEL_INIT
	#define CONFIG_SPL_BSS_START_ADDR	0x20080000
	#define CONFIG_SPL_BSS_MAX_SIZE		SZ_64K
	#define CONFIG_SYS_SPL_MALLOC_START	(CONFIG_SPL_BSS_START_ADDR + CONFIG_SPL_BSS_MAX_SIZE)
	#define CONFIG_SYS_SPL_MALLOC_SIZE	SZ_64K
	#define CONFIG_SPL_STACK			(CONFIG_SYS_SPL_MALLOC_START + CONFIG_SYS_SPL_MALLOC_SIZE + CONFIG_SPL_STACK_SIZE)
	#define CONFIG_SPL_STACK_SIZE		SZ_64K
#endif

#define CONFIG_SYS_MONITOR_LEN	(0)
#define CONFIG_SYS_MALLOC_LEN	(1024 * 1024)

/*
 * Network Settings
 */
#define CONFIG_HOSTNAME		"sc58x"
#define CONFIG_DW_ALTDESCRIPTOR
#define CONFIG_DW_AXI_BURST_LEN 16

/*
 * USB Settings
 */
#define MUSB_HW_VERSION2
#define CONFIG_USB_STORAGE

/*
 * Misc Settings
 */
#define CONFIG_BOOTCOMMAND	"run ramboot"
#define INITRAMADDR "0xC5000000"

/*
 * Per-board Environment Settings
 */

#define ADI_ENV_SETTINGS \
	"initrd_high=0xFFFFFFFF\0" \
	"fdt_high=0xFFFFFFFF\0" \
	ADI_INIT_ETHERNET \
	ADI_NFS_BOOT \
	ADI_RAM_BOOT \
	ADI_MMC_BOOT \
	ADI_UPDATE_SPI_UBOOT_ONLY \
	ADI_ERASE_SPI \
	ADI_UPDATE_SPI \
	ADI_SPI_BOOT

#endif