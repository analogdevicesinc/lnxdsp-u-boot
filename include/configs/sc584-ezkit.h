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

#ifndef __CONFIG_SC584_EZKIT_H
#define __CONFIG_SC584_EZKIT_H

#include <asm/mach-adi/common/config.h>
#include <linux/sizes.h>

/*
 * Processor Settings
 */
#define CONFIG_LOADADDR	0x89000000
#define CONFIG_DTBLOADADDR	"0x8b000000"
#define CONFIG_MACH_TYPE	MACH_TYPE_SC584_EZKIT
#define CONFIG_SYS_BOOTM_LEN 0x1800000

/*
 * Memory Settings
 */
#define MEM_MT47H128M16RT
#define MEM_DMC0

#define CONFIG_SYS_SDRAM_BASE	0x89000000
#define CONFIG_SYS_SDRAM_SIZE	0x7000000
#define CONFIG_SYS_LOAD_ADDR	0x0
#define CONFIG_SYS_INIT_SP_ADDR (CONFIG_SYS_SDRAM_BASE + 0x3f000)

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
#define CONFIG_DTBNAME		"sc584-ezkit.dtb"
#define CONFIG_HOSTNAME		"sc58x"
#define CONFIG_DW_ALTDESCRIPTOR

/*
 * USB Settings
 */
#define MUSB_HW_VERSION2
#define CONFIG_USB_STORAGE

/*
 * Misc Settings
 */
#define CONFIG_BOOTCOMMAND	"run ramboot"
#define INITRAMADDR "0x8C000000"

#include <configs/sc_adi_common.h>

#endif
