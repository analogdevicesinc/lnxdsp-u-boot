// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * (C) Copyright 2022 - Analog Devices, Inc.
 *
 * Written and/or maintained by Timesys Corporation
 *
 * Contact: Nathan Barrett-Morrison <nathan.morrison@timesys.com>
 * Contact: Greg Malysa <greg.malysa@timesys.com>
 */

#ifndef DMCINIT_H_
#define DMCINIT_H_

#include <linux/types.h>
#include <asm/io.h>

#ifdef MEM_MT41K512M16HA
    #include "mt41k512m16ha.h"
#elif defined(MEM_MT41K128M16JT)
    #include "mt41k128m16jt.h"
#elif defined(MEM_MT47H128M16RT)
    #include "mt47h128m16rt.h"
#elif defined(MEM_IS43TR16512BL)
    #include "is43tr16512bl.h"
#else
    #error "No DDR part name is defined for this board."
#endif

#define BITP_DMC_CFG_EXTBANK                 12            /* External Banks */
#define BITP_DMC_CFG_SDRSIZE                  8            /* SDRAM Size */
#define BITP_DMC_CFG_SDRWID                   4            /* SDRAM Width */
#define BITP_DMC_CFG_IFWID                    0            /* Interface Width */

#define BITM_DMC_CFG_EXTBANK                 0x0000F000  /* External Banks */
#define ENUM_DMC_CFG_EXTBANK1                0x00000000  /* EXTBANK: 1 External Bank */
#define BITM_DMC_CFG_SDRSIZE                 0x00000F00  /* SDRAM Size */
#define ENUM_DMC_CFG_SDRSIZE64               0x00000000  /* SDRSIZE: 64M Bit SDRAM (LPDDR Only) */
#define ENUM_DMC_CFG_SDRSIZE128              0x00000100  /* SDRSIZE: 128M Bit SDRAM (LPDDR Only) */
#define ENUM_DMC_CFG_SDRSIZE256              0x00000200  /* SDRSIZE: 256M Bit SDRAM */
#define ENUM_DMC_CFG_SDRSIZE512              0x00000300  /* SDRSIZE: 512M Bit SDRAM */
#define ENUM_DMC_CFG_SDRSIZE1G               0x00000400  /* SDRSIZE: 1G Bit SDRAM */
#define ENUM_DMC_CFG_SDRSIZE2G               0x00000500  /* SDRSIZE: 2G Bit SDRAM */
#define ENUM_DMC_CFG_SDRSIZE4G               0x00000600  /* SDRSIZE: 4G Bit SDRAM */
#define ENUM_DMC_CFG_SDRSIZE8G               0x00000700  /* SDRSIZE: 8G Bit SDRAM */
#define BITM_DMC_CFG_SDRWID                  0x000000F0  /* SDRAM Width */
#define ENUM_DMC_CFG_SDRWID16                0x00000020  /* SDRWID: 16-Bit Wide SDRAM */
#define BITM_DMC_CFG_IFWID                   0x0000000F  /* Interface Width */
#define ENUM_DMC_CFG_IFWID16                 0x00000002  /* IFWID: 16-Bit Wide Interface */

#define BITM_DMC_CTL_DDR3EN                  0x00000001
#define BITM_DMC_CTL_INIT                    0x00000004
#define BITP_DMC_STAT_INITDONE               2            /* Initialization Done */
#define BITM_DMC_STAT_INITDONE               0x00000004

