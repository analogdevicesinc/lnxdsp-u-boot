/*
 * Copyright (C) 2014 Analog Devices Inc.
 * Licensed under the GPL-2 or later.
 */

#ifndef DMCINIT_H_
#define DMCINIT_H_

#include <linux/types.h>
#include <asm/io.h>

enum DDR_MODE {
	DDR3_MODE,
	DDR2_MODE,
	LPDDR_MODE,
};

struct dmc_param {
	uint32_t dmc_no;
	uint32_t ddr_mode;
	uint32_t cclk_dclk_ratio;
	uint32_t padctl2_value;
	uint32_t dmc_cphyctl_value;
	uint32_t dmc_cfg_value;
	uint32_t dmc_ctl_value;
	uint32_t dmc_tr0_value;
	uint32_t dmc_tr1_value;
	uint32_t dmc_tr2_value;
	uint32_t dmc_mr0_value;
	uint32_t dmc_mr1_value;
	uint32_t dmc_mr2_value;
	uint32_t dmc_data_calib_add_value;
	bool phy_init_required;
	bool anomaly_20000037_applicable;
};

#define BITP_DMC_CFG_EXTBANK                 12                               /* External Banks */
#define BITP_DMC_CFG_SDRSIZE                  8                               /* SDRAM Size */
#define BITP_DMC_CFG_SDRWID                   4                               /* SDRAM Width */
#define BITP_DMC_CFG_IFWID                    0                               /* Interface Width */

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

#define BITM_DMC_CTL_DDR3EN 0x00000001
#define BITM_DMC_CTL_INIT   0x00000004
#define BITP_DMC_STAT_INITDONE                2                               /* Initialization Done */
#define BITM_DMC_STAT_INITDONE                0x00000004

#define BITP_DMC_CTL_ZQCL                    25                               /* ZQ Calibration Long */
#define BITP_DMC_CTL_ZQCS                    24                               /* ZQ Calibration Short */
#define BITP_DMC_CTL_DLLCAL                  13                               /* DLL Calibration Start */
#define BITP_DMC_CTL_PPREF                   12                               /* Postpone Refresh */
#define BITP_DMC_CTL_RDTOWR                   9                               /* Read-to-Write Cycle */
#define BITP_DMC_CTL_ADDRMODE                 8                               /* Addressing (Page/Bank) Mode */
#define BITP_DMC_CTL_RESET                    7                               /* Reset SDRAM */
#define BITP_DMC_CTL_PREC                     6                               /* Precharge */
#define BITP_DMC_CTL_DPDREQ                   5                               /* Deep Power Down Request */
#define BITP_DMC_CTL_PDREQ                    4                               /* Power Down Request */
#define BITP_DMC_CTL_SRREQ                    3                               /* Self Refresh Request */
#define BITP_DMC_CTL_INIT                     2                               /* Initialize DRAM Start */
#define BITP_DMC_CTL_LPDDR                    1                               /* Low Power DDR Mode */
#define BITP_DMC_CTL_DDR3EN                   0                               /* DDR3 Mode */

#define DMC_DLLCALRDCNT         72
#define DMC_DATACYC                     9


#define DMC_PADCTL0_VALUE      (BITM_DMC_PAD_CTL0_PUCALEN| BITM_DMC_PAD_CTL0_PDCALEN| BITM_DMC_PAD_CTL0_RTTCALEN)
#define DMC_PADCTL1_VALUE      0

#ifdef CONFIG_TARGET_SC584_EZKIT
#define DMC_PADCTL2_VALUE      0x0078283C
#else
#define DMC_PADCTL2_VALUE      0x00783C3C
#endif

#define DMC_CPHYCTL_VALUE      0x0000001A
#define DMC0_DATA_CALIB_ADD    0x80000000
#define DMC1_DATA_CALIB_ADD    0xC0000000

#define ENUM_DMC_PHY_CTL4_DDRMODE_DDR3  0x00000000
#define ENUM_DMC_PHY_CTL4_DDRMODE_DDR2  0x00000001
#define ENUM_DMC_PHY_CTL4_DDRMODE_LPDDR 0x00000003

#define BITM_DMC_PAD_CTL0_CALSTART              0x10000000
#define BITM_DMC_PAD_CTL0_PUCALEN               0x20000000
#define BITM_DMC_PAD_CTL0_PDCALEN               0x40000000
#define BITM_DMC_PAD_CTL0_RTTCALEN              0x80000000

