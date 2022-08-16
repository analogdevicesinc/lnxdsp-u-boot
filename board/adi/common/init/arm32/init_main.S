/*
 * Copyright (C) 2013 Analog Devices Inc.
 * Licensed under the GPL-2 or later.
 */

#include <linux/linkage.h>
#define CONFIG_INIT_ELF_STACK_ADDR	0x200a0000

.arm
ENTRY(_main)
	/*
	 * disable interrupts (FIQ and IRQ), also set the cpu to SVC32 mode,
	 * except if in HYP mode already
	 */
	mrs	r0, cpsr
	and	r1, r0, #0x1f		@ mask mode bits
	teq	r1, #0x1a		@ test for HYP mode
	bicne	r0, r0, #0x1f		@ clear all mode bits
	orrne	r0, r0, #0x13		@ set SVC mode
	orr	r0, r0, #0xc0		@ disable FIQ and IRQ
	msr	cpsr,r0

	/* Set up stack pointer */
	ldr	sp, =(CONFIG_INIT_ELF_STACK_ADDR)

	bl	initcode

	/* wait for JTAG */
loop:
	b loop
ENDPROC(_main)

.thumb
ENTRY(_start)
	bl	_main
ENDPROC(_start)
