/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * (C) Copyright 2022 - Analog Devices, Inc.
 *
 * Written and/or maintained by Timesys Corporation
 *
 * Contact: Nathan Barrett-Morrison <nathan.morrison@timesys.com>
 * Contact: Greg Malysa <greg.malysa@timesys.com>
 */

#include <common.h>
#include <netdev.h>
#include <phy.h>
#include <asm/io.h>
#include <asm/mach-types.h>
#include <asm/mach-adi/common/sc5xx.h>
#include <asm-generic/gpio.h>
#include <linux/delay.h>
#include <watchdog.h>

#include "../common/soc.h"

extern char __rel_dyn_start, __rel_dyn_end;
extern char __bss_start, __bss_end;

static void bss_clear(void)
{
	u32 bss_start = (u32)&__bss_start;
	u32 bss_end = (u32)&__bss_end;
	u32 rel_dyn_end = (u32)&__rel_dyn_end;
	u32 *to;

	if (rel_dyn_end >= bss_start && rel_dyn_end <= bss_end)
		to = (u32 *)rel_dyn_end;
	else
		to = (u32 *)bss_start;

	int i, sz;

	sz = bss_end - (u32)to;
	for (i = 0; i < sz; i += 4)
		*to++ = 0;
}

DECLARE_GLOBAL_DATA_PTR;
int board_early_init_f(void)
{
	bss_clear();

#ifdef CONFIG_HW_WATCHDOG
	hw_watchdog_init();
#endif

	return 0;
}

void set_spu_securep_msec(int n, bool msec)
{
	void __iomem *p = (void __iomem *)(REG_SPU0_CTL + 0xA00 + 4 * n);
	u32 securep = readl(p);

	if (msec)
		writel(securep | 0x2, p);
	else
		writel(securep & ~0x2, p);
}

unsigned long flash_init(void)
{

#if 0
	/*  Enable bank 1 of smc because AMS1 is connected to enable of SRAM */
	*pREG_SMC0_GCTL = 0x10;
	*pREG_SMC0_B1CTL = 0x00000001;
	*pREG_SMC0_B1TIM = 0x05220522;
#endif
	return 0;
}

int dram_init(void)
{
	gd->ram_size = CONFIG_SYS_SDRAM_SIZE;
	return 0;
}

void s_init(void)
{
}

#ifdef CONFIG_DESIGNWARE_ETH
int board_phy_config(struct phy_device *phydev)
{
	fixup_dp83867_phy(phydev);
	return 0;
}
#endif

int board_init(void)
{
	gd->bd->bi_arch_number = MACH_TYPE_SC573_EZKIT;
	/* boot param addr */
	gd->bd->bi_boot_params = CONFIG_SYS_SDRAM_BASE + (0x100);

	// configure eth0 for RGMII
	writel((readl(REG_PADS0_PCFG0) | 0xc), REG_PADS0_PCFG0);

	set_spu_securep_msec(41, 1);
	set_spu_securep_msec(45, 1);
	set_spu_securep_msec(140, 1);

	return 0;
}

