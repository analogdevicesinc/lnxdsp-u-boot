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
#include <asm/arch/cpu.h>
#include <asm/arch/portmux.h>

#ifdef CONFIG_DM_SPI
#include "adi_spi3_dm.h"
#else
#include "adi_spi3.h"
#endif

#define MAX_SPI_NUM 2

#define SPI_PINS(n) \
	{ 0, P_SPI##n##_SCK, P_SPI##n##_MISO, P_SPI##n##_MOSI, 0, 0, 0}
#define QSPI_PINS(n) \
	{ 0, P_SPI##n##_SCK, P_SPI##n##_MISO, P_SPI##n##_MOSI, \
	P_SPI##n##_D2, P_SPI##n##_D3, 0}
static unsigned short pins[][7] = {
	[0] = SPI_PINS(0),
	[1] = SPI_PINS(1),
	[2] = QSPI_PINS(2),
};

static const unsigned short cs_pins[][7] = {
	[0] = {
		P_SPI0_SSEL1, P_SPI0_SSEL2, 0, 0, 0, 0, 0,
	},
	[1] = {
		P_SPI1_SSEL1, P_SPI1_SSEL2, 0, 0, 0, 0, 0,
	},
	[2] = {
		P_SPI2_SSEL1, P_SPI2_SSEL2, 0, 0, 0, 0, 0,
	},
};

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
	pins[bus][0] = cs_pins[bus][cs - 1];	
	priv->pins = pins[bus];
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
		pins[bus][0] = cs_pins[bus][cs - 1];
		sdev->pins = pins[bus];
	}
	return sdev;
}
#endif
