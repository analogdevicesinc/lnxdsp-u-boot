/*
 * clocks.c - figure out sclk/cclk/vco and such
 *
 * Copyright (c) 2014 Analog Devices Inc.
 *
 * Licensed under the GPL-2 or later.
 */

#ifndef __ARCH_CLOCK_H
#define __ARCH_CLOCK_H

extern unsigned long get_vco(void);
extern unsigned long get_cclk(void);
extern unsigned long get_sclk(void);
extern unsigned long get_sclk0(void);
extern unsigned long get_sclk1(void);
extern unsigned long get_dclk(void);
extern unsigned long get_oclk(void);

# define get_uart_clk get_sclk0
# define get_i2c_clk get_sclk0
#if defined(CONFIG_SC58X) || defined(CONFIG_SC57X)
# define get_spi_clk get_sclk1
#else
# define get_spi_clk get_sclk0
#endif
#endif
