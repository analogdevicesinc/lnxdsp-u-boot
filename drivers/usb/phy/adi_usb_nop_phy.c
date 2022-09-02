// SPDX-License-Identifier: GPL-2.0+
/*
 * NOP USB transceiver for all USB transceiver which are either built-in
 * into USB IP or which are mostly autonomous.
 *
 * Copyright (C) 2009 Texas Instruments Inc
 * Copyright (C) 2017 Texas Instruments Incorporated - http://www.ti.com/
 * Copyright (C) 2021 Linaro
 * Author: Ajay Kumar Gupta <ajay.gupta@ti.com>
 *         Jean-Jacques Hiblot  <jjhiblot@ti.com>
 *         Ying-Chun Liu (PaulLiu) <paul.liu@linaro.org>
 * Current status:
 *      This provides a "nop" transceiver for PHYs which are
 *      autonomous such as isp1504, isp1707, etc.
 */

#include <clk.h>
#include <common.h>
#include <generic-phy.h>
#include <asm/gpio.h>
#include <dm.h>
#include <dm/device.h>
#include <dm/device_compat.h>
#include <linux/delay.h>
#include <power/regulator.h>

struct usb_nop_phy_priv {
	struct clk_bulk bulk;
	struct udevice *vcc;
	struct gpio_desc gpiod_reset;
};

static int usb_nop_phy_reset(struct phy *phy)
{
	int ret = 0;
	struct usb_nop_phy_priv *priv = dev_get_priv(phy->dev);

	if (!dm_gpio_is_valid(&priv->gpiod_reset))
		return ret;

	ret = dm_gpio_set_value(&priv->gpiod_reset, 0);
	mdelay(20);
	ret = dm_gpio_set_value(&priv->gpiod_reset, 1);

	return ret;
}

static int usb_nop_phy_init(struct phy *phy)
{
	struct usb_nop_phy_priv *priv = dev_get_priv(phy->dev);
	int ret = 0;

	if (CONFIG_IS_ENABLED(DM_REGULATOR) && priv->vcc) {
		ret = regulator_set_enable(priv->vcc, true);
		if (ret < 0) {
			dev_err(phy->dev, "Failed to enable power: %d\n", ret);
			return ret;
		}
	}

	if (CONFIG_IS_ENABLED(CLK)) {
		ret = clk_enable_bulk(&priv->bulk);
		if (ret < 0) {
			dev_err(phy->dev, "Failed to enable clk: %d\n", ret);
			return ret;
		}
	}

	usb_nop_phy_reset(phy);

	return ret;
}

static int usb_nop_phy_probe(struct udevice *dev)
{
	struct usb_nop_phy_priv *priv = dev_get_priv(dev);
	u32 clk_rate = 0;
	int i, ret = 0;

	/* Get all clocks. Actually only main_clk should be assigned */
	if (CONFIG_IS_ENABLED(CLK)) {
		ret = clk_get_bulk(dev, &priv->bulk);
		if (ret < 0)
			dev_info(dev, "failed to get clock: %d\n", ret);
	}

	/* Get vcc-supply */
	if (CONFIG_IS_ENABLED(DM_REGULATOR)) {
		ret = device_get_supply_regulator(dev, "vcc-supply", &priv->vcc);
		if (ret < 0) {
			dev_info(dev, "failed to get vcc-supply: %d\n", ret);
			priv->vcc = NULL;
		}
	}

	ret = ofnode_read_u32(dev_ofnode(dev), "clock-frequency", &clk_rate);
	if (ret < 0)
		clk_rate = 0;

	ret = gpio_request_by_name(dev, "reset", 0, &priv->gpiod_reset, GPIOD_IS_OUT);
	if (ret){
		pr_err("%s: Warning, reset line not found\n", __func__);
	}

	if (clk_rate)
		for (i = 0; i < priv->bulk.count; i++) {
			ret = clk_set_rate(&priv->bulk.clks[i], clk_rate);
			if (ret < 0) {
				dev_err(dev,
					"Failed to set clk rate %d: %d\n",
					clk_rate,
					ret);
				return ret;
			}
		}

	return 0;
}

static const struct udevice_id usb_nop_phy_ids[] = {
	{ .compatible = "usb-nop-xceiv" },
	{ }
};

static struct phy_ops usb_nop_phy_ops = {
	.init = usb_nop_phy_init,
	.reset = usb_nop_phy_reset,
};

U_BOOT_DRIVER(usb_nop_phy) = {
	.name	= "usb_nop_phy",
	.id	= UCLASS_PHY,
	.of_match = usb_nop_phy_ids,
	.ops = &usb_nop_phy_ops,
	.probe = usb_nop_phy_probe,
	.priv_auto_alloc_size	= sizeof(struct usb_nop_phy_priv),
};
