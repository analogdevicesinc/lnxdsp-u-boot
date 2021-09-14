/*
 * U-boot - main board file
 *
 * Copyright (c) 2013-2014 Analog Devices Inc.
 *
 * Licensed under the GPL-2 or later.
 */

#ifndef __BOARD_SOFT_SWITCH_H__
#define __BOARD_SOFT_SWITCH_H__

#include <common.h>
#include <i2c.h>

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
	int ret = 0;

	ret = i2c_set_bus_num(config->i2c_bus);
	if (ret)
		return ret;

	if(config->is23018){
		ret = i2c_write(config->i2c_addr, GPPUA, 1, &config->pullup0, 1);
		if (ret)
			return ret;
		ret = i2c_write(config->i2c_addr, GPPUB, 1, &config->pullup1, 1);
		if (ret)
			return ret;
	}

	ret = i2c_write(config->i2c_addr, OLATA, 1, &config->value0, 1);
	if (ret)
		return ret;
	ret = i2c_write(config->i2c_addr, OLATB, 1, &config->value1, 1);
	if (ret)
		return ret;

	ret = i2c_write(config->i2c_addr, IODIRA, 1, &config->dir0, 1);
	if (ret)
		return ret;
	return i2c_write(config->i2c_addr, IODIRB, 1, &config->dir1, 1);
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
