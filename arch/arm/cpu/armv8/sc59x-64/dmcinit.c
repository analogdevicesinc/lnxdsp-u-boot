/*
**
** Copyright (C) 2018-2020 Analog Devices Inc., All Rights Reserved.
**
*/

#include <config.h>
#include <common.h>
#include <asm/arch/sc59x_64.h>
#include "dmcinit.h"
#include <stdbool.h>

#define CONFIG_DMC0

#ifdef CONFIG_DMC0

#define DQS_Default_Delay 3ul

#define DelayTrim 1
#define Lane0_DQS_Delay 1
#define Lane1_DQS_Delay 1

#define TrigCalib 0ul
#define CLKDIR 0ul
#define OfstdCycle 2ul

#define cclkdclk_ratio (1.5f)

#define DqsTrim 0
#define Dqscode 0ul

#define ClkTrim 0
#define Clkcode 0ul

#define PHY_CALIB_METHOD2 1

static ADI_DMC_CONFIG config =
{
        CFG0_REG_DDR_DLLCTLCFG,
        CFG0_REG_DMC_MR2MR3,
        CFG0_REG_DMC_CTL_VALUE,
        CFG0_REG_DMC_MRMR1,
        CFG0_REG_DMC_TR0_VALUE,
        CFG0_REG_DMC_TR1_VALUE,
        CFG0_REG_DMC_TR2_VALUE,
        0x00785A64ul,
        0ul,
        0x70000000ul
};

__attribute__((always_inline)) static inline void dmcdelay(uint32_t delay)
{
    uint32_t i;

    /* Convert DDR cycles to core clock cycles */
    float f = (float)delay * cclkdclk_ratio;
    delay = (uint32_t)f;

    for(i=delay; i>0ul; i--){
        __asm__("NOP");
    }
}

/* DMC phy ZQ calibration routine
 * The first step in DMC initialization
 */
