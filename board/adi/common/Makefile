#
# U-boot - Makefile
#
# Copyright (c) 2022 Analog Device Inc.
#
# (C) Copyright 2000-2012
# Wolfgang Denk, DENX Software Engineering, wd@denx.de.
#
# SPDX-License-Identifier:      GPL-2.0+
#

obj-y += soc.o init/

ifdef CONFIG_SC59X
ifeq ($(CONFIG_SPL),n)
obj-y += arm32/lowlevel_init.o
endif
endif

ifneq ($(filter y,$(CONFIG_SC59X_64) $(CONFIG_SC59X)),)
obj-$(CONFIG_SPL_BUILD) += sc59x/sc59x-shared-spl.o
obj-y += sc59x/sc59x-shared.o
endif

obj-$(CONFIG_SPL_BUILD) += spl.o