#define BITP_DMC_CTL_AL_EN                   27
#define BITP_DMC_CTL_ZQCL                    25            /* ZQ Calibration Long */
#define BITP_DMC_CTL_ZQCS                    24            /* ZQ Calibration Short */
#define BITP_DMC_CTL_DLLCAL                  13            /* DLL Calibration Start */
#define BITP_DMC_CTL_PPREF                   12            /* Postpone Refresh */
#define BITP_DMC_CTL_RDTOWR                   9            /* Read-to-Write Cycle */
#define BITP_DMC_CTL_ADDRMODE                 8            /* Addressing (Page/Bank) Mode */
#define BITP_DMC_CTL_RESET                    7            /* Reset SDRAM */
#define BITP_DMC_CTL_PREC                     6            /* Precharge */
#define BITP_DMC_CTL_DPDREQ                   5            /* Deep Power Down Request */
#define BITP_DMC_CTL_PDREQ                    4            /* Power Down Request */
#define BITP_DMC_CTL_SRREQ                    3            /* Self Refresh Request */
#define BITP_DMC_CTL_INIT                     2            /* Initialize DRAM Start */
#define BITP_DMC_CTL_LPDDR                    1            /* Low Power DDR Mode */
#define BITP_DMC_CTL_DDR3EN                   0            /* DDR3 Mode */

#define DMC_PADCTL0_VALUE                    (BITM_DMC_PAD_CTL0_PUCALEN| BITM_DMC_PAD_CTL0_PDCALEN| BITM_DMC_PAD_CTL0_RTTCALEN)
#define DMC_PADCTL1_VALUE                    0

#ifdef CONFIG_TARGET_SC584_EZKIT
#define DMC_PADCTL2_VALUE                    0x0078283C
#elif CONFIG_TARGET_SC573_EZKIT
#define DMC_PADCTL2_VALUE                    0x00782828
#else
#define DMC_PADCTL2_VALUE                    0x00783C3C
#endif

#define DMC_CPHYCTL_VALUE                    0x0000001A
#define DMC0_DATA_CALIB_ADD                  0x80000000
#define DMC1_DATA_CALIB_ADD                  0xC0000000

#define ENUM_DMC_PHY_CTL4_DDRMODE_DDR3       0x00000000
#define ENUM_DMC_PHY_CTL4_DDRMODE_DDR2       0x00000001
#define ENUM_DMC_PHY_CTL4_DDRMODE_LPDDR      0x00000003

#define BITM_DMC_PAD_CTL0_CALSTART           0x10000000
#define BITM_DMC_PAD_CTL0_PUCALEN            0x20000000
#define BITM_DMC_PAD_CTL0_PDCALEN            0x40000000
#define BITM_DMC_PAD_CTL0_RTTCALEN           0x80000000
#define BITM_DMC_PHY_STAT0_DLLLOCK           0x80000000

#define BITP_DMC_MR1_QOFF                    12            /*  Output Buffer Enable */
#define BITP_DMC_MR1_TDQS                    11            /*  Termination Data Strobe */
#define BITP_DMC_MR1_RTT2                     9            /*  Rtt_nom */
#define BITP_DMC_MR1_WL                       7            /*  Write Leveling Enable. */
#define BITP_DMC_MR1_RTT1                     6            /*  Rtt_nom */
#define BITP_DMC_MR1_DIC1                     5            /*  Output Driver Impedance Control */
#define BITP_DMC_MR1_AL                       3            /*  Additive Latency */
#define BITP_DMC_MR1_RTT0                     2            /*  Rtt_nom */
#define BITP_DMC_MR1_DIC0                     1            /*  Output Driver Impedance control */
#define BITP_DMC_MR1_DLLEN                    0            /*  DLL Enable */

#define BITP_DMC_MR2_CWL                      3            /* CAS write Latency */

#define BITP_DMC_TR0_TMRD                    28            /* Timing Mode Register Delay */
#define BITP_DMC_TR0_TRC                     20            /* Timing Row Cycle */
#define BITP_DMC_TR0_TRAS                    12            /* Timing Row Active Time */
#define BITP_DMC_TR0_TRP                      8            /* Timing RAS Precharge. */
#define BITP_DMC_TR0_TWTR                     4            /* Timing Write to Read */
#define BITP_DMC_TR0_TRCD                     0            /* Timing RAS to CAS Delay */

#define BITP_DMC_TR1_TRRD                    28            /* Timing Read-Read Delay */
#define BITP_DMC_TR1_TRFC                    16            /* Timing Refresh-to-Command */
#define BITP_DMC_TR1_TREF                     0            /* Timing Refresh Interval */

