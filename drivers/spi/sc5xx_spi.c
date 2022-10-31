// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * (C) Copyright 2022 - Analog Devices, Inc.
 *
 * Written and/or maintained by Timesys Corporation
 *
 * Converted to driver model by Nathan Barrett-Morrison
 *
 * Contact: Nathan Barrett-Morrison <nathan.morrison@timesys.com>
 * Contact: Greg Malysa <greg.malysa@timesys.com>
 *
 */

#include <common.h>
#include <spi.h>
#include <asm/arch-adi/sc5xx/cpu.h>

#ifdef CONFIG_DM_SPI
#include "adi_spi3_dm.h"
#else
#include "adi_spi3.h"
#endif

