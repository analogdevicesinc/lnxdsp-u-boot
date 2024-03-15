/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * (C) Copyright 2022 - Analog Devices, Inc.
 *
 * Written and/or maintained by Timesys Corporation
 *
 * Contact: Nathan Barrett-Morrison <nathan.morrison@timesys.com>
 * Contact: Greg Malysa <greg.malysa@timesys.com>
 *
 * U-boot - SPL management
 *
 */

#include <asm/arch-adi/sc5xx/spl.h>

// Table 43-14 in SC598 hardware reference manual
const struct adi_boot_args adi_rom_boot_args[] = {
	// JTAG/no boot
	[0] = {0, 0, 0},
	// SPI master, used for qspi as well
	[1] = {0x60040000, 0x00040000, 0x20620247},
	// SPI slave
	[2] = {0, 0, 0x00000212},
	// UART slave
	[3] = {0, 0, 0x00000013},
	// Linkport slave
	[4] = {0, 0, 0x00000014},
	// OSPI master
	[5] = {0x60040000, 0, 0x00000008},
	// eMMC
	[6] = {0x201, 0, 0x86009},
	// reserved, also no boot
	[7] = {0, 0, 0}
};

unsigned long spl_mmc_get_uboot_raw_sector(struct mmc *mmc,
					   unsigned long raw_sect)
{
	unsigned int mmc_sector_offs = 0;

#if CONFIG_IS_ENABLED(OF_CONTROL) && !CONFIG_IS_ENABLED(OF_PLATDATA)
    mmc_sector_offs = fdtdec_get_config_int(gd->fdt_blob,
					    "u-boot,spl-mmc-sector-offset",
					 mmc_sector_offs);
#endif

	return mmc_sector_offs;
}

struct ADI_ROM_BOOT_CONFIG {
	void *src;
	void *dest;
	int32_t byte_count;
	int32_t flags;
	uint32_t block_count;
	uint32_t block_current;
	void *next_dxe;
	uint32_t byte_address;
	uint32_t *control_register;
	int32_t control_value;
	uint32_t *peripheral_base;
	uint32_t *aux_control_register;
	uint32_t *aux_peripheral_base;
	uint32_t *sec_control_register;
	void *dma_base_register;
	int32_t load_type;
	struct {
		uint32_t operation;
		uint32_t id;
		void *src;
		void *destination;
		uint32_t byte_count;
		int32_t done_detect;
		uint32_t crc_ctl;
		uint32_t fill_value;
		uint32_t crc_poly;
		uint32_t crc_compare;
	} mdma_cfg;
	uint16_t data_width;
	uint16_t src_modify_mult;
	uint16_t dst_modify_mult;
	uint16_t usr_short;
	int32_t user_long;
	int32_t reserved0;
	void *mode_data;
	int32_t boot_command;
	void *boot_header;
	void *temp_buffer;
	void *reserved1;
	int32_t temp_byte_count;
	void *temp_src;
	int32_t page_byte_count;
	struct {
		struct {
			uint8_t *buffer;
			uint32_t size;
			uint32_t page_size;
		} buffer[2];
		uint32_t state;
		void *src;
		void *dma;
	} boot_buffers;
	struct {
		void *init;
		void *config;
		void *load;
		void *cleanup;
		void *reserved0;
		int32_t reserved1;
	} registry;
	void (*error)(struct ADI_ROM_BOOT_CONFIG *);
	// @todo there are more fields but this should be enough for what we need to see for now
};

static void adi_rom_boot_error_handler(struct ADI_ROM_BOOT_CONFIG *config) {
	printf("bootrom failed, rcu msg = 0x%x\n", readl(pRCU_MSG));
	while (1)
		;
}

int32_t adi_rom_boot_hook(struct ADI_ROM_BOOT_CONFIG *config, int32_t cause) {
	config->error = &adi_rom_boot_error_handler;
	return 0;
}

#ifdef CONFIG_ADI_FALCON
void spl_board_prepare_ethernet(void)
{
	// select RGMII, little endian for both ports
	writel((readl(REG_PADS0_PCFG0) | 0xc), REG_PADS0_PCFG0);
	writel(readl(REG_PADS0_PCFG0) & ~(1 << 19), REG_PADS0_PCFG0);
	writel(readl(REG_PADS0_PCFG0) & ~(1 << 20), REG_PADS0_PCFG0);
}

void spl_board_prepare_for_linux(void)
{
	spl_board_prepare_ethernet();
	adi_disable_ospi(1);
}

#if defined(CONFIG_SPL_LOAD_FIT)
int board_fit_config_name_match(const char *name)
{
	return 0;
}
#endif

#endif
