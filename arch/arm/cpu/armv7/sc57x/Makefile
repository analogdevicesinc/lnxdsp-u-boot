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

obj-y	:= lowlevel_init.o
obj-y	+= soc.o timer.o gpio.o
obj-y	+= clocks.o
extra-y := init-$(CONFIG_SYS_BOARD)
extra-y += init-$(CONFIG_SYS_BOARD).elf
ifeq ($(CONFIG_SC57X_CHAIN_BOOT),y)
extra-y += init-$(CONFIG_SYS_BOARD).ldr
endif
extra-y += init.o
extra-y += init_start.o
extra-y += init_main.o

quiet_cmd_link_init = LD      $@
      cmd_link_init = $(LD) -Ttext=0x20000000 -nostdlib $(LDFLAGS) -o $@ $^

$(obj)/init-$(CONFIG_SYS_BOARD): $(obj)/init_start.o $(obj)/init.o
	$(call if_changed,link_init)

$(obj)/init-$(CONFIG_SYS_BOARD).elf: $(obj)/init_main.o $(obj)/init.o
	$(call if_changed,link_init)

ifeq ($(CONFIG_SC57X_CHAIN_BOOT),y)
quiet_cmd_ldr_init =
      cmd_ldr_init = $(LDR) -T $(CONFIG_CPU) -c $@ $^  --bcode=1 --use-vmas

$(obj)/init-$(CONFIG_SYS_BOARD).ldr: $(obj)/init-$(CONFIG_SYS_BOARD)
	$(call if_changed,ldr_init)
endif
