/*
 * U-boot - main board file
 *
 * Copyright (c) 2013-2014 Analog Devices Inc.
 *
 * Licensed under the GPL-2 or later.
 */

#ifndef __BOARD_SOFT_SWITCH_H__
#define __BOARD_SOFT_SWITCH_H__

#define IODIRA          0x0
#define IODIRB          0x1
#define OLATA           0x14
#define OLATB           0x15

struct switch_config {
        uchar dir0; /* IODIRA */
        uchar dir1; /* IODIRB */
        uchar value0; /* OLATA */
        uchar value1; /* OLATB */
};

static inline int setup_soft_switch(int addr, struct switch_config *config)
{
	int ret = 0;

	ret = i2c_write(addr, OLATA, 1, &config->value0, 1);
	if (ret)
		return ret;
	ret = i2c_write(addr, OLATB, 1, &config->value1, 1);
	if (ret)
		return ret;

	ret = i2c_write(addr, IODIRA, 1, &config->dir0, 1);
	if (ret)
		return ret;
	return i2c_write(addr, IODIRB, 1, &config->dir1, 1);
}


int setup_board_switches(void);

#endif /* __BOARD_SOFT_SWITCH_H__ */
