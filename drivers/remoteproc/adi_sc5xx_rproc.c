// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * (C) Copyright 2022 - Analog Devices, Inc.
 *
 * Written and/or maintained by Timesys Corporation
 *
 * Contact: Nathan Barrett-Morrison <nathan.morrison@timesys.com>
 * Contact: Greg Malysa <greg.malysa@timesys.com>
 *
 * Analog Devices SC5xx remoteproc driver for loading code onto SHARC cores
 */

#include <common.h>
#include <asm/io.h>
#include <dm.h>
#include <linux/delay.h>
#include <time.h>
#include <remoteproc.h>

/* Register offsets */
#define ADI_RCU_REG_CTL				0x00
#define ADI_RCU_REG_STAT			0x04
#define ADI_RCU_REG_CRCTL			0x08
#define ADI_RCU_REG_CRSTAT			0x0c
#define ADI_RCU_REG_SRRQSTAT		0x18
#define ADI_RCU_REG_SIDIS			0x1c
#define ADI_RCU_REG_SISTAT			0x20
#define ADI_RCU_REG_BCODE			0x28
#define ADI_RCU_REG_SVECT0			0x2c
#define ADI_RCU_REG_SVECT1			0x30
#define ADI_RCU_REG_SVECT2			0x34
#define ADI_RCU_REG_MSG				0x6c
#define ADI_RCU_REG_MSG_SET			0x70
#define ADI_RCU_REG_MSG_CLR			0x74

/* Register bit definitions */
#define ADI_RCU_CTL_SYSRST		BIT(0)

/* Bit values for the RCU0_MSG register */
#define RCU0_MSG_C0IDLE			0x00000100		/* Core 0 Idle */
#define RCU0_MSG_C1IDLE			0x00000200		/* Core 1 Idle */
#define RCU0_MSG_C2IDLE			0x00000400		/* Core 2 Idle */
#define RCU0_MSG_CRR0			0x00001000		/* Core 0 reset request */
#define RCU0_MSG_CRR1			0x00002000		/* Core 1 reset request */
#define RCU0_MSG_CRR2			0x00004000		/* Core 2 reset request */
#define RCU0_MSG_C1ACTIVATE		0x00080000		/* Core 1 Activated */
#define RCU0_MSG_C2ACTIVATE		0x00100000		/* Core 2 Activated */

struct sc5xx_rproc_data {
	/* Address to load to svect when rebooting core */
	u32 load_addr;

	/* RCU parameters */
	void *rcu_base;
	u32 svect_offset;
	u32 coreid;
};

typedef struct block_code_flag {
	uint32_t bCode:4,			/* 0-3 */
			 bFlag_save:1,		/* 4 */
			 bFlag_aux:1,		/* 5 */
			 bReserved:1,		/* 6 */
			 bFlag_forward:1,	/* 7 */
			 bFlag_fill:1,		/* 8 */
			 bFlag_quickboot:1, /* 9 */
			 bFlag_callback:1,	/* 10 */
			 bFlag_init:1,		/* 11 */
			 bFlag_ignore:1,	/* 12 */
			 bFlag_indirect:1,	/* 13 */
			 bFlag_first:1,		/* 14 */
			 bFlag_final:1,		/* 15 */
			 bHdrCHK:8,			/* 16-23 */
			 bHdrSIGN:8;		/* 0xAD, 0xAC or 0xAB */
} BCODE_FLAG_t;

typedef struct ldr_hdr {
	BCODE_FLAG_t bcode_flag;
	uint32_t target_addr;
	uint32_t byte_count;
	uint32_t argument;
} LDR_Ehdr_t;

static int is_final(LDR_Ehdr_t *hdr) {
	return hdr->bcode_flag.bFlag_final;
}

static int is_empty(LDR_Ehdr_t *hdr) {
	return hdr->bcode_flag.bFlag_ignore || (hdr->byte_count == 0);
}

static int adi_valid_firmware(LDR_Ehdr_t *adi_ldr_hdr)
{
	if (!adi_ldr_hdr->byte_count
		&& (adi_ldr_hdr->bcode_flag.bHdrSIGN == 0xAD
			|| adi_ldr_hdr->bcode_flag.bHdrSIGN == 0xAC
			|| adi_ldr_hdr->bcode_flag.bHdrSIGN == 0xAB))
		return 1;

	return 0;
}

