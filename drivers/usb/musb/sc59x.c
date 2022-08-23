/*
 * Analog Devices SC58x MUSB HCD driver for u-boot
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
#include <usb.h>
#include <adi/common/sc5xx.h>
#include "musb_core.h"

/* MUSB platform configuration */
struct musb_config musb_cfg = {
	.regs       = (struct musb_regs *)REG_USB0_FADDR,
	.timeout    = 0x3FFFFFF,
	.musb_speed = 0,
};

struct musb_dma_regs {
	u8 irq;
	u8 __pad0[3];
	u16 ctl;
	u16 __pad1;
	u32 addr;
	u32 cnt;
} __packed;

/*
 * This function read or write data to endpoint fifo
 * use DMA polling method to avoid buffer alignment issues
 *
 * ep		- Endpoint number
 * length	- Number of bytes to write to FIFO
 * fifo_data	- Pointer to data buffer to be read/write
 * is_write	- Flag for read or write
 */
void rw_fifo(u8 ep, u32 length, void *fifo_data, int write)
{
	struct musb_dma_regs *regs;
	u32 val = (u32)fifo_data;

	regs = (void *)REG_USB0_DMA_IRQ;
	regs += ep;

	writel(val, &regs->addr);
	writel(length, &regs->cnt);

	/* Enable the DMA */
	val = (ep << 4) | (1 << BITP_USB_DMA_CTL_EN)
		| (1 << BITP_USB_DMA_CTL_IE);
	if (write)
		val |= 1 << BITP_USB_DMA_CTL_DIR;
	writew(val, &regs->ctl);

	/* Wait for complete */
	while (!(readb(REG_USB0_DMA_IRQ) & (1 << ep)))
		continue;

	/* Reset DMA */
	writew(0, &regs->ctl);
}

void write_fifo(u8 ep, u32 length, void *fifo_data)
{
	rw_fifo(ep, length, fifo_data, 1);
}

void read_fifo(u8 ep, u32 length, void *fifo_data)
{
	rw_fifo(ep, length, fifo_data, 0);
}

int musb_platform_init(void)
{
	writew(20 << 1, REG_USB0_PLL_OSC);
	writeb(0x0, REG_USB0_VBUS_CTL);
	writeb(0x80, REG_USB0_PHY_CTL);
	return 0;
}

void musb_platform_deinit(void)
{
}
