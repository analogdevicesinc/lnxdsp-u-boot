// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * (C) Copyright 2022 - Analog Devices, Inc.
 *
 * Written and/or maintained by Timesys Corporation
 *
 * Contact: Nathan Barrett-Morrison <nathan.morrison@timesys.com>
 * Contact: Greg Malysa <greg.malysa@timesys.com>
 *
 * Based on Rockchip's sdhci.c file
 */

#include <common.h>
#include <dm.h>
#include <dt-structs.h>
#include <linux/err.h>
#include <linux/libfdt.h>
#include <malloc.h>
#include <mapmem.h>
#include <sdhci.h>
#include <clk.h>

/* 400KHz is max freq for card ID etc. Use that as min */
#define EMMC_MIN_FREQ	400000

struct adi_sdhc_plat {
#if CONFIG_IS_ENABLED(OF_PLATDATA)
	struct dtd_adi_sdhci dtplat;
#endif
	struct mmc_config cfg;
	struct mmc mmc;
};

struct adi_sdhc {
	struct sdhci_host host;
	void *base;
};

static int designware_sdhci_probe(struct udevice *dev)
{
	struct mmc_uclass_priv *upriv = dev_get_uclass_priv(dev);
	struct adi_sdhc_plat *plat = dev_get_platdata(dev);
	struct adi_sdhc *prv = dev_get_priv(dev);
	struct sdhci_host *host = &prv->host;
	int max_frequency, ret;
	struct clk clk;

#if CONFIG_IS_ENABLED(OF_PLATDATA)
	struct dtd_adi_sdhci *dtplat = &plat->dtplat;

	host->name = dev->name;
	host->ioaddr = map_sysmem(dtplat->reg[0], dtplat->reg[1]);
	max_frequency = dtplat->max_frequency;
	ret = clk_get_by_driver_info(dev, dtplat->clocks, &clk);
#else
	max_frequency = dev_read_u32_default(dev, "max-frequency", 0);
	ret = clk_get_by_index(dev, 0, &clk);
#endif

	host->quirks = 0 /*SDHCI_QUIRK_BROKEN_HISPD_MODE*/;
	host->max_clk = max_frequency;
	/*
	 * The sdhci-driver only supports 4bit and 8bit, as sdhci_setup_cfg
	 * doesn't allow us to clear MMC_MODE_4BIT.  Consequently, we don't
	 * check for other bus-width values.
	 */
	if (host->bus_width == 8)
		host->host_caps |= MMC_MODE_8BIT;

	host->mmc = &plat->mmc;
	host->mmc->priv = &prv->host;
	host->mmc->dev = dev;
	upriv->mmc = host->mmc;

	ret = sdhci_setup_cfg(&plat->cfg, host, 0, EMMC_MIN_FREQ);
	if (ret)
		return ret;

	return sdhci_probe(dev);
}

static int designware_sdhci_ofdata_to_platdata(struct udevice *dev)
{
#if !CONFIG_IS_ENABLED(OF_PLATDATA)
	struct sdhci_host *host = dev_get_priv(dev);

	host->name = dev->name;
	host->ioaddr = dev_read_addr_ptr(dev);
	host->bus_width = dev_read_u32_default(dev, "bus-width", 4);
#endif

	return 0;
}

static int adi_sdhci_bind(struct udevice *dev)
{
	struct adi_sdhc_plat *plat = dev_get_platdata(dev);

	return sdhci_bind(dev, &plat->mmc, &plat->cfg);
}

static const struct udevice_id designware_sdhci_ids[] = {
	{ .compatible = "designware,sdhci-5.1" },
	{ }
};

U_BOOT_DRIVER(designware_sdhci_drv) = {
	.name		= "adi_sdhci",
	.id		= UCLASS_MMC,
	.of_match	= designware_sdhci_ids,
	.ofdata_to_platdata = designware_sdhci_ofdata_to_platdata,
	.ops		= &sdhci_ops,
	.bind		= adi_sdhci_bind,
	.probe		= designware_sdhci_probe,
	.priv_auto_alloc_size = sizeof(struct adi_sdhc),
	.platdata_auto_alloc_size = sizeof(struct adi_sdhc_plat),
};
