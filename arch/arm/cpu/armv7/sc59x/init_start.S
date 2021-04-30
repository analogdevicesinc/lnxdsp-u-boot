/*
 * Copyright (C) 2013 Analog Devices Inc.
 * Licensed under the GPL-2 or later.
 */

#include <linux/linkage.h>

.arm

ENTRY(_start)
	push	{r0, r1, lr}

	bl	initcode

	mov	r0, #0
	pop	{r0, r1, pc}
ENDPROC(_start)
