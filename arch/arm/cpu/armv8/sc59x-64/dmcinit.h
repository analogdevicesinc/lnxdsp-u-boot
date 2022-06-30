/*
** Copyright (C) 2018-2020 Analog Devices Inc., All Rights Reserved.
**
** This file was originally generated automatically based upon options
** selected in the DMC Initialization configuration dialog.
*/

/*!
* @file      adi_dmc.h
*
* @brief     DMC Configuration header file
*
* @details
*            DMC Configuration header file
*/

#ifndef DMCINIT_H_
#define DMCINIT_H_

#include <linux/types.h>
#include <asm/io.h>
#include "adsp598.h"

#define CONFIG_DMC0

#ifdef CONFIG_DMC0

#define BITP_DMC_TR2_TWR            12 /* Timing Write Recovery */

#define BITP_DMC_EMR1_WL            7
#define BITM_DMC_EMR1_WL            (1ul<<BITP_DMC_EMR1_WL)

#define BITP_DMC_CTL_AL_EN          27 /* AL_EN */
#define BITM_DMC_CTL_AL_EN          (1uL<<BITP_DMC_CTL_AL_EN)

#define BITM_REG10_MSEL3            0x000007F0
#define BITP_REG10_MSEL3            4

#define BITM_REG10_DSEL3            0x0001F000
#define BITP_REG10_DSEL3            12

#define pREG_PLL_CFG_tst            ((volatile uint32_t*)0x310046E8)

#define pREG_MISC_REG10_tst         ((volatile uint32_t*)0x310A902C)

#define pREG_DMC0_DDR_SCRATCH_STAT0 ((volatile uint32_t*)0x3107107C)
#define pREG_DMC0_DDR_SCRATCH_STAT1 ((volatile uint32_t*)0x31071080)

#define pREG_DMC0_DDR_SCRATCH_2     ((volatile uint32_t*)0x31071074)
#define pREG_DMC0_DDR_SCRATCH_3     ((volatile uint32_t*)0x31071078)
#define pREG_DMC0_DDR_SCRATCH_6     ((volatile uint32_t*)0x31071084)
#define pREG_DMC0_DDR_SCRATCH_7     ((volatile uint32_t*)0x31071088)

#define pREG_DMC0_DDR_LANE0_CTL0    ((volatile uint32_t*)0x31071000)
#define pREG_DMC0_DDR_LANE0_CTL1    ((volatile uint32_t*)0x31071004)
#define pREG_DMC0_DDR_LANE1_CTL0    ((volatile uint32_t*)0x3107100C)
#define pREG_DMC0_DDR_LANE1_CTL1    ((volatile uint32_t*)0x31071010)
#define pREG_DMC0_DDR_ROOT_CTL      ((volatile uint32_t*)0x31071018)
#define pREG_DMC0_DDR_ZQ_CTL0       ((volatile uint32_t*)0x31071034)
#define pREG_DMC0_DDR_ZQ_CTL1       ((volatile uint32_t*)0x31071038)
#define pREG_DMC0_DDR_ZQ_CTL2       ((volatile uint32_t*)0x3107103C)
#define pREG_DMC0_DDR_CA_CTL        ((volatile uint32_t*)0x31071068)

#define pREG_DMC0_CTL               ((volatile uint32_t*)0x31070004)
#define pREG_DMC0_STAT              ((volatile uint32_t*)0x31070008)
#define pREG_DMC0_CFG               ((volatile uint32_t*)0x31070040)
#define pREG_DMC0_TR0               ((volatile uint32_t*)0x31070044)
#define pREG_DMC0_TR1               ((volatile uint32_t*)0x31070048)
#define pREG_DMC0_TR2               ((volatile uint32_t*)0x3107004C)
#define pREG_DMC0_MSK               ((volatile uint32_t*)0x3107005C)
#define pREG_DMC0_MR                ((volatile uint32_t*)0x31070060)
#define pREG_DMC0_MR1               ((volatile uint32_t*)0x31070064)
#define pREG_DMC0_MR2               ((volatile uint32_t*)0x31070068)
#define pREG_DMC0_EMR3              ((volatile uint32_t*)0x3107006C)
#define pREG_DMC0_DLLCTL            ((volatile uint32_t*)0x31070080)

