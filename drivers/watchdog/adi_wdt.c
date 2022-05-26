/*
 * adi_wtd.c - driver for ADI on-chip watchdog
 *
 * Copyright (c) 2014 Analog Devices Inc.
 *
 * Licensed under the GPL-2 or later.
 */

#include <common.h>
#include <watchdog.h>

#if defined(CONFIG_SC59X_64)
#include <asm/arch/sc59x_64.h>
#elif defined(CONFIG_SC59X)
#include <asm/arch/sc59x.h>
#elif defined(CONFIG_SC58X)
#include <asm/arch/sc58x.h>
#elif defined(CONFIG_SC57X)
#include <asm/arch/sc57x.h>
#endif
#include <asm/arch/clock.h>
#include <asm/arch/watchdog.h>
#include <asm/io.h>

void hw_watchdog_reset(void)
{
	writel(0, WDOG_STAT);
}

void hw_watchdog_init(void)
{
	int i;
	u32 sctl_val;

	/* Disable SYSCD_RESETb input and clear the RCU0 reset status */
	writel(0xf, RCU0_STAT);
	writel(0x0, RCU0_CTL);

	/* reset the SEC controller */
	writel(0x2, SEC0_GCTL);
	writel(0x2, SEC0_FCTL);
	while (i++ < 100000)
		;

	/* enable SEC fault event */
	writel(0x1, SEC0_GCTL);

	/* ANOMALY 36100004 Spurious External Fault event occurs when FCTL
	 * is re-programmed when currently active fault is not cleared
	 */
	writel(0xc0, SEC0_FCTL);
	writel(0xc1, SEC0_FCTL);

	/* enable SEC fault source for watchdog0 */
	sctl_val = readl(SEC0_SCTL0 + 3 * 8) | 0x6;
	writel(sctl_val, SEC0_SCTL0 + 3 * 8);

	/* Enable SYSCD_RESETb input */
	writel(0x100, RCU0_CTL);

	/* enable watchdog0 */
	writel(WDDIS, WDOG_CTL);
	writel(CONFIG_WATCHDOG_TIMEOUT_MSECS / 1000 * get_sclk(), WDOG_CNT);
	hw_watchdog_reset();
	writel(WDEN, WDOG_CTL);
}
