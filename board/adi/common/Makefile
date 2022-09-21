# SPDX-License-Identifier: GPL-2.0-or-later
#
# (C) Copyright 2022 - Analog Devices, Inc.
#
# Written and/or maintained by Timesys Corporation
#
# Contact: Nathan Barrett-Morrison <nathan.morrison@timesys.com>
# Contact: Greg Malysa <greg.malysa@timesys.com>
#

obj-y += soc.o init/

ifeq ($(CONFIG_SPL),n)
obj-y += arm32/lowlevel_init.o
endif

ifneq ($(filter y,$(CONFIG_SC59X_64) $(CONFIG_SC59X)),)
obj-$(CONFIG_SPL_BUILD) += sc59x/sc59x-shared-spl.o
obj-y += sc59x/sc59x-shared.o
endif

obj-$(CONFIG_SPL_BUILD) += spl.o