#define BITM_DMC_PHY_STAT0_DLLLOCK              0x80000000

#define BITP_DMC_MR1_AL                       3

#define BITP_DMC_MR2_CWL                      3                               /* CAS write Latency */


#define BITP_DMC_TR0_TMRD                    28                               /* Timing Mode Register Delay */
#define BITP_DMC_TR0_TRC                     20                               /* Timing Row Cycle */
#define BITP_DMC_TR0_TRAS                    12                               /* Timing Row Active Time */
#define BITP_DMC_TR0_TRP                      8                               /* Timing RAS Precharge. */
#define BITP_DMC_TR0_TWTR                     4                               /* Timing Write to Read */
#define BITP_DMC_TR0_TRCD                     0                               /* Timing RAS to CAS Delay */

#define BITP_DMC_TR1_TRRD                    28                               /* Timing Read-Read Delay */
#define BITP_DMC_TR1_TRFC                    16                               /* Timing Refresh-to-Command */
#define BITP_DMC_TR1_TREF                     0                               /* Timing Refresh Interval */

#define BITP_DMC_TR2_TCKE                    20                               /* Timing Clock Enable */
#define BITP_DMC_TR2_TXP                     16                               /* Timing Exit Powerdown */
#define BITP_DMC_TR2_TWR                     12                               /* Timing Write Recovery */
#define BITP_DMC_TR2_TRTP                     8                               /* Timing Read-to-Precharge */
#define BITP_DMC_TR2_TFAW                     0                               /* Timing Four-Activated-Window */

#define BITP_DMC_MR_PD                       12                               /* Active Powerdown Mode */
#define BITP_DMC_MR_WRRECOV                   9                               /* Write Recovery */
#define BITP_DMC_MR_DLLRST                    8                               /* DLL Reset */
#define BITP_DMC_MR_CL                        4                               /* CAS Latency */
#define BITP_DMC_MR_CL0                       2                               /* CAS Latency */
#define BITP_DMC_MR_BLEN                      0                               /* Burst Length */

#define BITP_DMC_DLLCTL_DATACYC               8                               /* Data Cycles */
#define BITP_DMC_DLLCTL_DLLCALRDCNT           0                               /* DLL Calibration RD Count */

#ifdef MEM_MT41K512M16HA
/* Default DDR3 part assumed: MT41K512M16HA-107, 8Gb part */
/* For DCLK= 450 MHz */
# define MEM_DDR3
# define DMC_TRCD                        7
# define DMC_TWTR                        4
# define DMC_TRP                         7
# define DMC_TRAS                        10
# define DMC_TRC                         16
# define DMC_TMRD                        4
# define DMC_TREF                        3510
# define DMC_TRFC                        158
# define DMC_TRRD                        6
# define DMC_TFAW                        16
# define DMC_TRTP                        4
# define DMC_TWR                         7
# define DMC_TXP                         3
# define DMC_TCKE                        3
# define DMC_CL0                         0
# define DMC_CL123                       3
# define DMC_WRRECOV                     (DMC_TWR-1)
# define DMC_AL                          0                    /* TRCD-1 */
# define DMC_WL                          6
# define DMC_BL8
# define DMC_RDTOWR                      2
# define SDR_CHIP_SIZE                   ENUM_DMC_CFG_SDRSIZE8G	

#elif defined(MEM_MT41K128M16JT)
/* Default DDR3 part assumed: MT41K128M16JT-125, 2Gb part */
/* For DCLK= 450 MHz */
# define MEM_DDR3
# define DMC_TRCD                        6
# define DMC_TWTR                        4
# define DMC_TRP                         6
# define DMC_TRAS                        17
# define DMC_TRC                         23
# define DMC_TMRD                        4
# define DMC_TREF                        3510
# define DMC_TRFC                        72
# define DMC_TRRD                        4
# define DMC_TFAW                        17
# define DMC_TRTP                        4
# define DMC_TWR                         7
# define DMC_TXP                         4
# define DMC_TCKE                        3
# define DMC_CL0                         0
# define DMC_CL123                       3
# define DMC_WRRECOV                     (DMC_TWR-1)
# define DMC_AL                          0			/* TRCD-1 */
# define DMC_WL                          6
# define DMC_BL8
# define DMC_RDTOWR                      2
# define SDR_CHIP_SIZE                   ENUM_DMC_CFG_SDRSIZE2G

