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
	| | | 4--------- BCM89810_EN   |  | | | 4--------- SPI2FLASH_EN
	| | | | 3------- DP83865_EN    |  | | | | 3------- EMPTY
	| | | | | 2----- UART0_EN      |  | | | | | 2----- AUDIO_JACK_SEL
	| | | | | | 1--- UART0_FLOW_EN |  | | | | | | 1--- ADAU1979_EN
	| | | | | | | 0- EEPROM_EN     |  | | | | | | | 0- ADAU1962_EN
	| | | | | | | |                |  | | | | | | | |
	O O O O O O O O                |  O O O O O O O O   (I/O direction)
	0 0 0 1 0 0 0 0                |  0 0 0 0 X 1 0 0   (value being set)
*/
		.i2c_bus = 0,
		.i2c_addr = 0x21,
		.dir0 = 0x0, /* all output */
		.dir1 = 0x0, /* all output */
#ifdef CONFIG_PHY_BCM89810
		.value0 = 0x8,
#else
		.value0 = 0x10,
#endif
		.value1 = 0x4,
		.is23018 = 0x0,
	},
	{
/*
	U48 Port A                       U48 Port B

	7---------------  FLG3_LOOP_EN   |  7---------------  EMPTY
	| 6-------------  FLG2_LOOP_EN   |  | 6-------------  EMPTY
	| | 5-----------  FLG1_LOOP_EN   |  | | 5-----------  EMPTY
	| | | 4---------  FLG0_LOOP_EN   |  | | | 4---------  AD2410_MASTER
	| | | | 3-------  LEDS_EN        |  | | | | 3-------  ENGINE_RPM_OE
	| | | | | 2-----  PUSHBUTTON3_EN |  | | | | | 2-----  THUMBWHEEL_OE
	| | | | | | 1---  PUSHBUTTON2_EN |  | | | | | | 1---  ADAU1977_FAULT_EN
	| | | | | | | 0-  PUSHBUTTON1_EN |  | | | | | | | 0-  ADAU1977_EN
	| | | | | | | |                  |  | | | | | | | |
	O O O O O O O O                  |  O O O O O O O O   (I/O direction)
	0 0 0 0 0 0 0 0                  |  X X X 1 0 0 0 0   (value being set)
*/
		.i2c_bus = 0,
		.i2c_addr = 0x22,
		.dir0 = 0x0, /* all output */
		.dir1 = 0x0, /* all output */
		.value0 = 0x0,
		.value1 = 0x10,
		.is23018 = 0x0,
	},
};
