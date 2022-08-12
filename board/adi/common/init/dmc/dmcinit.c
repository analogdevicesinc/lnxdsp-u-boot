/*
**
** Copyright (C) 2018-2022 Analog Devices Inc., All Rights Reserved.
**
*/

#include <config.h>
#include <common.h>
#if defined(CONFIG_SC59X_64)
#include <asm/arch/sc59x_64.h>
#elif defined(CONFIG_SC59X)
#include <asm/arch/sc59x.h>
#elif defined(CONFIG_SC58X)
#include <asm/arch/sc58x.h>
#elif defined(CONFIG_SC57X)
#include <asm/arch/sc57x.h>
#endif
#include "dmcinit.h"
#include <stdbool.h>

static struct dmc_param dmc;

__attribute__((always_inline)) static inline void calibration_legacy(){
    int i;
    uint32_t temp;

    /* 1. Set DDR mode to DDR3/DDR2/LPDDR in DMCx_PHY_CTL4 register */
    if (dmc.ddr_mode == DDR3_MODE)
        writel(ENUM_DMC_PHY_CTL4_DDR3, dmc.registers->REG_DMC_PHY_CTL4);
    else if (dmc.ddr_mode == DDR2_MODE)
        writel(ENUM_DMC_PHY_CTL4_DDR2, dmc.registers->REG_DMC_PHY_CTL4);
    else if (dmc.ddr_mode == LPDDR_MODE)
        writel(ENUM_DMC_PHY_CTL4_LPDDR, dmc.registers->REG_DMC_PHY_CTL4);

   /* 2. Make sure that the bits 6, 7, 25, and 27 of the DMC_PHY_
    * CTL3 register are set */
    writel(0x0A0000C0, dmc.registers->REG_DMC_PHY_CTL3);

   /* 3. For DDR2/DDR3 mode, make sure that the bits 0, 1, 2, 3 of
    * the DMC_PHY_CTL0 register and the bits 26, 27, 28, 29, 30, 31
    * of the DMC_PHY_CTL2 are set. */
    if (dmc.ddr_mode == DDR3_MODE ||
        dmc.ddr_mode == DDR2_MODE) {
        writel(0XFC000000, dmc.registers->REG_DMC_PHY_CTL2);
        writel(0x0000000f, dmc.registers->REG_DMC_PHY_CTL0);
    }

    writel(0x00000000, dmc.registers->REG_DMC_PHY_CTL1);

    /* 4. For DDR3 mode, set bit 1 and configure bits [5:2] of the
     * DMC_CPHY_CTL register with WL=CWL+AL in DCLK cycles. */
    if (dmc.ddr_mode == DDR3_MODE)
        writel(dmc.dmc_cphyctl_value, dmc.registers->REG_DMC_CPHY_CTL);
    /* 5. Perform On Die Termination(ODT) and Driver Impedance Calibration*/
    if (dmc.ddr_mode == LPDDR_MODE) {
        /* Bypass processor ODT */
        writel(0x80000, dmc.registers->REG_DMC_PHY_CTL1);
    } else {
        /* Make sure that the bits RTTCALEN, PDCALEN, PUCALEN of register*/
        temp = BITM_DMC_CAL_PADCTL0_RTTCALEN | BITM_DMC_CAL_PADCTL0_PDCALEN | BITM_DMC_CAL_PADCTL0_PUCALEN;
        writel(temp, dmc.registers->REG_DMC_CAL_PADCTL0);
        /* Configure ODT and drive impedance values in the
         * DMCx_CAL_PADCTL2 register */
        writel(dmc.padctl2_value, dmc.registers->REG_DMC_CAL_PADCTL2);
        /* start calibration */
        temp |= BITM_DMC_CAL_PADCTL0_CALSTRT;
        writel(temp, dmc.registers->REG_DMC_CAL_PADCTL0);
        /* Wait for PAD calibration to complete - 300 DCLK cycle.
         * Worst case: CCLK=450 MHz, DCLK=125 MHz */
        for (i = 0; i < 300 * dmc.cclk_dclk_ratio; i++);
    }
}

