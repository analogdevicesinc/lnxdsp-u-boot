/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Macros for populating pinmux properties on the pincontroller
 *
 * (C) Copyright 2024 - Analog Devices, Inc.
 */

#ifndef DT_BINDINGS_PINCTRL_ADI_ADSP_H
#define DT_BINDINGS_PINCTRL_ADI_ADSP_H

#define ADI_ADSP_PIN(port, pin) (16 * (port - 'A') + pin)

#define ADI_ADSP_PINFUNC_GPIO     0
#define ADI_ADSP_PINFUNC_ALT0     1
#define ADI_ADSP_PINFUNC_ALT1     2
#define ADI_ADSP_PINFUNC_ALT2     3
#define ADI_ADSP_PINFUNC_ALT3     4

#endif
