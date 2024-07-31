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

#ifndef __CONFIG_SC589_EZKIT_H
#define __CONFIG_SC589_EZKIT_H

#include <configs/sc589.h>

/*
 * Processor Settings
 */
#define CONFIG_MACH_TYPE	MACH_TYPE_SC589_EZKIT

/*
 * Network Settings
 */
#define CONFIG_DTBNAME		"sc589-ezkit.dtb"

/* Per-board SPI Partitioning Offsets (16MB):
 *
 * 0x0000000 - 0x001FFFF : U-Boot Stage 1     (  128KB)
 * 0x0020000 - 0x00CFFFF : U-Boot Stage 2     (  720KB)
 * 0x00D0000 - 0x00DFFFF : U-Boot Environment (   64KB)
 * 0x00E0000 - 0x06DFFFF : FIT or DTB+zImage  ( 6144KB)
 * 0x06E0000 - 0x0FFFFFF : Root File System   ( 9592KB)
 */
#define ADI_SPI_SIZE     "0x1000000" //16MB
#define ADI_UBOOT_OFFSET "0x20000"
#define ADI_IMG_OFFSET   "0xE0000"
#define ADI_RFS_OFFSET   "0x6E0000"
#define ADI_JFFS2_FILE   "tiny" //use the adsp-sc5xx-tiny image

#endif