__attribute__((always_inline)) static inline void calibration_method1(){

#ifdef CONFIG_SC59X
    writel(dmc.dmc_zqctl0_value, dmc.registers->REG_DMC_DDR_ZQ_CTL0);
    writel(dmc.dmc_zqctl1_value ,dmc.registers->REG_DMC_DDR_ZQ_CTL1);
    writel(dmc.dmc_zqctl2_value, dmc.registers->REG_DMC_DDR_ZQ_CTL2);

    /* Generate the trigger */
    writel(0x0ul, dmc.registers->REG_DMC_DDR_CA_CTL);
    writel(0x0ul, dmc.registers->REG_DMC_DDR_ROOT_CTL);
    writel(0x00010000ul, dmc.registers->REG_DMC_DDR_ROOT_CTL);
    dmcdelay(8000u, dmc.cclk_dclk_ratio);

    /* The [31:26] bits may change if pad ring changes */
    writel(0x0C000001ul|DMC_TRIG_CALIB,  dmc.registers->REG_DMC_DDR_CA_CTL);
    dmcdelay(8000u, dmc.cclk_dclk_ratio);
    writel(0x0ul, dmc.registers->REG_DMC_DDR_CA_CTL);
    writel(0x0ul, dmc.registers->REG_DMC_DDR_ROOT_CTL);
#endif

}

__attribute__((always_inline)) static inline void calibration_method2(){

#ifdef CONFIG_SC59X
  /* Reset trigger */
  writel(0x0ul, dmc.registers->REG_DMC_DDR_CA_CTL);
  writel(0x0ul, dmc.registers->REG_DMC_DDR_ROOT_CTL);
  dmcdelay(5000u, dmc.cclk_dclk_ratio);

  writel(0x0ul, dmc.registers->REG_DMC_DDR_SCRATCH_3);
  writel(0x0ul, dmc.registers->REG_DMC_DDR_SCRATCH_2);
  dmcdelay(5000u, dmc.cclk_dclk_ratio);

  /* Writing internal registers in calib pad to zero. Calib mode set to 1 [26], trig M1 S1 write [16],
   * this enables usage of scratch registers instead of ZQCTL registers
   */
  writel(0x04010000ul, dmc.registers->REG_DMC_DDR_ROOT_CTL);
  dmcdelay(5000u, dmc.cclk_dclk_ratio);

  /* TRIGGER FOR M2-S2 WRITE     -> slave id 31:26  trig m2,s2 write bit 1->1
   * slave1 address is 4
   */
  writel(0x10000002ul, dmc.registers->REG_DMC_DDR_CA_CTL);
  dmcdelay(5000u, dmc.cclk_dclk_ratio);

  /* reset Trigger */
  writel(0x0u, dmc.registers->REG_DMC_DDR_CA_CTL);
  writel(0x0u, dmc.registers->REG_DMC_DDR_ROOT_CTL); 

  /* write to slave 1, make the power down bit high */
  writel(0x1ul<<12, dmc.registers->REG_DMC_DDR_SCRATCH_3);
  writel(0x0ul, dmc.registers->REG_DMC_DDR_SCRATCH_2);
  dmcdelay(5000u, dmc.cclk_dclk_ratio);

  /* Calib mode set to 1 [26], trig M1 S1 write [16] */
  writel(0x04010000ul, dmc.registers->REG_DMC_DDR_ROOT_CTL);
  dmcdelay(5000u, dmc.cclk_dclk_ratio);

  writel(0x10000002ul, dmc.registers->REG_DMC_DDR_CA_CTL);
  dmcdelay(5000u, dmc.cclk_dclk_ratio);

  writel(0x0ul, dmc.registers->REG_DMC_DDR_CA_CTL);
  writel(0x0ul, dmc.registers->REG_DMC_DDR_ROOT_CTL); 
  dmcdelay(5000u, dmc.cclk_dclk_ratio);

  /* for slave 0 */
  writel(dmc.dmc_zqctl0_value, dmc.registers->REG_DMC_DDR_SCRATCH_2);
  dmcdelay(5000u, dmc.cclk_dclk_ratio);

  /* Calib mode set to 1 [26], trig M1 S1 write [16] */
  writel(0x04010000ul, dmc.registers->REG_DMC_DDR_ROOT_CTL);
  dmcdelay(5000u, dmc.cclk_dclk_ratio);

  writel(0x0C000002ul, dmc.registers->REG_DMC_DDR_CA_CTL);
  dmcdelay(5000u, dmc.cclk_dclk_ratio);

  writel(0x0ul, dmc.registers->REG_DMC_DDR_CA_CTL);
  writel(0x0ul, dmc.registers->REG_DMC_DDR_ROOT_CTL); 
  dmcdelay(5000u, dmc.cclk_dclk_ratio);

  /* writing to slave 1
  calstrt is 0, but other programming is done */
  /* make power down LOW again, to kickstart BIAS circuit */
  writel(0x0ul, dmc.registers->REG_DMC_DDR_SCRATCH_3);
  writel(0x70000000ul, dmc.registers->REG_DMC_DDR_SCRATCH_2);
  dmcdelay(5000u, dmc.cclk_dclk_ratio);

  /* write to ca_ctl lane, calib mode set to 1 [26], trig M1 S1 write [16]*/
  writel(0x04010000ul, dmc.registers->REG_DMC_DDR_ROOT_CTL);
  dmcdelay(5000u, dmc.cclk_dclk_ratio);

  /*  copies data to lane controller slave
   *  TRIGGER FOR M2-S2 WRITE     -> slave id 31:26  trig m2,s2 write bit 1->1
   *  slave1 address is 4
   */
  writel(0x10000002ul, dmc.registers->REG_DMC_DDR_CA_CTL);
  dmcdelay(5000u, dmc.cclk_dclk_ratio);

  /* reset Trigger */
  writel(0x0ul, dmc.registers->REG_DMC_DDR_CA_CTL);
  writel(0x0ul, dmc.registers->REG_DMC_DDR_ROOT_CTL);
#endif

}