__attribute__((always_inline)) static inline void adi_dmc_phy_calibration()
{
#if PHY_CALIB_METHOD2
  uint32_t stat_value = 0x0u;
  uint32_t drv_pu , drv_pd, odt_pu, odt_pd;
  uint32_t ROdt, ClkDqsDrvImpedance;

  *pREG_DMC0_DDR_CA_CTL = 0x0ul;
  *pREG_DMC0_DDR_ROOT_CTL = 0x0ul;
  *pREG_DMC0_DDR_SCRATCH_3 = 0x0ul;
  *pREG_DMC0_DDR_SCRATCH_2 = 0x0ul;
  *pREG_DMC0_DDR_ROOT_CTL = 0x04010000ul;
  dmcdelay(2500u);
  *pREG_DMC0_DDR_CA_CTL = 0x10000002ul;
  dmcdelay(2500u);
  *pREG_DMC0_DDR_CA_CTL = 0x0u;
  *pREG_DMC0_DDR_ROOT_CTL = 0x0u;
  *pREG_DMC0_DDR_SCRATCH_3 = 0x1ul<<12;
  *pREG_DMC0_DDR_SCRATCH_2 = 0x0ul;
  dmcdelay(2500u);
  *pREG_DMC0_DDR_ROOT_CTL = 0x04010000ul;
  dmcdelay(2500u);
  *pREG_DMC0_DDR_CA_CTL = 0x10000002ul;
  dmcdelay(2500u);
  *pREG_DMC0_DDR_CA_CTL = 0x0ul;
  *pREG_DMC0_DDR_ROOT_CTL = 0x0ul;
  *pREG_DMC0_DDR_SCRATCH_3 = 0x0ul;
  *pREG_DMC0_DDR_SCRATCH_2 = config.ulDDR_ZQCTL0;
  *pREG_DMC0_DDR_ROOT_CTL = 0x04010000ul;
  dmcdelay(2500u);
  *pREG_DMC0_DDR_CA_CTL = 0x0C000002ul;
  dmcdelay(2500u);
  *pREG_DMC0_DDR_CA_CTL = 0x0ul;
  *pREG_DMC0_DDR_ROOT_CTL = 0x0ul;
  *pREG_DMC0_DDR_SCRATCH_3 = 0x0ul;
  *pREG_DMC0_DDR_SCRATCH_2 = 0x30000000ul;
  *pREG_DMC0_DDR_ROOT_CTL = 0x04010000ul;
  dmcdelay(2500u);
  *pREG_DMC0_DDR_CA_CTL = 0x10000002ul;
  dmcdelay(2500u);
  *pREG_DMC0_DDR_CA_CTL = 0x0ul;
  *pREG_DMC0_DDR_ROOT_CTL = 0x0ul;
  *pREG_DMC0_DDR_SCRATCH_3 = 0x0ul;
  *pREG_DMC0_DDR_SCRATCH_2 = 0x0ul;
  *pREG_DMC0_DDR_SCRATCH_3 = 0x0ul;
  *pREG_DMC0_DDR_SCRATCH_2 = 0x0ul;
  *pREG_DMC0_DDR_ROOT_CTL = 0x04010000ul;
  dmcdelay(2500u);
  *pREG_DMC0_DDR_CA_CTL = 0x10000002ul;
  dmcdelay(2500u);
  *pREG_DMC0_DDR_CA_CTL = 0x0ul;
  *pREG_DMC0_DDR_ROOT_CTL = 0x0ul;
  *pREG_DMC0_DDR_SCRATCH_3 = 0x0ul;
  *pREG_DMC0_DDR_SCRATCH_2 = 0x0ul;
  *pREG_DMC0_DDR_SCRATCH_3 = 0x0ul;
  *pREG_DMC0_DDR_SCRATCH_2 = 0x50000000ul;
  *pREG_DMC0_DDR_ROOT_CTL = 0x04010000ul;
  dmcdelay(2500u);
  *pREG_DMC0_DDR_CA_CTL = 0x10000002ul;
  dmcdelay(2500u);
  *pREG_DMC0_DDR_CA_CTL = 0u;
  *pREG_DMC0_DDR_ROOT_CTL = 0u;
  *pREG_DMC0_DDR_CA_CTL = 0x0C000004u;
  dmcdelay(2500u);
  *pREG_DMC0_DDR_ROOT_CTL = BITM_DMC_DDR_ROOT_CTL_TRIG_RD_XFER_ALL;
  dmcdelay(2500u);
  *pREG_DMC0_DDR_CA_CTL = 0u;
  *pREG_DMC0_DDR_ROOT_CTL = 0u;
  /* calculate ODT PU and PD values */
  stat_value = ((*pREG_DMC0_DDR_SCRATCH_7 & 0x0000FFFFu)<<16);
  stat_value |= (*pREG_DMC0_DDR_SCRATCH_6 & 0xFFFF0000u)>>16;
  ClkDqsDrvImpedance = ((config.ulDDR_ZQCTL0) & BITM_DMC_DDR_ZQ_CTL0_IMPWRDQ) >> BITP_DMC_DDR_ZQ_CTL0_IMPWRDQ;
  ROdt = ((config.ulDDR_ZQCTL0) & BITM_DMC_DDR_ZQ_CTL0_IMPRTT) >> BITP_DMC_DDR_ZQ_CTL0_IMPRTT;
  drv_pu = stat_value & 0x0000003Fu;
  drv_pd = (stat_value>>12) & 0x0000003Fu;
  odt_pu = (drv_pu * ClkDqsDrvImpedance)/ ROdt;
  odt_pd = (drv_pd * ClkDqsDrvImpedance)/ ROdt;
  *pREG_DMC0_DDR_SCRATCH_2 |= ((1uL<<24)                     |
                              ((drv_pd & 0x0000003Fu))       |
                              ((odt_pd & 0x0000003Fu)<<6)    |
                              ((drv_pu & 0x0000003Fu)<<12)   |
                              ((odt_pu & 0x0000003Fu)<<18));
  *pREG_DMC0_DDR_ROOT_CTL = 0x0C010000u;
  dmcdelay(2500u);
  *pREG_DMC0_DDR_CA_CTL = 0x08000002u;
  dmcdelay(2500u);
  *pREG_DMC0_DDR_CA_CTL = 0u;
  *pREG_DMC0_DDR_ROOT_CTL = 0u;
  *pREG_DMC0_DDR_ROOT_CTL = 0x04010000u;
  dmcdelay(2500u);
  *pREG_DMC0_DDR_CA_CTL = 0x80000002u;
  dmcdelay(2500u);
  *pREG_DMC0_DDR_CA_CTL = 0u;
  *pREG_DMC0_DDR_ROOT_CTL = 0u;
#else /* PHY_CALIB_METHOD2 */
  *pREG_DMC0_DDR_ZQ_CTL0 = config.ulDDR_ZQCTL0;
  *pREG_DMC0_DDR_ZQ_CTL1 = config.ulDDR_ZQCTL1;
  *pREG_DMC0_DDR_ZQ_CTL2 = config.ulDDR_ZQCTL2;

  /* Generate the trigger */
  *pREG_DMC0_DDR_CA_CTL = 0x0ul;
  *pREG_DMC0_DDR_ROOT_CTL = 0x0ul;
  *pREG_DMC0_DDR_ROOT_CTL = 0x00010000ul;
  dmcdelay(8000u);

  /* The [31:26] bits may change if pad ring changes */
  *pREG_DMC0_DDR_CA_CTL = 0x0C000001ul|TrigCalib;
  dmcdelay(8000u);
  *pREG_DMC0_DDR_CA_CTL = 0x0ul;
  *pREG_DMC0_DDR_ROOT_CTL = 0x0ul;
#endif /* PHY_CALIB_METHOD2 */

#if DqsTrim
  /* DQS duty trim */
  *pREG_DMC0_DDR_LANE0_CTL0 |= ((Dqscode)<<BITP_DMC_DDR_LANE0_CTL0_BYPENB) & (BITM_DMC_DDR_LANE1_CTL0_BYPENB|BITM_DMC_DDR_LANE0_CTL0_BYPSELP|BITM_DMC_DDR_LANE0_CTL0_BYPCODE);
  *pREG_DMC0_DDR_LANE1_CTL0 |= ((Dqscode)<<BITP_DMC_DDR_LANE1_CTL0_BYPENB) & (BITM_DMC_DDR_LANE1_CTL1_BYPCODE|BITM_DMC_DDR_LANE1_CTL0_BYPSELP|BITM_DMC_DDR_LANE1_CTL0_BYPCODE);
#endif

#if ClkTrim
  /* Clock duty trim */
  *pREG_DMC0_DDR_CA_CTL |= (((Clkcode <<BITP_DMC_DDR_CA_CTL_BYPCODE1)&BITM_DMC_DDR_CA_CTL_BYPCODE1)|BITM_DMC_DDR_CA_CTL_BYPENB|((CLKDIR<<BITP_DMC_DDR_CA_CTL_BYPSELP)&BITM_DMC_DDR_CA_CTL_BYPSELP));
#endif

}

