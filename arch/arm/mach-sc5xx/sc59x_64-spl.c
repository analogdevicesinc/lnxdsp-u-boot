/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * (C) Copyright 2022 - Analog Devices, Inc.
 *
 * Written and/or maintained by Timesys Corporation
 *
 * Contact: Nathan Barrett-Morrison <nathan.morrison@timesys.com>
 * Contact: Greg Malysa <greg.malysa@timesys.com>
 *
 * U-boot - SPL management
 *
 */

#include <asm/arch-adi/sc5xx/spl.h>

// Table 47-14 in SC598 hardware reference manual
const struct adi_boot_args adi_rom_boot_args[] = {
	// JTAG/no boot
	[0] = {0, 0, 0},
	// SPI master, used for qspi as well
	[1] = {0x60040000, 0x00040000, 0x00000207},
	// SPI slave
	[2] = {0, 0, 0x00000212},
	// UART slave
	[3] = {0, 0, 0x00000013},
	// Linkport slave
	[4] = {0, 0, 0x00000014},
	// OSPI master
	[5] = {0x60040000, 0, 0x00000008},
	// eMMC
	[6] = {0x201, 0, 0x86009},
	// reserved, also no boot
	[7] = {0, 0, 0}
};
