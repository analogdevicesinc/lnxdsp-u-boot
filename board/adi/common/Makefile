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

obj-y += init/

ifndef CONFIG_SC59X_64
obj-y += arm32/soc.o
endif

ifdef CONFIG_SC59X_64
obj-y += arm64/soc.o
endif

ifdef CONFIG_SC59X
ifeq ($(CONFIG_SPL),n)
obj-y   := arm32/lowlevel_init.o
endif
endif

obj-$(CONFIG_SPL_BUILD) += spl.o
