/*
 * U-boot - SPL management
 *
 * Copyright (c) 2022 Analog Devices Inc.
 *
 * Licensed under the GPL-2 or later.
 */

#include <common.h>
#include <netdev.h>
#include <phy.h>
#include <asm/io.h>
#include <asm/mach-types.h>
#include <asm/mach-adi/common/sc5xx.h>
#include <linux/delay.h>
#include <watchdog.h>
#include <asm/spl.h>

void spi_flash_override_defaults(unsigned int * bus,
				unsigned int * cs,
				unsigned int * speed,
				unsigned int * mode)
{
	if(*bus == 0 && *cs == 0){
		*speed = ADI_OSPI_SF_DEFAULT_SPEED;
	}else{
		*speed = CONFIG_SF_DEFAULT_SPEED;
	}
}

void adi_multiplex_ospi(){
#ifdef CONFIG_CADENCE_QSPI
#endif
}

int adi_enable_ethernet_softconfig()
{
	struct gpio_desc *eth1;
	struct gpio_desc *eth1_reset;
	struct gpio_desc *gige_reset;

	gpio_hog_lookup_name("~eth1-en", &eth1);
	gpio_hog_lookup_name("~eth1-reset", &eth1_reset);
	gpio_hog_lookup_name("~gige-reset", &gige_reset);

	dm_gpio_set_value(eth1, 1);
	dm_gpio_set_value(eth1_reset, 0);
	dm_gpio_set_value(gige_reset, 1);
}

int adi_disable_ethernet_softconfig()
{
	struct gpio_desc *eth1;
	struct gpio_desc *eth1_reset;
	struct gpio_desc *gige_reset;

	gpio_hog_lookup_name("~eth1-en", &eth1);
	gpio_hog_lookup_name("~eth1-reset", &eth1_reset);
	gpio_hog_lookup_name("~gige-reset", &gige_reset);

	dm_gpio_set_value(eth1, 1);
	dm_gpio_set_value(eth1_reset, 0);
	dm_gpio_set_value(gige_reset, 0);
}

#ifdef CONFIG_OF_BOARD_SETUP
int ft_board_setup(void *blob, struct bd_info *bd)
{
	return 0;
}
#endif
