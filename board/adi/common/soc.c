/*
 * Copyright (C) 2013 Analog Devices Inc.
 * Licensed under the GPL-2 or later.
 */

#include <common.h>
#include <asm/mach-adi/common/sc5xx.h>
#include <asm/io.h>
#include <asm/system.h>
#include <cpu_func.h>

void reset_cpu(ulong addr)
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
		CONFIG_ADI_CPU,  get_sc_boot_mode(CONFIG_SC_BOOT_MODE));

	print_cpu_id();

	return 0;
}
