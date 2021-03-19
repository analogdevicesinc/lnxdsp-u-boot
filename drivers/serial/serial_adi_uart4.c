/*
 * Copyright (C) 2013 Analog Devices Inc.
 * Licensed under the GPL-2 or later.
 */

#include <common.h>
#include <post.h>
#include <watchdog.h>
#include <serial.h>
#include <asm/io.h>
#include <linux/compiler.h>
#include <adi_uart4.h>

DECLARE_GLOBAL_DATA_PTR;

#define CONSOLE_PORT CONFIG_UART_CONSOLE

unsigned int baudrate = CONFIG_BAUDRATE;
static void uart_setbrg(void);

static inline int uart_init(void)
{
	struct uart4_reg *regs = adi_uart4_get_regs(CONSOLE_PORT);
	unsigned short *pins = adi_uart4_get_pins(CONSOLE_PORT);

	if (peripheral_request_list(pins, "adi-uart4"))
		return -1;

	/* always enable UART to 8-bit mode */
	writel(UEN | UMOD_UART | WLS_8, &regs->control);

	uart_setbrg();
	writel(-1, &regs->status);

	return 0;
}

static inline int uart_uninit(void)
{
	struct uart4_reg *regs = adi_uart4_get_regs(CONSOLE_PORT);
	unsigned short *pins = adi_uart4_get_pins(CONSOLE_PORT);

	/* disable the UART by clearing UEN */
	writel(0, &regs->control);

	peripheral_free_list(pins);

	return 0;
}

static void serial_set_divisor(uint16_t divisor)
{
	struct uart4_reg *regs = adi_uart4_get_regs(CONSOLE_PORT);
	/* Program the divisor to get the baud rate we want */
	writel(divisor, &regs->clock);
}

void uart_putc(const char c)
{
	struct uart4_reg *regs = adi_uart4_get_regs(CONSOLE_PORT);
	/* send a \r for compatibility */
	if (c == '\n')
		uart_putc('\r');

	WATCHDOG_RESET();

	/* wait for the hardware fifo to clear up */
	while (!(readl(&regs->status) & THRE))
		continue;

	/* queue the character for transmission */
	writel(c, &regs->thr);

	WATCHDOG_RESET();
}

static int uart_tstc(void)
{
	struct uart4_reg *regs = adi_uart4_get_regs(CONSOLE_PORT);

	WATCHDOG_RESET();

	return (readl(&regs->status) & DR) ? 1 : 0;
}

static int uart_getc(void)
{
	struct uart4_reg *regs = adi_uart4_get_regs(CONSOLE_PORT);
	u32 uart_rbr_val;

	/* wait for data ! */
	while (!uart_tstc())
		continue;

	/* grab the new byte */
	uart_rbr_val = readl(&regs->rbr);
	writel(-1, &regs->status);

	return uart_rbr_val;
}

static void uart_setbrg(void)
{
	uint16_t divisor = (get_uart_clk() + (baudrate * 8)) / (baudrate * 16);
	baudrate = gd->baudrate;

	/* Program the divisor to get the baud rate we want */
	serial_set_divisor(divisor);
}

struct serial_device adi_serial_device = {
	.name   = "adi_uart4",
	.start  = uart_init,
	.stop   = uart_uninit,
	.setbrg = uart_setbrg,
	.getc   = uart_getc,
	.tstc   = uart_tstc,
	.putc   = uart_putc,
	.puts   = default_serial_puts,
};

__weak struct serial_device *default_serial_console(void)
{
	return &adi_serial_device;
}

void adi_uart4_serial_initialize(void)
{
	serial_register(&adi_serial_device);
}