#define pREG_CDU0_CLKINSEL          ((volatile uint32_t*)0x3108F044)

#ifdef MEM_DDR2
# error "DDR2 Not yet implemented"
#endif

#define MEM_DDR3
#ifdef MEM_DDR3

/* DMC0 setup for the EV-21593-SOM and EV-SC594-SOM :
 * - uses a single 8GB IS43TR16512BL-125KBL DDR3 chip configured for
 *   800 MHz DCLK.
 * DMC0 setup for the EV-SC594-SOMS :
 * - uses a single 4GB IS43TR16256BL-093NBL DDR3 chip configured for
 *   800 MHz DCLK.
 */

/*****************************************DMC_CTL**********************************************/
/*! enable DDR3 controller */
#define CFG0_BIT_DMC_CTL_DDR3EN                     1ul
/*! Constant value, should not be altered*/
#define CFG0_BIT_DMC_CTL_RDTOWR                     5ul


#define CFG0_REG_DMC_CTL_VALUE      ((CFG0_BIT_DMC_CTL_DDR3EN<<BITP_DMC_CTL_DDR3EN)|(BITM_DMC_CTL_INIT)|(CFG0_BIT_DMC_CTL_RDTOWR<<BITP_DMC_CTL_RDTOWR)|(BITM_DMC_CTL_AL_EN))

/*****************************************DMC_CFG**********************************************/

/*! Value for the DMC_CFG (DMC Configuration) register. */
#define CFG0_REG_DMC_CFG_VALUE \
  (ENUM_DMC_CFG_IFWID16     | /* Interface Width - always 16-bit */ \
   ENUM_DMC_CFG_SDRWID16    | /* SDRAM Width - always 16-bit */ \
   CFG0_BIT_DMC_CFG_SDRSIZE | /* SDRAM Size */ \
   ENUM_DMC_CFG_EXTBANK1)     /* External Banks - always 1 bank */
/*****************************************DMC_DLLCTL*******************************************/
/*! DLL Calibration RD Count */
#define ADI_DMC_PARAM_DLLCOUNT                    240ul

/*! Data Cycles - ceil( 15ns/DDRclock in Hz) */
#define CFG0_REG_DMC_DATACYC                        12ul

/*! Value for the DMC_DLLCTL (DMC DLL Control) register. */
#define CFG0_REG_DMC_DLLCTL_VALUE \
  ((ADI_DMC_PARAM_DLLCOUNT<<BITP_DMC_DLLCTL_DLLCALRDCNT) | \
   (CFG0_REG_DMC_DATACYC<<BITP_DMC_DLLCTL_DATACYC))

/*****************************************DMC_TR0**********************************************/
/*! Trcd value in DDR clock cycles*/
#define CFG0_BIT_DMC_TR0_TRCD                       11ul
/*! Twtr value in DDR clock cycles*/
#define CFG0_BIT_DMC_TR0_TWTR                       6ul
/*! Trp value in DDR clock cycles*/
#define CFG0_BIT_DMC_TR0_TRP                        11ul
/*! Tras value in DDR clock cycles*/
#define CFG0_BIT_DMC_TR0_TRAS                       28ul
/*! Trc value in DDR clock cycles*/
#define CFG0_BIT_DMC_TR0_TRC                        39ul
/*! Tmrd value in DDR clock cycles*/
#define CFG0_BIT_DMC_TR0_TMRD                       4ul

#define CFG0_REG_DMC_TR0_VALUE      ((CFG0_BIT_DMC_TR0_TRCD<<BITP_DMC_TR0_TRCD)|(CFG0_BIT_DMC_TR0_TWTR<<BITP_DMC_TR0_TWTR)|(CFG0_BIT_DMC_TR0_TRP<<BITP_DMC_TR0_TRP)|(CFG0_BIT_DMC_TR0_TRAS<<BITP_DMC_TR0_TRAS)|(CFG0_BIT_DMC_TR0_TRC<<BITP_DMC_TR0_TRC)|(CFG0_BIT_DMC_TR0_TMRD<<BITP_DMC_TR0_TMRD))

