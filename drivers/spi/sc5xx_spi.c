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


#ifdef CONFIG_DM_SPI
void adi_spi_setup_reg(struct adi_spi_platdata * platdata, int bus)
{
	switch (bus) {
	case 0:
		platdata->regs = (struct spi_regs *)SPI0_REGBASE;
		break;
	case 1:
		platdata->regs = (struct spi_regs *)SPI1_REGBASE;
		break;
	default:
		platdata->regs = (struct spi_regs *)SPI2_REGBASE;
		break;
	}
}

void adi_spi_setup_cs(struct adi_spi_priv * priv, int bus, unsigned int cs)
{
}
#else
struct adi_spi_slave *adi_spi_setup(unsigned int bus, unsigned int cs)
{
	struct adi_spi_slave *sdev;

	sdev = spi_alloc_slave(struct adi_spi_slave, bus, cs);
	if (sdev) {
		switch (bus) {
		case 0:
			sdev->regs = (struct spi_regs *)SPI0_REGBASE;
			break;
		case 1:
			sdev->regs = (struct spi_regs *)SPI1_REGBASE;
			break;
		default:
			sdev->regs = (struct spi_regs *)SPI2_REGBASE;
			break;
		}
	}
	return sdev;
}
#endif
