#
# U-boot - config.mk
#
# Copyright (c) 2014 Analog Device Inc.
#
# (C) Copyright 2000 - 2012
# Wolfgang Denk, DENX Software Engineering, wd@denx.de.
#
# SPDX-License-Identifier:	GPL-2.0+
#
ifeq ($(CONFIG_BUILD_LDR),y)
	ifeq ($(CONFIG_SPL_BUILD),y)
		INPUTS-y += u-boot.ldr u-boot-$(CONFIG_SYS_BOARD).ldr

		LDR_FLAGS-y :=

		LDR_FLAGS += --bcode=$(CONFIG_SC_BOOT_MODE)
		LDR_FLAGS += --use-vmas

		LDR_FLAGS += $(LDR_FLAGS-y)

		# Set some default LDR flags based on boot mode.
		LDR_FLAGS += $(LDR_FLAGS-$(CONFIG_SC_BOOT_MODE))
	else
		ifneq ($(CONFIG_SPL),y)
			INPUTS-y += u-boot.ldr u-boot-$(CONFIG_SYS_BOARD).ldr
			INPUTS-y += u-boot-$(CONFIG_SYS_BOARD).ldr.emmc_boot_stage1
			INPUTS-y += u-boot-$(CONFIG_SYS_BOARD).ldr.emmc_boot_stage2

			LDR_FLAGS-y :=

			LDR_FLAGS += --bcode=$(CONFIG_SC_BOOT_MODE)
			LDR_FLAGS += --use-vmas
			ifneq ($(CONFIG_SC59X_CHAIN_BOOT),y)
				LDR_FLAGS += --initcode $(CPUDIR)/$(SOC)/init-$(CONFIG_SYS_BOARD)
			endif

			LDR_FLAGS += $(LDR_FLAGS-y)

			# Set some default LDR flags based on boot mode.
			LDR_FLAGS += $(LDR_FLAGS-$(CONFIG_SC_BOOT_MODE))

			EMMC_BOOT_FLAGS-y :=
			EMMC_BOOT_FLAGS += --bcode=$(CONFIG_SC_BOOT_MODE)
			EMMC_BOOT_FLAGS += --use-vmas
			EMMC_BOOT_FLAGS += $(EMMC_BOOT_FLAGS-y)
			EMMC_BOOT_FLAGS += $(EMMC_BOOT_FLAGS-$(CONFIG_SC_BOOT_MODE))
		endif
	endif
endif
