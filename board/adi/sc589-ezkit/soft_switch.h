/*
 * soft_swich.h
 *
 * Copyright (c) 2013-2014 Analog Devices Inc.
 *
 * Licensed under the GPL-2 or later.
 */

#include <asm/mach-adi/common/soft_switch.h>

#define NUM_SWITCH      2

static struct switch_config switch_config_array[NUM_SWITCH] = {
	{
/*
	U47 Port A                     U47 Port B

	7--------------- CAN1_EN       |  7--------------- SPDIF_DIGITAL_EN
	| 6------------- CAN0_EN       |  | 6------------- SPDIF_OPTICAL_EN
	| | 5----------- MLB3_EN       |  | | 5----------- SPI2D2_D3_EN
	| | | 4--------- ETH1_EN       |  | | | 4--------- SPI2FLASH_EN
	| | | | 3------- ETH0_EN       |  | | | | 3------- SD_WP_EN
	| | | | | 2----- UART0_EN      |  | | | | | 2----- AUDIO_JACK_EN
	| | | | | | 1--- UART0_FLOW_EN |  | | | | | | 1--- ADAU1979_EN
	| | | | | | | 0- EEPROM_EN     |  | | | | | | | 0- ADAU1962_EN
	| | | | | | | |                |  | | | | | | | |
	O O O O O O O O                |  O O O O O O O O   (I/O direction)
	0 0 0 0 0 0 0 0                |  0 0 0 0 0 1 0 0   (value being set)
*/
		.i2c_bus = 0,
		.i2c_addr = 0x21,
		.dir0 = 0x0, /* all output */
		.dir1 = 0x0, /* all output */
		.value0 = 0x0,
		.value1 = 0x4,
		.is23018 = 0x0,
	},
	{
/*
	U48 Port A                       U48 Port B

	7---------------  EMPTY          |  7---------------  PCIE_RC_EN
	| 6-------------  EMPTY          |  | 6-------------  EMPTY
	| | 5-----------  EMPTY          |  | | 5-----------  EMPTY
	| | | 4---------  LED12_EN       |  | | | 4---------  EMPTY
	| | | | 3-------  LED11_EN       |  | | | | 3-------  EMPTY
	| | | | | 2-----  LED10_EN       |  | | | | | 2-----  EMPTY
	| | | | | | 1---  PUSHBUTTON2_EN |  | | | | | | 1---  EMPTY
	| | | | | | | 0-  PUSHBUTTON1_EN |  | | | | | | | 0-  EMPTY
	| | | | | | | |                  |  | | | | | | | |
	O O O O O O O O                  |  O O O O O O O O   (I/O direction)
	X X X 0 0 0 0 0                  |  0 X X X X X X X   (value being set)
*/
		.i2c_bus = 0,
		.i2c_addr = 0x22,
		.dir0 = 0x0, /* all output */
		.dir1 = 0x0, /* all output */
		.value0 = 0x0,
		.value1 = 0x0,
		.is23018 = 0x0,
	},
};
