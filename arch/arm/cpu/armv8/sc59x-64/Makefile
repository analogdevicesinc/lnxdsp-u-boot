#
# U-boot - Makefile
#
# Copyright (c) 2013 Analog Device Inc.
#
# (C) Copyright 2000-2012
# Wolfgang Denk, DENX Software Engineering, wd@denx.de.
#
# SPDX-License-Identifier:      GPL-2.0+
#

obj-y	+= soc.o timer.o gpio.o
obj-y	+= clocks.o
obj-y	+= init.o
extra-y := init-$(CONFIG_SYS_BOARD)
extra-y += init-$(CONFIG_SYS_BOARD).elf
ifeq ($(CONFIG_SC59X_CHAIN_BOOT),y)
extra-y += init-$(CONFIG_SYS_BOARD).ldr
endif
extra-y += init_start.o
extra-y += init_main.o

quiet_cmd_link_init = LD      $@
      cmd_link_init = $(LD) -Ttext=0x20080000 -nostdlib $(LDFLAGS) -o $@ $^

$(obj)/init-$(CONFIG_SYS_BOARD): $(obj)/init_start.o $(obj)/init.o
	$(call if_changed,link_init)

$(obj)/init-$(CONFIG_SYS_BOARD).elf: $(obj)/init_main.o $(obj)/init.o
	$(call if_changed,link_init)

