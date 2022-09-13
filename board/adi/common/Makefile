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

ifdef CONFIG_SC59X_64
obj-y += sc59x/sc59x-shared.o
obj-$(CONFIG_SPL_BUILD) += sc59x/sc59x-shared-spl.o
else
obj-y += arm32/timer.o
endif

ifdef CONFIG_SC59X
obj-y += sc59x/sc59x-shared.o
obj-$(CONFIG_SPL_BUILD) += sc59x/sc59x-shared-spl.o
ifeq ($(CONFIG_SPL),n)
obj-y += arm32/lowlevel_init.o
endif
endif

obj-$(CONFIG_SPL_BUILD) += spl.o
