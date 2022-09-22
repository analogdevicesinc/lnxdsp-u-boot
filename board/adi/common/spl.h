// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * (C) Copyright 2022 - Analog Devices, Inc.
 *
 * Written and/or maintained by Timesys Corporation
 *
 * Contact: Nathan Barrett-Morrison <nathan.morrison@timesys.com>
 * Contact: Greg Malysa <greg.malysa@timesys.com>
 */
#ifndef BOARD_ADI_SPL_H
#define BOARD_ADI_SPL_H

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

struct adi_boot_args {
	u32 addr;
	u32 flags;
	u32 cmd;
};

#if defined(CONFIG_SC59X)
	#define pRCU_STAT           ((void __iomem *)0x3108C004)
	#define pREG_PADS0_PCFG0    ((void __iomem *)0x31004604)
	#define pREG_PADS0_PCFG1    ((void __iomem *)0x31004608)
	#define ADI_ROM_ADDRESS		((void (*)(void *, uint32_t, int32_t, void *, uint32_t))0x000000e9)
#elif defined(CONFIG_SC59X_64)
	#define pRCU_STAT           ((void __iomem *)0x3108C004)
	#define pREG_EMSI0_SWRST    ((void __iomem *)0x310C702F)
	#define pREG_PADS0_PCFG0    ((void __iomem *)0x31004604)
	#define pREG_PADS0_PCFG1    ((void __iomem *)0x31004608)
	#define ADI_ROM_ADDRESS		((void (*)(void *, uint32_t, int32_t, void *, uint32_t))0x000000e4)
#elif defined(CONFIG_SC57X)
	#define pRCU_STAT           ((void __iomem *)0x3108C004)
	#define pREG_PADS0_PCFG0    ((void __iomem *)0x31004404)
	#define ADI_ROM_ADDRESS		((void (*)(void *, uint32_t, int32_t, void *, uint32_t))0x000000e1)
#elif defined(CONFIG_SC58X)
	#define pRCU_STAT			((void __iomem *)0x3108B004)
	#define pREG_PADS0_PCFG0	((void __iomem *)0x31004404)
	#define ADI_ROM_ADDRESS		((void (*)(void *, uint32_t, int32_t, void *, uint32_t))0x000000e1)
#endif

extern u32 bmode;

extern void adi_check_pushbuttons(void);

extern const struct adi_boot_args adi_rom_boot_args[8];

#endif