#elif defined(MEM_MT47H128M16RT)
/* Default DDR2 part: MT47H128M16RT-25E XIT:C, 2 Gb part */
/* For DCLK= 400 MHz */
# define MEM_DDR2
# define DMC_TRCD			5
# define DMC_TWTR			3
# define DMC_TRP			5
# define DMC_TRAS			16
# define DMC_TRC			22
# define DMC_TMRD			2
# define DMC_TREF			3120
# define DMC_TRFC			78
# define DMC_TRRD			4
# define DMC_TFAW			18
# define DMC_TRTP			3
# define DMC_TWR			6
# define DMC_TXP			2
# define DMC_TCKE			3
# define DMC_CL				5
# define DMC_WRRECOV			(DMC_TWR-1)
# define DMC_AL				4			/* TRCD-1 */
# define DMC_BL				4
# define DMC_RDTOWR			2
# define SDR_CHIP_SIZE		ENUM_DMC_CFG_SDRSIZE2G

#else
# error "No DDR part name is defined for this board."
#endif

#ifdef MEM_DDR2
# define DMC_MR0_VALUE		(((DMC_BL/4+1)<<BITP_DMC_MR_BLEN)|(DMC_CL<<BITP_DMC_MR_CL)|(DMC_WRRECOV<<BITP_DMC_MR_WRRECOV))
# define DMC_MR1_VALUE		(DMC_AL<<BITP_DMC_MR1_AL|0x04)
# define DMC_MR2_VALUE		0
# define DMC_CTL_VALUE		((DMC_RDTOWR<<BITP_DMC_CTL_RDTOWR)|(1 << BITP_DMC_CTL_DLLCAL)|(BITM_DMC_CTL_INIT))
#endif

#ifdef MEM_DDR3
# define DMC_MR0_VALUE		((0<<BITP_DMC_MR_BLEN)|(DMC_CL0<<BITP_DMC_MR_CL0)|(DMC_CL123<<BITP_DMC_MR_CL)|(DMC_WRRECOV<<BITP_DMC_MR_WRRECOV)|(1<<8))
# define DMC_MR1_VALUE		((DMC_AL<<BITP_DMC_MR1_AL)|0x6)
# define DMC_MR2_VALUE		(((DMC_WL-5)<<BITP_DMC_MR2_CWL))
# define DMC_CTL_VALUE		((DMC_RDTOWR<<BITP_DMC_CTL_RDTOWR)|(BITM_DMC_CTL_INIT)|(BITM_DMC_CTL_DDR3EN))
#endif

#define DMC_DLLCTL_VALUE	(DMC_DATACYC<<BITP_DMC_DLLCTL_DATACYC)|(DMC_DLLCALRDCNT<<BITP_DMC_DLLCTL_DLLCALRDCNT)
#define DMC_CFG_VALUE 		(ENUM_DMC_CFG_IFWID16|ENUM_DMC_CFG_SDRWID16|SDR_CHIP_SIZE|ENUM_DMC_CFG_EXTBANK1)
#define DMC_TR0_VALUE		((DMC_TRCD<<BITP_DMC_TR0_TRCD)|(DMC_TWTR<<BITP_DMC_TR0_TWTR)|(DMC_TRP<<BITP_DMC_TR0_TRP)|(DMC_TRAS<<BITP_DMC_TR0_TRAS)|(DMC_TRC<<BITP_DMC_TR0_TRC)|(DMC_TMRD<<BITP_DMC_TR0_TMRD))
#define DMC_TR1_VALUE		((DMC_TREF<<BITP_DMC_TR1_TREF)|(DMC_TRFC<<BITP_DMC_TR1_TRFC)|(DMC_TRRD<<BITP_DMC_TR1_TRRD))
#define DMC_TR2_VALUE		((DMC_TFAW<<BITP_DMC_TR2_TFAW)|(DMC_TRTP<<BITP_DMC_TR2_TRTP)|(DMC_TWR<<BITP_DMC_TR2_TWR)|(DMC_TXP<<BITP_DMC_TR2_TXP)|(DMC_TCKE<<BITP_DMC_TR2_TCKE))

void DMC_Config(void);
#endif
