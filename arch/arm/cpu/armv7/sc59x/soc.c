/*
 * Copyright (C) 2013 Analog Devices Inc.
 * Licensed under the GPL-2 or later.
 */

#include <common.h>
#include <asm/arch-sc59x/sc59x.h>
#include <asm/io.h>
#include <asm/system.h>
#include <asm/arch/portmux.h>
#include <cpu_func.h>

void reset_cpu(ulong addr)
{
	writel(1, RCU0_CTL);
}

int arch_cpu_init(void)
{
	return 0;
}

u32 get_cpu_id(void)
{
	u32 cpuid = 0;

	__asm__ __volatile__("mrc p15, 0, %0, c0, c0, 0" : "=r"(cpuid));

	return cpuid;
}

int print_cpuinfo(void)
{
	char buf[32];

	printf("CPU:   ADSP %s (Detected Rev: %d.%d) (%s boot)\n",
		CONFIG_ADI_CPU, get_cpu_id() & 0xf00000 >> 20, get_cpu_id() & 0xf,
		get_sc_boot_mode(CONFIG_SC_BOOT_MODE));

	printf("VCO: %s MHz, ", strmhz(buf, get_vco()));
	printf("Cclk0: %s MHz, ", strmhz(buf, get_cclk()));
	printf("Sclk0: %s MHz, ", strmhz(buf, get_sclk0()));
	printf("Sclk1: %s MHz, ", strmhz(buf, get_sclk1()));
	printf("DCLK: %s MHz\n", strmhz(buf, get_dclk()));
	printf("OCLK: %s MHz\n", strmhz(buf, get_oclk()));

	return 0;
}
