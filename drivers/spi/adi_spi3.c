/*
 * Analog Devices SPI3 controller driver
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
#include <malloc.h>
#include <log.h>
#include <console.h>
#include <spi.h>
#include <asm/io.h>
#include <asm/gpio.h>
#ifndef CONFIG_ARCH_HEADER_IN_MACH
#include <asm/clock.h>
#else
#include <asm/mach-adi/common/clock.h>
#endif
#include "adi_spi3.h"

#define to_adi_spi_slave(s) container_of(s, struct adi_spi_slave, slave)

// @todo gpio cs technically not supported/used at the moment
int spi_cs_is_valid(unsigned int bus, unsigned int cs)
{
	if (is_gpio_cs(cs))
		return 1;
	else
		return adi_spi_cs_valid(bus, cs);
}

void spi_cs_activate(struct spi_slave *slave)
{
	struct adi_spi_slave *sdev = to_adi_spi_slave(slave);

	if (is_gpio_cs(slave->cs)) {
		unsigned int cs = gpio_cs(slave->cs);
		gpio_set_value(cs, sdev->cs_pol);
	} else {
		u32 ssel;
		ssel = readl(&sdev->regs->ssel);
		ssel |= BIT_SSEL_EN(slave->cs);
		if (sdev->cs_pol)
			ssel |= BIT_SSEL_VAL(slave->cs);
		else
			ssel &= ~BIT_SSEL_VAL(slave->cs);
		writel(ssel, &sdev->regs->ssel);
	}
}

void spi_cs_deactivate(struct spi_slave *slave)
{
	struct adi_spi_slave *sdev = to_adi_spi_slave(slave);

	if (is_gpio_cs(slave->cs)) {
		unsigned int cs = gpio_cs(slave->cs);
		gpio_set_value(cs, !sdev->cs_pol);
		gpio_set_value(cs, 1);
	} else {
		u32 ssel;
		ssel = readl(&sdev->regs->ssel);
		if (sdev->cs_pol)
			ssel &= ~BIT_SSEL_VAL(slave->cs);
		else
			ssel |= BIT_SSEL_VAL(slave->cs);
		/* deassert cs */
		writel(ssel, &sdev->regs->ssel);
		/* disable cs */
		ssel &= ~BIT_SSEL_EN(slave->cs);
		writel(ssel, &sdev->regs->ssel);
	}
}

void spi_init(void)
{
}

void spi_set_speed(struct spi_slave *slave, uint hz)
{
	struct adi_spi_slave *sdev = to_adi_spi_slave(slave);
	u32 clock;

	clock = get_spi_clk() / hz;
	if (clock)
		clock--;
	sdev->clock = clock;
}

struct spi_slave *spi_setup_slave(unsigned int bus, unsigned int cs,
		unsigned int max_hz, unsigned int mode)
{
	struct adi_spi_slave *sdev;

	if (!spi_cs_is_valid(bus, cs))
		return NULL;

	if (max_hz > get_spi_clk())
		return NULL;

	sdev = adi_spi_setup(bus, cs);
	if (!sdev)
		return NULL;

	if (is_gpio_cs(cs)) {
		cs = gpio_cs(cs);
		gpio_request(cs, "adi-spi3");
		gpio_direction_output(cs, !sdev->cs_pol);
		sdev->pins[0] = P_DONTCARE;
	}
	peripheral_request_list(sdev->pins, "adi-spi3");

	sdev->control = SPI_CTL_EN | SPI_CTL_MSTR;
	if (mode & SPI_CPHA)
		sdev->control |= SPI_CTL_CPHA;
	if (mode & SPI_CPOL)
		sdev->control |= SPI_CTL_CPOL;
	if (mode & SPI_LSB_FIRST)
		sdev->control |= SPI_CTL_LSBF;
	sdev->control &= ~SPI_CTL_ASSEL;
	sdev->cs_pol = mode & SPI_CS_HIGH ? 1 : 0;
	spi_set_speed(&sdev->slave, max_hz);
#ifdef CONFIG_SPI_FLASH_MMAP
	sdev->slave.memory_map = (void *)CONFIG_SPI_MM_BASE;
#endif

#ifdef DBB
	/* Select quad rx and tx mode for SPI data transfer */
	sdev->slave.op_mode_rx = SPI_OPM_RX_QOF << 1;
	sdev->slave.op_mode_tx = SPI_OPM_TX_QPP;
#endif

	writel(0x0, &sdev->regs->control);
	writel(0x0, &sdev->regs->rx_control);
	writel(0x0, &sdev->regs->tx_control);

	return &sdev->slave;
}

