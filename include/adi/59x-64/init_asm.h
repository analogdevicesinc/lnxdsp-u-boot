#ifndef INIT_ASM
#define INIT_ASM

#ifdef CONFIG_ARM64
#include <asm/system.h>
#endif

#ifdef __ASSEMBLY__

.macro set_register addr val
	ldr w13, =\val
	ldr x14, =\addr
	str w13, [x14]
.endm

.macro setup_smpu
	set_register 0x31007800 0x500 //SMPU0
	set_register 0x31083800 0x500 //SMPU2
	set_register 0x31084800 0x500 //SMPU3
	set_register 0x31085800 0x500 //SMPU4
	set_register 0x31086800 0x500 //SMPU5
	set_register 0x31087800 0x500 //SMPU6
	set_register 0x310A0800 0x500 //SMPU9
	set_register 0x310A1800 0x500 //SMPU11
	set_register 0x31012800 0x500 //SMPU12
.endm

.macro setup_spu0_wp
	ldr w13, =0
	ldr x14, =0x3108B400
	ldr x15, =0
	setup_spu0_loop_wp:
		str w13, [x14]
		add x15, x15, #1
		add x14, x14, #4
		cmp x15, #214
		bne setup_spu0_loop_wp
.endm

.macro setup_spu0
	ldr w13, =0
	ldr x14, =0x3108BA00
	ldr x15, =0
	setup_spu0_loop:
		str w13, [x14]
		add x15, x15, #1
		add x14, x14, #4
		cmp x15, #214
		bne setup_spu0_loop
.endm

.macro sc598_ddr_fixups
	/* Alter outstanding transactions property of A55*/
	ldr x11,=0x30643108  /* SCB6 A55 M0 Ib.fn Mod */
	mov w12, #0x1
	str w12,[x11]
	isb

/* Do not modify prefetch behaviors -- This appears to be introducing rare memory bit flips

	# configure DDR prefetch behavior, per ADI
	set_register 0x31076000 0x1
	isb

	# disable prefetching associated with L1, L2, and L3 caches
	mrs x11, S3_0_c15_c1_4
	# L3WSCTL, disable streaming
	orr x11, x11, #(1 << 30)
	orr x11, x11, #(1 << 29)
	# L2WSCTL, disable streaming
	orr x11, x11, #(1 << 28)
	orr x11, x11, #(1 << 27)
	# L1WSCTL, disable streaming
	orr x11, x11, #(1 << 26)
	orr x11, x11, #(1 << 25)
	# L1PCTL, disable prefetch
	bic x11, x11, #(1 << 15)
	bic x11, x11, #(1 << 14)
	bic x11, x11, #(1 << 13)
	# L3PCTL, disable prefetch
	orr x11, x11, #(1 << 12)
	bic x11, x11, #(1 << 11)
	bic x11, x11, #(1 << 10)
	msr S3_0_c15_c1_4, x11

*/

.endm

.macro sc598_setup
#ifdef CONFIG_SPL_BUILD
	sc598_ddr_fixups
	setup_spu0
	setup_spu0_wp
	setup_smpu
#endif
.endm

#endif
#endif
