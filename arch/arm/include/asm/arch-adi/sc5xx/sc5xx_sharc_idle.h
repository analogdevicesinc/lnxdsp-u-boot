/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * (C) Copyright 2024 - Analog Devices, Inc.
 */

#ifndef _SC5XX_SHARC_IDLE_H_
#define _SC5XX_SHARC_IDLE_H_

#define ADI_RCU_REG_BASE 0x3108c000

/* Register offsets */
#define ADI_RCU_REG_MSG				0x6c
#define ADI_RCU_REG_MSG_SET			0x70
#define ADI_RCU_REG_MSG_CLR			0x74

/* Bit values for the RCU0_MSG register */
#define RCU0_MSG_C0IDLE			0x00000100		/* Core 0 Idle */
#define RCU0_MSG_C1IDLE			0x00000200		/* Core 1 Idle */
#define RCU0_MSG_C2IDLE			0x00000400		/* Core 2 Idle */

#ifdef CONFIG_SET_SHARC_IDLE
/**
 * set_sharc_cores_idle() - set the idle flag in RCU_MSG for the SHARC cores. 
 * 
 * Returns: 0 on success, an error code otherwise.
 */
int set_sharc_cores_idle(void);
#endif // CONFIG_SET_SHARC_IDLE

#endif // _SC5XX_SHARC_IDLE_H_