#define BITP_DMC_TR2_TCKE                    20            /* Timing Clock Enable */
#define BITP_DMC_TR2_TXP                     16            /* Timing Exit Powerdown */
#define BITP_DMC_TR2_TWR                     12            /* Timing Write Recovery */
#define BITP_DMC_TR2_TRTP                     8            /* Timing Read-to-Precharge */
#define BITP_DMC_TR2_TFAW                     0            /* Timing Four-Activated-Window */

#define BITP_DMC_MR_PD                       12            /* Active Powerdown Mode */
#define BITP_DMC_MR_WRRECOV                   9            /* Write Recovery */
#define BITP_DMC_MR_DLLRST                    8            /* DLL Reset */
#define BITP_DMC_MR_CL                        4            /* CAS Latency */
#define BITP_DMC_MR_CL0                       2            /* CAS Latency */
#define BITP_DMC_MR_BLEN                      0            /* Burst Length */

#define BITP_DMC_DLLCTL_DATACYC               8            /*  Data Cycles */
#define BITP_DMC_DLLCTL_DLLCALRDCNT           0            /*  DLL Calibration RD Count */

#define BITM_DMC_DLLCTL_DATACYC               0x00000F00    /*  Data Cycles */
#define BITM_DMC_DLLCTL_DLLCALRDCNT           0x000000FF    /*  DLL Calibration RD Count */

#define BITP_DMC_STAT_PHYRDPHASE              20            /*  PHY Read Phase */

#define BITM_DMC_DDR_LANE0_CTL0_CB_RSTDAT     0x08000000    /*  Reset the Data Pads */
#define BITM_DMC_DDR_LANE1_CTL0_CB_RSTDAT     0x08000000    /*  Reset the Data Pads */
#define BITM_DMC_DDR_LANE0_CTL1_COMP_DCYCLE   0x00000002    /*  Compute Datacycle */
#define BITM_DMC_DDR_LANE1_CTL1_COMP_DCYCLE   0x00000002    /*  Compute Datacycle */
#define BITM_DMC_DDR_LANE1_CTL0_CB_RSTDLL     0x00000100    /*  Reset the Lane DLL */
#define BITM_DMC_DDR_LANE0_CTL0_CB_RSTDLL     0x00000100    /*  Reset the Lane DLL */
#define BITM_DMC_DDR_LANE0_CTL0_CB_RSTDAT     0x08000000    /*  Reset the Data Pads */
#define BITP_DMC_DDR_ROOT_CTL_PIPE_OFSTDCYCLE 10            /*  Pipeline offset for PHYC_DATACYCLE */
#define BITM_DMC_DDR_ROOT_CTL_SW_REFRESH      0x00002000    /*  Refresh Lane DLL Code */
#define BITM_DMC_DDR_CA_CTL_SW_REFRESH        0x00004000    /*  Refresh Lane DLL Code */

#define BITP_DMC_CTL_RL_DQS                   26            /*  RL_DQS */
#define BITM_DMC_CTL_RL_DQS                   0x04000000    /*  RL_DQS */
#define BITP_DMC_EMR3_MPR                     2             /*  Multi Purpose Read Enable (Read Leveling) */
#define BITM_DMC_EMR3_MPR                     0x00000004    /*  Multi Purpose Read Enable (Read Leveling) */
#define BITM_DMC_MR1_WL                       0x00000080    /*  Write Leveling Enable. */
#define BITM_DMC_STAT_PHYRDPHASE              0x00F00000    /*  PHY Read Phase */

#define BITP_DMC_DDR_LANE0_CTL1_BYPCODE       10
#define BITM_DMC_DDR_LANE0_CTL1_BYPCODE       0x00007C00
#define BITP_DMC_DDR_LANE0_CTL1_BYPDELCHAINEN 15 
#define BITM_DMC_DDR_LANE0_CTL1_BYPDELCHAINEN 0x00008000

#define DMC_ZQCTL0_VALUE                      0x00785A64
#define DMC_ZQCTL1_VALUE                      0
#define DMC_ZQCTL2_VALUE                      0x70000000