__attribute__((always_inline)) inline void adi_dmc_lane_reset(bool reset, uint32_t dmc_no){

#ifdef CONFIG_SC59X
    uint32_t temp;

    if (reset) {
        temp = readl(REG_DMC_DDR_LANE0_CTL0(dmc_no));
        temp |= BITM_DMC_DDR_LANE0_CTL0_CB_RSTDLL;
        writel(temp, REG_DMC_DDR_LANE0_CTL0(dmc_no));

        temp = readl(REG_DMC_DDR_LANE1_CTL0(dmc_no));
        temp |= BITM_DMC_DDR_LANE1_CTL0_CB_RSTDLL;
        writel(temp, REG_DMC_DDR_LANE1_CTL0(dmc_no));
    } else {
        temp = readl(REG_DMC_DDR_LANE0_CTL0(dmc_no));
        temp &= ~BITM_DMC_DDR_LANE0_CTL0_CB_RSTDLL;
        writel(temp, REG_DMC_DDR_LANE0_CTL0(dmc_no));

        temp = readl(REG_DMC_DDR_LANE1_CTL0(dmc_no));
        temp &= ~BITM_DMC_DDR_LANE1_CTL0_CB_RSTDLL;
        writel(temp, REG_DMC_DDR_LANE1_CTL0(dmc_no));
    }
    dmcdelay(9000u, dmc.cclk_dclk_ratio);
#endif

}

#pragma GCC push_options
#pragma GCC optimize ("O0")
__attribute__((always_inline)) static inline void dummy_read(){
    /* Do not let this be optimized out by compiler */
    readl(0x80000000);
}
#pragma GCC pop_options

