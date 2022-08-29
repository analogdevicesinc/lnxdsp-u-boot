/*
 * i2c.c - driver for ADI TWI/I2C
 *
 * Copyright (c) 2006-2014 Analog Devices Inc.
 *
 * 2022 - Converted to driver model by Timesys Corporation
 *        Nathan Barrett-Morrison <nathan.morrison@timesys.com>
 *
 * Licensed under the GPL-2 or later.
 */

#include <common.h>
#include <command.h>
#include <i2c.h>
#include <asm/mach-adi/common/clock.h>
#include <asm/mach-adi/common/twi.h>
#include <asm/io.h>
#include <console.h>
#include <clk.h>
#include <dm.h>
#include <mapmem.h>
#include <linux/delay.h>

/* Every register is 32bit aligned, but only 16bits in size */
#define ureg(name) u16 name; u16 __pad_##name;
struct twi_regs {
	ureg(clkdiv);
	ureg(control);
	ureg(slave_ctl);
	ureg(slave_stat);
	ureg(slave_addr);
	ureg(master_ctl);
	ureg(master_stat);
	ureg(master_addr);
	ureg(int_stat);
	ureg(int_mask);
	ureg(fifo_ctl);
	ureg(fifo_stat);
	u8 __pad[0x50];
	ureg(xmt_data8);
	ureg(xmt_data16);
	ureg(rcv_data8);
	ureg(rcv_data16);
};
#undef ureg

/*
 * The way speed is changed into duty often results in integer truncation
 * with 50% duty, so we'll force rounding up to the next duty by adding 1
 * to the max.  In practice this will get us a speed of something like
 * 385 KHz.  The other limit is easy to handle as it is only 8 bits.
 */
#define I2C_SPEED_MAX             400000
#define I2C_SPEED_TO_DUTY(speed)  (5000000 / (speed))
#define I2C_DUTY_MAX              (I2C_SPEED_TO_DUTY(I2C_SPEED_MAX) + 1)
#define I2C_DUTY_MIN              0xff	/* 8 bit limited */

#define I2C_M_COMBO		0x4
#define I2C_M_STOP		0x2
#define I2C_M_READ		0x1

/* All transfers are described by this data structure */
struct adi_i2c_msg {
	u8 flags;
	u32 len;		/* msg length */
	u8 *buf;		/* pointer to msg data */
	u32 olen;		/* addr length */
	u8 *obuf;		/* addr buffer */
};

struct adi_i2c_dev {
	struct twi_regs  __iomem *base;
	u32 i2c_clk;
	uint speed;
};

/* Allow msec timeout per ~byte transfer */
#define I2C_TIMEOUT 10

/**
 * wait_for_completion - manage the actual i2c transfer
 *	@msg: the i2c msg
 */
static int wait_for_completion(struct twi_regs *twi, struct adi_i2c_msg *msg)
{
	u16 int_stat, ctl;
	ulong timebase = get_timer(0);

	do {
		int_stat = readw(&twi->int_stat);

		if (int_stat & XMTSERV) {
			writew(XMTSERV, &twi->int_stat);
			if (msg->olen) {
				writew(*(msg->obuf++), &twi->xmt_data8);
				--msg->olen;
			} else if (!(msg->flags & I2C_M_COMBO) && msg->len) {
				writew(*(msg->buf++), &twi->xmt_data8);
				--msg->len;
			} else {
				ctl = readw(&twi->master_ctl);
				if (msg->flags & I2C_M_COMBO)
					writew(ctl | RSTART | MDIR,
							&twi->master_ctl);
				else
					writew(ctl | STOP, &twi->master_ctl);
			}
		}
		if (int_stat & RCVSERV) {
			writew(RCVSERV, &twi->int_stat);
			if (msg->len) {
				*(msg->buf++) = readw(&twi->rcv_data8);
				--msg->len;
			} else if (msg->flags & I2C_M_STOP) {
				ctl = readw(&twi->master_ctl);
				writew(ctl | STOP, &twi->master_ctl);
			}
		}
		if (int_stat & MERR) {
			writew(MERR, &twi->int_stat);
			return msg->len;
		}
		if (int_stat & MCOMP) {
			writew(MCOMP, &twi->int_stat);
			if (msg->flags & I2C_M_COMBO && msg->len) {
				ctl = readw(&twi->master_ctl);
				ctl = (ctl & ~RSTART) |
					(min(msg->len, 0xff) << 6) | MEN | MDIR;
				writew(ctl, &twi->master_ctl);
			} else
				break;
		}

		/* If we were able to do something, reset timeout */
		if (int_stat)
			timebase = get_timer(0);

	} while (get_timer(timebase) < I2C_TIMEOUT);

	return msg->len;
}