#define DMC_TRIG_CALIB                        0
#define DMC_OFSTDCYCLE                        2

#define BITP_DMC_CAL_PADCTL0_RTTCALEN        31            /*  RTT Calibration Enable */
#define BITP_DMC_CAL_PADCTL0_PDCALEN         30            /*  PULLDOWN Calibration Enable */
#define BITP_DMC_CAL_PADCTL0_PUCALEN         29            /*  PULLUP Calibration Enable */
#define BITP_DMC_CAL_PADCTL0_CALSTRT         28            /*  Start New Calibration ( Hardware Cleared) */
#define BITM_DMC_CAL_PADCTL0_RTTCALEN        0x80000000    /*  RTT Calibration Enable */
#define BITM_DMC_CAL_PADCTL0_PDCALEN         0x40000000    /*  PULLDOWN Calibration Enable */
#define BITM_DMC_CAL_PADCTL0_PUCALEN         0x20000000    /*  PULLUP Calibration Enable */
#define BITM_DMC_CAL_PADCTL0_CALSTRT         0x10000000    /*  Start New Calibration ( Hardware Cleared) */
#define ENUM_DMC_PHY_CTL4_DDR3               0x00000000    /*  DDRMODE: DDR3 Mode */
#define ENUM_DMC_PHY_CTL4_DDR2               0x00000001    /*  DDRMODE: DDR2 Mode */
#define ENUM_DMC_PHY_CTL4_LPDDR              0x00000003    /*  DDRMODE: LPDDR Mode */

#define BITP_DMC_DDR_ZQ_CTL0_IMPRTT          16            /*  Data/DQS ODT */
#define BITP_DMC_DDR_ZQ_CTL0_IMPWRDQ          8            /*  Data/DQS/DM/CLK Drive Strength */
#define BITP_DMC_DDR_ZQ_CTL0_IMPWRADD         0            /*  Address/Command Drive Strength */
#define BITM_DMC_DDR_ZQ_CTL0_IMPRTT          0x00FF0000    /* Data/DQS ODT */
#define BITM_DMC_DDR_ZQ_CTL0_IMPWRDQ         0x0000FF00    /* Data/DQS/DM/CLK Drive Strength */
#define BITM_DMC_DDR_ZQ_CTL0_IMPWRADD        0x000000FF    /* Address/Command Drive Strength */

#define BITM_DMC_DDR_ROOT_CTL_TRIG_RD_XFER_ALL   0x00200000    /* All Lane Read Status */

#define BITM_REG10_MSEL3            0x000007F0
#define BITP_REG10_MSEL3            4

#define BITM_REG10_DSEL3            0x0001F000
#define BITP_REG10_DSEL3            12

#ifdef MEM_DDR2
    #define DMC_MR0_VALUE \
        ((DMC_BL/4+1)<<BITP_DMC_MR_BLEN)| \
        (DMC_CL<<BITP_DMC_MR_CL)| \
        (DMC_WRRECOV<<BITP_DMC_MR_WRRECOV)

    #define DMC_MR1_VALUE \
        (DMC_MR1_AL<<BITP_DMC_MR1_AL|0x04) \

    #define DMC_MR2_VALUE 0
    #define DMC_MR3_VALUE 0

    #define DMC_CTL_VALUE \
        (DMC_RDTOWR<<BITP_DMC_CTL_RDTOWR)| \
        (1 << BITP_DMC_CTL_DLLCAL)| \
        (BITM_DMC_CTL_INIT)
#endif

