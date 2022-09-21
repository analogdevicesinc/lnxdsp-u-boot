// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * (C) Copyright 2022 - Analog Devices, Inc.
 *
 * Written and/or maintained by Timesys Corporation
 *
 * Contact: Nathan Barrett-Morrison <nathan.morrison@timesys.com>
 * Contact: Greg Malysa <greg.malysa@timesys.com>
 */

#include "../common/sc59x/sc59x-shared-spl.h"

// cf. include/adi/cortex-a5/defSC59x_rom_jumptable.h
void (*adi_rom_boot)(void *addr, uint32_t flags, int32_t blocks, void *pHook, uint32_t cmd) = 0x000000E9;

// Table 45-14 in sc594 HRM
const struct adi_boot_args adi_rom_boot_args[] = {
	// JTAG/no boot
	[0] = {0, 0, 0},
	// SPI master, used for qspi as well
	[1] = {0x60020000, 0x00040000, 0x00000207},
	// SPI slave
	[2] = {0, 0, 0x00000212},
	// UART slave
	[3] = {0, 0, 0x00000013},
	// Linkport slave
	[4] = {0, 0, 0x00000014},
	// OSPI master
	[5] = {0x60020000, 0, 0x00000008},
	// reserved, no boot
	[6] = {0, 0, 0},
	// reserved, also no boot
	[7] = {0, 0, 0}
};
