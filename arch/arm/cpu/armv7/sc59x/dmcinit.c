/*
**
** Copyright (C) 2018-2020 Analog Devices Inc., All Rights Reserved.
**
*/

#include <config.h>
#include <common.h>
#include <asm/arch/sc59x.h>
#include "dmcinit.h"
#include <stdbool.h>

#define CONFIG_DMC0

#ifdef CONFIG_DMC0

#define DelayTrim 0
#define Bypasscode 0ul

#define DqsTrim 0
#define Dqscode 0ul

#define ClkTrim 0
#define Clkcode 0ul

#define TrigCalib 0ul
#define CLKDIR 0ul
#define OfstdCycle 2ul

#define PHY_CALIB_METHOD2 1

#define cclkdclk_ratio (1.25f)

#pragma GCC push_options
#pragma GCC optimize ("O0")

static inline void dmcdelay(uint32_t delay)
{
  /* There is no zero-overhead loop on ARM, so assume each iteration takes
   * 4 processor cycles (based on examination of -O3 and -Ofast output).
   */
  uint32_t i, remainder;

  /* Convert DDR cycles to core clock cycles */
  float f = (float)delay * cclkdclk_ratio;
  delay = (uint32_t)(f+0.5);

  /* Round up to multiple of 4 */
  remainder = delay % 4;
  if (remainder != 0u) {
    delay += (4u - remainder);
  }

  for(i=0; i<delay; i+=4) {
    asm("nop");
  }
}

#pragma GCC pop_options

/* DMC phy ZQ calibration routine
 * The first step in DMC initialization
 */
