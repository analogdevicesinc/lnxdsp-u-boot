/*
 * Analog Devices sc57x spi driver
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
#include "adi_spi3.h"

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
		P_SPI0_SSEL1, P_SPI0_SSEL2, P_SPI0_SSEL3, P_SPI0_SSEL4,
		P_SPI0_SSEL5, P_SPI0_SSEL6, P_SPI0_SSEL7,
	},
	[1] = {
		P_SPI1_SSEL1, P_SPI1_SSEL2, P_SPI1_SSEL3, P_SPI1_SSEL4,
		P_SPI1_SSEL5, P_SPI1_SSEL6, P_SPI1_SSEL7,
	},
	[2] = {
		P_SPI2_SSEL1, P_SPI2_SSEL2, P_SPI2_SSEL3, P_SPI2_SSEL4,
		P_SPI2_SSEL5,
	},
};

int adi_spi_cs_valid(unsigned int bus, unsigned int cs)
{
	if (bus > MAX_SPI_NUM)
		return 0;
	if (bus == MAX_SPI_NUM)
		return cs >= 1 && cs <= (MAX_CTRL_CS - 2);
	else
		return cs >= 1 && cs <= MAX_CTRL_CS;
}

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
