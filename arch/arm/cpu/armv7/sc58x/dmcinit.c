#include <config.h>
#include <common.h>
#include <asm/arch/sc58x.h>
#include "dmcinit.h"

static struct dmc_param DMC_Param_List;
void DMC_PHY_Init(struct dmc_param DMC_Param_List)
{
	int i;
	uint32_t temp;

	if (DMC_Param_List.dmc_no == 0) {
		/* 1. Set DDR mode to DDR3/DDR2/LPDDR in DMCx_PHY_CTL4 register */
		if (DMC_Param_List.ddr_mode == DDR3_MODE)
			writel(ENUM_DMC_PHY_CTL4_DDR3, REG_DMC0_PHY_CTL4);
		else if (DMC_Param_List.ddr_mode == DDR2_MODE)
			writel(ENUM_DMC_PHY_CTL4_DDR2, REG_DMC0_PHY_CTL4);
		else if (DMC_Param_List.ddr_mode == LPDDR_MODE)
			writel(ENUM_DMC_PHY_CTL4_LPDDR, REG_DMC0_PHY_CTL4);

	   /* 2. Make sure that the bits 6, 7, 25, and 27 of the DMC_PHY_
		* CTL3 register are set */
		writel(0x0A0000C0, REG_DMC0_PHY_CTL3);

	   /* 3. For DDR2/DDR3 mode, make sure that the bits 0, 1, 2, 3 of
		* the DMC_PHY_CTL0 register and the bits 26, 27, 28, 29, 30, 31
		* of the DMC_PHY_CTL2 are set. */
		if (DMC_Param_List.ddr_mode == DDR3_MODE ||
		    DMC_Param_List.ddr_mode == DDR2_MODE) {
			writel(0XFC000000, REG_DMC0_PHY_CTL2);
			writel(0x0000000f, REG_DMC0_PHY_CTL0);
		}

		writel(0x00000000, REG_DMC0_PHY_CTL1);

		/* 4. For DDR3 mode, set bit 1 and configure bits [5:2] of the
		 * DMC_CPHY_CTL register with WL=CWL+AL in DCLK cycles. */
		if (DMC_Param_List.ddr_mode == DDR3_MODE)
			writel(DMC_Param_List.dmc_cphyctl_value, REG_DMC0_CPHY_CTL);
		/* 5. Perform On Die Termination(ODT) and Driver Impedance Calibration*/
		if (DMC_Param_List.ddr_mode == LPDDR_MODE) {
			/* Bypass processor ODT */
			writel(0x80000, REG_DMC0_PHY_CTL1);
		} else {
			/* Make sure that the bits RTTCALEN, PDCALEN, PUCALEN of register*/
			temp = BITM_DMC_CAL_PADCTL0_RTTCALEN | BITM_DMC_CAL_PADCTL0_PDCALEN | BITM_DMC_CAL_PADCTL0_PUCALEN;
			writel(temp, REG_DMC0_CAL_PADCTL0);
			/* Configure ODT and drive impedance values in the
			 * DMCx_CAL_PADCTL2 register */
			writel(DMC_Param_List.padctl2_value, REG_DMC0_CAL_PADCTL2);
			/* start calibration */
			temp |= BITM_DMC_CAL_PADCTL0_CALSTRT;
			writel(temp, REG_DMC0_CAL_PADCTL0);
			/* Wait for PAD calibration to complete - 300 DCLK cycle.
			 * Worst case: CCLK=450 MHz, DCLK=125 MHz */
			for (i = 0; i < 300 * DMC_Param_List.cclk_dclk_ratio; i++);
		}
	}

	if (DMC_Param_List.dmc_no == 1) {
		/* 1. Set DDR mode to DDR3/DDR2/LPDDR in DMCx_PHY_CTL4 register */
		if (DMC_Param_List.ddr_mode == DDR3_MODE)
			writel(ENUM_DMC_PHY_CTL4_DDR3, REG_DMC1_PHY_CTL4);
		else if (DMC_Param_List.ddr_mode == DDR2_MODE)
			writel(ENUM_DMC_PHY_CTL4_DDR2, REG_DMC1_PHY_CTL4);
		else if (DMC_Param_List.ddr_mode == LPDDR_MODE)
			writel(ENUM_DMC_PHY_CTL4_LPDDR, REG_DMC1_PHY_CTL4);
		/* 2. Make sure that the bits 6, 7, 25, and 27 of the
		 * DMC_PHY_CTL3 register are set */
		writel(0xA00000C0, REG_DMC1_PHY_CTL3);
	    /* 3. For DDR2/DDR3 mode, make sure that the bits 0, 1, 2, 3 of the
		 * DMC_PHY_CTL0 register and the bits 26, 27, 28, 29, 30, 31 of the
		 * DMC_PHY_CTL2 are set. */
		if (DMC_Param_List.ddr_mode == DDR3_MODE ||
		    DMC_Param_List.ddr_mode == DDR2_MODE) {
			writel(0XFC000000, REG_DMC1_PHY_CTL2);
			writel(0x0000000f, REG_DMC1_PHY_CTL0);
		}
		/* 4. For DDR3 mode, set bit 1 and configure bits [5:2] of the
		 * DMC_CPHY_CTL register with WL=CWL+AL in DCLK cycles. */
		if (DMC_Param_List.ddr_mode == DDR3_MODE)
			writel(DMC_Param_List.dmc_cphyctl_value, REG_DMC1_CPHY_CTL);
	   /* 5. Perform On Die Termination (ODT) and Driver Impedance Calibration */
		if (DMC_Param_List.ddr_mode == LPDDR_MODE) {
			/* Bypass processor ODT */
			writel(0x80000, REG_DMC1_PHY_CTL1);
		} else {
			/* Configure ODT and drive impedance values in the
			 * DMCx_CAL_PADCTL2 register */
			writel(DMC_Param_List.padctl2_value, REG_DMC1_CAL_PADCTL2);
			/* Make sure that the bits RTTCALEN, PDCALEN, PUCALEN of register
			 * DMCx_CAL_PADCTL0 are set (they are set at reset) and start calibration */
			writel(BITM_DMC_CAL_PADCTL0_RTTCALEN |
			       BITM_DMC_CAL_PADCTL0_PDCALEN |
			       BITM_DMC_CAL_PADCTL0_PUCALEN |
			       BITM_DMC_CAL_PADCTL0_CALSTRT, REG_DMC1_CAL_PADCTL0);
			/* Wait for PAD calibration to complete - 300 DCLK cycle.
			 * Worst case: CCLK=450 MHz, DCLK=125 MHz */
			for (i = 0; i < 300 * DMC_Param_List.cclk_dclk_ratio; i++);
		}
	}
}

