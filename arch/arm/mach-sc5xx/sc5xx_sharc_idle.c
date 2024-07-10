/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * (C) Copyright 2024 - Analog Devices, Inc.
 */

#include <asm/io.h>
#include <asm/arch-adi/sc5xx/sc5xx_sharc_idle.h>
#include <log.h>

#ifdef CONFIG_SET_SHARC_IDLE
static int sharc_set_idle(unsigned int coreid) {
	writew(RCU0_MSG_C0IDLE << coreid, ADI_RCU_REG_BASE + ADI_RCU_REG_MSG_SET);

	return 0;
}

extern int set_sharc_cores_idle(void) {
    debug("Setting the SHARC cores to IDLE...\n");

	int ret;

    for (int c = 1; c <= 2; c++) {
        ret = sharc_set_idle(c);
        if (ret) {
            printf("Error setting SHARC Core%d idle.\n", c - 1);
            return ret;
        }
    }

	return 0;
}
#endif // CONFIG_SET_SHARC_IDLE