#ifdef MEM_DDR3
    #define DMC_MR0_VALUE \
        (0<<BITP_DMC_MR_BLEN)| \
        (DMC_CL0<<BITP_DMC_MR_CL0)| \
        (DMC_CL123<<BITP_DMC_MR_CL)| \
        (DMC_WRRECOV<<BITP_DMC_MR_WRRECOV)| \
        (1<<BITP_DMC_MR_DLLRST)

    #define DMC_MR1_VALUE \
        (DMC_MR1_DLLEN<<BITP_DMC_MR1_DLLEN)| \
        (DMC_MR1_DIC0<<BITP_DMC_MR1_DIC0)| \
        (DMC_MR1_RTT0<<BITP_DMC_MR1_RTT0)| \
        (DMC_MR1_AL<<BITP_DMC_MR1_AL)| \
        (DMC_MR1_DIC1<<BITP_DMC_MR1_DIC1)| \
        (DMC_MR1_RTT1<<BITP_DMC_MR1_RTT1)| \
        (DMC_MR1_RTT2<<BITP_DMC_MR1_RTT2)| \
        (DMC_MR1_WL<<BITP_DMC_MR1_WL)| \
        (DMC_MR1_TDQS<<BITP_DMC_MR1_TDQS)| \
        (DMC_MR1_QOFF<<BITP_DMC_MR1_QOFF)

    #define DMC_MR2_VALUE \
        ((DMC_WL)<<BITP_DMC_MR2_CWL)

    #define DMC_MR3_VALUE \
        ((DMC_WL)<<BITP_DMC_MR2_CWL)

    #define DMC_CTL_VALUE \
        (DMC_RDTOWR<<BITP_DMC_CTL_RDTOWR)| \
        (BITM_DMC_CTL_INIT)| \
        (BITM_DMC_CTL_DDR3EN)| \
        (DMC_CTL_AL_EN<<BITP_DMC_CTL_AL_EN)
#endif


#define DMC_DLLCTL_VALUE \
    (DMC_DATACYC<<BITP_DMC_DLLCTL_DATACYC)| \
    (DMC_DLLCALRDCNT<<BITP_DMC_DLLCTL_DLLCALRDCNT)

#define DMC_CFG_VALUE \
    ENUM_DMC_CFG_IFWID16| \
    ENUM_DMC_CFG_SDRWID16| \
    SDR_CHIP_SIZE| \
    ENUM_DMC_CFG_EXTBANK1

#define DMC_TR0_VALUE \
    (DMC_TRCD<<BITP_DMC_TR0_TRCD)| \
    (DMC_TWTR<<BITP_DMC_TR0_TWTR)| \
    (DMC_TRP<<BITP_DMC_TR0_TRP)| \
    (DMC_TRAS<<BITP_DMC_TR0_TRAS)| \
    (DMC_TRC<<BITP_DMC_TR0_TRC)| \
    (DMC_TMRD<<BITP_DMC_TR0_TMRD)

#define DMC_TR1_VALUE \
    (DMC_TREF<<BITP_DMC_TR1_TREF)| \
    (DMC_TRFC<<BITP_DMC_TR1_TRFC)| \
    (DMC_TRRD<<BITP_DMC_TR1_TRRD)

#define DMC_TR2_VALUE \
    (DMC_TFAW<<BITP_DMC_TR2_TFAW)| \
    (DMC_TRTP<<BITP_DMC_TR2_TRTP)| \
    (DMC_TWR<<BITP_DMC_TR2_TWR)| \
    (DMC_TXP<<BITP_DMC_TR2_TXP)| \
    (DMC_TCKE<<BITP_DMC_TR2_TCKE)


enum DDR_MODE {
    DDR3_MODE,
    DDR2_MODE,
    LPDDR_MODE,
};

typedef enum {
    CALIBRATION_LEGACY,
    CALIBRATION_METHOD1,
    CALIBRATION_METHOD2,
}CALIBRATION_MODE;