/* dmc_ctrl_init() configures DMC controller
 * It returns ADI_DMC_SUCCESS on success and ADI_DMC_FAILURE on failure
 */

__attribute__((always_inline)) static inline void adi_dmc_ctrl_init()
{
  uint32_t phyphase,rd_cnt,t_EMR1,t_EMR3, t_CTL,data_cyc;

  /* program timing registers*/
  *pREG_DMC0_CFG = (config.ulDDR_DLLCTLCFG) & 0xFFFFul;  
  *pREG_DMC0_TR0 = config.ulDDR_TR0;

  *pREG_DMC0_TR1 = config.ulDDR_TR1;
  *pREG_DMC0_TR2 = config.ulDDR_TR2;
  /* program shadow registers */
  *pREG_DMC0_MR   = ((config.ulDDR_MREMR1) >> 16ul) & 0xFFFFul;
  *pREG_DMC0_MR1  = (config.ulDDR_MREMR1) & 0xFFFFul;
  *pREG_DMC0_MR2  = (config.ulDDR_EMR2EMR3)>>16ul & 0xFFFFul;
  *pREG_DMC0_EMR3 = (config.ulDDR_EMR2EMR3) & 0xFFFFul;
  /* program Dll timing register */
  *pREG_DMC0_DLLCTL = ((config.ulDDR_DLLCTLCFG) >> 16ul) & 0xFFFFul;

  dmcdelay(2000u);

  *pREG_DMC0_DDR_CA_CTL |= BITM_DMC_DDR_CA_CTL_SW_REFRESH;

  dmcdelay(5u);

  *pREG_DMC0_DDR_ROOT_CTL |= BITM_DMC_DDR_ROOT_CTL_SW_REFRESH | (OfstdCycle << BITP_DMC_DDR_ROOT_CTL_PIPE_OFSTDCYCLE);

  /* Start DMC initialization */
  *pREG_DMC0_CTL = config.ulDDR_CTL;

  dmcdelay(722000u);

  /* Add necessary delay depending on the configuration */
  t_EMR1 = (config.ulDDR_MREMR1 & BITM_DMC_MR1_WL)>>BITP_DMC_MR1_WL;

  dmcdelay(600u);

  if(t_EMR1 != 0u)
  {
     dmcdelay(600u);
     while(((*pREG_DMC0_MR1 & BITM_DMC_MR1_WL)>>BITP_DMC_MR1_WL) != 0ul) { }
  }


  t_EMR3 = (config.ulDDR_EMR2EMR3 & BITM_DMC_EMR3_MPR)>>BITP_DMC_EMR3_MPR;
  dmcdelay(2000u);
  if(t_EMR3 != 0u)
  {
     dmcdelay(2000u);
     while(((*pREG_DMC0_EMR3 & BITM_DMC_EMR3_MPR)>>BITP_DMC_EMR3_MPR) != 0ul) { }
  }


  t_CTL = (config.ulDDR_CTL & BITM_DMC_CTL_RL_DQS)>>BITP_DMC_CTL_RL_DQS;
  dmcdelay(600u);
  if(t_CTL != 0u)
  {
     dmcdelay(600u);
     while(((*pREG_DMC0_CTL & BITM_DMC_CTL_RL_DQS)>>BITP_DMC_CTL_RL_DQS) != 0ul){ }
  }

  /* check if DMC initialization finished, if not return error */
  while((*pREG_DMC0_STAT & BITM_DMC_STAT_INITDONE) != BITM_DMC_STAT_INITDONE) {
  }


  /* End of DMC controller configuration, Start of Phy control registers */

  /* toggle DCYCLE */

  *pREG_DMC0_DDR_LANE0_CTL1 |= BITM_DMC_DDR_LANE0_CTL1_COMP_DCYCLE;
  *pREG_DMC0_DDR_LANE1_CTL1 |= BITM_DMC_DDR_LANE1_CTL1_COMP_DCYCLE;

  dmcdelay(10u);

  *pREG_DMC0_DDR_LANE0_CTL1 &= (~BITM_DMC_DDR_LANE0_CTL1_COMP_DCYCLE);
  *pREG_DMC0_DDR_LANE1_CTL1 &= (~BITM_DMC_DDR_LANE1_CTL1_COMP_DCYCLE);

  /* toggle RSTDAT */
  *pREG_DMC0_DDR_LANE0_CTL0 |= BITM_DMC_DDR_LANE0_CTL0_CB_RSTDAT;
  *pREG_DMC0_DDR_LANE0_CTL0 &= (~BITM_DMC_DDR_LANE0_CTL0_CB_RSTDAT);
  *pREG_DMC0_DDR_LANE1_CTL0 |= BITM_DMC_DDR_LANE1_CTL0_CB_RSTDAT; 
  *pREG_DMC0_DDR_LANE1_CTL0 &= (~BITM_DMC_DDR_LANE1_CTL0_CB_RSTDAT);

  dmcdelay(2500u);

  /* Program phyphase*/

  phyphase = (*pREG_DMC0_STAT & BITM_DMC_STAT_PHYRDPHASE)>>BITP_DMC_STAT_PHYRDPHASE; 
  data_cyc = (phyphase << BITP_DMC_DLLCTL_DATACYC) & BITM_DMC_DLLCTL_DATACYC;
  rd_cnt = ((config.ulDDR_DLLCTLCFG) >> 16);
  rd_cnt <<= BITP_DMC_DLLCTL_DLLCALRDCNT;
  rd_cnt &= BITM_DMC_DLLCTL_DLLCALRDCNT;
  *pREG_DMC0_DLLCTL = rd_cnt|data_cyc;
  *pREG_DMC0_CTL = (config.ulDDR_CTL & (~BITM_DMC_CTL_INIT) & (~BITM_DMC_CTL_RL_DQS));

#if DelayTrim

  /* DQS delay trim*/
  uint32_t stat_value, WL_code_LDQS, WL_code_UDQS;

  /* For LDQS */
  *pREG_DMC0_DDR_LANE0_CTL1 = (*pREG_DMC0_DDR_LANE0_CTL1) | (0x000000D0);
  dmcdelay(2500u);
  *pREG_DMC0_DDR_ROOT_CTL=0x00400000;
  dmcdelay(2500u);
  *pREG_DMC0_DDR_ROOT_CTL =0x0;
  stat_value = (*pREG_DMC0_DDR_SCRATCH_STAT0 & (0xFFFF0000))>>16;
  WL_code_LDQS = (stat_value) & (0x0000001F);

  *pREG_DMC0_DDR_LANE0_CTL1 &= ~(BITM_DMC_DDR_LANE0_CTL1_BYPCODE|BITM_DMC_DDR_LANE0_CTL1_BYPDELCHAINEN);
  /* If write leveling is enabled */
  if((config.ulDDR_MREMR1 & BITM_DMC_MR1_WL)>>BITP_DMC_MR1_WL == true)
  {
  *pREG_DMC0_DDR_LANE0_CTL1 |= (((WL_code_LDQS + Lane0_DQS_Delay)<<BITP_DMC_DDR_LANE0_CTL1_BYPCODE) & BITM_DMC_DDR_LANE0_CTL1_BYPCODE)|BITM_DMC_DDR_LANE0_CTL1_BYPDELCHAINEN;
  }
  else
  {
  *pREG_DMC0_DDR_LANE0_CTL1 |= (((DQS_Default_Delay + Lane0_DQS_Delay)<<BITP_DMC_DDR_LANE0_CTL1_BYPCODE) & BITM_DMC_DDR_LANE0_CTL1_BYPCODE)|BITM_DMC_DDR_LANE0_CTL1_BYPDELCHAINEN;
  }
  dmcdelay(2500u);

  /* For UDQS */
  *pREG_DMC0_DDR_LANE1_CTL1 = (*pREG_DMC0_DDR_LANE1_CTL1) | (0x000000D0);
  dmcdelay(2500u);
  *pREG_DMC0_DDR_ROOT_CTL=0x00800000;
  dmcdelay(2500u);
  *pREG_DMC0_DDR_ROOT_CTL =0x0;
  stat_value = (*pREG_DMC0_DDR_SCRATCH_STAT1 & (0xFFFF0000))>>16;
  WL_code_UDQS = (stat_value) & (0x0000001F);

  *pREG_DMC0_DDR_LANE1_CTL1 &= ~(BITM_DMC_DDR_LANE0_CTL1_BYPCODE|BITM_DMC_DDR_LANE0_CTL1_BYPDELCHAINEN);
  /* If write leveling is enabled */
  if((config.ulDDR_MREMR1 & BITM_DMC_MR1_WL)>>BITP_DMC_MR1_WL == true)
  {
  *pREG_DMC0_DDR_LANE1_CTL1 |= (((WL_code_UDQS + Lane1_DQS_Delay)<<BITP_DMC_DDR_LANE0_CTL1_BYPCODE) & BITM_DMC_DDR_LANE0_CTL1_BYPCODE)|BITM_DMC_DDR_LANE0_CTL1_BYPDELCHAINEN;
  }
  else
  {
  *pREG_DMC0_DDR_LANE1_CTL1 |= (((DQS_Default_Delay + Lane1_DQS_Delay)<<BITP_DMC_DDR_LANE0_CTL1_BYPCODE) & BITM_DMC_DDR_LANE0_CTL1_BYPCODE)|BITM_DMC_DDR_LANE0_CTL1_BYPDELCHAINEN;
  }
  dmcdelay(2500u);

#endif

  return ADI_DMC_SUCCESS;
}

