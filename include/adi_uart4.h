/*
 * adi_uart4.h - common serial defines for early debug and serial driver.
 *            any functions defined here must be always_inline since
 *            initcode cannot have function calls.
 *
 * Copyright (c) 2013 Analog Devices Inc.
 *
 * Licensed under the GPL-2 or later.
 */

#ifndef __ADI_UART4_H__
#define __ADI_UART4_H__

#include <asm/arch/serial.h>
#include <serial.h>

/*
 * UART4 Masks
 */

/* UART_CONTROL */
#define UEN			(1 << 0)
#define LOOP_ENA		(1 << 1)
#define UMOD			(3 << 4)
#define UMOD_UART		(0 << 4)
#define UMOD_MDB		(1 << 4)
#define UMOD_IRDA		(1 << 4)
#define WLS			(3 << 8)
#define WLS_5			(0 << 8)
#define WLS_6			(1 << 8)
#define WLS_7			(2 << 8)
#define WLS_8			(3 << 8)
#define STB			(1 << 12)
#define STBH			(1 << 13)
#define PEN			(1 << 14)
#define EPS			(1 << 15)
#define STP			(1 << 16)
#define FPE			(1 << 17)
#define FFE			(1 << 18)
#define SB			(1 << 19)
#define FCPOL			(1 << 22)
#define RPOLC			(1 << 23)
#define TPOLC			(1 << 24)
#define MRTS			(1 << 25)
#define XOFF			(1 << 26)
#define ARTS			(1 << 27)
#define ACTS			(1 << 28)
#define RFIT			(1 << 29)
#define RFRT			(1 << 30)

/* UART_STATUS */
#define DR			(1 << 0)
#define OE			(1 << 1)
#define PE			(1 << 2)
#define FE			(1 << 3)
#define BI			(1 << 4)
#define THRE			(1 << 5)
#define TEMT			(1 << 7)
#define TFI			(1 << 8)
#define ASTKY			(1 << 9)
#define ADDR			(1 << 10)
#define RO			(1 << 11)
#define SCTS			(1 << 12)
#define CTS			(1 << 16)
#define RFCS			(1 << 17)

/* UART_EMASK */
#define ERBFI			(1 << 0)
#define ETBEI			(1 << 1)
#define ELSI			(1 << 2)
#define EDSSI			(1 << 3)
#define EDTPTI			(1 << 4)
#define ETFI			(1 << 5)
#define ERFCI			(1 << 6)
#define EAWI			(1 << 7)
#define ERXS			(1 << 8)
#define ETXS			(1 << 9)


#ifndef CONFIG_UART_CONSOLE
# define CONFIG_UART_CONSOLE 0
#endif

#ifndef __ASSEMBLY__

struct uart4_reg {
	u32 revid;
	u32 control;
	u32 status;
	u32 scr;
	u32 clock;
	u32 emask;
	u32 emaskst;
	u32 emaskcl;
	u32 rbr;
	u32 thr;
	u32 taip;
	u32 tsr;
	u32 rsr;
	u32 txdiv_cnt;
	u32 rxdiv_cnt;
};

static inline struct uart4_reg *adi_uart4_get_regs(int portnum)
{
	return (struct uart4_reg *) adi_uart4_ports[portnum];
}

static inline unsigned short *adi_uart4_get_pins(int portnum)
{
	return (unsigned short *) adi_uart4_pinmux_pins[portnum];
}

__attribute__((always_inline))
static inline void serial_early_puts(const char *s)
{
#ifdef CONFIG_DEBUG_EARLY_SERIAL
		serial_puts("Early: ");
		serial_puts(s);
#endif
}

__attribute__((always_inline))
static inline void serial_early_do_portmux(void)
{
#if defined(CONFIG_SC59X)
	switch (CONFIG_UART_CONSOLE) {
	case 0:
		serial_early_do_mach_portmux('A', PORT_x_MUX_6_MASK,
		PORT_x_MUX_6_FUNC_2, PA6); /* TX: A; mux 6; func 2; PA6 */
		serial_early_do_mach_portmux('A', PORT_x_MUX_7_MASK,
		PORT_x_MUX_7_FUNC_2, PA7); /* RX: A; mux 6; func 2; PA7 */
		break;
	case 1:
		serial_early_do_mach_portmux('D', PORT_x_MUX_5_MASK,
		PORT_x_MUX_5_FUNC_1, PD5); /* TX: D; mux 5; func 1; PD5 */
		serial_early_do_mach_portmux('D', PORT_x_MUX_4_MASK,
		PORT_x_MUX_4_FUNC_1, PD4); /* RX: D; mux 4; func 1; PD4 */
		break;
	case 2:
		serial_early_do_mach_portmux('D', PORT_x_MUX_11_MASK,
		PORT_x_MUX_11_FUNC_2, PD11); /* TX: D; mux 11; func 2; PD11 */
		serial_early_do_mach_portmux('D', PORT_x_MUX_10_MASK,
		PORT_x_MUX_10_FUNC_2, PD10); /* RX: D; mux 10; func 2; PD10 */
		break;
	}
#else
	switch (CONFIG_UART_CONSOLE) {
	case 0:
		serial_early_do_mach_portmux('C', PORT_x_MUX_13_MASK,
		PORT_x_MUX_13_FUNC_1, PC13); /* TX: C; mux 13; func 1; PC13 */
		serial_early_do_mach_portmux('C', PORT_x_MUX_14_MASK,
		PORT_x_MUX_14_FUNC_1, PC13); /* RX: C; mux 13; func 1; PC14 */
		break;
	case 1:
		serial_early_do_mach_portmux('B', PORT_x_MUX_2_MASK,
		PORT_x_MUX_2_FUNC_2, PB2); /* TX: B; mux 2; func 2; PB2 */
		serial_early_do_mach_portmux('B', PORT_x_MUX_3_MASK,
		PORT_x_MUX_3_FUNC_2, PB3); /* RX: B; mux 3; func 2; PB3 */
		break;
	case 2:
		serial_early_do_mach_portmux('D', PORT_x_MUX_12_MASK,
		PORT_x_MUX_12_FUNC_1, PD12); /* TX: D; mux 12; func 1; PD12 */
		serial_early_do_mach_portmux('D', PORT_x_MUX_13_MASK,
		PORT_x_MUX_13_FUNC_1, PD13); /* RX: D; mux 13; func 1; PD13 */
		break;
	}
#endif
}

__attribute__((always_inline))
static inline int serial_early_init(void)
{
	struct uart4_reg *reg = adi_uart4_get_regs(CONFIG_UART_CONSOLE);

	/* handle portmux crap on different Blackfins */
	serial_early_do_portmux();

	/* always enable UART to 8-bit mode */
	writel(UEN | UMOD_UART | WLS_8, &reg->control);

	return 0;
}

__attribute__((always_inline))
static inline int serial_early_uninit(void)
{
	struct uart4_reg *reg = adi_uart4_get_regs(CONFIG_UART_CONSOLE);

	/* disable the UART by clearing UEN */
	writel(0, &reg->control);

	return 0;
}

__attribute__((always_inline))
static inline void serial_early_set_baud(uint32_t baud)
{
	struct uart4_reg *reg = adi_uart4_get_regs(CONFIG_UART_CONSOLE);
	uint16_t divisor = (CONFIG_SCLK_HZ + (baud * 8)) / (baud * 16);

	/* Program the divisor to get the baud rate we want */
	writel(divisor, &reg->clock);
}

#endif

#endif
