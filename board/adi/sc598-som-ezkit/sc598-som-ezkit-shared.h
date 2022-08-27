/*
 * U-boot - Functions which are shared between u-boot proper + u-boot SPL
 *
 * Copyright (c) 2022 Analog Devices Inc.
 *
 * Licensed under the GPL-2 or later.
 */

#ifndef SC598_SOM_EZKIT_SHARED
#define SC598_SOM_EZKIT_SHARED

int adi_initialize_soft_switches();
int adi_enable_ethernet_softconfig();
int adi_disable_ethernet_softconfig();

extern struct switch_config switch_config_array_current_state[NUM_SWITCH];

#endif
