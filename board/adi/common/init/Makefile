# SPDX-License-Identifier: GPL-2.0-or-later
#
# (C) Copyright 2022 - Analog Devices, Inc.
#
# Written and/or maintained by Timesys Corporation
#
# Contact: Nathan Barrett-Morrison <nathan.morrison@timesys.com>
# Contact: Greg Malysa <greg.malysa@timesys.com>
#

ifdef CONFIG_SC59X_64
ADI_ARCH=arm64
else
ADI_ARCH=arm32
endif

ifdef CONFIG_SC59X
obj-y += dmc/dmcinit.o
endif

ifdef CONFIG_SC59X_64
obj-y += dmc/dmcinit.o
endif

obj-y += init.o

extra-y := init-$(CONFIG_SYS_BOARD)
extra-y += init-$(CONFIG_SYS_BOARD).elf
extra-y += $(ADI_ARCH)/init_start.o
extra-y += $(ADI_ARCH)/init_main.o
obj-y += dmc/dmcinit.o
extra-y += init_return_stack.o

ifdef CONFIG_SC57X
INIT_TEXT_BASE = 0x20000000
endif

ifdef CONFIG_SC58X
INIT_TEXT_BASE = 0x20080000
endif

ifdef CONFIG_SC59X
INIT_TEXT_BASE = 0x20080000
endif

ifdef CONFIG_SC59X_64
INIT_TEXT_BASE = 0x20080000
extra-y += $(ADI_ARCH)/init_emmc_boot.o
extra-y += init-$(CONFIG_SYS_BOARD).emmc_boot
endif

quiet_cmd_link_init = LD      $@
      cmd_link_init = $(LD) -Ttext=$(INIT_TEXT_BASE) -nostdlib $(LDFLAGS) -o $@ $^

$(obj)/init-$(CONFIG_SYS_BOARD): $(obj)/$(ADI_ARCH)/init_start.o $(obj)/init.o $(obj)/init_return_stack.o $(obj)/dmc/dmcinit.o
	$(call if_changed,link_init)

$(obj)/init-$(CONFIG_SYS_BOARD).elf: $(obj)/$(ADI_ARCH)/init_main.o $(obj)/init.o $(obj)/init_return_stack.o $(obj)/dmc/dmcinit.o
	$(call if_changed,link_init)

ifdef CONFIG_SC59X_64
#For booting via eMMC with the eMMC boot trampoline code
$(obj)/init-$(CONFIG_SYS_BOARD).emmc_boot: $(obj)/$(ADI_ARCH)/init_start.o $(obj)/init.o $(obj)/$(ADI_ARCH)/init_emmc_boot.o $(obj)/dmc/dmcinit.o
	$(call if_changed,link_init)
endif
