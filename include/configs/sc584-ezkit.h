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

/*
 * Processor Settings
 */
#define CONFIG_ADI_CPU		"ADSP-SC584-0.1"
#define CONFIG_LOADADDR	0x89000000
#define CONFIG_DTBLOADADDR	"0x8b000000"
#define CONFIG_MACH_TYPE	MACH_TYPE_SC584_EZKIT

#define CONFIG_SYS_ARM_CACHE_WRITETHROUGH
#define CONFIG_SYS_BOOTM_LEN 0x1800000
#define CONFIG_SYS_TIMERBASE	TIMER0_CONFIG

/*
 * Memory Settings
 */
#define MEM_MT47H128M16RT
#define MEM_DMC0

#define	CONFIG_NR_DRAM_BANKS		1
#define CONFIG_SYS_SDRAM_BASE	0x89000000
#define CONFIG_SYS_SDRAM_SIZE	0x7000000
#define CONFIG_SYS_TEXT_BASE	0x89200000
#define CONFIG_SYS_LOAD_ADDR	0x0
#define CONFIG_SYS_INIT_SP_ADDR (CONFIG_SYS_SDRAM_BASE + 0x3f000)

#define CONFIG_SYS_MONITOR_LEN	(0)
#define CONFIG_SYS_MALLOC_LEN	(1024 * 1024)

/*
 * Network Settings
 */
#define ADI_CMDS_NETWORK
#define CONFIG_NETCONSOLE
#define CONFIG_NET_MULTI
#define CONFIG_DTBNAME		"sc584-ezkit.dtb"
#define CONFIG_HOSTNAME		"sc58x"
#define CONFIG_DESIGNWARE_ETH
#define CONFIG_DW_PORTS		1
#define CONFIG_DW_ALTDESCRIPTOR
#define CONFIG_MII
#define CONFIG_PHYLIB
#define CONFIG_PHY_TI
#define CONFIG_PHY_NATSEMI

/*
 * SPI Settings
 */
#define CONFIG_CMD_SPI
#define CONFIG_ENV_SPI_MAX_HZ	5000000
#define CONFIG_SF_DEFAULT_SPEED	5000000
#define CONFIG_SPI_FLASH
#define CONFIG_SPI_FLASH_WINBOND

/*
 * USB Settings
 */
#define MUSB_HW_VERSION2
#define CONFIG_USB_STORAGE

/*
 * Env Storage Settings
 */
#define CONFIG_ENV_IS_IN_SPI_FLASH
#define CONFIG_ENV_SPI_BUS 2
#define CONFIG_ENV_SPI_CS 1

/*
 * Misc Settings
 */
#define CONFIG_UART_CONSOLE	0
#define CONFIG_BAUDRATE		115200
#define CONFIG_CMD_BOOTZ

#define CONFIG_BOOTCOMMAND	"run ramboot"
#define INITRAMADDR "0x8C000000"

#include <configs/sc_adi_common.h>

#endif