typedef struct{
    uint32_t REG_DMC_PHY_CTL4;
    uint32_t REG_DMC_PHY_CTL3;
    uint32_t REG_DMC_PHY_CTL2;
    uint32_t REG_DMC_PHY_CTL1;
    uint32_t REG_DMC_PHY_CTL0;
    uint32_t REG_DMC_CPHY_CTL;
    uint32_t REG_DMC_CAL_PADCTL2;
    uint32_t REG_DMC_CAL_PADCTL0;
    uint32_t REG_DMC_CFG;
    uint32_t REG_DMC_TR0;
    uint32_t REG_DMC_TR1;
    uint32_t REG_DMC_TR2;
    uint32_t REG_DMC_MR;
    uint32_t REG_DMC_EMR1;
    uint32_t REG_DMC_EMR2;
    uint32_t REG_DMC_DLLCTL;
    uint32_t REG_DMC_CTL;
    uint32_t REG_DMC_STAT;
    uint32_t VAL_DMC_DATA_CALIB_ADD;
    uint32_t REG_DMC_DT_CALIB_ADDR;
#if defined(CONFIG_SC59X) || defined(CONFIG_SC59X_64)
    uint32_t REG_DMC_EMR3;
    uint32_t REG_DMC_DDR_CA_CTL;
    uint32_t REG_DMC_DDR_ROOT_CTL;
    uint32_t REG_DMC_DDR_LANE0_CTL0;
    uint32_t REG_DMC_DDR_LANE0_CTL1;
    uint32_t REG_DMC_DDR_LANE1_CTL0;
    uint32_t REG_DMC_DDR_LANE1_CTL1;
    uint32_t REG_DMC_DDR_SCRATCH_2;
    uint32_t REG_DMC_DDR_SCRATCH_3;
    uint32_t REG_DMC_DDR_ZQ_CTL0;
    uint32_t REG_DMC_DDR_ZQ_CTL1;
    uint32_t REG_DMC_DDR_ZQ_CTL2;
    uint32_t REG_DMC_DDR_SCRATCH_6;
    uint32_t REG_DMC_DDR_SCRATCH_7;
    uint32_t REG_DMC_DDR_SCRATCH_STAT0;
    uint32_t REG_DMC_DDR_SCRATCH_STAT1;
#endif
}DMC_REGISTERS;

struct dmc_param {
    uint32_t dmc_no;
    DMC_REGISTERS * reg;
    uint32_t ddr_mode;
    uint32_t padctl2_value;
    uint32_t dmc_cphyctl_value;
    uint32_t dmc_cfg_value;
    uint32_t dmc_dllctl_value;
    uint32_t dmc_ctl_value;
    uint32_t dmc_tr0_value;
    uint32_t dmc_tr1_value;
    uint32_t dmc_tr2_value;
    uint32_t dmc_mr0_value;
    uint32_t dmc_mr1_value;
    uint32_t dmc_mr2_value;
    uint32_t dmc_mr3_value;
    uint32_t dmc_zqctl0_value;
    uint32_t dmc_zqctl1_value;
    uint32_t dmc_zqctl2_value;
    uint32_t dmc_data_calib_add_value;
    bool phy_init_required;
    bool anomaly_20000037_applicable;
    CALIBRATION_MODE calib_mode;
};

void DMC_Config(void);
void adi_dmc_lane_reset(bool reset, uint32_t dmc_no);

#if defined(CONFIG_SC59X_64)
#define cclk_dclk_ratio 1500
#elif defined(CONFIG_SC59X)
#define cclk_dclk_ratio 1250
#else
#define cclk_dclk_ratio 1000
#endif

__attribute__((always_inline)) static inline void dmcdelay(uint32_t delay)
{
  /* There is no zero-overhead loop on ARM, so assume each iteration takes
   * 4 processor cycles (based on examination of -O3 and -Ofast output).
   */
  uint32_t i, remainder;

  /* Convert DDR cycles to core clock cycles */
  uint32_t f = delay * cclk_dclk_ratio;
  delay = f+500;
  delay /= 1000;

  /* Round up to multiple of 4 */
  remainder = delay % 4;
  if (remainder != 0u) {
    delay += (4u - remainder);
  }

  for(i=0; i<delay; i+=4) {
    asm("nop");
  }
}