/*****************************************DMC_TR1**********************************************/
/*! Tref value in DDR clock cycles*/
#define CFG0_BIT_DMC_TR1_TREF                        6240ul

/*! Trrd value in DDR clock cycles*/
#define CFG0_BIT_DMC_TR1_TRRD                        6ul

#define CFG0_REG_DMC_TR1_VALUE      ((CFG0_BIT_DMC_TR1_TREF<<BITP_DMC_TR1_TREF)|(CFG0_BIT_DMC_TR1_TRFC<<BITP_DMC_TR1_TRFC)|(CFG0_BIT_DMC_TR1_TRRD<<BITP_DMC_TR1_TRRD))

/*****************************************DMC_TR2**********************************************/

/*! Tfaw value in DDR clock cycles*/
#define CFG0_BIT_DMC_TR2_TFAW                          32ul
/*! Trtp value in DDR clock cycles*/
#define CFG0_BIT_DMC_TR2_TRTP                          6ul
/*! Twr value in DDR clock cycles*/
#define CFG0_BIT_DMC_TR2_TWR                           12ul
/*! Txp value in DDR clock cycles*/
#define CFG0_BIT_DMC_TR2_TXP                           5ul
/*! Tcke value in DDR clock cycles*/
#define CFG0_BIT_DMC_TR2_TCKE                          4ul

#define CFG0_REG_DMC_TR2_VALUE      ((CFG0_BIT_DMC_TR2_TFAW<<BITP_DMC_TR2_TFAW)|(CFG0_BIT_DMC_TR2_TRTP<<BITP_DMC_TR2_TRTP)|(CFG0_BIT_DMC_TR2_TWR<<BITP_DMC_TR2_TWR)|(CFG0_BIT_DMC_TR2_TXP<<BITP_DMC_TR2_TXP)|(CFG0_BIT_DMC_TR2_TCKE<<BITP_DMC_TR2_TCKE))

/****************************************DMC  DLLCTLCFG**********************************************/

/*! The DMC_CFG and DMC_DLLCTL values are combined for ulDDR_DLLCTLCFG field. */
#define CFG0_REG_DDR_DLLCTLCFG \
  (CFG0_REG_DMC_CFG_VALUE |    \
   (CFG0_REG_DMC_DLLCTL_VALUE<<16))

/****************************************DMC  MR0**********************************************/

/*! Constant value, should not be altered*/
#define CFG0_BIT_DMC_MR0_BLEN                           0ul
/*! CAS Read latency bit 0 value*/
#define CFG0_BIT_DMC_MR0_CL0                            0ul
/*! CAS Read latency [2:1] bits*/
#define CFG0_BIT_DMC_MR0_CL                             7ul
/*! Constant value, should not be altered*/
#define CFG0_BIT_DMC_MR0_DLLRST                         1ul
/*! Write recovery value*/
#define CFG0_BIT_DMC_MR0_WRRECOV                        6ul

#define CFG0_REG_DMC_MR0_VALUE             ((CFG0_BIT_DMC_MR0_BLEN<<BITP_DMC_MR_BLEN)|(CFG0_BIT_DMC_MR0_CL0<<BITP_DMC_MR_CL0)|(CFG0_BIT_DMC_MR0_CL<<BITP_DMC_MR_CL)|(CFG0_BIT_DMC_MR0_DLLRST<<BITP_DMC_MR_DLLRST)|(CFG0_BIT_DMC_MR0_WRRECOV<<BITP_DMC_MR_WRRECOV))

/****************************************DMC  MR1**********************************************/
/*! Constant value, should not be altered*/
#define CFG0_BIT_DMC_MR1_DLLEN                            0ul
/*! Bit 0 of the output DS*/
#define CFG0_BIT_DMC_MR1_DIC0                             0ul
/*! Bit 0 of the ODT*/
#define CFG0_BIT_DMC_MR1_RTT0                             0ul
/*! Additive latency setting*/
#define CFG0_BIT_DMC_MR1_AL                               0ul
/*! Bit 1 of the output DS*/
#define CFG0_BIT_DMC_MR1_DIC1                             0ul
/*! Bit 1 of the ODT*/
#define CFG0_BIT_DMC_MR1_RTT1                             1ul
/*! Bit 2 of the ODT*/
#define CFG0_BIT_DMC_MR1_RTT2                             0ul
#define CFG0_BIT_DMC_MR1_TDQS                             0ul
#define CFG0_BIT_DMC_MR1_QOFF                             0ul