void spi_free_slave(struct spi_slave *slave)
{
	struct adi_spi_slave *sdev = to_adi_spi_slave(slave);

	peripheral_free_list(sdev->pins);
	if (is_gpio_cs(slave->cs))
		gpio_free(gpio_cs(slave->cs));

	free(sdev);
}

int spi_claim_bus(struct spi_slave *slave)
{
	struct adi_spi_slave *sdev = to_adi_spi_slave(slave);

	debug("%s: bus:%i cs:%i\n", __func__, slave->bus, slave->cs);

	writel(sdev->control, &sdev->regs->control);
	writel(sdev->clock, &sdev->regs->clock);
	writel(0x0, &sdev->regs->delay);

	return 0;
}

void spi_release_bus(struct spi_slave *slave)
{
	struct adi_spi_slave *sdev = to_adi_spi_slave(slave);

	debug("%s: bus:%i cs:%i\n", __func__, slave->bus, slave->cs);

	writel(0x0, &sdev->regs->rx_control);
	writel(0x0, &sdev->regs->tx_control);
	writel(0x0, &sdev->regs->control);
}

#ifndef CONFIG_SPI_IDLE_VAL
# define CONFIG_SPI_IDLE_VAL 0xff
#endif

static int spi_pio_xfer(struct adi_spi_slave *sdev, const u8 *tx, u8 *rx,
			uint bytes, unsigned long flags)
{
	u8 value;

	/* discard invalid rx data and empty rfifo */
	while (!(readl(&sdev->regs->status) & SPI_STAT_RFE))
		readl(&sdev->regs->rfifo);

#ifdef DBB
	if (flags & SPI_XFER_QUAD) {
		/* switch current SPI transfer to quad SPI mode */
		writel((sdev->control & ~SPI_CTL_SOSI) | SPI_CTL_MIO_QUAD,
			&sdev->regs->control);

		/* Data can only be transfered in one direction in quad SPI
		 * mode, trgger the transfer in respective direction.
		 */
		if (rx) {
			writel(0x0, &sdev->regs->tx_control);
			writel(SPI_RXCTL_REN | SPI_RXCTL_RTI,
				&sdev->regs->rx_control);

			while (bytes--) {
				while (readl(&sdev->regs->status) &
					SPI_STAT_RFE)
					if (ctrlc())
						return -1;
				value = readl(&sdev->regs->rfifo);
				*rx++ = value;
			}
		} else if (tx) {
			writel(0x0, &sdev->regs->rx_control);
			writel(SPI_TXCTL_TEN | SPI_TXCTL_TTI,
				&sdev->regs->tx_control);

			while (bytes--) {
				value = *tx++;
				writel(value, &sdev->regs->tfifo);
				while (readl(&sdev->regs->status) &
					SPI_STAT_TFF)
					if (ctrlc())
						return -1;
			}

			/* Wait till the tfifo is empty */
			while ((readl(&sdev->regs->status) & SPI_STAT_TFS) !=
				0x40000)
				if (ctrlc())
					return -1;
		} else
			return -1;

	} else {
#endif
		/* Set current SPI transfer in normal mode and trigger
		 * the bi-direction transfer by tx write operation.
		 */
		writel(sdev->control, &sdev->regs->control);
		writel(SPI_RXCTL_REN, &sdev->regs->rx_control);
		writel(SPI_TXCTL_TEN | SPI_TXCTL_TTI, &sdev->regs->tx_control);

		while (bytes--) {
			value = (tx ? *tx++ : CONFIG_SPI_IDLE_VAL);
			debug("%s: tx:%x ", __func__, value);
			writel(value, &sdev->regs->tfifo);
			while (readl(&sdev->regs->status) & SPI_STAT_RFE)
				if (ctrlc())
					return -1;
			value = readl(&sdev->regs->rfifo);
			if (rx)
				*rx++ = value;
			debug("rx:%x\n", value);
		}
//	}

	return 0;
}

int spi_xfer(struct spi_slave *slave, unsigned int bitlen, const void *dout,
		void *din, unsigned long flags)
{
	struct adi_spi_slave *sdev = to_adi_spi_slave(slave);
	const u8 *tx = dout;
	u8 *rx = din;
	uint bytes = bitlen / 8;
	int ret = 0;

	debug("%s: bus:%i cs:%i bitlen:%i bytes:%i flags:%lx\n", __func__,
		slave->bus, slave->cs, bitlen, bytes, flags);

#ifdef CONFIG_SPI_FLASH_MMAP
	if (flags & SPI_XFER_MMAP) {
		u32 control = sdev->control;
		u32 delay = SPI_DLY_LEADX | SPI_DLY_LAGX | 3;
		u32 ssel = readl(&sdev->regs->ssel);

#if defined(CONFIG_SPI_FLASH_STMICRO)
		/* Use Fast read quad IO command in memory mapped mode */
		u32 mmrdh =  3 << SPI_MMRDH_ADRSIZE_OFF;
		mmrdh |= 0xEB | 5 << SPI_MMRDH_DMYSIZE_OFF;
#elif defined(CONFIG_SPI_FLASH_ISSI)
		u32 mmrdh =  4 << SPI_MMRDH_ADRSIZE_OFF;
		mmrdh |= 0xEC | 3 << SPI_MMRDH_DMYSIZE_OFF;
		/* TODO : Temporary fix to enable bit 11 to  set bus width to 4.
		Need to make this part specific */
		mmrdh |= (1 << 11);
#else
		u32 mmrdh =  3 << SPI_MMRDH_ADRSIZE_OFF;
		/* Use Octal word read quad IO command in memory mapped mode */
		mmrdh |= 0xE3 | 1 << SPI_MMRDH_DMYSIZE_OFF;
#endif
		mmrdh |= 1 << SPI_MMRDH_TRIDMY_OFF;
		/* Ask SPI controller to send out address via 4-pin mode */
		mmrdh |= SPI_MMRDH_ADRPINS | SPI_MMRDH_MERGE;

		control |= SPI_CTL_MMSE | SPI_CTL_MIO_QUAD;
		/* Enable hardware auto chip select signal */
		control |= SPI_CTL_ASSEL | SPI_CTL_SELST;
		control |= SPI_CTL_EMISO | SPI_CTL_SIZE32;
		control |= SPI_CTL_FMODE;
		control &= ~SPI_CTL_SOSI;

		writel(mmrdh, &sdev->regs->mmrdh);
		/* Set memory map top boundary */
		writel((u32)slave->memory_map + CONFIG_SPI_MM_SIZE,
			&sdev->regs->mmtop);
		writel(delay, &sdev->regs->delay);
		writel(SPI_RXCTL_REN, &sdev->regs->rx_control);
		writel(SPI_TXCTL_TEN | SPI_TXCTL_TTI, &sdev->regs->tx_control);
		writel(control, &sdev->regs->control);
		/* Enable hardware ssel pin */
		writel(ssel | BIT_SSEL_EN(slave->cs), &sdev->regs->ssel);
		return 0;
	} else if (flags & SPI_XFER_MMAP_END) {
		u32 ssel = readl(&sdev->regs->ssel);

		/* Restore control reg for non memory mapped transfer */
		writel(sdev->control, &sdev->regs->control);
		writel(0, &sdev->regs->delay);
		/* Disable hardware ssel pin */
		writel(ssel & ~BIT_SSEL_EN(slave->cs), &sdev->regs->ssel);
		return 0;
	}
#endif

	if (bitlen == 0)
		goto done;

	/* we can only do 8 bit transfers */
	if (bitlen % 8) {
		flags |= SPI_XFER_END;
		goto done;
	}

	if (flags & SPI_XFER_BEGIN)
		spi_cs_activate(slave);

	ret = spi_pio_xfer(sdev, tx, rx, bytes, flags);

 done:
	if (flags & SPI_XFER_END)
		spi_cs_deactivate(slave);

	return ret;
}
