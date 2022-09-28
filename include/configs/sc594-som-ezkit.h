// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * (C) Copyright 2022 - Analog Devices, Inc.
 *
 * Written and/or maintained by Timesys Corporation
 *
 * Contact: Nathan Barrett-Morrison <nathan.morrison@timesys.com>
 * Contact: Greg Malysa <greg.malysa@timesys.com>
 *
 */

#ifndef __CONFIG_SC594_EZKIT_H
#define __CONFIG_SC594_EZKIT_H

#include <asm/mach-adi/common/config.h>
#include <linux/sizes.h>

/*
 * Processor Settings
 */
#define CONFIG_LOADADDR	0x82000000
#define CONFIG_DTBLOADADDR	"0x84000000"
#define CONFIG_MACH_TYPE	MACH_TYPE_SC594_SOM_EZKIT
#define CONFIG_SYS_BOOTM_LEN 0x1800000

//L2 SRAM:
//TEXT:   0x20080000 to 0x2009FFFF (128 KB)
//BSS:    0x200A0000 to 0x200AFFFF (64  KB)
//MALLOC: 0x200B0000 to 0x200BFFFF (64  KB)
//STACK:  0x200C0000 to 0x200CFFFF (64  KB)
#define CONFIG_SPL_MAX_SIZE		SZ_128K //(overkill)
#ifdef CONFIG_SPL_BUILD
#define CONFIG_SKIP_LOWLEVEL_INIT
#define CONFIG_SPL_BSS_START_ADDR	0x200A0000
#define CONFIG_SPL_BSS_MAX_SIZE		SZ_64K
#define CONFIG_SYS_SPL_MALLOC_START	(CONFIG_SPL_BSS_START_ADDR + CONFIG_SPL_BSS_MAX_SIZE)
#define CONFIG_SYS_SPL_MALLOC_SIZE	SZ_64K
#define CONFIG_SPL_STACK		(CONFIG_SYS_SPL_MALLOC_START + CONFIG_SYS_SPL_MALLOC_SIZE + CONFIG_SPL_STACK_SIZE)
#define CONFIG_SPL_STACK_SIZE		SZ_64K

//Parameters used for Falcon boot
#define CONFIG_SYS_SPI_ARGS_OFFS   0xA0000    // This is where the DTB should be stored
#define CONFIG_SYS_SPI_ARGS_SIZE   0x10000    // Max size of the DTB
#define CONFIG_SYS_SPI_KERNEL_OFFS 0xC0000   // Where the kernel Image should be stored
#define CONFIG_SYS_SPL_ARGS_ADDR   0x84000000 // Where to load the DTB into RAM
#define CONFIG_SYS_LOAD_ADDR       0x82000000 // Where to load the Image into RAM
#define CONFIG_SYS_SPI_KERNEL_SKIP_HEADER
#endif

/*
 * Memory Settings
 */
#define MEM_IS43TR16512BL
#define MEM_ISSI_8Gb_DDR3_800MHZ
#define MEM_DMC0

#define CONFIG_SYS_SDRAM_BASE	0x82000000
#define CONFIG_SYS_SDRAM_SIZE	0x3E000000
#define CONFIG_SYS_TEXT_BASE	0xB2200000
#ifndef CONFIG_SPL_BUILD
#define CONFIG_SYS_LOAD_ADDR	0x0
#endif
#define CONFIG_SYS_INIT_SP_ADDR (CONFIG_SYS_SDRAM_BASE + 0x3f000)

#define CONFIG_SYS_MONITOR_LEN	(0)
#define CONFIG_SYS_MALLOC_LEN	(1024 * 1024)

/*
 * Network Settings
 */
#define CONFIG_DTBNAME		"sc594-som-ezkit.dtb"
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
#define ADI_LINUX_MEMSIZE	"992M"

#define CONFIG_BOOTCOMMAND	"run ospiboot"
#define INITRAMADDR "0x85000000"

#define ADI_ENV_SETTINGS \
	"usb_pgood_delay=1000\0" \
	"initrd_high=0xFFFFFFFF\0" \
	"fdt_high=0xFFFFFFFF\0" \
	ADI_INIT_ETHERNET \
	ADI_NFS_BOOT \
	ADI_RAM_BOOT \
	ADI_UPDATE_SPI_UBOOT_ONLY \
	ADI_UPDATE_SPI \
	ADI_OSPI_BOOT \
	ADI_SPI_BOOT

/* Per-board QSPI/OSPI Partitioning Offsets (64MB/32MB):
 *
 * 0x0000000 - 0x001FFFF : U-Boot Stage 1     (  128KB)
 * 0x0020000 - 0x00DFFFF : U-Boot Stage 2     (  768KB)
 * 0x00E0000 - 0x00FFFFF : U-Boot Environment (  128KB)
 * 0x0100000 - 0x08FFFFF : FIT or DTB+zImage  ( 8196KB)
 * 0x0900000 - end       : Root File System
 */
#define ADI_SPI_SIZE     "0x4000000" //64MB
#define ADI_OSPI_SIZE    "0x2000000" //32MB
#define ADI_UBOOT_OFFSET "0x20000"
#define ADI_IMG_OFFSET   "0x0100000"
#define ADI_RFS_OFFSET   "0x0900000"
#define ADI_JFFS2_FILE   "minimal" //use the adsp-sc5xx-minimal image

#include <configs/sc_adi_common.h>

#endif
