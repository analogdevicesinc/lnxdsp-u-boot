/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * (C) Copyright 2022 - Analog Devices, Inc.
 *
 * Written and/or maintained by Timesys Corporation
 *
 * Contact: Nathan Barrett-Morrison <nathan.morrison@timesys.com>
 * Contact: Greg Malysa <greg.malysa@timesys.com>
 */

#include <netdev.h>
#include <phy.h>
#include <asm/io.h>
#include <asm/mach-types.h>
#include <asm/arch-adi/sc5xx/sc5xx.h>
#include <linux/delay.h>
#include <watchdog.h>
#include <asm/armv8/mmu.h>

#include <asm/arch-adi/sc5xx/sc59x-shared.h>
#include <asm/arch-adi/sc5xx/soc.h>

static struct mm_region sc598_mem_map[] = {
	{
		/* Peripherals */
		.virt = 0x0UL,
		.phys = 0x0UL,
		.size = 0x80000000UL,
		.attrs = PTE_BLOCK_MEMTYPE(MT_DEVICE_NGNRNE) |
			 PTE_BLOCK_NON_SHARE |
			 PTE_BLOCK_PXN | PTE_BLOCK_UXN
	}, {
		/* DDR */
		.virt = 0x80000000UL,
		.phys = 0x80000000UL,
		.size = 0x40000000UL,
		.attrs = PTE_BLOCK_MEMTYPE(MT_NORMAL) |
			 PTE_BLOCK_INNER_SHARE
	}, {
		/* List terminator */
		0,
	}
};

struct mm_region *mem_map = sc598_mem_map;

extern char __bss_start, __bss_end;
static void bss_clear(void)
{
	u32 *to = (void *)&__bss_start;
	int i, sz;

	sz = &__bss_end - &__bss_start;
	for (i = 0; i < sz; i += 4)
		*to++ = 0;
}

DECLARE_GLOBAL_DATA_PTR;
int board_early_init_f(void)
{
	bss_clear();
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

void adi_eth_init(void) 
{
#if defined(CONFIG_ADI_CARRIER_SOMCRR_EZKIT) || defined(CONFIG_ADI_CARRIER_SOMCRR_EZLITE)
	// Reset PHYs handled through DM-based softconfig driver
	adi_enable_ethernet_softconfig();
	mdelay(20);
	adi_disable_ethernet_softconfig();
	mdelay(90);
	adi_enable_ethernet_softconfig();
	mdelay(20);
#endif

	// select RGMII, little endian for all eth ports
	writel((readl(REG_PADS0_PCFG0) | 0xc), REG_PADS0_PCFG0);
	writel(readl(REG_PADS0_PCFG0) & ~(1 << 19), REG_PADS0_PCFG0);
	writel(readl(REG_PADS0_PCFG0) & ~(1 << 20), REG_PADS0_PCFG0);
}

int board_phy_config(struct phy_device *phydev)
{
#ifdef CONFIG_ADI_CARRIER_SOMCRR_EZKIT
	fixup_dp83867_phy(phydev);
#endif
	return 0;
}

int board_init(void)
{
#ifdef CONFIG_ADI_CARRIER_SOMCRR_EZLITE
	gd->bd->bi_arch_number = MACH_TYPE_SC598_SOM_EZLITE;
#else
	gd->bd->bi_arch_number = MACH_TYPE_SC598_SOM_EZKIT;
#endif

	adi_eth_init();

	return 0;
}

