/*
 * serial.h - common ADI uart4 serial defines SC57x.
 *
 * Copyright (c) 2013 Analog Devices Inc.
 *
 * Licensed under the GPL-2 or later.
 */

#ifndef __SC57X_SERIAL_H__
#define __SC57X_SERIAL_H__

#include <asm/mach-adi/common/sc5xx.h>
#include <asm/arch/gpio.h>
#include <asm/arch/ports.h>
#include <asm/arch/clock.h>

static const void *const adi_uart4_ports[3] = {
	(void *)UART0_REVID,
	(void *)UART1_REVID,
	(void *)UART2_REVID,
};

static const unsigned short adi_uart4_port0_pins[] = {
	P_UART0_TX, P_UART0_RX, 0
};

static const unsigned short adi_uart4_port1_pins[] = {
	P_UART1_TX, P_UART1_RX, 0
};

static const unsigned short adi_uart4_port2_pins[] = {
	P_UART2_TX, P_UART2_RX, 0
};

static const unsigned short *const adi_uart4_pinmux_pins[3] = {
	adi_uart4_port0_pins,
	adi_uart4_port1_pins,
	adi_uart4_port2_pins,
};

__attribute__((always_inline))
static inline void serial_early_do_mach_portmux(char port, int mux_mask,
	int mux_func, int port_pin)
{
	switch (port) {
	case 'B':
		writel((readl(PORTB_MUX) & ~mux_mask) | mux_func, PORTB_MUX);
		writel(port_pin, PORTB_FER_SET);
		break;
	case 'C':
		writel((readl(PORTC_MUX) & ~mux_mask) | mux_func, PORTC_MUX);
		writel(port_pin, PORTC_FER_SET);
		break;
	case 'D':
		writel((readl(PORTD_MUX) & ~mux_mask) | mux_func, PORTD_MUX);
		writel(port_pin, PORTD_FER_SET);
		break;
	}
}

#endif
