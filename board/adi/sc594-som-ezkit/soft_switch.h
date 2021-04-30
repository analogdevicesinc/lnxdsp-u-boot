/*
 * soft_swich.h
 *
 * Copyright (c) 2013-2014 Analog Devices Inc.
 *
 * Licensed under the GPL-2 or later.
 */

#include <asm/arch/soft_switch.h>

#define NUM_SWITCH      2

static struct switch_config switch_config_array_ethernet_enabled[NUM_SWITCH] = {
	{
/*
	U6 Port A                      U6 Port B

	7--------------- ADAU1979_EN   |  7--------------- GIGE_RESET
	| 6------------- ADAU1962_EN   |  | 6------------- ETH1_RESET
	| | 5----------- ADAU_RESET    |  | | 5----------- ETH1_EN
	| | | 4---------               |  | | | 4--------- MLB_EN
	| | | | 3-------               |  | | | | 3------- AUDIO_JACK_SEL
	| | | | | 2----- MICROSD_SPI   |  | | | | | 2----- SPDIF_OPTICAL_EN
	| | | | | | 1--- PB_EN         |  | | | | | | 1--- SPDIF_DIGITAL_EN
	| | | | | | | 0- EEPROM_EN     |  | | | | | | | 0- OCTAL_SPI_CS_EN
	| | | | | | | |                |  | | | | | | | |
	O O O O O O O O                |  O O O O O O O O   (I/O direction)
	0 0 0 0 0 0 0 0                |  1 1 0 0 0 0 0 1   (value being set)
*/
		.i2c_bus = 2,
		.i2c_addr = 0x22,
		.dir0 = 0x0, /* all output */
		.dir1 = 0x0, /* all output */
		.value0 = 0x0,
		.value1 = 0xC0,
	},

	{
/*
	U16 Port A                      U16 Port B

	7--------------- OSPIFLASH_CS  |  7--------------- Unused
	| 6------------- UART0_FLOW_EN |  | 6------------- Unused
	| | 5----------- UART0_EN      |  | | 5----------- Unused
	| | | 4--------- SPI2D2_D3_EN  |  | | | 4--------- Unused
	| | | | 3------- SPI2FLASH_CS  |  | | | | 3------- Unused
	| | | | | 2----- LED           |  | | | | | 2----- Unused
	| | | | | | 1--- LED           |  | | | | | | 1--- Unused
	| | | | | | | 0- LED           |  | | | | | | | 0- Unused
	| | | | | | | |                |  | | | | | | | |
	O O O O O O O O                |  O O O O O O O O   (I/O direction)
	0 0 0 1 1 1 1 1                |  0 0 0 0 0 0 0 0   (value being set)
*/
		.i2c_bus = 2,
		.i2c_addr = 0x21,
		.dir0 = 0x0, /* all output */
		.dir1 = 0x0, /* all output */
		.value0 = 0x7,
		.value1 = 0x0,
	},

};

static struct switch_config switch_config_array_ethernet_disabled[NUM_SWITCH] = {
	{
/*
	U6 Port A                      U6 Port B

	7--------------- ADAU1979_EN   |  7--------------- GIGE_RESET
	| 6------------- ADAU1962_EN   |  | 6------------- ETH1_RESET
	| | 5----------- ADAU_RESET    |  | | 5----------- ETH1_EN
	| | | 4---------               |  | | | 4--------- MLB_EN
	| | | | 3-------               |  | | | | 3------- AUDIO_JACK_SEL
	| | | | | 2----- MICROSD_SPI   |  | | | | | 2----- SPDIF_OPTICAL_EN
	| | | | | | 1--- PB_EN         |  | | | | | | 1--- SPDIF_DIGITAL_EN
	| | | | | | | 0- EEPROM_EN     |  | | | | | | | 0- OCTAL_SPI_CS_EN
	| | | | | | | |                |  | | | | | | | |
	O O O O O O O O                |  O O O O O O O O   (I/O direction)
	0 0 0 0 0 0 0 0                |  0 0 0 0 0 0 0 1   (value being set)
*/
		.i2c_bus = 2,
		.i2c_addr = 0x22,
		.dir0 = 0x0, /* all output */
		.dir1 = 0x0, /* all output */
		.value0 = 0x0,
		.value1 = 0x0,
	},

	{
/*
	U16 Port A                      U16 Port B

	7--------------- OSPIFLASH_CS  |  7--------------- Unused
	| 6------------- UART0_FLOW_EN |  | 6------------- Unused
	| | 5----------- UART0_EN      |  | | 5----------- Unused
	| | | 4--------- SPI2D2_D3_EN  |  | | | 4--------- Unused
	| | | | 3------- SPI2FLASH_CS  |  | | | | 3------- Unused
	| | | | | 2----- LED           |  | | | | | 2----- Unused
	| | | | | | 1--- LED           |  | | | | | | 1--- Unused
	| | | | | | | 0- LED           |  | | | | | | | 0- Unused
	| | | | | | | |                |  | | | | | | | |
	O O O O O O O O                |  O O O O O O O O   (I/O direction)
	0 0 0 1 1 0 0 0                |  0 0 0 0 0 0 0 0   (value being set)
*/
		.i2c_bus = 2,
		.i2c_addr = 0x21,
		.dir0 = 0x0, /* all output */
		.dir1 = 0x0, /* all output */
		.value0 = 0x0,
		.value1 = 0x0,
	},

};
