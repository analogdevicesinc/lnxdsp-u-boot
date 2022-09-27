// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * (C) Copyright 2022 - Analog Devices, Inc.
 *
 * Written and/or maintained by Timesys Corporation
 *
 * Contact: Nathan Barrett-Morrison <nathan.morrison@timesys.com>
 * Contact: Greg Malysa <greg.malysa@timesys.com>
 */

#if defined(CONFIG_SC57X) || defined(CONFIG_SC59X)
#define REG_SPU0_SECUREC0              0x3108B980
#define REG_SPU0_SECUREC1              0x3108B984
#define REG_SPU0_SECUREC2              0x3108B988
#define REG_SPU0_SECUREP_START         0x3108BA00
#define REG_SPU0_SECUREP_END           0x3108BD24
#endif

#ifdef CONFIG_SC59X_64
#define REG_RCU0_BCODE                 0x3108C028
#define REG_TSGENWR0_CNTCR             0x310AE000            /*  TSGENWR0 Counter Control Register */
#endif

#ifdef CONFIG_SC58X
#define REG_SPU0_SECUREC0              0x3108C980
#define REG_SPU0_SECUREC1              0x3108C984
#define REG_SPU0_SECUREC2              0x3108C988
#define REG_SPU0_SECUREP_START         0x3108CA00
#define REG_SPU0_SECUREP_END           0x3108CD24
#endif

#define REG_ARMPMU0_PMCR               0x31121E04
#define REG_ARMPMU0_PMUSERENR          0x31121E08
#define REG_ARMPMU0_PMLAR              0x31121FB0

void adi_initcode(void);
void adi_initcode_shared(void);