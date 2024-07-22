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

#ifndef __CONFIG_SC598_SOM_H
#define __CONFIG_SC598_SOM_H

#include <asm/arch-adi/sc5xx/config.h>
#include <linux/sizes.h>
#include <linux/kconfig.h>

#define CONFIG_SYS_BOOTM_LEN 0x1800000

/* GIC */
#define CONFIG_GICV3
#define GICD_BASE 0x31200000
#define GICR_BASE 0x31240000

/*
 * Processor Settings
 */
#define CONFIG_LOADADDR	0x90000000
#define CONFIG_DTBLOADADDR	"0x99000000"
#ifdef CONFIG_ADI_CARRIER_SOMCRR_EZLITE
#define CONFIG_MACH_TYPE MACH_TYPE_SC598_SOM_EZLITE;
#else
#define CONFIG_MACH_TYPE MACH_TYPE_SC598_SOM_EZKIT;
#endif

//L2 SRAM:
//TEXT:   0x20080000 to 0x200AFFFF (192 KB)
//BSS:    0x200B0000 to 0x200BFFFF (64  KB)
//MALLOC: 0x200C0000 to 0x200CFFFF (64  KB)
//STACK:  0x200D0000 to 0x200E4FFF (80  KB)
#define CONFIG_SPL_MAX_SIZE		SZ_192K
#ifdef CONFIG_SPL_BUILD
#define CONFIG_SPL_BSS_START_ADDR	0x200B0000
#define CONFIG_SPL_BSS_MAX_SIZE		SZ_64K
#define CONFIG_SYS_SPL_MALLOC_START	(CONFIG_SPL_BSS_START_ADDR + CONFIG_SPL_BSS_MAX_SIZE)
#define CONFIG_SYS_SPL_MALLOC_SIZE	SZ_64K
#define CONFIG_SPL_STACK		(CONFIG_SYS_SPL_MALLOC_START + CONFIG_SYS_SPL_MALLOC_SIZE + CONFIG_SPL_STACK_SIZE)
#define CONFIG_SPL_STACK_SIZE		(SZ_64K + SZ_16K)

//Parameters used for Falcon boot
#define CFG_SYS_SPI_ARGS_OFFS   0xE0000    // This is where the DTB should be stored
#define CFG_SYS_SPI_ARGS_SIZE   0x10000    // Max size of the DTB
#define CFG_SYS_SPI_KERNEL_OFFS 0x120000   // Where the kernel Image or FIT should be stored
#define CONFIG_SYS_SPL_ARGS_ADDR   0x99000000 // Where to load the DTB into RAM
#define CONFIG_SYS_SPI_KERNEL_SKIP_HEADER
#define CONFIG_SYS_MMCSD_RAW_MODE_KERNEL_SECTOR 4096 // Position of kernel Image in sectors
#define CONFIG_SYS_MMCSD_RAW_MODE_ARGS_SECTOR   2048 // Position of DTB in sectors
#define CONFIG_SYS_MMCSD_RAW_MODE_ARGS_SECTORS  256  // Size of DTB in sectors

#define CONFIG_SYS_LOAD_ADDR 0x96000000 //Address used to load the FIT image into during Falcon+FIT
#endif

/*
 * Memory Settings
 */
#define MEM_IS43TR16512BL
#define MEM_ISSI_4Gb_DDR3_800MHZ
#define MEM_DMC0


#define	CONFIG_NR_DRAM_BANKS		1
#define CFG_SYS_SDRAM_BASE	0x90000000
#define CFG_SYS_SDRAM_SIZE	0x10000000 
#ifndef CONFIG_SPL_BUILD
#define CONFIG_SYS_LOAD_ADDR	0x0
#endif
#define HAS_CUSTOM_SYS_INIT_SP_ADDR
#define CUSTOM_SYS_INIT_SP_ADDR CONFIG_TEXT_BASE

#define CONFIG_SYS_MONITOR_LEN	0
#define CONFIG_SYS_MALLOC_LEN	(1024 * 1024 * 1)

/*
 * Network Settings
 */
