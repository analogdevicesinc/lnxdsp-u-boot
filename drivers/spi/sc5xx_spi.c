/*
 * Analog Devices sc58x spi driver
 *
 * Copyright (c) 2014 Analog Devices Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <common.h>
#include <spi.h>
#include <asm/mach-adi/common/cpu.h>

#ifdef CONFIG_DM_SPI
#include "adi_spi3_dm.h"
#else
#include "adi_spi3.h"
#endif

#define MAX_SPI_NUM 2

int adi_spi_cs_valid(unsigned int bus, unsigned int cs)
{
	if (bus > MAX_SPI_NUM)
		return 0;
	return cs >= 1 && cs <= (MAX_CTRL_CS - 5);
}