#define CFG0_REG_DMC_MR1_VALUE ((CFG0_BIT_DMC_MR1_DLLEN<<BITP_DMC_MR1_DLLEN)|(CFG0_BIT_DMC_MR1_DIC0<<BITP_DMC_MR1_DIC0)|(CFG0_BIT_DMC_MR1_RTT0<<BITP_DMC_MR1_RTT0)|(CFG0_BIT_DMC_MR1_AL<<BITP_DMC_MR1_AL)|(CFG0_BIT_DMC_MR1_DIC1<<BITP_DMC_MR1_DIC1)|(CFG0_BIT_DMC_MR1_RTT1<<BITP_DMC_MR1_RTT1)|(CFG0_BIT_DMC_MR1_RTT2<<BITP_DMC_MR1_RTT2)|(CFG0_BIT_DMC_MR1_TDQS<<BITP_DMC_MR1_TDQS)|(CFG0_BIT_DMC_MR1_QOFF<<BITP_DMC_MR1_QOFF)|(BITM_DMC_EMR1_WL))

/**************************************** DMC  MR2MR3**********************************************/
/*! CAS write latency*/
#define CFG0_BIT_DMC_MR2_CWL                              3ul
#define CFG0_REG_DMC_MR2MR3      ((((CFG0_BIT_DMC_MR2_CWL<<BITP_DMC_MR2_CWL))<<16)|BITM_DMC_EMR3_MPR)
#define CFG0_REG_DMC_MRMR1        (CFG0_REG_DMC_MR1_VALUE|(CFG0_REG_DMC_MR0_VALUE<<16))

#if defined(MEM_ISSI_4Gb_DDR3_800MHZ)
#define CFG0_BIT_DMC_CFG_SDRSIZE                    (ENUM_DMC_CFG_SDRSIZE4G)
#define CFG0_BIT_DMC_TR1_TRFC                        208ul
#elif defined(MEM_ISSI_8Gb_DDR3_800MHZ)
#define CFG0_BIT_DMC_CFG_SDRSIZE                    (ENUM_DMC_CFG_SDRSIZE8G)
#define CFG0_BIT_DMC_TR1_TRFC                        280ul
#else
# error "No DDR part name is defined for this board."
#endif

#endif

typedef enum
{
  ADI_DMC_SUCCESS=0u,
  ADI_DMC_FAILURE
}ADI_DMC_RESULT;

/* structure which holds DMC register values */
typedef struct
{
  uint32_t ulDDR_DLLCTLCFG;               /*!< Content of DDR DLLCTL and DMC_CFG register     */
  uint32_t ulDDR_EMR2EMR3;                /*!< Content of the DDR EMR2 and EMR3 Register      */
  uint32_t ulDDR_CTL;                     /*!< Content of the DDR Control                   */
  uint32_t ulDDR_MREMR1;                  /*!< Content of the DDR MR and EMR1 Register      */
  uint32_t ulDDR_TR0;                     /*!< Content of the DDR Timing Register      */
  uint32_t ulDDR_TR1;                     /*!< Content of the DDR Timing Register      */
  uint32_t ulDDR_TR2;                     /*!< Content of the DDR Timing Register      */
  uint32_t ulDDR_ZQCTL0;                  /*!< Content of ZQCTL0 register */
  uint32_t ulDDR_ZQCTL1;                  /*!< Content of ZQCTL1 register */
  uint32_t ulDDR_ZQCTL2;                  /*!< Content of ZQCTL2 register */
}ADI_DMC_CONFIG;

void DMC_Config(void);
void adi_dmc_lane_reset(bool reset);

#endif /* CONFIG_DMC0 */

#endif /* ADI_DMC_H */
