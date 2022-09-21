// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * (C) Copyright 2022 - Analog Devices, Inc.
 *
 * Written and/or maintained by Timesys Corporation
 *
 * Contact: Nathan Barrett-Morrison <nathan.morrison@timesys.com>
 * Contact: Greg Malysa <greg.malysa@timesys.com>
 *
 * Based on U-Boots pre-existing crt0_64.s and start.S for ARM64
 */

#include <linux/linkage.h>
#include <asm/macro.h>

ENTRY(_start)
	.align 3

	/* Store stack from ROM */
	stp     x29, x30, [sp, #-32]!
	mov     x29, sp
	str     x0, [sp, #24]

	bl	adi_initcode

	/* Restore stack, return to ROM */
	nop
	ldp     x29, x30, [sp], #32
	ret
ENDPROC(_start)