#ifdef CONFIG_ADI_CARRIER_SOMCRR_EZLITE
#define CONFIG_DTBNAME		"sc598-som-ezlite.dtb"
#else
#define CONFIG_DTBNAME		"sc598-som-ezkit.dtb"
#endif
#define CONFIG_HOSTNAME		"sc59x"
#define CONFIG_DW_PORTS		1

/*
 * USB Settings
 */
#define MUSB_HW_VERSION2
#define CONFIG_USB_STORAGE

/* OSPI - Via Device Tree Support
 *
 */
//Set this to 1 if you would like to use the maximum SPI speeds for OSPI and will not be using QSPI
#define ADI_USE_MACRONIX_OSPI 0
#define ADI_USE_MACRONIX_OSPI_DTR 0

//Allow U-Boot to dynamically pinmux between the three conflicting ezkit peripherals (OSPI/QSPI/UART)
//This allows U-boot to use all three peripherals
#define ADI_DYNAMIC_OSPI_QSPI_UART_MANAGEMENT

#define CONFIG_CQSPI_REF_CLK		500000000 // = 500 MHz

/*
 * SPI - Via Device Tree Support
 */
#if ADI_USE_MACRONIX_OSPI_DTR
	//Maximum DTR read speed = 128mbyte / 1.90s = 67.36 mbyte/s
	#define ADI_OSPI_ENV_SPI_MAX_HZ		CONFIG_CQSPI_REF_CLK / 12 //41.66 MHz DTR (Double Transfer Rate)
	#define ADI_OSPI_SF_DEFAULT_SPEED	CONFIG_CQSPI_REF_CLK / 12 //41.66 MHz DTR (Double Transfer Rate)
#else
	//Maximum STR read speed = 128mbyte / 2.17s = 58.98 mbyte/s
	#define ADI_OSPI_ENV_SPI_MAX_HZ		CONFIG_CQSPI_REF_CLK / 6 //83.33 MHz STR (Single Transfer Rate)
	#define ADI_OSPI_SF_DEFAULT_SPEED	CONFIG_CQSPI_REF_CLK / 6 //83.33 MHz STR (Single Transfer Rate)
#endif

#define CONFIG_ENV_SPI_MAX_HZ	10000000

/*
 * Misc Settings
 */
#define CONFIG_UART_CONSOLE	0

#if ADI_USE_MACRONIX_OSPI
#define CONFIG_BOOTCOMMAND	"run ospiboot"
#else
#define CONFIG_BOOTCOMMAND	"run spiboot"
#endif
#define INITRAMADDR "0x9c000000"

#define ADI_ENV_SETTINGS \
	"fdt_high=0xFFFFFFFFFFFFFFFF\0" \
	"initrd_high=0xFFFFFFFFFFFFFFFF\0" \
	ADI_INIT_ETHERNET \
	ADI_NFS_BOOT \
	ADI_RAM_BOOT \
	ADI_UPDATE_SPI_UBOOT_ONLY \
	ADI_ERASE_SPI \
	ADI_UPDATE_SPI \
	ADI_ERASE_OSPI \
	ADI_OSPI_BOOT \
	ADI_SPI_BOOT \
	ADI_MMC_BOOT

/* Per-board QSPI/OSPI Partitioning Offsets (64MB/32MB):
 *
 * 0x0000000 - 0x003FFFF : U-Boot Stage 1     (  256KB)
 * 0x0040000 - 0x017FFFF : U-Boot Stage 2     ( 1310KB)
 * 0x0180000 - 0x019FFFF : U-Boot Environment (  128KB)
 * 0x01a0000 - 0x101FFFF : FIT or DTB+zImage  (15204KB)
 * 0x1020000 - end       : Root File System
 */
#define ADI_SPI_SIZE     "0x4000000" //64MB
#define ADI_OSPI_SIZE    "0x4000000" //64MB
#define ADI_UBOOT_OFFSET "0x40000"
#define ADI_IMG_OFFSET   "0x01a0000"
#define ADI_RFS_OFFSET   "0x1020000"
#define ADI_JFFS2_FILE   "minimal" //use the adsp-sc5xx-minimal image

/* Push button polarties -- used for Falcon boot interrupt */
#define ADI_PB1_POLARITY 1
#define ADI_PB2_POLARITY 1

#include <configs/sc_adi_common.h>

#endif
