// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * (C) Copyright 2022 - Analog Devices, Inc.
 *
 * Written and/or maintained by Timesys Corporation
 *
 * Author: Nathan Barrett-Morrison <nathan.morrison@timesys.com>
 * Additional Contact: Greg Malysa <greg.malysa@timesys.com>
 *
 * mcp23017 and mcp23018, i2c-based GPIO expander driver
 */

// #include <common.h>
#include <dm.h>
#include <i2c.h>
#include <log.h>
#include <asm-generic/gpio.h>
#include <asm/global_data.h>
#include <linux/bitops.h>
#include <linux/types.h>

#define PORTA_MAX			7  /* Pins 0 through 7 */
#define PORTB_MAX			15 /* Pins 8 through 15 */

#define PORT_A_IO_ADDR		0x00
#define PORT_A_PULLUP_ADDR	0x0C
#define PORT_A_GPIO_ADDR	0x12
#define PORT_A_OUTPUT_ADDR	0x14

#define PORT_B_IO_ADDR		0x01
#define PORT_B_PULLUP_ADDR	0x0D
#define PORT_B_GPIO_ADDR	0x13
#define PORT_B_OUTPUT_ADDR	0x15

struct mcp2301x_port {
	u32 io_direction_addr;
	u32 pullup_en_addr;
	u32 output_latch_addr;
	u32 gpio_addr;

	u8 io_direction;
	u8 pullup_en;
	u8 output_latch;
};

struct mcp2301x_chip {
	struct mcp2301x_port port_a;
	struct mcp2301x_port port_b;
	bool drive_pullups;
};

static struct mcp2301x_port *mcp2301x_select_port(struct mcp2301x_chip *plat,
						  u32 *offset)
{
	if (*offset <= PORTA_MAX)
		return &plat->port_a;

	if (*offset <= PORTB_MAX) {
		*offset = *offset - 8;
		return &plat->port_b;
	}

	pr_err("%s: Pin offset %x is greater than the number of supported pins.\n",
	       __func__, *offset);
	return 0;
}

static int mcp2301x_direction_input(struct udevice *dev, u32 offset)
{
	struct mcp2301x_chip *plat = dev_get_plat(dev);
	struct mcp2301x_port *port;
	int ret;

	port = mcp2301x_select_port(plat, &offset);

	if (port) {
		port->io_direction |= BIT(offset);

		ret = dm_i2c_write(dev, port->io_direction_addr, &port->io_direction, 1);
		if (ret)
			return ret;
	} else {
		return -EINVAL;
	}

	return 0;
}

static int mcp2301x_direction_output(struct udevice *dev,
				     u32 offset, int value)
{
	struct mcp2301x_chip *plat = dev_get_plat(dev);
	struct mcp2301x_port *port;
	int ret;

	port = mcp2301x_select_port(plat, &offset);

	if (port) {
		port->io_direction &= ~BIT(offset);

		if (value)
			port->output_latch |= BIT(offset);
		else
			port->output_latch &= ~BIT(offset);

		if (plat->drive_pullups) {
			ret = dm_i2c_write(dev, port->pullup_en_addr, &port->output_latch, 1);
			if (ret)
				return ret;
		}

		ret = dm_i2c_write(dev, port->io_direction_addr, &port->io_direction, 1);
		if (ret)
			return ret;

		ret = dm_i2c_write(dev, port->output_latch_addr, &port->output_latch, 1);
		if (ret)
			return ret;
	} else {
		return -EINVAL;
	}

	return 0;
}

static int mcp2301x_get_value(struct udevice *dev, u32 offset)
{
	struct mcp2301x_chip *plat = dev_get_plat(dev);
	struct mcp2301x_port *port;
	int ret;
	u8 value;

	port = mcp2301x_select_port(plat, &offset);

	if (port) {
		ret = dm_i2c_read(dev, port->gpio_addr, &value, 1);
		if (ret)
			return ret;

		return (value & BIT(offset)) ? 1 : 0;
	} else {
		return -EINVAL;
	}
}

static int mcp2301x_set_value(struct udevice *dev, u32 offset,
			      int32_t value)
{
	return mcp2301x_direction_output(dev, offset, value);
}

static int mcp2301x_ofdata_platdata(struct udevice *dev)
{
	struct mcp2301x_chip *plat = dev_get_plat(dev);
	struct gpio_dev_priv *priv = dev_get_uclass_priv(dev);
	int node = dev_of_offset(dev);

	plat->port_a.io_direction_addr	= PORT_A_IO_ADDR;
	plat->port_a.pullup_en_addr		= PORT_A_PULLUP_ADDR;
	plat->port_a.gpio_addr			= PORT_A_GPIO_ADDR;
	plat->port_a.output_latch_addr	= PORT_A_OUTPUT_ADDR;

	plat->port_b.io_direction_addr	= PORT_B_IO_ADDR;
	plat->port_b.pullup_en_addr		= PORT_B_PULLUP_ADDR;
	plat->port_b.gpio_addr			= PORT_B_GPIO_ADDR;
	plat->port_b.output_latch_addr	= PORT_B_OUTPUT_ADDR;

	priv->gpio_count = 16;
	priv->bank_name = fdt_get_name(gd->fdt_blob, node, NULL);

	if (dev_read_bool(dev, "drive-pullups"))
		plat->drive_pullups = 1;
	else
		plat->drive_pullups = 0;

	return 0;
}

static const struct dm_gpio_ops mcp2301x_ops = {
	.direction_input  = mcp2301x_direction_input,
	.direction_output = mcp2301x_direction_output,
	.get_value		  = mcp2301x_get_value,
	.set_value		  = mcp2301x_set_value,
};

static const struct udevice_id mcp2301x_of_match_list[] = {
	{ .compatible = "microchip,mcp2301x"},
	{ .compatible = "microchip,mcp23017"},
	{ .compatible = "microchip,mcp23018"},
	{ /* sentinel */ }
};

U_BOOT_DRIVER(gpio_mcp2301x) = {
	.name					  = "gpio_mcp2301x",
	.id						  = UCLASS_GPIO,
	.ops					  = &mcp2301x_ops,
	.of_match				  = mcp2301x_of_match_list,
	.of_to_plat		  = mcp2301x_ofdata_platdata,
	.plat_auto = sizeof(struct mcp2301x_chip),
	.flags					  = DM_FLAG_PRE_RELOC,
};
