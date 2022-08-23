/*
 * config.h - common defines for SC57x boards
 *
 * Copyright (c) 2013 Analog Devices Inc.
 *
 * Licensed under the GPL-2 or later.
 */

#ifndef __ASM_SC5XX_CONFIG_H__
#define __ASM_SC5XX_CONFIG_H__

/* Bootmode defines -- your config needs to select this via CONFIG_SC_BOOT_MODE.
 * Depending on your cpu, some of these may not be valid, check your HRM.
 * The actual values here are meaningless as long as they're unique.
 */
#define SC_BOOT_IDLE        0       /* do nothing, just idle */
#define SC_BOOT_SPI_MASTER  1       /* boot ldr out of serial flash */
#define SC_BOOT_SPI_SLAVE   2       /* boot ldr as spi slave */
#define SC_BOOT_LP_SLAVE    6      /* boot ldr from link port */
#define SC_BOOT_UART        7       /* boot ldr over uart */

#ifndef __ASSEMBLY__
static inline const char *get_sc_boot_mode(int boot)
{
	switch (boot) {
	case SC_BOOT_SPI_MASTER: return "spi flash";
	case SC_BOOT_SPI_SLAVE:  return "spi slave";
	case SC_BOOT_UART:       return "uart";
	case SC_BOOT_IDLE:       return "idle";
	case SC_BOOT_LP_SLAVE:   return "link port slave";
	default:                   return "INVALID";
	}
}
#endif

/* Define the default SPI CS used when booting out of SPI */
#define BFIN_BOOT_SPI_SSEL 1

/* There is no NetBSD port */
#undef CONFIG_BOOTM_NETBSD

#endif
