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
#include <asm/arch-adi/sc5xx/sc5xx.h>
#include <asm/io.h>
#include <asm/system.h>
#include <cpu_func.h>

#include <asm/arch-adi/sc5xx/soc.h>

void reset_cpu(void)
{
	writel(1, RCU0_CTL);
}

#ifndef CONFIG_ARM64
#ifndef CONFIG_SYS_DCACHE_OFF
void enable_caches(void)
{
	/* Enable D-cache. I-cache is already enabled in start.S */
	dcache_enable();
}
#endif

#ifndef CONFIG_SYS_L2CACHE_OFF
/*
 * Sets L2 cache related parameters before enabling data cache
 */
void v7_outer_cache_enable(void)
{
}
#endif
#endif

int arch_cpu_init(void)
{
	return 0;
}

void print_cpu_id(void)
{
#ifndef CONFIG_ARM64
	u32 cpuid = 0;

	__asm__ __volatile__("mrc p15, 0, %0, c0, c0, 0" : "=r"(cpuid));

	printf("Detected Revision: %d.%d\n", cpuid & 0xf00000 >> 20, cpuid & 0xf);
#endif
}

int print_cpuinfo(void)
{
	printf("CPU:   ADSP %s (%s boot)\n",
	       CONFIG_LDR_CPU,  get_sc_boot_mode(CONFIG_SC_BOOT_MODE));

	print_cpu_id();

	return 0;
}

void fixup_dp83867_phy(struct phy_device *phydev)
{
	int phy_data = 0;

	phy_data = phy_read(phydev, MDIO_DEVAD_NONE, 0x32);
	phy_write(phydev, MDIO_DEVAD_NONE, 0x32, (1 << 7) | phy_data);
	int cfg3 = 0;
	#define MII_DP83867_CFG3    (0x1e)
	/*
	 * Pin INT/PWDN on DP83867 should be configured as an Interrupt Output
	 * instead of a Power-Down Input on ADI SC5XX boards in order to
	 * prevent the signal interference from other peripherals during they
	 * are running at the same time.
	 */
	cfg3 = phy_read(phydev, MDIO_DEVAD_NONE, MII_DP83867_CFG3);
	cfg3 |= (1 << 7);
	phy_write(phydev, MDIO_DEVAD_NONE, MII_DP83867_CFG3, cfg3);

	// Mystery second port fixup on ezkits with two PHYs
	if (CONFIG_DW_PORTS & 2)
		phy_write(phydev, MDIO_DEVAD_NONE, 0x11, 3);

#ifdef CONFIG_ADI_BUG_EZKHW21
	phydev->advertising &= PHY_BASIC_FEATURES;
	phydev->speed = SPEED_100;
#endif

	if (phydev->drv->config)
		phydev->drv->config(phydev);

#ifdef CONFIG_ADI_BUG_EZKHW21
	phy_write(phydev, MDIO_DEVAD_NONE, 0, 0x3100);
#endif
}
