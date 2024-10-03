/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * (C) Copyright 2022 - Analog Devices, Inc.
 *
 * Written and/or maintained by Timesys Corporation
 *
 * Contact: Nathan Barrett-Morrison <nathan.morrison@timesys.com>
 * Contact: Greg Malysa <greg.malysa@timesys.com>
 *
 * U-boot - Functions which are shared between u-boot proper + u-boot SPL
 *
 */

#include <common.h>
#include <netdev.h>
#include <phy.h>
#include <asm/io.h>
#include <asm/gpio.h>
#include <asm/mach-types.h>
#include <asm/arch-adi/sc5xx/sc5xx.h>
#include <linux/delay.h>
#include <watchdog.h>
#include <asm/armv8/mmu.h>
#include <asm/spl.h>
#include <asm/arch-adi/sc5xx/sc59x-shared.h>

int adi_enable_ethernet_softconfig(void)
{
	struct gpio_desc *eth1;
	struct gpio_desc *eth1_reset;
	struct gpio_desc *gige_reset;
	int ret;

#if defined(CONFIG_ADI_CARRIER_SOMCRR_EZKIT)
	ret = gpio_hog_lookup_name("~eth1-en", &eth1);
	if (ret) {
		printf("Failed to find ~eth1-en GPIO\n");
		return ret;
	}

	ret = gpio_hog_lookup_name("~eth1-reset", &eth1_reset);
	if (ret) {
		printf("Failed to find ~eth1-reset GPIO\n");
		return ret;
	}

	ret = gpio_hog_lookup_name("~gige-reset", &gige_reset);
	if (ret) {
		printf("Failed to find ~gige-reset GPIO\n");
		return ret;
	}

	dm_gpio_set_value(eth1, 1);
	dm_gpio_set_value(eth1_reset, 0);
	dm_gpio_set_value(gige_reset, 1);
#elif defined(CONFIG_ADI_CARRIER_SOMCRR_EZLITE)
	ret = gpio_hog_lookup_name("~eth0-reset", &gige_reset);
	if (ret) {
		printf("Failed to find ~eth0-reset GPIO\n");
		return ret;
	}

	dm_gpio_set_value(gige_reset, 1);
#endif

	return 0;
}

int adi_disable_ethernet_softconfig(void)
{
	struct gpio_desc *eth1;
	struct gpio_desc *eth1_reset;
	struct gpio_desc *gige_reset;
	int ret;

#if defined(CONFIG_ADI_CARRIER_SOMCRR_EZKIT)
	ret = gpio_hog_lookup_name("~eth1-en", &eth1);
	if (ret) {
		printf("Failed to find ~eth1-en GPIO\n");
		return ret;
	}

	ret = gpio_hog_lookup_name("~eth1-reset", &eth1_reset);
	if (ret) {
		printf("Failed to find ~eth1-reset GPIO\n");
		return ret;
	}

	ret = gpio_hog_lookup_name("~gige-reset", &gige_reset);
	if (ret) {
		printf("Failed to find ~gige-reset GPIO\n");
		return ret;
	}

	dm_gpio_set_value(eth1, 1);
	dm_gpio_set_value(eth1_reset, 0);
	dm_gpio_set_value(gige_reset, 0);
#elif defined(CONFIG_ADI_CARRIER_SOMCRR_EZLITE)
	ret = gpio_hog_lookup_name("~eth0-reset", &gige_reset);
	if (ret) {
		printf("Failed to find ~eth0-reset GPIO\n");
		return ret;
	}

	dm_gpio_set_value(gige_reset, 0);
#endif

	return 0;
}

void spi_flash_override_defaults(unsigned int *bus,
				 unsigned int *cs,
				unsigned int *speed,
				unsigned int *mode)
{
	if (*bus == 0 && *cs == 0) {
		*speed = ADI_OSPI_SF_DEFAULT_SPEED;
	} else {
		*speed = CONFIG_SF_DEFAULT_SPEED;
	}
}

#ifdef CONFIG_OF_BOARD_SETUP
int ft_board_setup(void *blob, struct bd_info *bd)
{
	return 0;
}
#endif
