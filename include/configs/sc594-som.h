/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * (C) Copyright 2022 - Analog Devices, Inc.
 *
 * Written and/or maintained by Timesys Corporation
 *
 * Contact: Nathan Barrett-Morrison <nathan.morrison@timesys.com>
 * Contact: Greg Malysa <greg.malysa@timesys.com>
 *
 */

#ifndef __CONFIG_SC594_SOM_H
#define __CONFIG_SC594_SOM_H

#include <asm/arch-adi/sc5xx/config.h>
#include <linux/sizes.h>

/*
 * Processor Settings
 */
#define CONFIG_LOADADDR	0xA2000000
#define CONFIG_DTBLOADADDR	"0xA4000000"
#ifdef CONFIG_ADI_CARRIER_SOMCRR_EZLITE
#define CONFIG_MACH_TYPE MACH_TYPE_SC594_SOM_EZLITE;
#else
#define CONFIG_MACH_TYPE MACH_TYPE_SC594_SOM_EZKIT;
#endif

#define CONFIG_SYS_BOOTM_LEN 0x1800000

//L2 SRAM:
//TEXT:   0x20080000 to 0x200AFFFF (192 KB)
//BSS:    0x200B0000 to 0x200BFFFF (64  KB)
//MALLOC: 0x200C0000 to 0x200CFFFF (64  KB)
//STACK:  0x200D0000 to 0x200E4FFF (80  KB)
#define CONFIG_SPL_MAX_SIZE		(SZ_128K + SZ_64K)
#ifdef CONFIG_SPL_BUILD
#define CONFIG_SKIP_LOWLEVEL_INIT
#define CONFIG_SPL_BSS_START_ADDR	0x200B0000
#define CONFIG_SPL_BSS_MAX_SIZE		SZ_64K
#define CONFIG_SYS_SPL_MALLOC_START	(CONFIG_SPL_BSS_START_ADDR + CONFIG_SPL_BSS_MAX_SIZE)
#define CONFIG_SYS_SPL_MALLOC_SIZE	SZ_64K
#define CONFIG_SPL_STACK		(CONFIG_SYS_SPL_MALLOC_START + CONFIG_SYS_SPL_MALLOC_SIZE + CONFIG_SPL_STACK_SIZE)
#define CONFIG_SPL_STACK_SIZE		(SZ_64K + SZ_16K)

//Parameters used for Falcon boot
#define CFG_SYS_SPI_ARGS_OFFS   0xA0000    // This is where the DTB should be stored
#define CFG_SYS_SPI_ARGS_SIZE   0x10000    // Max size of the DTB
#define CFG_SYS_SPI_KERNEL_OFFS 0x120000   // Where the kernel Image or FIT should be stored
#define CONFIG_SYS_SPL_ARGS_ADDR   0xA0000000 // Where to load the DTB into RAM
#define CONFIG_SYS_LOAD_ADDR       0xA2000000 // Where to load the Image into RAM
#define CONFIG_SYS_SPI_KERNEL_SKIP_HEADER
#endif

/*
 * Memory Settings
 */
#define MEM_IS43TR16512BL
#define MEM_ISSI_8Gb_DDR3_800MHZ
#define MEM_DMC0

#define CFG_SYS_SDRAM_BASE	0xA2000000 //Remaining region is reserved for SHARC
#define CFG_SYS_SDRAM_SIZE	0x1E000000 //480 MB is available to ARM
#ifndef CONFIG_SPL_BUILD
#define CONFIG_SYS_LOAD_ADDR	0x0
#endif
#define HAS_CUSTOM_SYS_INIT_SP_ADDR
#define CUSTOM_SYS_INIT_SP_ADDR (CFG_SYS_SDRAM_BASE + 0x3f000)

#define CONFIG_SYS_MONITOR_LEN	(0)
#define CONFIG_SYS_MALLOC_LEN	(1024 * 1024)

/*
 * Network Settings
 */
#ifdef CONFIG_ADI_CARRIER_SOMCRR_EZLITE
#define CONFIG_DTBNAME		"sc594-som-ezlite.dtb"
#else
#define CONFIG_DTBNAME		"sc594-som-ezkit.dtb"
#endif
#define CONFIG_HOSTNAME		"sc59x"
#define CONFIG_DW_ALTDESCRIPTOR
#define CONFIG_DW_AXI_BURST_LEN 16

/* OSPI - Via Device Tree Support
 *
 */
#define CONFIG_CQSPI_REF_CLK		500000000

/*
 * SPI - Via Device Tree Support
 */
#define ADI_OSPI_SF_DEFAULT_SPEED CONFIG_CQSPI_REF_CLK / 6 //83.33 MHz STR (Single Transfer Rate)

/*
 * Misc Settings
 */
#define ADI_LINUX_MEMSIZE	"480M"

#define CONFIG_BOOTCOMMAND	"run ospiboot"
#define INITRAMADDR "0xA8000000"

#define ADI_ENV_SETTINGS \
	"usb_pgood_delay=1000\0" \
	"initrd_high=0xFFFFFFFF\0" \
	"fdt_high=0xFFFFFFFF\0" \
	ADI_INIT_ETHERNET \
	ADI_NFS_BOOT \
	ADI_RAM_BOOT \
	ADI_UPDATE_SPI_UBOOT_ONLY \
	ADI_ERASE_SPI \
	ADI_UPDATE_SPI \
	ADI_ERASE_OSPI \
	ADI_OSPI_BOOT \
	ADI_SPI_BOOT

/* Per-board QSPI/OSPI Partitioning Offsets (64MB/32MB):
 *
 * 0x0000000 - 0x003FFFF : U-Boot Stage 1     (  256KB)
 * 0x0040000 - 0x00FFFFF : U-Boot Stage 2     (  768KB)
 * 0x0100000 - 0x011FFFF : U-Boot Environment (  128KB)
 * 0x0120000 - 0x101FFFF : FIT or DTB+zImage  (15360KB)
 * 0x1020000 - end       : Root File System
 */
#define ADI_SPI_SIZE     "0x4000000" //64MB
#define ADI_OSPI_SIZE    "0x2000000" //32MB
#define ADI_UBOOT_OFFSET "0x40000"
#define ADI_IMG_OFFSET   "0x0120000"
#define ADI_RFS_OFFSET   "0x1020000"
#define ADI_JFFS2_FILE   "minimal" //use the adsp-sc5xx-minimal image

/* Push button polarties -- used for Falcon boot interrupt */
#define ADI_PB1_POLARITY 1
#define ADI_PB2_POLARITY 1

#include <configs/sc_adi_common.h>

#endif
