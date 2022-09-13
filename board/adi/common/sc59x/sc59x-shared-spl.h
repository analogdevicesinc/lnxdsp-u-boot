/*
 * U-boot - SPL management
 *
 * Copyright (c) 2022 Analog Devices Inc.
 *
 * Licensed under the GPL-2 or later.
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
#include "../common/sc59x/sc59x-shared.h"

struct adi_boot_args {
	u32 addr;
	u32 flags;
	u32 cmd;
};

#define pRCU_STAT 0x3108c004

extern u32 bmode;

extern void adi_check_pushbuttons(void);

#endif