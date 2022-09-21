// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * (C) Copyright 2022 - Analog Devices, Inc.
 *
 * Written and/or maintained by Timesys Corporation
 *
 * Contact: Nathan Barrett-Morrison <nathan.morrison@timesys.com>
 * Contact: Greg Malysa <greg.malysa@timesys.com>
 *
 * U-boot - SPL management
 *
 */

#ifndef SC59X_SHARED_SPL
#define SC59X_SHARED_SPL

#include <common.h>
#include <netdev.h>
#include <phy.h>
#include <asm/io.h>
#include <asm/mach-types.h>
#include <asm/mach-adi/common/sc5xx.h>
#include <linux/delay.h>
#include <linux/stringify.h>
#include <watchdog.h>
#include <asm/spl.h>
#include <asm/mach-adi/common/rcu.h>
#include "sc59x-shared.h"

struct adi_boot_args {
	u32 addr;
	u32 flags;
	u32 cmd;
};

#define pRCU_STAT 0x3108c004

extern u32 bmode;

extern void adi_check_pushbuttons(void);

extern void (*adi_rom_boot)(void *addr, uint32_t flags, int32_t blocks, void *pHook, uint32_t cmd);

extern const struct adi_boot_args adi_rom_boot_args[8];

#endif
