/*
 * Copyright (C) 2020 Analog Devices Inc.
 * Licensed under the GPL-2 or later.
 *
 * Based on U-Boots pre-existing crt0_64.s and start.S for ARM64
 */

#include <linux/linkage.h>
#include <asm/macro.h>
#include <adi/59x-64/init_asm.h>

#define CONFIG_INIT_ELF_STACK_ADDR	0x200a0000

ENTRY(_main)
	ldr	x0, =(CONFIG_INIT_ELF_STACK_ADDR)
	bic	sp, x0, #0xf	/* 16-byte alignment for ABI compliance */
	bl	adi_initcode

	sc598_ddr_fixups

	/* This is loaded in via JTAG (GDB+OpenOCD) -- No need to return to ROM */
	loop_forever:
		nop
		bl loop_forever

ENDPROC(_main)

ENTRY(_start)
	.align 3

		/*
		 * Could be EL3/EL2/EL1, Initial State:
		 * Little Endian, MMU Disabled, i/dCache Disabled
		 */
		switch_el x1, 3f, 2f, 1f
	3:
		mrs	x0, scr_el3
		orr	x0, x0, #0xf			/* SCR_EL3.NS|IRQ|FIQ|EA */
		msr	scr_el3, x0
		msr	cptr_el3, xzr			/* Enable FP/SIMD */
		b	0f
	2:
		mov	x0, #0x33ff
		msr	cptr_el2, x0			/* Enable FP/SIMD */
		b	0f
	1:
		mov	x0, #3 << 20
		msr	cpacr_el1, x0			/* Enable FP/SIMD */
	0:
		isb

		bl	_main
ENDPROC(_start)