__attribute__((always_inline)) static inline void dmc_controller_init(){
#ifdef CONFIG_SC59X
    uint32_t phyphase, rd_cnt, t_EMR1, t_EMR3, t_CTL, data_cyc, temp;
#endif

    /* 1. Program the DMC controller registers: DMCx_CFG, DMCx_TR0,
     * DMCx_TR1, DMCx_TR2, DMCx_MR(DDR2/LPDDR)/DMCx_MR0(DDR3),
     * DMCx_EMR1(DDR2)/DMCx_MR1(DDR3),
     * DMCx_EMR2(DDR2)/DMCx_EMR(LPDDR)/DMCx_MR2(DDR3) */
    writel(dmc.dmc_cfg_value, dmc.registers->REG_DMC_CFG);
    writel(dmc.dmc_tr0_value, dmc.registers->REG_DMC_TR0);
    writel(dmc.dmc_tr1_value, dmc.registers->REG_DMC_TR1);
    writel(dmc.dmc_tr2_value, dmc.registers->REG_DMC_TR2);
    writel(dmc.dmc_mr0_value, dmc.registers->REG_DMC_MR);
    writel(dmc.dmc_mr1_value, dmc.registers->REG_DMC_EMR1);
    writel(dmc.dmc_mr2_value, dmc.registers->REG_DMC_EMR2);

#ifdef CONFIG_SC59X
    writel(dmc.dmc_mr3_value, dmc.registers->REG_DMC_EMR3);
    writel(dmc.dmc_dllctl_value, dmc.registers->REG_DMC_DLLCTL);

    temp = readl(dmc.registers->REG_DMC_DDR_CA_CTL);
    temp |= BITM_DMC_DDR_CA_CTL_SW_REFRESH;
    writel(temp, dmc.registers->REG_DMC_DDR_CA_CTL);

    temp = readl(dmc.registers->REG_DMC_DDR_ROOT_CTL);
    temp |= BITM_DMC_DDR_ROOT_CTL_SW_REFRESH | (DMC_OFSTDCYCLE << BITP_DMC_DDR_ROOT_CTL_PIPE_OFSTDCYCLE);
    writel(temp, dmc.registers->REG_DMC_DDR_ROOT_CTL);
#endif

    /* 2. Make sure that the REG_DMC_DT_CALIB_ADDR register is programmed
     * to an unused DMC location corresponding to a burst of 16 bytes
     * (by default it is the starting address of the DMC address range). */
#ifndef CONFIG_SC59X
    writel(dmc.dmc_data_calib_add_value, dmc.registers->REG_DMC_DT_CALIB_ADDR);
#endif
    /* 3. Program the DMCx_CTL register with INIT bit set to start
     * the DMC initialization sequence */
    writel(dmc.dmc_ctl_value, dmc.registers->REG_DMC_CTL);
    /* 4. Wait for the DMC initialization to complete by polling
     * DMCx_STAT.INITDONE bit. */

#ifdef CONFIG_SC59X
    dmcdelay(722000u, dmc.cclk_dclk_ratio);

    /* Add necessary delay depending on the configuration */
    t_EMR1 = (dmc.dmc_mr1_value & BITM_DMC_MR1_WL)>>BITP_DMC_MR1_WL;
    dmcdelay(600u, dmc.cclk_dclk_ratio);  
    if(t_EMR1 != 0u)
        while ((readl(dmc.registers->REG_DMC_EMR1) & BITM_DMC_MR1_WL) != 0);

    t_EMR3 = (dmc.dmc_mr3_value & BITM_DMC_EMR3_MPR)>>BITP_DMC_EMR3_MPR;
    dmcdelay(2000u, dmc.cclk_dclk_ratio);
    if(t_EMR3 != 0u)
        while ((readl(dmc.registers->REG_DMC_EMR3) & BITM_DMC_EMR3_MPR) != 0);


    t_CTL = (dmc.dmc_ctl_value & BITM_DMC_CTL_RL_DQS)>>BITP_DMC_CTL_RL_DQS;
    dmcdelay(600u, dmc.cclk_dclk_ratio);  
    if(t_CTL != 0u)
        while ((readl(dmc.registers->REG_DMC_CTL) & BITM_DMC_CTL_RL_DQS) != 0);
#endif

    /* check if DMC initialization finished*/
    while ((readl(dmc.registers->REG_DMC_STAT) & BITM_DMC_STAT_INITDONE) == 0);

#ifdef CONFIG_SC59X
    /* toggle DCYCLE */
    temp = readl(dmc.registers->REG_DMC_DDR_LANE0_CTL1);
    temp |= BITM_DMC_DDR_LANE0_CTL1_COMP_DCYCLE;
    writel(temp, dmc.registers->REG_DMC_DDR_LANE0_CTL1);

    temp = readl(dmc.registers->REG_DMC_DDR_LANE1_CTL1);
    temp |= BITM_DMC_DDR_LANE1_CTL1_COMP_DCYCLE;
    writel(temp, dmc.registers->REG_DMC_DDR_LANE1_CTL1);

    temp = readl(dmc.registers->REG_DMC_DDR_LANE0_CTL1);
    temp &= (~BITM_DMC_DDR_LANE0_CTL1_COMP_DCYCLE);
    writel(temp, dmc.registers->REG_DMC_DDR_LANE0_CTL1);

    temp = readl(dmc.registers->REG_DMC_DDR_LANE1_CTL1);
    temp &= (~BITM_DMC_DDR_LANE1_CTL1_COMP_DCYCLE);
    writel(temp, dmc.registers->REG_DMC_DDR_LANE1_CTL1);

    /* toggle RSTDAT */
    temp = readl(dmc.registers->REG_DMC_DDR_LANE0_CTL0);
    temp |= BITM_DMC_DDR_LANE0_CTL0_CB_RSTDAT;
    writel(temp, dmc.registers->REG_DMC_DDR_LANE0_CTL0);

    temp = readl(dmc.registers->REG_DMC_DDR_LANE0_CTL0);
    temp &= (~BITM_DMC_DDR_LANE0_CTL0_CB_RSTDAT);
    writel(temp, dmc.registers->REG_DMC_DDR_LANE0_CTL0);

    temp = readl(dmc.registers->REG_DMC_DDR_LANE1_CTL0);
    temp |= BITM_DMC_DDR_LANE1_CTL0_CB_RSTDAT;
    writel(temp, dmc.registers->REG_DMC_DDR_LANE1_CTL0);

    temp = readl(dmc.registers->REG_DMC_DDR_LANE1_CTL0);
    temp &= (~BITM_DMC_DDR_LANE1_CTL0_CB_RSTDAT);
    writel(temp, dmc.registers->REG_DMC_DDR_LANE1_CTL0);

    dmcdelay(2500u, dmc.cclk_dclk_ratio);

    /* Program phyphase*/
    phyphase = (readl(dmc.registers->REG_DMC_STAT) & BITM_DMC_STAT_PHYRDPHASE)>>BITP_DMC_STAT_PHYRDPHASE; 
    data_cyc = (phyphase << BITP_DMC_DLLCTL_DATACYC) & BITM_DMC_DLLCTL_DATACYC;
    rd_cnt = dmc.dmc_dllctl_value;
    rd_cnt <<= BITP_DMC_DLLCTL_DLLCALRDCNT;
    rd_cnt &= BITM_DMC_DLLCTL_DLLCALRDCNT;
    writel(rd_cnt|data_cyc, dmc.registers->REG_DMC_DLLCTL);
    writel((dmc.dmc_ctl_value & (~BITM_DMC_CTL_INIT) & (~BITM_DMC_CTL_RL_DQS)), dmc.registers->REG_DMC_CTL);
#else
    /* 5. Program the DMCx_CTL.DLLCTL register with 0x948 value
     * (DATACYC=9,    DLLCALRDCNT=72). */
    writel(0x00000948, dmc.registers->REG_DMC_DLLCTL);
#endif

    /* 6. Workaround for anomaly#20000037 */
    if (dmc.anomaly_20000037_applicable == true) {
        /* Perform dummy read to any DMC location */
        dummy_read();

        writel(readl(dmc.registers->REG_DMC_PHY_CTL0) | 0x1000, dmc.registers->REG_DMC_PHY_CTL0);
        /* Clear DMCx_PHY_CTL0.RESETDAT bit */
        writel(readl(dmc.registers->REG_DMC_PHY_CTL0) & (~0x1000), dmc.registers->REG_DMC_PHY_CTL0);
    }
}

