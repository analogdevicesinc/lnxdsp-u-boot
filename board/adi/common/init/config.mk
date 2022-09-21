# SPDX-License-Identifier: GPL-2.0-or-later
#
# (C) Copyright 2022 - Analog Devices, Inc.
#
# Written and/or maintained by Timesys Corporation
#
# Contact: Nathan Barrett-Morrison <nathan.morrison@timesys.com>
# Contact: Greg Malysa <greg.malysa@timesys.com>
#

INPUTS-y += u-boot.ldr u-boot-$(CONFIG_SYS_BOARD).ldr

LDR_FLAGS += --bcode=$(CONFIG_SC_BOOT_MODE)
LDR_FLAGS += --use-vmas

ifndef CONFIG_SC59X_64
	ifneq ($(CONFIG_SPL_BUILD),y)
		LDR_FLAGS += --initcode board/adi/common/init/init-$(CONFIG_SYS_BOARD)
	endif

	# Select the Analog Devices processor.
	PLATFORM_RELFLAGS += -fno-stack-protector -std=gnu89 -mfpu=neon-vfpv4 -march=armv7-a
endif

ifdef CONFIG_SC59X_64
ifneq ($(CONFIG_SPL_BUILD),y)
		INPUTS-y += u-boot-$(CONFIG_SYS_BOARD).ldr.emmc_boot_stage1
		INPUTS-y += u-boot-$(CONFIG_SYS_BOARD).ldr.emmc_boot_stage2

		EMMC_BOOT_FLAGS += --bcode=$(CONFIG_SC_BOOT_MODE)
		EMMC_BOOT_FLAGS += --use-vmas
endif
endif



