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

ifdef CONFIG_SC59X
obj-y += dmc/dmcinit.o
obj-y += init.o
endif

extra-y := init-$(CONFIG_SYS_BOARD)
extra-y += init-$(CONFIG_SYS_BOARD).elf
extra-y += init.o
extra-y += init_start.o
extra-y += init_main.o
extra-y += dmc/dmcinit.o

ifdef CONFIG_SC57X
INIT_TEXT_BASE = 0x20000000
endif

ifdef CONFIG_SC58X
INIT_TEXT_BASE = 0x20080000
endif

ifdef CONFIG_SC59X
INIT_TEXT_BASE = 0x20080000
extra-y += init_return_stack.o
endif

quiet_cmd_link_init = LD      $@
      cmd_link_init = $(LD) -Ttext=$(INIT_TEXT_BASE) -nostdlib $(LDFLAGS) -o $@ $^

$(obj)/init-$(CONFIG_SYS_BOARD): $(obj)/init_start.o $(obj)/init.o $(obj)/dmc/dmcinit.o
	touch $(obj)/built-in.o
	$(call if_changed,link_init)

$(obj)/init-$(CONFIG_SYS_BOARD).elf: $(obj)/init_main.o $(obj)/init.o $(obj)/dmc/dmcinit.o
	$(call if_changed,link_init)