__attribute__((always_inline)) inline void dmc_init()
{
    /* PHY Calibration+Initialization */
    if (dmc.phy_init_required == true){
      switch(dmc.calib_mode){
        case CALIBRATION_LEGACY:
          calibration_legacy();
          break;
        case CALIBRATION_METHOD1:
          calibration_method1();
          break;
        case CALIBRATION_METHOD2:
          calibration_method2();
          break;
      }
    }

    /* Controller Initialization */
    dmc_controller_init();
}

__attribute__((always_inline)) inline void __dmc_config(uint32_t dmc_no){
    /* Initialize the DMC parameters list */
    dmc.dmc_no = dmc_no;

    if(dmc_no == 0){
        dmc.registers = &dmc_registers0;
    }else if(dmc_no == 1){
        dmc.registers = &dmc_registers1;
    }

#ifdef CONFIG_TARGET_SC584_EZKIT
    dmc.ddr_mode = DDR2_MODE;
#else
    dmc.ddr_mode = DDR3_MODE;
#endif

    dmc.phy_init_required = true;

#ifdef CONFIG_SC59X
    dmc.cclk_dclk_ratio = 1250;
    dmc.anomaly_20000037_applicable = false;    
    dmc.dmc_dllctl_value = DMC_DLLCTL_VALUE;
    dmc.calib_mode = CALIBRATION_METHOD2;
#else
    dmc.cclk_dclk_ratio = 1000;
    dmc.anomaly_20000037_applicable = true;
    dmc.calib_mode = CALIBRATION_LEGACY;
#endif

    dmc.dmc_ctl_value = DMC_CTL_VALUE;
    dmc.dmc_cfg_value = DMC_CFG_VALUE;
    dmc.dmc_tr0_value = DMC_TR0_VALUE;
    dmc.dmc_tr1_value = DMC_TR1_VALUE;
    dmc.dmc_tr2_value = DMC_TR2_VALUE;
    dmc.dmc_mr0_value = DMC_MR0_VALUE;
    dmc.dmc_mr1_value = DMC_MR1_VALUE;
    dmc.dmc_mr2_value = DMC_MR2_VALUE;

#ifdef CONFIG_SC59X
    dmc.dmc_mr2_value = DMC_MR3_VALUE;
    dmc.dmc_zqctl0_value = DMC_ZQCTL0_VALUE;
    dmc.dmc_zqctl1_value = DMC_ZQCTL1_VALUE;
    dmc.dmc_zqctl2_value = DMC_ZQCTL2_VALUE;
#endif

    dmc.padctl2_value = DMC_PADCTL2_VALUE;
    dmc.dmc_cphyctl_value = DMC_CPHYCTL_VALUE;
    dmc.dmc_data_calib_add_value = dmc.registers->VAL_DMC_DATA_CALIB_ADD;

    /* Initialize DMC now */
    dmc_init();
}

void DMC_Config(void)
{
#ifdef MEM_DMC0
    __dmc_config(0);
#endif

#ifdef MEM_DMC1
    __dmc_config(1);
#endif
}