static u32 read32(u32 addr)
{
	return *(volatile u32 *)addr;
}

void DMC_Controller_Init(struct dmc_param DMC_Param_List)
{
	if (DMC_Param_List.dmc_no == 0) {
		/* 1. Program the DMC controller registers: DMCx_CFG, DMCx_TR0,
		 * DMCx_TR1, DMCx_TR2, DMCx_MR(DDR2/LPDDR)/DMCx_MR0(DDR3),
		 * DMCx_EMR1(DDR2)/DMCx_MR1(DDR3),
		 * DMCx_EMR2(DDR2)/DMCx_EMR(LPDDR)/DMCx_MR2(DDR3) */
		writel(DMC_Param_List.dmc_cfg_value, REG_DMC0_CFG);
		writel(DMC_Param_List.dmc_tr0_value, REG_DMC0_TR0);
		writel(DMC_Param_List.dmc_tr1_value, REG_DMC0_TR1);
		writel(DMC_Param_List.dmc_tr2_value, REG_DMC0_TR2);
		writel(DMC_Param_List.dmc_mr0_value, REG_DMC0_MR);
		writel(DMC_Param_List.dmc_mr1_value, REG_DMC0_EMR1);
		writel(DMC_Param_List.dmc_mr2_value, REG_DMC0_EMR2);
		/* 2. Make sure that the DMC_DT_CALIB_ADDR register is programmed
		 * to an unused DMC location corresponding to a burst of 16 bytes
		 * (by default it is the starting address of the DMC address range). */
		writel(DMC_Param_List.dmc_data_calib_add_value, REG_DMC0_DT_CALIB_ADDR);
		/* 3. Program the DMCx_CTL register with INIT bit set to start
		 * the DMC initialization sequence */
		writel(DMC_Param_List.dmc_ctl_value, REG_DMC0_CTL);
		/* 4. Wait for the DMC initialization to complete by polling
		 * DMCx_STAT.INITDONE bit. */
		while ((readl(REG_DMC0_STAT) & BITM_DMC_STAT_INITDONE) == 0);

		/* 5. Program the DMCx_CTL.DLLCTL register with 0x948 value
		 * (DATACYC=9,	DLLCALRDCNT=72). */
		writel(0x00000948, REG_DMC0_DLLCTL);
		/* 6. Workaround for anomaly#20000037 */
		if (DMC_Param_List.anomaly_20000037_applicable == true) {
			/* Perform dummy read to any DMC location */
			read32(0x80000000); // readl broken ?

			writel(readl(REG_DMC0_PHY_CTL0) | 0x1000, REG_DMC0_PHY_CTL0);
			/* Clear DMCx_PHY_CTL0.RESETDAT bit */
			writel(readl(REG_DMC0_PHY_CTL0) & (~0x1000), REG_DMC0_PHY_CTL0);
		}
	}

	if (DMC_Param_List.dmc_no == 1) {
		/* 1. Program the DMC controller registers: DMCx_CFG, DMCx_TR0,
		 * DMCx_TR1, DMCx_TR2, DMCx_MR(DDR2/LPDDR)/DMCx_MR0(DDR3),
		 * DMCx_EMR1(DDR2)/DMCx_MR1(DDR3),
		 * DMCx_EMR2(DDR2)/DMCx_EMR(LPDDR)/DMCx_MR2(DDR3) */
		writel(DMC_Param_List.dmc_cfg_value, REG_DMC1_CFG);
		writel(DMC_Param_List.dmc_tr0_value, REG_DMC1_TR0);
		writel(DMC_Param_List.dmc_tr1_value, REG_DMC1_TR1);
		writel(DMC_Param_List.dmc_tr2_value, REG_DMC1_TR2);
		writel(DMC_Param_List.dmc_mr0_value, REG_DMC1_MR);
		writel(DMC_Param_List.dmc_mr1_value, REG_DMC1_EMR1);
		writel(DMC_Param_List.dmc_mr2_value, REG_DMC1_EMR2);

		/* 2. Make sure that the DMC_DT_CALIB_ADDR register is programmed to
		 * an unused DMC location corresponding to a burst of 16 bytes
		 * (by default it is the starting address of the DMC address range). */
		writel(DMC_Param_List.dmc_data_calib_add_value, REG_DMC1_DT_CALIB_ADDR);

		/* 3. Program the DMCx_CTL register with INIT bit set to start the
		 * DMC initialization sequence */
		writel(DMC_Param_List.dmc_ctl_value, REG_DMC1_CTL);

		/* 4. Wait for the DMC initialization to complete by polling
		 * DMCx_STAT.INITDONE bit. */
		while ((readl(REG_DMC1_STAT) & BITM_DMC_STAT_INITDONE) == 0);

		/* 5. Program the DMCx_CTL.DLLCTL register with 0x948 value
		 * (DATACYC=9,	DLLCALRDCNT=72). */
		writel(0x00000948, REG_DMC1_DLLCTL);

		/* 6. Workaround for anomaly#20000037 */
		if (DMC_Param_List.anomaly_20000037_applicable == true) {
			/* Perform dummy read to any DMC location */
			read32(0x80000000); // readl broken ?

			/* Set DMCx_PHY_CTL0.RESETDAT bit */
			writel(readl(REG_DMC1_PHY_CTL0) | 0x1000, REG_DMC1_PHY_CTL0);
			/* Clear DMCx_PHY_CTL0.RESETDAT bit */
			writel(readl(REG_DMC1_PHY_CTL0) & (~0x1000), REG_DMC1_PHY_CTL0);
		}
	}
}

