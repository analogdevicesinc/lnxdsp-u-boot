/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * (C) Copyright 2022 - Analog Devices, Inc.
 *
 * Written and/or maintained by Timesys Corporation
 *
 * Contact: Nathan Barrett-Morrison <nathan.morrison@timesys.com>
 * Contact: Greg Malysa <greg.malysa@timesys.com>
 */

#include <common.h>
#include <asm/arch-adi/sc5xx/sc5xx.h>
#include <asm/io.h>
#include <asm/system.h>
#include <cpu_func.h>

#include <asm/arch-adi/sc5xx/soc.h>
#include <asm/arch-adi/sc5xx/idcode.h>

void reset_cpu(void)
{
	writel(1, RCU0_CTL);
}

#ifndef CONFIG_ARM64
#ifndef CONFIG_SYS_DCACHE_OFF
void enable_caches(void)
{
	/* Enable D-cache. I-cache is already enabled in start.S */
	dcache_enable();
}
#endif

#ifndef CONFIG_SYS_L2CACHE_OFF
/*
 * Sets L2 cache related parameters before enabling data cache
 */
void v7_outer_cache_enable(void)
{
}
#endif
#endif

int arch_cpu_init(void)
{
	return 0;
}




enum adsp_family get_device_type(u32 device_type)
{
	if(GRFN_SC58X_JTAGID == device_type)
		return sc58x;
	else if(GRFN_SC59X_JTAGID == device_type)
		return sc59x;
	else
		log_debug("unknown [%04x]\n",device_type);
	
	return unknown;
}

/* 
* Revision ID is similarly coded for different boards
* and as a result, is currently set to be shared by different 
* devices
*
* Since the actual silicon version is a floating point, the 
* information is stored as an 8 bit number in decimal. The parsed 
* revision ID can be obtained by dividing the returned value by 100.
*
* Eg: The revision 1.2 is returned as 120
* */
u8 get_si_version(u32 si_version)
{
	if (GRFN_SC58X_SI_REVID_0_0 == si_version)
		return 0;
	else if (GRFN_SC58X_SI_REVID_0_1 == si_version)
		return 10;
	else if (GRFN_SC58X_SI_REVID_1_0 == si_version)
		return 100;
	else if (GRFN_SC58X_SI_REVID_1_2 == si_version)
		return 120;
	else
		log_debug("x.x [%01x]\n",si_version);

	return 0xFF;
}

int auth_reading(u32 mfid,u32 lsb)
{
	if ((mfid != ADI_MNFID) || (lsb != ADI_LSB)) {
		log_debug("WARNING: UNEXPECTED VALUES IN ID CODE\n");
		log_debug("[MNFID]EXPECTED: %02x, GOT: %02x\n", ADI_MNFID, mfid);
		log_debug("[LSB]EXPECTED: %02x, GOT: %02x\n", ADI_LSB, lsb);
		log_debug("ABORTING AUTO DETECTION\n");
		return -1;
	} else {
		printf("[Detected] ADI Device\n");
	}
	return 0;
}

void read_tapc_idcode(void)
{
	u32 id = *(TAPC_IDCODE);
	
	u32 si_version = ((id & BITM_SI_REVID) & BITM_SI_REVID) >> 28;
	u32 device_type = ((id & BITM_JTAGID) & BITM_JTAGID) >> 12;
	u32 mfid = ((id & BITM_MNFID) & BITM_MNFID) >> 1;
	u32 lsb = (id & BITM_LSB) & BITM_LSB;

	enum adsp_family family_type;
	u8 parsed_si_version;

	if(!auth_reading(mfid,lsb)) {
		parsed_si_version = get_si_version(si_version);
		family_type = get_device_type(device_type);
	}else{
		log_debug("SKIPPING IDCODE REGISTER PARSING, DEVICE NOT RECOGNISED\n");
	}


	/* Only append details which have been detected correctly */
	printf("[Detected] ");
	if(family_type != unknown) {
		if(family_type == sc58x)
			printf("family: adsp-sc58x ");
		if(family_type == sc59x)
			printf("family: adsp-sc59x ");
	} 

	if(parsed_si_version != 0xFF) {
		printf("silicon version: %d.%d\n",parsed_si_version/100,parsed_si_version%100);
	}

}


void print_cpu_id(void)
{
#ifndef CONFIG_ARM64
	u32 cpuid = 0;

	__asm__ __volatile__("mrc p15, 0, %0, c0, c0, 0" : "=r"(cpuid));

	printf("[Detected] Revision: %d.%d\n", cpuid & 0xf00000 >> 20, cpuid & 0xf);
#endif
}

int print_cpuinfo(void)
{

	printf("CPU:   ADSP %s (%s boot)\n",
	       CONFIG_LDR_CPU,  get_sc_boot_mode(CONFIG_SC_BOOT_MODE));

	print_cpu_id();
	log_debug("Reading on-board IDCODE register\n");
	read_tapc_idcode();

	return 0;
}

void fixup_dp83867_phy(struct phy_device *phydev)
{
	int phy_data = 0;

	phy_data = phy_read(phydev, MDIO_DEVAD_NONE, 0x32);
	phy_write(phydev, MDIO_DEVAD_NONE, 0x32, (1 << 7) | phy_data);
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

	// Mystery second port fixup on ezkits with two PHYs
	if (CONFIG_DW_PORTS & 2)
		phy_write(phydev, MDIO_DEVAD_NONE, 0x11, 3);

#ifdef CONFIG_ADI_BUG_EZKHW21
	phydev->advertising &= PHY_BASIC_FEATURES;
	phydev->speed = SPEED_100;
#endif

	if (phydev->drv->config)
		phydev->drv->config(phydev);

#ifdef CONFIG_ADI_BUG_EZKHW21
	phy_write(phydev, MDIO_DEVAD_NONE, 0, 0x3100);
#endif
}
