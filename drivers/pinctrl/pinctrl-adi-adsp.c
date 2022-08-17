// @todo spdx identifier
/**
 * dm pinctrl implementation for ADI ADSP SoCs
 *
 * Copyright 2022 (c), Analog Devices, Inc.
 * Author: Greg Malysa <greg.malysa@timesys.com>
 */

#include <common.h>
#include <dm.h>
#include <dm/device.h>
#include <dm/pinctrl.h>
#include <linux/bitops.h>
#include <linux/compat.h>
#include <linux/io.h>

#define ADSP_PORT_MMIO_SIZE		0x80
#define ADSP_PORT_PIN_SIZE		16

#define ADSP_PORT_PORT_MUX_BITS		2
#define ADSP_PORT_PORT_MUX_MASK		0x03
#define ADSP_PINCTRL_FUNCTION_COUNT 4

#define ADSP_PORT_REG_FER			0x00
#define ADSP_PORT_REG_FER_SET		0x04
#define ADSP_PORT_REG_FER_CLEAR		0x08
#define ADSP_PORT_REG_DATA			0x0c
#define ADSP_PORT_REG_DATA_SET		0x10
#define ADSP_PORT_REG_DATA_CLEAR	0x14
#define ADSP_PORT_REG_DIR			0x18
#define ADSP_PORT_REG_DIR_SET		0x1c
#define ADSP_PORT_REG_DIR_CLEAR		0x20
#define ADSP_PORT_REG_INEN			0x24
#define ADSP_PORT_REG_INEN_SET		0x28
#define ADSP_PORT_REG_INEN_CLEAR	0x2c
#define ADSP_PORT_REG_PORT_MUX		0x30
#define ADSP_PORT_REG_DATA_TGL		0x34
#define ADSP_PORT_REG_POLAR			0x38
#define ADSP_PORT_REG_POLAR_SET		0x3c
#define ADSP_PORT_REG_POLAR_CLEAR	0x40
#define ADSP_PORT_REG_LOCK			0x44
#define ADSP_PORT_REG_TRIG_TGL		0x48

struct adsp_pinctrl_priv {
	void __iomem *base;
	int npins;
	char pinbuf[16];
};

static u32 get_port(unsigned pin) {
	return pin / ADSP_PORT_PIN_SIZE;
}

static u32 get_offset(unsigned pin) {
	return pin % ADSP_PORT_PIN_SIZE;
}

static int adsp_pinctrl_get_pins_count(struct udevice *udev) {
	struct adsp_pinctrl_priv *priv = dev_get_priv(udev);
	return priv->npins;
}

static const char *adsp_pinctrl_get_pin_name(struct udevice *udev, unsigned selector) {
	struct adsp_pinctrl_priv *priv = dev_get_priv(udev);

	if (selector < priv->npins) {
		snprintf(priv->pinbuf, sizeof(priv->pinbuf), "pin%d", selector);
		return priv->pinbuf;
	}

	return "";
}

static int adsp_pinctrl_get_function_count(struct udevice *udev) {
	return ADSP_PINCTRL_FUNCTION_COUNT;
}

static const char *adsp_pinctrl_get_function_name(struct udevice *udev,
	unsigned selector)
{
	struct adsp_pinctrl_priv *priv = dev_get_priv(udev);

	if (selector < ADSP_PINCTRL_FUNCTION_COUNT) {
		snprintf(priv->pinbuf, sizeof(priv->pinbuf), "alt%d", selector);
		return priv->pinbuf;
	}

	return "";
}

static int adsp_pinctrl_pinmux_set(struct udevice *udev, unsigned pin, unsigned func)
{
	struct adsp_pinctrl_priv *priv = dev_get_priv(udev);
	void __iomem *portbase;
	u32 port, offset;
	u32 val;

	if (pin >= priv->npins)
		return -ENODEV;

	if (func >= ADSP_PINCTRL_FUNCTION_COUNT)
		return -EINVAL;

	port = get_port(pin);
	offset = get_offset(pin);
	portbase = priv->base + port * ADSP_PORT_MMIO_SIZE;

	val = ioread32(portbase + ADSP_PORT_REG_PORT_MUX);
	val &= ~(ADSP_PORT_PORT_MUX_MASK << (ADSP_PORT_PORT_MUX_BITS * offset));
	val |= func << (ADSP_PORT_PORT_MUX_BITS * offset);
	iowrite32(val, portbase + ADSP_PORT_REG_PORT_MUX);

	iowrite32(BIT(offset), portbase + ADSP_PORT_REG_FER_SET);
	return 0;
}

static int adsp_pinctrl_set_state(struct udevice *udev, struct udevice *config) {
	struct adsp_pinctrl_priv *priv = dev_get_priv(udev);
	const struct fdt_property *pinlist;
	int length = 0;
	int ret, i;
	u32 pin, function;

	pinlist = dev_read_prop(config, "adi,pins", &length);
	if (!pinlist) {
		dev_err(udev, "missing adi,pins property in pinctrl config node\n");
		return -EINVAL;
	}

	if (length % (sizeof(uint32_t) * 2)) {
		dev_err(udev, "adi,pins property must be a multiple of two uint32_ts\n");
		return -EINVAL;
	}

	for (i = 0; i < length / sizeof(uint32_t); i += 2) {
		ret = dev_read_u32_index(config, "adi,pins", i, &pin);
		if (ret)
			return ret;

		ret = dev_read_u32_index(config, "adi,pins", i+1, &function);
		if (ret)
			return ret;

		ret = adsp_pinctrl_pinmux_set(udev, pin, function);
		if (ret)
			return ret;
	}

	return 0;
}

const struct pinctrl_ops adsp_pinctrl_ops = {
//	.get_pins_count = adsp_pinctrl_get_pins_count,
//	.get_pin_name = adsp_pinctrl_get_pin_name,
//	.get_functions_count = adsp_pinctrl_get_function_count,
//	.get_function_name = adsp_pinctrl_get_function_name,
//	.pinmux_set = adsp_pinctrl_pinmux_set,
	.set_state = adsp_pinctrl_set_state,
};

static int adsp_pinctrl_probe(struct udevice *udev) {
	struct adsp_pinctrl_priv *priv = dev_get_priv(udev);

	priv->base = dev_read_addr(udev);
	priv->npins = dev_read_u32_default(udev, "adi,npins", 0);

	if (!priv->npins) {
		dev_err(udev, "Missing adi,npins property!\n");
		return -ENOENT;
	}

	return 0;
}

static const struct udevice_id adsp_pinctrl_match[] = {
	{ .compatible = "adi,adsp-pinctrl" },
	{ },
};

U_BOOT_DRIVER(adi_adsp_pinctrl) = {
	.name = "adi_adsp_pinctrl",
	.id = UCLASS_PINCTRL,
	.of_match = adsp_pinctrl_match,
	.probe = adsp_pinctrl_probe,
	.priv_auto_alloc_size = sizeof(struct adsp_pinctrl_priv),
	.ops = &adsp_pinctrl_ops,
	.flags = DM_FLAG_PRE_RELOC,
};
