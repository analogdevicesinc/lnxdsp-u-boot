/*
 * U-boot - main board file
 *
 * Copyright (c) 2013-2014 Analog Devices Inc.
 *
 * 2022 - Converted to driver model by Timesys Corporation
 *        Nathan Barrett-Morrison <nathan.morrison@timesys.com>
 *
 * Licensed under the GPL-2 or later.
 */

#ifndef __BOARD_SOFT_SWITCH_H__
#define __BOARD_SOFT_SWITCH_H__

#include <common.h>
#include <i2c.h>
#include <dm.h>

#define IODIRA          0x0
#define IODIRB          0x1
#define GPPUA           0xC
#define GPPUB           0xD
#define OLATA           0x14
#define OLATB           0x15

struct switch_config {
	u32 i2c_bus;
	u32 i2c_addr; /* i2c address of the switch */
	u8 dir0; /* IODIRA */
	u8 dir1; /* IODIRB */
	u8 value0; /* OLATA */
	u8 value1; /* OLATB */
	u8 is23018;
	u8 pullup0;
	u8 pullup1;
};

static inline int setup_soft_switch(struct switch_config *config)
{

	u32 ret;
	struct udevice *dev = NULL;

	ret = i2c_get_chip_for_busnum(config->i2c_bus, config->i2c_addr, 1, &dev);
	if (ret) {
		printf("%s: No bus %d\n", __func__, config->i2c_bus);
		return ret;
	}

	if(config->is23018){
		ret = dm_i2c_write(dev, GPPUA, &config->pullup0, 1);
		if (ret)
			return ret;
		ret = dm_i2c_write(dev, GPPUB, &config->pullup1, 1);
		if (ret)
			return ret;
	}

	ret = dm_i2c_write(dev, OLATA, &config->value0, 1);
	if (ret)
		return ret;
	ret = dm_i2c_write(dev, OLATB, &config->value1, 1);
	if (ret)
		return ret;

	ret = dm_i2c_write(dev, IODIRA, &config->dir0, 1);
	if (ret)
		return ret;
	return dm_i2c_write(dev, IODIRB, &config->dir1, 1);
}

static inline int setup_soft_switches(struct switch_config array[], int count)
{
	int ret;
	int i;

	for (i = 0; i < count; i++) {
		ret = setup_soft_switch(&array[i]);
		if (ret)
			return ret;
	}
	return 0;
}

#endif /* __BOARD_SOFT_SWITCH_H__ */
