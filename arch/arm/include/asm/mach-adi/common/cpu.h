/*
 * Copyright (C) 2013 Analog Devices Inc.
 * Licensed under the GPL-2 or later.
 */

#include <asm/mach-adi/common/sc5xx.h>

/*
 * Timer Configuration Register Bits
  */
#define TIMER_EMU_RUN       0x8000
#define TIMER_BPER_EN       0x4000
#define TIMER_BWID_EN       0x2000
#define TIMER_BDLY_EN       0x1000
#define TIMER_OUT_DIS       0x0800
#define TIMER_TIN_SEL       0x0400
#define TIMER_CLK_SEL       0x0300
#define TIMER_CLK_SCLK      0x0000
#define TIMER_CLK_ALT_CLK0  0x0100
#define TIMER_CLK_ALT_CLK1  0x0300
#define TIMER_PULSE_HI      0x0080
#define TIMER_SLAVE_TRIG    0x0040
#define TIMER_IRQ_MODE      0x0030
#define TIMER_IRQ_ACT_EDGE  0x0000
#define TIMER_IRQ_DLY       0x0010
#define TIMER_IRQ_WID_DLY   0x0020
#define TIMER_IRQ_PER       0x0030
#define TIMER_MODE          0x000f
#define TIMER_MODE_WDOG_P   0x0008
#define TIMER_MODE_WDOG_W   0x0009
#define TIMER_MODE_PWM_CONT 0x000c
#define TIMER_MODE_PWM      0x000d
#define TIMER_MODE_WDTH     0x000a
#define TIMER_MODE_WDTH_D   0x000b
#define TIMER_MODE_EXT_CLK  0x000e
#define TIMER_MODE_PININT   0x000f

#define __BFP(m) u16 m; u16 __pad_##m

struct gptimer3 {
	__BFP(config);
	u32 counter;
	u32 period;
	u32 width;
	u32 delay;
};

struct gptimer3_group_regs {
	__BFP(run);
	__BFP(enable);
	__BFP(disable);
	__BFP(stop_cfg);
	__BFP(stop_cfg_set);
	__BFP(stop_cfg_clr);
	__BFP(data_imsk);
	__BFP(stat_imsk);
	__BFP(tr_msk);
	__BFP(tr_ie);
	__BFP(data_ilat);
	__BFP(stat_ilat);
	__BFP(err_status);
	__BFP(bcast_per);
	__BFP(bcast_wid);
	__BFP(bcast_dly);
};
