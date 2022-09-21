// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * (C) Copyright 2022 - Analog Devices, Inc.
 *
 * Written and/or maintained by Timesys Corporation
 *
 * Contact: Nathan Barrett-Morrison <nathan.morrison@timesys.com>
 * Contact: Greg Malysa <greg.malysa@timesys.com>
 */

#include <asm/mach-adi/common/sc5xx.h>
#include <asm/io.h>
#include <config.h>
#include "init.h"

void adi_initcode(void){
	adi_initcode_shared();
}