static int i2c_transfer(struct twi_regs *twi, uint8_t chip, uint offset,
			int olen, uint8_t *buffer, int len, uint8_t flags)
{
	int ret;
	u16 ctl;
	uchar offset_buffer[] = {
		(offset >>  0),
		(offset >>  8),
		(offset >> 16),
	};
	struct adi_i2c_msg msg = {
		.flags = flags | (len >= 0xff ? I2C_M_STOP : 0),
		.buf   = buffer,
		.len   = len,
		.obuf  = offset_buffer,
		.olen  = olen,
	};

	/* wait for things to settle */
	while (readw(&twi->master_stat) & BUSBUSY)
		if (ctrlc())
			return 1;

	/* Set Transmit device address */
	writew(chip, &twi->master_addr);

	/* Clear the FIFO before starting things */
	writew(XMTFLUSH | RCVFLUSH, &twi->fifo_ctl);
	writew(0, &twi->fifo_ctl);

	/* prime the pump */
	if (msg.olen) {
		len = (msg.flags & I2C_M_COMBO) ? msg.olen : msg.olen + len;
		writew(*(msg.obuf++), &twi->xmt_data8);
		--msg.olen;
	} else if (!(msg.flags & I2C_M_READ) && msg.len) {
		writew(*(msg.buf++), &twi->xmt_data8);
		--msg.len;
	}

	/* clear int stat */
	writew(-1, &twi->master_stat);
	writew(-1, &twi->int_stat);
	writew(0, &twi->int_mask);

	/* Master enable */
	ctl = readw(&twi->master_ctl);
	ctl = (ctl & FAST) | (min(len, 0xff) << 6) | MEN |
		((msg.flags & I2C_M_READ) ? MDIR : 0);
	writew(ctl, &twi->master_ctl);

	/* process the rest */
	ret = wait_for_completion(twi, &msg);

	if (ret) {
		ctl = readw(&twi->master_ctl) & ~MEN;
		writew(ctl, &twi->master_ctl);
		ctl = readw(&twi->control) & ~TWI_ENA;
		writew(ctl, &twi->control);
		ctl = readw(&twi->control) | TWI_ENA;
		writew(ctl, &twi->control);
	}

	return ret;
}

static int adi_i2c_read(struct twi_regs *twi, uint8_t chip,
			uint offset, int olen, uint8_t *buffer, int len)
{
	return i2c_transfer(twi, chip, offset, olen, buffer,
			len, olen ? I2C_M_COMBO : I2C_M_READ);
}

static int adi_i2c_write(struct twi_regs *twi, uint8_t chip,
			uint offset, int olen, uint8_t *buffer, int len)
{
	return i2c_transfer(twi, chip, offset, olen, buffer, len, 0);
}

static int adi_i2c_set_bus_speed(struct udevice *bus, uint speed){
	struct adi_i2c_dev *dev = dev_get_priv(bus);
	struct twi_regs *twi = dev->base;
	u16 clkdiv = I2C_SPEED_TO_DUTY(speed);

	/* Set TWI interface clock */
	if (clkdiv < I2C_DUTY_MAX || clkdiv > I2C_DUTY_MIN)
		return -1;
	clkdiv = (clkdiv << 8) | (clkdiv & 0xff);
	writew(clkdiv, &twi->clkdiv);

	/* Don't turn it on */
	writew(speed > 100000 ? FAST : 0, &twi->master_ctl);

	return 0;
}

static int adi_i2c_ofdata_to_platdata(struct udevice *bus){
	struct adi_i2c_dev *dev = dev_get_priv(bus);
	struct clk clock;

	dev->base = map_sysmem(dev_read_addr(bus), sizeof(struct twi_regs));

	if (!dev->base)
		return -ENOMEM;

	dev->speed = dev_read_u32_default(bus, "clock-frequency",
					  I2C_SPEED_FAST_RATE);

	if (!clk_get_by_index(bus, 0, &clock))
		dev->i2c_clk = clk_get_rate(&clock);

	return 0;
}

static int adi_i2c_probe_chip(struct udevice *bus, u32 chip_addr,
			      u32 chip_flags){
	struct adi_i2c_dev *dev = dev_get_priv(bus);
	u8 byte;

	i2c_transfer(dev->base, chip_addr, 0, 0, &byte, 1, 0);
	return 0;
}

static int adi_i2c_xfer(struct udevice *bus, struct i2c_msg *msg, int nmsgs){
	struct adi_i2c_dev *dev = dev_get_priv(bus);
	struct i2c_msg *dmsg, *omsg, dummy;

	memset(&dummy, 0, sizeof(struct i2c_msg));

	/* We expect either two messages (one with an offset and one with the
	 * actual data) or one message (just data)
	 */
	if (nmsgs > 2 || nmsgs == 0) {
		debug("%s: Only one or two messages are supported.", __func__);
		return -1;
	}

	omsg = nmsgs == 1 ? &dummy : msg;
	dmsg = nmsgs == 1 ? msg : msg + 1;

	if (dmsg->flags & I2C_M_RD)
		return adi_i2c_read(dev->base, dmsg->addr, omsg->buf, omsg->len,
				  dmsg->buf, dmsg->len);
	else
		return adi_i2c_write(dev->base, dmsg->addr, omsg->buf, omsg->len,
				   dmsg->buf, dmsg->len);
}


int adi_i2c_probe(struct udevice *bus)
{
	struct adi_i2c_dev *dev = dev_get_priv(bus);
	struct twi_regs *twi = dev->base;

	u16 prescale = ((get_i2c_clk() / 1000 / 1000 + 5) / 10) & 0x7F;

	/* Set TWI internal clock as 10MHz */
	writew(prescale, &twi->control);

	/* Set TWI interface clock as specified */
	adi_i2c_set_bus_speed(bus, dev->speed);

	/* Enable it */
	writew(TWI_ENA | prescale, &twi->control);

	return 0;
}

static const struct dm_i2c_ops adi_i2c_ops = {
	.xfer           = adi_i2c_xfer,
	.probe_chip     = adi_i2c_probe_chip,
	.set_bus_speed  = adi_i2c_set_bus_speed,
};

static const struct udevice_id adi_i2c_ids[] = {
	{ .compatible = "adi-i2c", },
	{ /* sentinel */ }
};

U_BOOT_DRIVER(i2c_adi) = {
	.name = "i2c_adi",
	.id = UCLASS_I2C,
	.of_match = adi_i2c_ids,
	.probe = adi_i2c_probe,
	.ofdata_to_platdata = adi_i2c_ofdata_to_platdata,
	.priv_auto_alloc_size = sizeof(struct adi_i2c_dev),
	.ops = &adi_i2c_ops,
	.flags = DM_FLAG_PRE_RELOC,
};