#if defined(CONFIG_SC59X) || defined(CONFIG_SC59X_64)
    #define REG_DMC_DDR_LANE0_CTL0(x) x == 0 ? REG_DMC0_DDR_LANE0_CTL0 : REG_DMC1_DDR_LANE0_CTL0
    #define REG_DMC_DDR_LANE1_CTL0(x) x == 0 ? REG_DMC0_DDR_LANE1_CTL0 : REG_DMC1_DDR_LANE1_CTL0
    #define REGISTERS_59X(x) \
        .REG_DMC_EMR3 = REG_DMC##x##_EMR3, \
        .REG_DMC_DDR_CA_CTL = REG_DMC##x##_DDR_CA_CTL, \
        .REG_DMC_DDR_ROOT_CTL = REG_DMC##x##_DDR_ROOT_CTL, \
        .REG_DMC_DDR_LANE0_CTL0 = REG_DMC##x##_DDR_LANE0_CTL0, \
        .REG_DMC_DDR_LANE0_CTL1 = REG_DMC##x##_DDR_LANE0_CTL1, \
        .REG_DMC_DDR_LANE1_CTL0 = REG_DMC##x##_DDR_LANE1_CTL0, \
        .REG_DMC_DDR_LANE1_CTL1 = REG_DMC##x##_DDR_LANE1_CTL1, \
        .REG_DMC_DDR_SCRATCH_2 = REG_DMC##x##_DDR_SCRATCH_2, \
        .REG_DMC_DDR_SCRATCH_3 = REG_DMC##x##_DDR_SCRATCH_3, \
        .REG_DMC_DDR_ZQ_CTL0 = REG_DMC##x##_DDR_ZQ_CTL0, \
        .REG_DMC_DDR_ZQ_CTL1 = REG_DMC##x##_DDR_ZQ_CTL1, \
        .REG_DMC_DDR_ZQ_CTL2 = REG_DMC##x##_DDR_ZQ_CTL2, \
        .REG_DMC_DDR_SCRATCH_6 = REG_DMC##x##_DDR_SCRATCH_6, \
        .REG_DMC_DDR_SCRATCH_7 = REG_DMC##x##_DDR_SCRATCH_7, \
        .REG_DMC_DDR_SCRATCH_STAT0 = REG_DMC##x##_DDR_SCRATCH_STAT0, \
        .REG_DMC_DDR_SCRATCH_STAT1 = REG_DMC##x##_DDR_SCRATCH_STAT1,
#else
    #define REGISTERS_59X(x)
#endif

#define DECLARE_DMC_REGISTERS(x) \
    static DMC_REGISTERS dmc_registers##x __attribute__ ((section (".text"))) = { \
        .REG_DMC_PHY_CTL4 = REG_DMC##x##_PHY_CTL4, \
        .REG_DMC_PHY_CTL3 = REG_DMC##x##_PHY_CTL3, \
        .REG_DMC_PHY_CTL2 = REG_DMC##x##_PHY_CTL2, \
        .REG_DMC_PHY_CTL1 = REG_DMC##x##_PHY_CTL1, \
        .REG_DMC_PHY_CTL0 = REG_DMC##x##_PHY_CTL0, \
        .REG_DMC_CPHY_CTL = REG_DMC##x##_CPHY_CTL, \
        .REG_DMC_CAL_PADCTL2 = REG_DMC##x##_CAL_PADCTL2, \
        .REG_DMC_CAL_PADCTL0 = REG_DMC##x##_CAL_PADCTL0, \
        .REG_DMC_CFG = REG_DMC##x##_CFG, \
        .REG_DMC_TR0 = REG_DMC##x##_TR0, \
        .REG_DMC_TR1 = REG_DMC##x##_TR1, \
        .REG_DMC_TR2 = REG_DMC##x##_TR2, \
        .REG_DMC_MR = REG_DMC##x##_MR, \
        .REG_DMC_EMR1 = REG_DMC##x##_EMR1, \
        .REG_DMC_EMR2 = REG_DMC##x##_EMR2, \
        .REG_DMC_DLLCTL = REG_DMC##x##_DLLCTL, \
        .REG_DMC_CTL = REG_DMC##x##_CTL, \
        .REG_DMC_STAT = REG_DMC##x##_STAT, \
        .VAL_DMC_DATA_CALIB_ADD = DMC##x##_DATA_CALIB_ADD, \
        .REG_DMC_DT_CALIB_ADDR = REG_DMC##x##_DT_CALIB_ADDR, \
        REGISTERS_59X(x) \
    };
#endif


