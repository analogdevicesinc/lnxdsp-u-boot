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

ifneq ($(filter y,$(CONFIG_SC59X_64) $(CONFIG_SC59X)),)
obj-y += sc59x/sc59x-shared.o
endif

obj-$(CONFIG_SPL_BUILD) += spl.o
obj-$(CONFIG_SPL_BUILD) += sc5xx_sharc_idle.o