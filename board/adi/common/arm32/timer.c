/*
 * Copyright (C) 2013 Analog Devices Inc.
 * Licensed under the GPL-2 or later.
 */

#include <clk.h>
#include <common.h>
#include <config.h>
#include <dm/uclass.h>
#include <asm/io.h>
#include <asm/mach-adi/common/cpu.h>
#include <dt-bindings/clock/adi-sc5xx-clock.h>

static struct gptimer3_group_regs *timer_group = (struct gptimer3_group_regs *)
							TIMER_GROUP;
struct gptimer3 *timer_base = (struct gptimer3 *)CONFIG_SYS_TIMERBASE;

#define MAX_TIM_LOAD	0xFFFFFFFF

static struct clk *sclk0 = NULL;

static int find_sclk0(void) {
	int ret;
	struct udevice *clkroot;

	ret = uclass_first_device(UCLASS_CLK, &clkroot);
	if (ret)
		return ret;

	ret = clk_get_by_id(ADI_TIMER_CLK_ID, &sclk0);
	if (ret)
		return ret;

	return 0;
}

static ulong get_rate(void) {
	if (!sclk0) {
		int ret = find_sclk0();
		if (ret)
			return CONFIG_CLKIN_HZ;
	}

	return clk_get_rate(sclk0);
}

unsigned long long get_ticks(void)
{
	return get_timer(0);
}

unsigned long timer_read_counter(void)
{
	return get_timer(0);
}

ulong get_tbclk(void)
{
	return CONFIG_SYS_HZ;
}

u32 get_gptimer_count(unsigned long id)
{
	return readl(&timer_base[id].counter);
}

void enable_gptimer(unsigned long id)
{
	u16 imask;
	imask = readw(&timer_group->data_imsk);
	imask &= ~(1 << id);
	writew(imask, &timer_group->data_imsk);
	writew((1 << id), &timer_group->enable);
}

void __udelay(unsigned long usec)
{
	long tmo = usec * (get_rate() / 1000) / 1000;
	unsigned long now, last = get_gptimer_count(0);

	while (tmo > 0) {
		now = get_gptimer_count(0);
		if (last > now) /* count up timer overflow */
			tmo -= MAX_TIM_LOAD - last + now + 1;
		else
			tmo -= now - last;
		last = now;
	}
}

int timer_init(void)
{
	(void) find_sclk0();
	writew(TIMER_OUT_DIS | TIMER_MODE_PWM_CONT | TIMER_PULSE_HI, &timer_base[0].config);
	writel(MAX_TIM_LOAD, &timer_base[0].period);
	writel(MAX_TIM_LOAD - 1, &timer_base[0].width);
	enable_gptimer(0);

	return 0;
}

ulong get_timer(ulong base)
{
	ulong rate = get_rate();
	ulong now = get_gptimer_count(0) / (rate / 1000);
	return now - base;
}
