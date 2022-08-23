/*
 * soft_swich.h
 *
 * Copyright (c) 2013-2014 Analog Devices Inc.
 *
 * Licensed under the GPL-2 or later.
 */

#include <asm/mach-adi/common/soft_switch.h>
#include "soft_switch_bits.h"

#define NUM_SWITCH      2

#define PortA_Address22                                                       \
		DISABLE_ADAU1979                                              | \
		DISABLE_ADAU1962                                              | \
		ENABLE_ADAU_RESET                                             | \
		DISABLE_MICROSD_SPI                                           | \
		DISABLE_PB                                                    | \
		DISABLE_EEPROM

#define PortB_Address22_EthEnable         \
		DISABLE_GIGE_RESET      | \
		ENABLE_ETH1_RESET       | \
		DISABLE_ETH1_EN

#define PortB_Address22_EthDisable         \
		ENABLE_GIGE_RESET        | \
		ENABLE_ETH1_RESET        | \
		DISABLE_ETH1_EN

#define PortB_Address22                  \
		DISABLE_MLB            | \
		DISABLE_AUDIO_JACK_SEL | \
		DISABLE_SPDIF_OPTICAL  | \
		DISABLE_SPDIF_DIGITAL  | \
		DISABLE_OCTAL_SPI_CS

#define PortA_Address21                                                               \
		DISABLE_UART0_FLOW                                                   | \
		ENABLE_UART0                                                         | \
		ENABLE_SPI2D2_D3                                                     | \
		ENABLE_SPI2FLASH_CS                                                  | \
		ENABLE_LED3                                                          | \
		ENABLE_LED2                                                          | \
		ENABLE_LED1

#define PortB_Address21             \
		ENABLE_EMMC_SOM   | \
		DISABLE_EMMC

static struct switch_config switch_config_array[NUM_SWITCH] = {
	{
		.i2c_bus = 2,
		.i2c_addr = 0x22,
		.dir0 = 0x0, /* all output */
		.dir1 = 0x0, /* all output */
		.value0 = PortA_Address22,
		.value1 = PortB_Address22 | PortB_Address22_EthDisable,
		.is23018 = 0x0,
	},
	{
		.i2c_bus = 2,
		.i2c_addr = 0x21,
		.dir0 = 0x0, /* all output */
		.dir1 = 0x0, /* all output */
		.value0 = PortA_Address21,
		.value1 = PortB_Address21,
		.is23018 = 0x0,
	},
};