void DMC_Init(struct dmc_param DMC_Param_List)
{
	/* PHY Initialization */
	if (DMC_Param_List.phy_init_required == true)
		DMC_PHY_Init(DMC_Param_List);
	/* Controller Initialization */
	DMC_Controller_Init(DMC_Param_List);
}

void DMC_Config(void)
{
#ifdef MEM_DMC0
	/* Initialize the DMC parameters list */
	DMC_Param_List.dmc_no = 0;
#ifdef CONFIG_TARGET_SC584_EZKIT
	DMC_Param_List.ddr_mode = DDR2_MODE;
#else
	DMC_Param_List.ddr_mode = DDR3_MODE;
#endif
	DMC_Param_List.phy_init_required = true;
	DMC_Param_List.cclk_dclk_ratio = 1;
	DMC_Param_List.anomaly_20000037_applicable = true;
	DMC_Param_List.dmc_ctl_value = DMC_CTL_VALUE;
	DMC_Param_List.dmc_cfg_value = DMC_CFG_VALUE;
	DMC_Param_List.dmc_tr0_value = DMC_TR0_VALUE;
	DMC_Param_List.dmc_tr1_value = DMC_TR1_VALUE;
	DMC_Param_List.dmc_tr2_value = DMC_TR2_VALUE;
	DMC_Param_List.dmc_mr0_value = DMC_MR0_VALUE;
	DMC_Param_List.dmc_mr1_value = DMC_MR1_VALUE;
	DMC_Param_List.dmc_mr2_value = DMC_MR2_VALUE;
	DMC_Param_List.padctl2_value = DMC_PADCTL2_VALUE;
	DMC_Param_List.dmc_cphyctl_value = DMC_CPHYCTL_VALUE;
	DMC_Param_List.dmc_data_calib_add_value = DMC0_DATA_CALIB_ADD;
	/* Initialize DMC0 now */
	DMC_Init(DMC_Param_List);
#endif
#ifdef MEM_DMC1
	/* Initialize the DMC parameters list */
	DMC_Param_List.dmc_no = 1;
	DMC_Param_List.ddr_mode = DDR3_MODE;
	DMC_Param_List.phy_init_required = true;
	DMC_Param_List.cclk_dclk_ratio = 1;
	DMC_Param_List.anomaly_20000037_applicable = true;
	DMC_Param_List.dmc_ctl_value = DMC_CTL_VALUE;
	DMC_Param_List.dmc_cfg_value = DMC_CFG_VALUE;
	DMC_Param_List.dmc_tr0_value = DMC_TR0_VALUE;
	DMC_Param_List.dmc_tr1_value = DMC_TR1_VALUE;
	DMC_Param_List.dmc_tr2_value = DMC_TR2_VALUE;
	DMC_Param_List.dmc_mr0_value = DMC_MR0_VALUE;
	DMC_Param_List.dmc_mr1_value = DMC_MR1_VALUE;
	DMC_Param_List.dmc_mr2_value = DMC_MR2_VALUE;
	DMC_Param_List.padctl2_value = DMC_PADCTL2_VALUE;
	DMC_Param_List.dmc_cphyctl_value = DMC_CPHYCTL_VALUE;
	DMC_Param_List.dmc_data_calib_add_value = DMC1_DATA_CALIB_ADD;
	/* Initialize DMC1 now */
	DMC_Init(DMC_Param_List);
#endif
}
