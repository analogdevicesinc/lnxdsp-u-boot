// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * (C) Copyright 2022 - Analog Devices, Inc.
 *
 * Written and/or maintained by Timesys Corporation
 *
 * Contact: Nathan Barrett-Morrison <nathan.morrison@timesys.com>
 * Contact: Greg Malysa <greg.malysa@timesys.com>
 *
 * U-boot - The SC598 EZKITs have pinmux conflicts between OSPI/QSPI/UART0
 *
 * UART0 uses PA6+7
 * OSPI0 uses PA0+1+2+3+4+5+6+7+8+9,PD4
 * SPI2 uses  PA0+1+2+3+4+5
 *
 * This file contains a set of functions which can be used to dynamically toggle between OSPI on/off
 *
 * When OSPI is turned on, the SPI2 and UART0 pinmuxes are cleared
 * When OSPI is turned off, the SPI2 and UART0 pinmuxes are set
 *
 * Toggling of the related GPIO expander pins (soft switches) is also performed
 */

#ifndef CONFIG_SC598_SOM_EZKIT_DYNAMIC_QSPI_OSPI_UART_MUX
#define CONFIG_SC598_SOM_EZKIT_DYNAMIC_QSPI_OSPI_UART_MUX

#define QSPI_PINS_SWITCH(n) \
	{ P_SPI##n##_SCK, P_SPI##n##_MISO, P_SPI##n##_MOSI, \
		P_SPI##n##_D2, P_SPI##n##_D3, P_SPI##n##_SSEL1, 0}

int adi_enable_ospi();
int adi_disable_ospi(bool changeMuxImmediately);

#endif