static void adi_dmc_phy_calibration(ADI_DMC_CONFIG *pConfig)
{
  /* Program the ODT and drive strength values */

#if PHY_CALIB_METHOD2

  /* Reset trigger */
  *pREG_DMC0_DDR_CA_CTL = 0x0ul;
  *pREG_DMC0_DDR_ROOT_CTL = 0x0ul;
  dmcdelay(5000u);

  *pREG_DMC0_DDR_SCRATCH_3 = 0x0ul;
  *pREG_DMC0_DDR_SCRATCH_2 = 0x0ul;
  dmcdelay(5000u);

  /* Writing internal registers in calib pad to zero. Calib mode set to 1 [26], trig M1 S1 write [16],
   * this enables usage of scratch registers instead of ZQCTL registers
   */
  *pREG_DMC0_DDR_ROOT_CTL = 0x04010000ul;
  dmcdelay(5000u);

  /* TRIGGER FOR M2-S2 WRITE     -> slave id 31:26  trig m2,s2 write bit 1->1
   * slave1 address is 4
   */
  *pREG_DMC0_DDR_CA_CTL = 0x10000002ul;
  dmcdelay(5000u);

  /* reset Trigger */
  *pREG_DMC0_DDR_CA_CTL = 0x0u;
  *pREG_DMC0_DDR_ROOT_CTL = 0x0u;

  /* write to slave 1, make the power down bit high */
  *pREG_DMC0_DDR_SCRATCH_3 = 0x1ul<<12;
  *pREG_DMC0_DDR_SCRATCH_2 = 0x0ul;
   dmcdelay(5000u);

  /* Calib mode set to 1 [26], trig M1 S1 write [16] */
  *pREG_DMC0_DDR_ROOT_CTL = 0x04010000ul;
  dmcdelay(5000u);

  *pREG_DMC0_DDR_CA_CTL = 0x10000002ul;
  dmcdelay(5000u);

  *pREG_DMC0_DDR_CA_CTL = 0x0ul;
  *pREG_DMC0_DDR_ROOT_CTL = 0x0ul;
  dmcdelay(5000u);

  /* for slave 0 */
  *pREG_DMC0_DDR_SCRATCH_2 = pConfig->ulDDR_ZQCTL0;
  dmcdelay(5000u);

  /* Calib mode set to 1 [26], trig M1 S1 write [16] */
  *pREG_DMC0_DDR_ROOT_CTL = 0x04010000ul;
  dmcdelay(5000u);

  *pREG_DMC0_DDR_CA_CTL = 0x0C000002ul;
  dmcdelay(5000u);

  *pREG_DMC0_DDR_CA_CTL = 0x0ul;
  *pREG_DMC0_DDR_ROOT_CTL = 0x0ul;
  dmcdelay(5000u);

  /* writing to slave 1
  calstrt is 0, but other programming is done */
  *pREG_DMC0_DDR_SCRATCH_3 = 0x0ul; /* make power down LOW again, to kickstart BIAS circuit */
  *pREG_DMC0_DDR_SCRATCH_2 = 0x70000000ul;
  dmcdelay(5000u);

  /* write to ca_ctl lane, calib mode set to 1 [26], trig M1 S1 write [16]*/
  *pREG_DMC0_DDR_ROOT_CTL = 0x04010000ul;
  dmcdelay(5000u);

  /*  copies data to lane controller slave
   *  TRIGGER FOR M2-S2 WRITE     -> slave id 31:26  trig m2,s2 write bit 1->1
   *  slave1 address is 4
   */
  *pREG_DMC0_DDR_CA_CTL = 0x10000002ul;
  dmcdelay(5000u);

  /* reset Trigger */
  *pREG_DMC0_DDR_CA_CTL = 0x0ul;
  *pREG_DMC0_DDR_ROOT_CTL = 0x0ul;

#else /* PHY_CALIB_METHOD2 */
  *pREG_DMC0_DDR_ZQ_CTL0 = pConfig->ulDDR_ZQCTL0;
  *pREG_DMC0_DDR_ZQ_CTL1 = pConfig->ulDDR_ZQCTL1;
  *pREG_DMC0_DDR_ZQ_CTL2 = pConfig->ulDDR_ZQCTL2;

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

#if DelayTrim
  /* DQS delay trim*/
  *pREG_DMC0_DDR_LANE0_CTL1 |= (((Bypasscode)<<BITP_DMC_DDR_LANE0_CTL1_BYPCODE) & BITM_DMC_DDR_LANE0_CTL1_BYPCODE)|BITM_DMC_DDR_LANE0_CTL1_BYPDELCHAINEN;
  *pREG_DMC0_DDR_LANE1_CTL1 |= (((Bypasscode)<<BITP_DMC_DDR_LANE1_CTL1_BYPCODE) & BITM_DMC_DDR_LANE1_CTL1_BYPCODE)|BITM_DMC_DDR_LANE1_CTL1_BYPDELCHAINEN;
#endif

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

static ADI_DMC_RESULT adi_dmc_ctrl_init(ADI_DMC_CONFIG *pConfig)
{

  uint32_t phyphase,rd_cnt,t_EMR1,t_EMR3, t_CTL,data_cyc;

  /* program timing registers*/
  *pREG_DMC0_CFG = (pConfig->ulDDR_DLLCTLCFG) & 0xFFFFul;
  *pREG_DMC0_TR0 = pConfig->ulDDR_TR0;
  *pREG_DMC0_TR1 = pConfig->ulDDR_TR1;
  *pREG_DMC0_TR2 = pConfig->ulDDR_TR2;

  /* program shadow registers */
  *pREG_DMC0_MR   = ((pConfig->ulDDR_MREMR1) >> 16ul) & 0xFFFFul;
  *pREG_DMC0_MR1  = (pConfig->ulDDR_MREMR1) & 0xFFFFul;
  *pREG_DMC0_MR2  = (pConfig->ulDDR_EMR2EMR3)>>16ul & 0xFFFFul;
  *pREG_DMC0_EMR3 = (pConfig->ulDDR_EMR2EMR3) & 0xFFFFul;

  /* program Dll timing register */
  *pREG_DMC0_DLLCTL = ((pConfig->ulDDR_DLLCTLCFG) >> 16ul) & 0xFFFFul;

  dmcdelay(2000u);

  *pREG_DMC0_DDR_CA_CTL |= BITM_DMC_DDR_CA_CTL_SW_REFRESH;

  dmcdelay(5u);

  *pREG_DMC0_DDR_ROOT_CTL |= BITM_DMC_DDR_ROOT_CTL_SW_REFRESH | (OfstdCycle << BITP_DMC_DDR_ROOT_CTL_PIPE_OFSTDCYCLE);

  /* Start DMC initialization */
  *pREG_DMC0_CTL = pConfig->ulDDR_CTL;

  dmcdelay(722000u);

  /* Add necessary delay depending on the configuration */
  t_EMR1 = (pConfig->ulDDR_MREMR1 & BITM_DMC_MR1_WL)>>BITP_DMC_MR1_WL;
  if(t_EMR1 != 0u)
  {
     dmcdelay(600u);
     while(((*pREG_DMC0_MR1 & BITM_DMC_MR1_WL)>>BITP_DMC_MR1_WL) != 0ul) { }
  }


  t_EMR3 = (pConfig->ulDDR_EMR2EMR3 & BITM_DMC_EMR3_MPR)>>BITP_DMC_EMR3_MPR;
  if(t_EMR3 != 0u)
  {
     dmcdelay(2000u);
     while(((*pREG_DMC0_EMR3 & BITM_DMC_EMR3_MPR)>>BITP_DMC_EMR3_MPR) != 0ul) { }
  }


  t_CTL = (pConfig->ulDDR_CTL & BITM_DMC_CTL_RL_DQS)>>BITP_DMC_CTL_RL_DQS;
  if(t_CTL != 0u)
  {
     dmcdelay(600u);
     while(((*pREG_DMC0_CTL & BITM_DMC_CTL_RL_DQS)>>BITP_DMC_CTL_RL_DQS) != 0ul)
     {
     }
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
  rd_cnt = ((pConfig->ulDDR_DLLCTLCFG) >> 16);
  rd_cnt <<= BITP_DMC_DLLCTL_DLLCALRDCNT;
  rd_cnt &= BITM_DMC_DLLCTL_DLLCALRDCNT;

  *pREG_DMC0_DLLCTL = rd_cnt|data_cyc;

  *pREG_DMC0_CTL = (pConfig->ulDDR_CTL & (~BITM_DMC_CTL_INIT) & (~BITM_DMC_CTL_RL_DQS));

  return ADI_DMC_SUCCESS;
}

void DMC_Config(void)
{
    static ADI_DMC_CONFIG config =
    {
            CFG0_REG_DDR_DLLCTLCFG,
            CFG0_REG_DMC_MR2MR3,
            CFG0_REG_DMC_CTL_VALUE,
            CFG0_REG_DMC_MRMR1,
            CFG0_REG_DMC_TR0_VALUE,
            CFG0_REG_DMC_TR1_VALUE,
            CFG0_REG_DMC_TR2_VALUE,
            0x00786464ul,
            0ul,
            0x70000000ul
    };

    /* Initialize DMC PHY registers */
    adi_dmc_phy_calibration(&config);

    /* Initialize DMC Controller */
    adi_dmc_ctrl_init(&config);

    /* Reason currently unknown: This must be run twice */
    adi_dmc_ctrl_init(&config);
}

#endif /* CONFIG_DMC0 */