__attribute__((always_inline)) inline void DMC_Config(void)
{
    /* Initialize DMC PHY registers */
    adi_dmc_phy_calibration();

    /* Initialize DMC Controller */
    adi_dmc_ctrl_init();
}

__attribute__((always_inline)) inline void adi_dmc_lane_reset(bool reset)
{
  if (reset) {
    *pREG_DMC0_DDR_LANE0_CTL0 |= BITM_DMC_DDR_LANE0_CTL0_CB_RSTDLL;
    *pREG_DMC0_DDR_LANE1_CTL0 |= BITM_DMC_DDR_LANE1_CTL0_CB_RSTDLL;
  } else {
    *pREG_DMC0_DDR_LANE0_CTL0 &= ~BITM_DMC_DDR_LANE0_CTL0_CB_RSTDLL;
    *pREG_DMC0_DDR_LANE1_CTL0 &= ~BITM_DMC_DDR_LANE1_CTL0_CB_RSTDLL;
  }
  dmcdelay(9000u);
}

__attribute__((always_inline)) inline adidelay(unsigned int num1)
{
    while(num1--);
}

__attribute__((always_inline)) inline adi_config_third_pll(uint32_t Msel, uint32_t Dsel)
{
  Msel--;
  Dsel--;

  *pREG_MISC_REG10_tst &= 0xFFFE0000;
  adidelay(4000);
  //update MSEL [10:4]
  *pREG_MISC_REG10_tst |= ((Msel<< BITP_REG10_MSEL3)& BITM_REG10_MSEL3); //0x500; //msel 80
  *pREG_MISC_REG10_tst |= 0x2;
  adidelay(100000);
  *pREG_MISC_REG10_tst |= 0x1;
  *pREG_MISC_REG10_tst |= 0x800;

  *pREG_MISC_REG10_tst &= 0xFFFFF7F8;
  adidelay(4000);
  *pREG_MISC_REG10_tst |= ((Dsel<< BITP_REG10_DSEL3)& BITM_REG10_DSEL3);//0x1000; //desl-5
  *pREG_MISC_REG10_tst |= 0x4;
  adidelay(100000);
  *pREG_MISC_REG10_tst |= 0x1;
  *pREG_MISC_REG10_tst |= 0x800;
}

#endif /* CONFIG_DMC0 */

