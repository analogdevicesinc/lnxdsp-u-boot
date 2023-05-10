/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * (C) Copyright 2022 - Analog Devices, Inc.
 *
 * Written and/or maintained by Timesys Corporation
 *
 * Contact: Nathan Barrett-Morrison <nathan.morrison@timesys.com>
 * Contact: Greg Malysa <greg.malysa@timesys.com>
 *
 * U-boot - Functions which are shared between u-boot proper + u-boot SPL
 *
 */

#ifndef SC59X_SHARED
#define SC59X_SHARED

int adi_enable_ethernet_softconfig(void);
int adi_disable_ethernet_softconfig(void);

#endif
