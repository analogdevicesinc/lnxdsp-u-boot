/*
 * Copyright (C) 2013 Analog Devices Inc.
 * Licensed under the GPL-2 or later.
 */

#include <common.h>
#include <config.h>
#include <asm/io.h>
#include <asm/arch/cpu.h>

DECLARE_GLOBAL_DATA_PTR;
static struct gptimer3_group_regs *timer_group = (struct gptimer3_group_regs *)
							TIMER_GROUP;
struct gptimer3 *timer_base = (struct gptimer3 *)CONFIG_SYS_TIMERBASE;

#define MAX_TIM_LOAD	0xFFFFFFFF
#define TIMER_CLOCK		(ADI_SCLK_HZ/CONFIG_CGU0_SCLK0_DIV)

/* Functions just to satisfy the linker */
/*
 * This function is derived from PowerPC code (read timebase as long long).
 * On Blackfin it just returns the timer value.
 */
//unsigned long long get_ticks(void)
//{
//	return get_timer(0);
//}

//unsigned long timer_read_counter(void)
//{
//	return get_timer(0);
//}

/*
 * This function is derived from PowerPC code (timebase clock frequency).
 * On Blackfin it returns the number of timer ticks per second.
 */
//ulong get_tbclk(void)
//{
//	ulong tbclk;

//	tbclk = CONFIG_SYS_HZ;

//	return tbclk;
//}

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
	long tmo = usec * (TIMER_CLOCK / 1000) / 1000;
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
	writew(TIMER_OUT_DIS | TIMER_MODE_PWM_CONT | TIMER_PULSE_HI, &timer_base[0].config);
	writel(MAX_TIM_LOAD, &timer_base[0].period);
	writel(MAX_TIM_LOAD - 1, &timer_base[0].width);
	gd->arch.lastinc = get_gptimer_count(0) /
			(TIMER_CLOCK / CONFIG_SYS_HZ);
	gd->arch.tbl = 0;
	enable_gptimer(0);

	return 0;
}

/*
 * Any network command or flash
 * command is started get_timer shall
 * be called before TCOUNT gets reset,
 * to implement the accurate timeouts.
 *
 * How ever milliconds doesn't return
 * the number that has been elapsed from
 * the last reset.
 *
 * As get_timer is used in the u-boot
 * only for timeouts this should be
 * sufficient
 */
ulong get_timer(ulong base)
{
	ulong now = get_gptimer_count(0) / (TIMER_CLOCK / CONFIG_SYS_HZ);

	if (now >= gd->arch.lastinc) {  /* normal mode (non roll) */
		/* move stamp fordward with absoulte diff ticks */
		gd->arch.tbl += (now - gd->arch.lastinc);
	} else {        /* we have rollover of incrementer */
		gd->arch.tbl += ((MAX_TIM_LOAD / (TIMER_CLOCK /
				CONFIG_SYS_HZ)) - gd->arch.lastinc) + now;
	}
	gd->arch.lastinc = now;
	return gd->arch.tbl - base;
}
