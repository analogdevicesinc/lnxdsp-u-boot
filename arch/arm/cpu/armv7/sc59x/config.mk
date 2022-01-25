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
		ifeq ($(CONFIG_ENV_IS_EMBEDDED_IN_LDR),y)
			CREATE_LDR_ENV = tools/envcrc --binary > env-ldr.o
		else
			CREATE_LDR_ENV =
		endif

		INPUTS-y += u-boot.ldr u-boot-$(CONFIG_SYS_BOARD).ldr

		LDR_FLAGS-y :=

		LDR_FLAGS += --bcode=$(CONFIG_SC_BOOT_MODE)
		LDR_FLAGS += --use-vmas
		ifneq ($(CONFIG_SC59X_CHAIN_BOOT),y)
			LDR_FLAGS += --initcode spl/u-boot-spl
		endif

		ifneq ($(CONFIG_SC_BOOT_MODE),SC_BOOT_UART)
			ifneq ($(CONFIG_SC59X_CHAIN_BOOT),y)
				LDR_FLAGS-$(CONFIG_ENV_IS_EMBEDDED_IN_LDR) += --punchit $$(($(CONFIG_ENV_OFFSET))):$$(($(CONFIG_ENV_SIZE))):env-ldr.o
			endif
		endif

		LDR_FLAGS += $(LDR_FLAGS-y)

		# Set some default LDR flags based on boot mode.
		LDR_FLAGS += $(LDR_FLAGS-$(CONFIG_SC_BOOT_MODE))
	else
		ifneq ($(CONFIG_SPL),y)
			ifeq ($(CONFIG_ENV_IS_EMBEDDED_IN_LDR),y)
				CREATE_LDR_ENV = tools/envcrc --binary > env-ldr.o
			else
				CREATE_LDR_ENV =
			endif

			INPUTS-y += u-boot.ldr u-boot-$(CONFIG_SYS_BOARD).ldr

			LDR_FLAGS-y :=

			LDR_FLAGS += --bcode=$(CONFIG_SC_BOOT_MODE)
			LDR_FLAGS += --use-vmas
			ifneq ($(CONFIG_SC59X_CHAIN_BOOT),y)
				LDR_FLAGS += --initcode $(CPUDIR)/$(SOC)/init-$(CONFIG_SYS_BOARD)
			endif

			ifneq ($(CONFIG_SC_BOOT_MODE),SC_BOOT_UART)
				ifneq ($(CONFIG_SC59X_CHAIN_BOOT),y)
					LDR_FLAGS-$(CONFIG_ENV_IS_EMBEDDED_IN_LDR) += --punchit $$(($(CONFIG_ENV_OFFSET))):$$(($(CONFIG_ENV_SIZE))):env-ldr.o
				endif
			endif

			LDR_FLAGS += $(LDR_FLAGS-y)

			# Set some default LDR flags based on boot mode.
			LDR_FLAGS += $(LDR_FLAGS-$(CONFIG_SC_BOOT_MODE))
		endif
	endif
endif

# Select the Analog Devices processor.
PLATFORM_RELFLAGS += -fno-stack-protector -std=gnu89 -mfpu=neon-vfpv4 -march=armv7-a
