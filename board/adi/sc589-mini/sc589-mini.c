/*
 * U-boot - main board file
 *
 * Copyright (c) 2013-2014 Analog Devices Inc.
 *
 * Licensed under the GPL-2 or later.
 */

#include <common.h>
#include <netdev.h>
#include <phy.h>
#include <asm/io.h>
#include <asm/gpio.h>
#include <asm/mach-types.h>
#include <asm/arch/portmux.h>
#include <asm/mach-adi/common/sc5xx.h>
#include <asm/mach-adi/common/dwmmc.h>
#include <linux/delay.h>
#include <watchdog.h>

extern char __rel_dyn_start, __rel_dyn_end;
extern char __bss_start, __bss_end;

static void bss_clear(void)
{
	u32 bss_start = (u32)&__bss_start;
	u32 bss_end = (u32)&__bss_end;
	u32 rel_dyn_end = (u32)&__rel_dyn_end;
	u32 *to;

	if (rel_dyn_end >= bss_start && rel_dyn_end <= bss_end)
		to = (u32 *)rel_dyn_end;
	else
		to = (u32 *)bss_start;

	int i, sz;

	sz = bss_end - (u32)to;
	for (i = 0; i < sz; i += 4)
		*to++ = 0;
}

DECLARE_GLOBAL_DATA_PTR;
int board_early_init_f(void)
{
	bss_clear();

#ifdef CONFIG_HW_WATCHDOG
	hw_watchdog_init();
#endif

	return 0;
}

void set_spu_securep_msec(int n, bool msec)
{
	void __iomem *p = (void __iomem *)(REG_SPU0_CTL + 0xA00 + 4 * n);
	u32 securep = readl(p);

	if (msec)
		writel(securep | 0x2, p);
	else
		writel(securep & ~0x2, p);
}

/* miscellaneous platform dependent initialisations */
int misc_init_r(void)
{
	printf("other init\n");
	set_spu_securep_msec(55, 1);
	set_spu_securep_msec(56, 1);
	set_spu_securep_msec(58, 1);
	set_spu_securep_msec(153, 1);
	return 0;
}

unsigned long flash_init(void)
{

#if 0
	/*  Enable bank 1 of smc because AMS1 is connected to enable of SRAM */
	*pREG_SMC0_GCTL = 0x10;
	*pREG_SMC0_B1CTL = 0x00000001;
	*pREG_SMC0_B1TIM = 0x05220522;
#endif
	return 0;
}

int dram_init(void)
{
	gd->ram_size = CONFIG_SYS_SDRAM_SIZE;
	return 0;
}

void s_init(void)
{
}

#ifdef CONFIG_DESIGNWARE_ETH
int board_eth_init(struct bd_info *bis)
{
	int ret = 0;
	static const unsigned short pins[] = P_RGMII0;

	gpio_request(GPIO_PB7, "ethphy_reset");
	gpio_request(GPIO_PF6, "ethphy_pwdn");
	gpio_direction_output(GPIO_PF6, 1);
	gpio_direction_output(GPIO_PB7, 1);
	mdelay(20);
	gpio_direction_output(GPIO_PB7, 0);
	mdelay(90);
	gpio_direction_output(GPIO_PB7, 1);
	mdelay(20);

	writel((readl(REG_PADS0_PCFG0) | 0xc), REG_PADS0_PCFG0);
	if (!peripheral_request_list(pins, "emac0"))
		ret += designware_initialize(REG_EMAC0_MACCFG,
				PHY_INTERFACE_MODE_RGMII);
	return ret;
}

int board_phy_config(struct phy_device *phydev)
{
	int  phy_data = 0;

	/* enable RGMII mode */
	phy_data = phy_read(phydev, MDIO_DEVAD_NONE, 0x32);
	phy_write(phydev, MDIO_DEVAD_NONE, 0x32, (1 << 7) | phy_data);
#ifdef CONFIG_PHY_TI
	int cfg3 = 0;
	#define MII_DP83867_CFG3    (0x1e)
	/*
	 * Pin INT/PWDN on DP83867 should be configured as an Interrupt Output
	 * instead of a Power-Down Input on ADI SC5XX boards in order to
	 * prevent the signal interference from other peripherals during they
	 * are running at the same time.
	 */
	 cfg3 = phy_read(phydev, MDIO_DEVAD_NONE, MII_DP83867_CFG3);
	 cfg3 |= (1 << 7);
	 phy_write(phydev, MDIO_DEVAD_NONE, MII_DP83867_CFG3, cfg3);
#endif

	if (phydev->drv->config)
		phydev->drv->config(phydev);

	return 0;
}
#endif

#ifdef CONFIG_GENERIC_MMC
int board_mmc_init(struct bd_info *bis)
{
	int ret;
#ifdef CONFIG_MMC_DW
	ret = sc5xx_dwmmc_init(bis);
	if (ret)
		printf("dwmmc init failed\n");
#endif
	return ret;
}
#endif

int board_init(void)
{
	gd->bd->bi_arch_number = MACH_TYPE_SC589_MINI;
	/* boot param addr */
	gd->bd->bi_boot_params = CONFIG_SYS_SDRAM_BASE + (0x100);

	return 0;
}

