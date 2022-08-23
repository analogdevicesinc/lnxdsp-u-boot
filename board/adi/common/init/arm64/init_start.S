/*
 * Copyright (C) 2020 Analog Devices Inc.
 * Licensed under the GPL-2 or later.
 *
 * Based on U-Boots pre-existing crt0_64.s and start.S for ARM64
 */

#include <linux/linkage.h>
#include <asm/macro.h>

#include <asm/mach-adi/59x-64/init_asm.h>

ENTRY(_start)
	.align 3

	/* Store stack from ROM */
	stp     x29, x30, [sp, #-32]!
	mov     x29, sp
	str     x0, [sp, #24]

	bl	adi_initcode

	sc598_ddr_fixups

	/* Restore stack, return to ROM */
	nop
	ldp     x29, x30, [sp], #32
	ret
ENDPROC(_start)
