/*
 * clocks.c - figure out sclk/cclk/vco and such
 *
 * Copyright (c) 2014 Analog Devices Inc.
 *
 * Licensed under the GPL-2 or later.
 */

#include <common.h>
#include <asm/mach-adi/common/sc5xx.h>
#include <asm/mach-adi/common/clock.h>

/* Get the voltage input multiplier */
unsigned long get_vco(void)
{
/*	static unsigned long cached_vco_pll_ctl, cached_vco;

	unsigned long msel, pll_ctl;

	pll_ctl = bfin_read_PLL_CTL();
	if (pll_ctl == cached_vco_pll_ctl)
		return cached_vco;
	else
		cached_vco_pll_ctl = pll_ctl;

	msel = (pll_ctl & MSEL) >> MSEL_P;
	if (0 == msel)
		msel = (MSEL >> MSEL_P) + 1;

	cached_vco = CONFIG_CLKIN_HZ;
	cached_vco >>= (pll_ctl & DF);
	cached_vco *= msel;

	return cached_vco;
*/
	return ADI_VCO_HZ;
}

/* Get the Core clock */
unsigned long get_cclk(void)
{
#if 0
	static unsigned long cached_cclk_pll_div, cached_cclk;
	unsigned long div, csel;

	if (pll_is_bypassed())
		return CONFIG_CLKIN_HZ;

	div = bfin_read_PLL_DIV();
	if (div == cached_cclk_pll_div)
		return cached_cclk;
	else
		cached_cclk_pll_div = div;

	csel = (div & CSEL) >> CSEL_P;

	cached_cclk = get_vco() / csel;

	return cached_cclk;
#endif
	return ADI_CCLK_HZ;
}

/* Get the System clock */
/*
static unsigned long cached_sclk_pll_div, cached_sclk;
static unsigned long cached_sclk0, cached_sclk1, cached_dclk;
static unsigned long _get_sclk(u_long *cache)
{
	unsigned long div, ssel;

	if (pll_is_bypassed())
		return CONFIG_CLKIN_HZ;

	div = bfin_read_PLL_DIV();
	if (div == cached_sclk_pll_div)
		return *cache;
	else
		cached_sclk_pll_div = div;

	ssel = (div & SYSSEL) >> SYSSEL_P;
	cached_sclk = get_vco() / ssel;

	ssel = (div & S0SEL) >> S0SEL_P;
	cached_sclk0 = cached_sclk / ssel;

	ssel = (div & S1SEL) >> S1SEL_P;
	cached_sclk1 = cached_sclk / ssel;

	ssel = (div & DSEL) >> DSEL_P;
	cached_dclk = get_vco() / ssel;

	return *cache;
}
*/

unsigned long get_sclk(void)
{
	return ADI_SCLK_HZ;
/*	return _get_sclk(&cached_sclk);*/
}

unsigned long get_sclk0(void)
{
	return get_sclk() / CONFIG_CGU0_SCLK0_DIV;
/*	return _get_sclk(&cached_sclk0);*/
}

unsigned long get_sclk1(void)
{
	return get_sclk() / CONFIG_CGU0_SCLK1_DIV;
/*	return _get_sclk(&cached_sclk1);*/
}

unsigned long get_oclk(void)
{
	return ADI_VCO_HZ / CONFIG_CGU0_OCLK_DIV;
/*	return _get_sclk(&cached_oclk);*/
}

unsigned long get_dclk(void)
{
	return ADI_VCO_HZ / CONFIG_CGU0_DCLK_DIV;
/*	return _get_sclk(&cached_dclk);*/
}
