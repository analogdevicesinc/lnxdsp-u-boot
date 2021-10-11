/*
 * soft_swich.h
 *
 * Copyright (c) 2013-2014 Analog Devices Inc.
 *
 * Licensed under the GPL-2 or later.
 */

#include <asm/arch/soft_switch.h>

#define NUM_SWITCH      2

#undef MX66LM1G45 /* use IS25LP512 */

static struct switch_config switch_config_array_ethernet_enabled[NUM_SWITCH] = {
	{
/*
	U6 Port A                      U6 Port B

	7--------------- ~ADAU1979_EN   |  7--------------- ~GIGE_RESET
	| 6------------- ~ADAU1962_EN   |  | 6------------- ~ETH1_RESET
	| | 5----------- ~ADAU_RESET    |  | | 5----------- ~ETH1_EN
	| | | 4--------- ~CAN_EN        |  | | | 4--------- ~MLB_EN
	| | | | 3------- ~FTDI_USB_EN   |  | | | | 3------- AUDIO_JACK_SEL
	| | | | | 2----- ~MICROSD_SPI   |  | | | | | 2----- ~SPDIF_OPTICAL_EN
	| | | | | | 1--- PB_EN          |  | | | | | | 1--- ~SPDIF_DIGITAL_EN
	| | | | | | | 0- EEPROM_EN      |  | | | | | | | 0- OCTAL_SPI_CS_EN
	| | | | | | | |                 |  | | | | | | | |
	O O O O O O O O                 |  O O O O O O O O   (I/O direction)
	1 1 1 1 0 1 0 0                 |  1 0 1 1 0 1 1 0   (value being set)
*/
		.i2c_bus = 2,
		.i2c_addr = 0x22,
		.dir0 = 0x0, /* all output */
		.dir1 = 0x0, /* all output */
		.value0 = 0xF4,
		.value1 = 0xB6,
		.is23018 = 0x0,
		.pullup0 = 0x0,
		.pullup1 = 0x0,
	},

	{
/*
	U16 Port A                      U16 Port B

	7--------------- Unused         |  7--------------- Unused
	| 6------------- ~UART0_FLOW_EN |  | 6------------- Unused
	| | 5----------- ~UART0_EN      |  | | 5----------- Unused
	| | | 4--------- ~SPI2D2_D3_EN  |  | | | 4--------- Unused
	| | | | 3------- ~SPI2FLASH_CS  |  | | | | 3------- Unused
	| | | | | 2----- LED            |  | | | | | 2----- Unused
	| | | | | | 1--- LED            |  | | | | | | 1--- ~EMMC_SOM_EN
	| | | | | | | 0- LED            |  | | | | | | | 0- ~EMMC_EN
	| | | | | | | |                 |  | | | | | | | |
	O O O O O O O O                 |  O O O O O O O O   (I/O direction)
	0 0 0 0 0 1 1 1                 |  0 0 0 0 0 0 1 0   (value being set)
*/
		.i2c_bus = 2,
		.i2c_addr = 0x20,
		.dir0 = 0x0, /* all output */
		.dir1 = 0x0, /* all output */
		.value0 = 0x07,
		.value1 = 0x2,
		.is23018 = 0x1,
		.pullup0 = 0x07,
		.pullup1 = 0x02,
	},

};

static struct switch_config switch_config_array_ethernet_disabled[NUM_SWITCH] = {
	{
/*
	U6 Port A                      U6 Port B

	7--------------- ~ADAU1979_EN   |  7--------------- ~GIGE_RESET
	| 6------------- ~ADAU1962_EN   |  | 6------------- ~ETH1_RESET
	| | 5----------- ~ADAU_RESET    |  | | 5----------- ~ETH1_EN
	| | | 4--------- ~CAN_EN        |  | | | 4--------- ~MLB_EN
	| | | | 3------- ~FTDI_USB_EN   |  | | | | 3------- AUDIO_JACK_SEL
	| | | | | 2----- ~MICROSD_SPI   |  | | | | | 2----- ~SPDIF_OPTICAL_EN
	| | | | | | 1--- PB_EN          |  | | | | | | 1--- ~SPDIF_DIGITAL_EN
	| | | | | | | 0- EEPROM_EN      |  | | | | | | | 0- OCTAL_SPI_CS_EN
	| | | | | | | |                 |  | | | | | | | |
	O O O O O O O O                 |  O O O O O O O O   (I/O direction)
	1 1 1 1 0 1 0 0                 |  0 0 1 1 0 1 1 0   (value being set)
*/
		.i2c_bus = 2,
		.i2c_addr = 0x22,
		.dir0 = 0x0, /* all output */
		.dir1 = 0x0, /* all output */
		.value0 = 0xF4,
		.value1 = 0x36,
		.is23018 = 0x0,
		.pullup0 = 0x0,
		.pullup1 = 0x0,
	},

	{
/*
	U16 Port A                      U16 Port B

	7--------------- Unused         |  7--------------- Unused
	| 6------------- ~UART0_FLOW_EN |  | 6------------- Unused
	| | 5----------- ~UART0_EN      |  | | 5----------- Unused
	| | | 4--------- ~SPI2D2_D3_EN  |  | | | 4--------- Unused
	| | | | 3------- ~SPI2FLASH_CS  |  | | | | 3------- Unused
	| | | | | 2----- LED            |  | | | | | 2----- Unused
	| | | | | | 1--- LED            |  | | | | | | 1--- ~EMMC_SOM_EN
	| | | | | | | 0- LED            |  | | | | | | | 0- ~EMMC_EN
	| | | | | | | |                 |  | | | | | | | |
	O O O O O O O O                 |  O O O O O O O O   (I/O direction)
	0 0 0 0 0 0 0 0                 |  0 0 0 0 0 0 1 0   (value being set)
*/
		.i2c_bus = 2,
		.i2c_addr = 0x20,
		.dir0 = 0x0, /* all output */
		.dir1 = 0x0, /* all output */
		.value0 = 0x0,
		.value1 = 0x2,
		.is23018 = 0x01,
		.pullup0 = 0x07,
		.pullup1 = 0x02,
	},

};