static int sharc_load(struct udevice *dev, ulong addr, ulong size) {
	struct sc5xx_rproc_data *priv = dev_get_priv(dev);
	size_t offset;
	u8 *buf = (u8 *) addr;
	LDR_Ehdr_t *ldr = (LDR_Ehdr_t *) addr;
	LDR_Ehdr_t *block_hdr;
	LDR_Ehdr_t *next_hdr;

	if (!adi_valid_firmware(ldr)) {
		printk("%s: Firmware at 0x%lx does not appear to be an LDR image\n", dev->name,
			addr);
		printk("%s: Note: Signed firmware is not currently supported\n", dev->name);
		return -EINVAL;
	}

	do {
		block_hdr = (LDR_Ehdr_t*) buf;
		offset = sizeof(LDR_Ehdr_t) + (block_hdr->bcode_flag.bFlag_fill ?
							0 : block_hdr->byte_count);
		next_hdr = (LDR_Ehdr_t *) (buf + offset);

		if (block_hdr->bcode_flag.bFlag_first)
			priv->load_addr = (unsigned long)block_hdr->target_addr;

		if (!is_empty(block_hdr)) {
			if (block_hdr->bcode_flag.bFlag_fill) {
				memset((void *)block_hdr->target_addr, block_hdr->argument,
					block_hdr->byte_count);
			}
			else {
				memcpy((void *)block_hdr->target_addr, buf+sizeof(LDR_Ehdr_t),
					block_hdr->byte_count);
			}
		}

		if (is_final(block_hdr))
			break;

		buf += offset;
	} while (1);

	return 0;
}

static int sharc_reset(struct sc5xx_rproc_data *priv) {
	u32 coreid = priv->coreid;
	u32 val;

	/* First put core in reset.
     * Clear CRSTAT bit for given coreid. */
	writel(1 << coreid, priv->rcu_base + ADI_RCU_REG_CRSTAT);

	/* Set SIDIS to disable the system interface */
	val = readl(priv->rcu_base + ADI_RCU_REG_SIDIS);
	writel(val | (1 << (coreid-1)), priv->rcu_base + ADI_RCU_REG_SIDIS);

	/*
	 * Wait for access to coreX have been disabled and all the pending
	 * transactions have completed
	 */
	udelay(50);

	/* Set CRCTL bit to put core in reset */
	val = readl(priv->rcu_base + ADI_RCU_REG_CRCTL);
	writel(val | (1 << coreid), priv->rcu_base + ADI_RCU_REG_CRCTL);

	/* Poll until Core is in reset */
	while (!(readl(priv->rcu_base + ADI_RCU_REG_CRSTAT) & (1 << coreid)))
		;

	/* Clear SIDIS to reenable the system interface */
	val = readl(priv->rcu_base + ADI_RCU_REG_SIDIS);
	writel(val & ~(1 << (coreid-1)), priv->rcu_base + ADI_RCU_REG_SIDIS);

	udelay(50);

	/* Take Core out of reset */
	val = readl(priv->rcu_base + ADI_RCU_REG_CRCTL);
	writel(val & ~(1 << coreid), priv->rcu_base + ADI_RCU_REG_CRCTL);

	/* Wait for done */
	udelay(50);
}

static int sharc_start(struct udevice *dev) {
	struct sc5xx_rproc_data *priv = dev_get_priv(dev);

	/* Write load address to appropriate SVECT for core */
	writel(priv->load_addr, priv->rcu_base + priv->svect_offset);

	sharc_reset(priv);

	/* Clear the IDLE bit when start the SHARC core */
	writel(RCU0_MSG_C0IDLE << priv->coreid, priv->rcu_base + ADI_RCU_REG_MSG_CLR);

	/* Notify CCES */
	writel(RCU0_MSG_C1ACTIVATE << (priv->coreid-1),
		priv->rcu_base + ADI_RCU_REG_MSG_SET);
	return 0;
}

static const struct dm_rproc_ops sc5xx_ops = {
	.load = sharc_load,
	.start = sharc_start,
};

static int sc5xx_probe(struct udevice *dev) {
	struct sc5xx_rproc_data *priv = dev_get_priv(dev);
	u32 coreid;
	u32 rcu;

	if (dev_read_u32(dev, "adi,coreid", &coreid)) {
		printk("%s: Missing property adi,coreid\n", dev->name);
		return -ENOENT;
	}

	priv->coreid = coreid;
	switch (coreid) {
	case 1:
		priv->svect_offset = ADI_RCU_REG_SVECT1;
		break;
	case 2:
		priv->svect_offset = ADI_RCU_REG_SVECT2;
		break;
	default:
		printk("%s: Invalid value %d for adi,coreid, must be 1 or 2\n", dev->name,
			coreid);
		return -EINVAL;
	}

	if (dev_read_u32(dev, "adi,rcu-reg", &rcu)) {
		printk("%s: Missing property adi,rcu-reg\n", dev->name);
		return -ENOENT;
	}

	priv->rcu_base = (void *)rcu;

	printk("%s: sc5xx remoteproc core %d available\n", dev->name, priv->coreid);

	return 0;
}

static const struct udevice_id sc5xx_ids[] = {
	{ .compatible = "adi,sc5xx-rproc" },
	{ }
};

U_BOOT_DRIVER(adi_sc5xx_rproc) = {
	.name = "adi_sc5xx_rproc",
	.of_match = sc5xx_ids,
	.id = UCLASS_REMOTEPROC,
	.ops = &sc5xx_ops,
	.probe = sc5xx_probe,
	.priv_auto_alloc_size = sizeof(struct sc5xx_rproc_data),
	.flags = 0,
};
