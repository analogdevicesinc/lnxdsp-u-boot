/*
 * Copyright (C) 2014 Analog Devices Inc.
 * Licensed under the GPL-2 or later.
 */

#ifndef CGUINIT_H_
#define CGUINIT_H_

#define CLKIN0              0
#define CLKIN1              1
#define CGU2_0_SYSCLK       2
#define CLKO0               3
#define	CLKO2               4
#define	CLKO3               5
#define	CLKO5               6
#define	CLKO7               7
#define	CLKO8               8
#define	USB_XCLK            9
#define	USBCLKBUF           10
#define	WOCLK0              11
#define	WOCLK1              12
#define	PMONCLK             13
#define	CLK1HZ              14
#define	MLBPLLCLK           15
#define	PCIPHYCLK           16
#define	TMU_CLK             17
#define	FROCLK              18

/*  PLL Multiplier and Divisor Selections (Required Value, Bit Position) */
#define MSEL(X)   		((X << BITP_CGU_CTL_MSEL) 				& BITM_CGU_CTL_MSEL) 						/* PLL Multiplier Select */
#define DF(X)	  		((X << BITP_CGU_CTL_DF) 				& BITM_CGU_CTL_DF )							/* Divide frequency[true or false] */
#define CSEL(X)   		((X  << BITP_CGU_DIV_CSEL)				& BITM_CGU_DIV_CSEL)						/* Core Clock Divisor Select */
#define SYSSEL(X) 		((X  << BITP_CGU_DIV_SYSSEL)  			& BITM_CGU_DIV_SYSSEL)						/* System Clock Divisor Select */
#define S0SEL(X)  		((X  << BITP_CGU_DIV_S0SEL)				& BITM_CGU_DIV_S0SEL)						/* SCLK0 Divisor Select  */
#define S1SEL(X)  		((X  << BITP_CGU_DIV_S1SEL)				& BITM_CGU_DIV_S1SEL)						/* SCLK1 Divisor Select  */
#define DSEL(X)   		((X  << BITP_CGU_DIV_DSEL)				& BITM_CGU_DIV_DSEL) 						/* DDR Clock Divisor Select */
#define OSEL(X)   		((X  << BITP_CGU_DIV_OSEL)				& BITM_CGU_DIV_OSEL) 						/* OUTCLK Divisor Select */
#define CLKOUTSEL(X)    ((X  << BITP_CGU_CLKOUTSEL_CLKOUTSEL)	& BITM_CGU_CLKOUTSEL_CLKOUTSEL) 			/* CLKOUT select	*/
#define USBCLKSEL(X)    ((X  << BITP_CGU_CLKOUTSEL_USBCLKSEL)	& BITM_CGU_CLKOUTSEL_USBCLKSEL) 			/* USBCLKSEL select */
#define S1SELEX(X)      ((X  << BITP_CGU_DIVEX_S1SELEX)         & BITM_CGU_DIVEX_S1SELEX)

#define DIV_MASK		(BITM_CGU_DIV_CSEL | BITM_CGU_DIV_S0SEL | BITM_CGU_DIV_SYSSEL |BITM_CGU_DIV_S1SEL | BITM_CGU_DIV_DSEL | BITM_CGU_DIV_OSEL)
#define CTL_MASK		(BITM_CGU_CTL_MSEL | BITM_CGU_CTL_DF)


#define BITM_ROM_SYSCTRL_CGU_DIV			0x0001
#define BITM_ROM_SYSCTRL_CGU_AUTO_DIS			0x0002
#define BITM_ROM_SYSCTRL_CGU_CTL			0x0004
#define BITM_CDU_STAT_CLKO8				0x00000100

struct CGU_Settings
{
	uint32_t ctl_MSEL:7;
	uint32_t ctl_DF:1;
	uint32_t div_CSEL:5;
	uint32_t div_SYSSEL:5;
	uint32_t div_S0SEL:3;
	uint32_t div_S1SEL:3;
	uint32_t div_DSEL:5;
	uint32_t div_OSEL:7;
	uint32_t divex_S1SELEX:8;
};


struct _CDU_MAPPINGS
{
	uint32_t cfg0_SEL:2;
	uint32_t cfg0_EN:1;
	uint32_t cfg1_SEL:2;
	uint32_t cfg1_EN:1;
	uint32_t cfg2_SEL:2;
	uint32_t cfg2_EN:1;
	uint32_t cfg3_SEL:2;
	uint32_t cfg3_EN:1;
	uint32_t cfg4_SEL:2;
	uint32_t cfg4_EN:1;
	uint32_t cfg5_SEL:2;
	uint32_t cfg5_EN:1;
	uint32_t cfg6_SEL:2;
	uint32_t cfg6_EN:1;
	uint32_t cfg7_SEL:2;
	uint32_t cfg7_EN:1;
	uint32_t cfg8_SEL:2;
	uint32_t cfg8_EN:1;
	uint32_t cfg9_SEL:2;
	uint32_t cfg9_EN:1;
	uint32_t reserved0:5; /* 32 bits */
};


struct _CGU_CLOCKDIVIDERS
{
	uint32_t ctl_MSEL:7;
	uint32_t ctl_DF:1;
	uint32_t div_CSEL:5;
	uint32_t div_SYSSEL:5;
	uint32_t div_S0SEL:3;
	uint32_t div_S1SEL:3;
	uint32_t div_DSEL:5;
	uint32_t div_OSEL:7;
	uint32_t reserved0:28; /* 64 bits */
};

/* =========================
        CGU
   ========================= */
#define _ADI_MSK( mask, type ) (mask) /* Make a bitmask */

#define BITM_CGU_CTL_LOCK                    (_ADI_MSK(0x80000000,uint32_t))  /* Lock */
#define BITM_CGU_CTL_WFI                     (_ADI_MSK(0x40000000,uint32_t))  /* Wait For Idle */

#define BITM_CGU_CTL_MSEL                    (_ADI_MSK(0x00007F00,uint32_t))  /* Multiplier Select */
#define ENUM_CGU_CTL_MSEL1TO127              (_ADI_MSK(0x00000000,uint32_t))  /* MSEL: MSEL = 1 to 127 */
#define BITM_CGU_CTL_DF                      (_ADI_MSK(0x00000001,uint32_t))  /* Divide Frequency */
#define BITM_CGU_CTL_S1SELEXEN               (_ADI_MSK(0x00020000,uint32_t))  /*  SCLK1 Extension Divider Enable */


#define BITM_CGU_DIV_LOCK                    (_ADI_MSK(0x80000000,uint32_t))  /* Lock */
#define BITM_CGU_DIV_UPDT                    (_ADI_MSK(0x40000000,uint32_t))  /* Update Clock Divisors */
#define BITM_CGU_DIV_ALGN                    (_ADI_MSK(0x20000000,uint32_t))  /* Align */

#define BITM_CGU_DIV_OSEL                    (_ADI_MSK(0x1FC00000,uint32_t))  /* OUTCLK Divisor */
#define ENUM_CGU_DIV_OSEL1TO127              (_ADI_MSK(0x00000000,uint32_t))  /* OSEL: OSEL = 1 to 127 */

#define BITM_CGU_DIV_DSEL                    (_ADI_MSK(0x001F0000,uint32_t))  /* DCLK Divisor */
#define ENUM_CGU_DIV_DSEL1TO31               (_ADI_MSK(0x00000000,uint32_t))  /* DSEL: DSEL = 1 to 31 */

#define BITM_CGU_DIV_S1SEL                   (_ADI_MSK(0x0000E000,uint32_t))  /* SCLK 1 Divisor */
#define ENUM_CGU_DIV_S1SEL1TO7               (_ADI_MSK(0x00000000,uint32_t))  /* S1SEL: S1SEL = 1 to 7 */

#define BITM_CGU_DIV_SYSSEL                  (_ADI_MSK(0x00001F00,uint32_t))  /* SYSCLK Divisor */
#define ENUM_CGU_DIV_SYSSEL1TO31             (_ADI_MSK(0x00000000,uint32_t))  /* SYSSEL: SYSSEL = 1 to 31 */

#define BITM_CGU_DIV_S0SEL                   (_ADI_MSK(0x000000E0,uint32_t))  /* SCLK 0 Divisor */
#define ENUM_CGU_DIV_S0SEL1TO7               (_ADI_MSK(0x00000000,uint32_t))  /* S0SEL: S0SEL = 1 to 7 */

#define BITM_CGU_DIV_CSEL                    (_ADI_MSK(0x0000001F,uint32_t))  /* CCLK Divisor */
#define ENUM_CGU_DIV_CSEL1TO31               (_ADI_MSK(0x00000000,uint32_t))  /* CSEL: CSEL= 1 to 31 */

#define BITP_CGU_DIVEX_S1SELEX               16
#define BITM_CGU_DIVEX_S1SELEX               (_ADI_MSK(0x00FF0000, uint32_t))    /*  SCLK 1 Extension Divisor */

/* -------------------------------------------------------------------------------
        CGU_PLLCTL                           Pos/Masks                        Description
   ------------------------------------------------------------------------------- */
#define BITP_CGU_PLLCTL_LOCK                 31                               /* Lock */
#define BITP_CGU_PLLCTL_PLLEN                 3                               /* PLL Enable */
#define BITP_CGU_PLLCTL_PLLDIS                2                               /* PLL Disable */
#define BITP_CGU_PLLCTL_PLLBPCL               1                               /* PLL Bypass Clear */
#define BITP_CGU_PLLCTL_PLLBPST               0                               /* PLL Bypass Set */
#define BITM_CGU_PLLCTL_LOCK                 (_ADI_MSK(0x80000000,uint32_t))  /* Lock */
#define BITM_CGU_PLLCTL_PLLEN                (_ADI_MSK(0x00000008,uint32_t))  /* PLL Enable */
#define BITM_CGU_PLLCTL_PLLDIS               (_ADI_MSK(0x00000004,uint32_t))  /* PLL Disable */
#define BITM_CGU_PLLCTL_PLLBPCL              (_ADI_MSK(0x00000002,uint32_t))  /* PLL Bypass Clear */
#define BITM_CGU_PLLCTL_PLLBPST              (_ADI_MSK(0x00000001,uint32_t))  /* PLL Bypass Set */

/* -------------------------------------------------------------------------------
        CGU_CLKOUTSEL                        Pos/Masks                        Description
   ------------------------------------------------------------------------------- */
#define BITP_CGU_CLKOUTSEL_LOCK              31                               /* Lock */
#define BITP_CGU_CLKOUTSEL_USBCLKSEL         16                               /* USBCLK Select */
#define BITP_CGU_CLKOUTSEL_CLKOUTSEL          0                               /* CLKOUT Select */
#define BITM_CGU_CLKOUTSEL_LOCK              (_ADI_MSK(0x80000000,uint32_t))  /* Lock */
#define BITM_CGU_CLKOUTSEL_USBCLKSEL         (_ADI_MSK(0x003F0000,uint32_t))  /* USBCLK Select */
#define BITM_CGU_CLKOUTSEL_CLKOUTSEL         (_ADI_MSK(0x0000001F,uint32_t))  /* CLKOUT Select */

/* -------------------------------------------------------------------------------
        CGU_OSCWDCTL                         Pos/Masks                        Description
   ------------------------------------------------------------------------------- */
#define BITP_CGU_OSCWDCTL_LOCK               31                               /* Lock */
#define BITP_CGU_OSCWDCTL_FAULTPINDIS        23                               /* Fault Pin disabled */
#define BITP_CGU_OSCWDCTL_MONDIS             15                               /* Oscillator Watchdog Monitor functions disabled */
#define BITP_CGU_OSCWDCTL_FAULTEN            14                               /* Fault enabled */
#define BITP_CGU_OSCWDCTL_BOUEN              13                               /* Bad Oscillator Upper Frequency limit detection enabled */
#define BITP_CGU_OSCWDCTL_BOUF                8                               /* Bad Oscillator Upper Frequency limit */
#define BITP_CGU_OSCWDCTL_CNGEN               7                               /* Clock not Good enabled */
#define BITP_CGU_OSCWDCTL_HODEN               6                               /* Harmonic Oscillation Detection enabled */
#define BITP_CGU_OSCWDCTL_HODF                0                               /* Watchdog lower frequency limit */
#define BITM_CGU_OSCWDCTL_LOCK               (_ADI_MSK(0x80000000,uint32_t))  /* Lock */
#define BITM_CGU_OSCWDCTL_FAULTPINDIS        (_ADI_MSK(0x00800000,uint32_t))  /* Fault Pin disabled */
#define BITM_CGU_OSCWDCTL_MONDIS             (_ADI_MSK(0x00008000,uint32_t))  /* Oscillator Watchdog Monitor functions disabled */
#define BITM_CGU_OSCWDCTL_FAULTEN            (_ADI_MSK(0x00004000,uint32_t))  /* Fault enabled */
#define BITM_CGU_OSCWDCTL_BOUEN              (_ADI_MSK(0x00002000,uint32_t))  /* Bad Oscillator Upper Frequency limit detection enabled */
#define BITM_CGU_OSCWDCTL_BOUF               (_ADI_MSK(0x00001F00,uint32_t))  /* Bad Oscillator Upper Frequency limit */
#define BITM_CGU_OSCWDCTL_CNGEN              (_ADI_MSK(0x00000080,uint32_t))  /* Clock not Good enabled */
#define BITM_CGU_OSCWDCTL_HODEN              (_ADI_MSK(0x00000040,uint32_t))  /* Harmonic Oscillation Detection enabled */
#define BITM_CGU_OSCWDCTL_HODF               (_ADI_MSK(0x0000003F,uint32_t))  /* Watchdog lower frequency limit */

/* -------------------------------------------------------------------------------
        CGU_TSCTL                            Pos/Masks                        Description
   ------------------------------------------------------------------------------- */
#define BITP_CGU_TSCTL_LOCK                  31                               /* Lock */
#define BITP_CGU_TSCTL_TSDIV                  4                               /* Counter's Clock Divider */
#define BITP_CGU_TSCTL_LOAD                   1                               /* Load Counter */
#define BITP_CGU_TSCTL_EN                     0                               /* Counter Enable */
#define BITM_CGU_TSCTL_LOCK                  (_ADI_MSK(0x80000000,uint32_t))  /* Lock */
#define BITM_CGU_TSCTL_TSDIV                 (_ADI_MSK(0x000000F0,uint32_t))  /* Counter's Clock Divider */
#define BITM_CGU_TSCTL_LOAD                  (_ADI_MSK(0x00000002,uint32_t))  /* Load Counter */
#define BITM_CGU_TSCTL_EN                    (_ADI_MSK(0x00000001,uint32_t))  /* Counter Enable */

/* -------------------------------------------------------------------------------
        CGU_CCBF_DIS                         Pos/Masks                        Description
   ------------------------------------------------------------------------------- */
#define BITP_CGU_CCBF_DIS_LOCK               31                               /* Lock */
#define BITP_CGU_CCBF_DIS_CCBF1               1                               /* Core Clock Buffer 1 */
#define BITP_CGU_CCBF_DIS_CCBF0               0                               /* Core Clock Buffer 0 */
#define BITM_CGU_CCBF_DIS_LOCK               (_ADI_MSK(0x80000000,uint32_t))  /* Lock */
#define BITM_CGU_CCBF_DIS_CCBF1              (_ADI_MSK(0x00000002,uint32_t))  /* Core Clock Buffer 1 */
#define BITM_CGU_CCBF_DIS_CCBF0              (_ADI_MSK(0x00000001,uint32_t))  /* Core Clock Buffer 0 */

/* -------------------------------------------------------------------------------
        CGU_CCBF_STAT                        Pos/Masks                        Description
   ------------------------------------------------------------------------------- */
#define BITP_CGU_CCBF_STAT_CCBF1              1                               /* Core Clock Buffer 1 */
#define BITP_CGU_CCBF_STAT_CCBF0              0                               /* Core Clock Buffer 0 */
#define BITM_CGU_CCBF_STAT_CCBF1             (_ADI_MSK(0x00000002,uint32_t))  /* Core Clock Buffer 1 */
#define BITM_CGU_CCBF_STAT_CCBF0             (_ADI_MSK(0x00000001,uint32_t))  /* Core Clock Buffer 0 */

/* -------------------------------------------------------------------------------
        CGU_SCBF_DIS                         Pos/Masks                        Description
   ------------------------------------------------------------------------------- */
#define BITP_CGU_SCBF_DIS_LOCK               31                               /* Lock */
#define BITP_CGU_SCBF_DIS_OUTCLKBF            3                               /* Output Clock 1 Buffer */
#define BITP_CGU_SCBF_DIS_DCLKBF              2                               /* DClock 1 Buffer */
#define BITP_CGU_SCBF_DIS_SCLK1BF             1                               /* System Clock 1 Buffer */
#define BITP_CGU_SCBF_DIS_SCLK0BF             0                               /* System Clock 0 Buffer */
#define BITM_CGU_SCBF_DIS_LOCK               (_ADI_MSK(0x80000000,uint32_t))  /* Lock */
#define BITM_CGU_SCBF_DIS_OUTCLKBF           (_ADI_MSK(0x00000008,uint32_t))  /* Output Clock 1 Buffer */
#define BITM_CGU_SCBF_DIS_DCLKBF             (_ADI_MSK(0x00000004,uint32_t))  /* DClock 1 Buffer */
#define BITM_CGU_SCBF_DIS_SCLK1BF            (_ADI_MSK(0x00000002,uint32_t))  /* System Clock 1 Buffer */
#define BITM_CGU_SCBF_DIS_SCLK0BF            (_ADI_MSK(0x00000001,uint32_t))  /* System Clock 0 Buffer */

/* -------------------------------------------------------------------------------
        CGU_SCBF_STAT                        Pos/Masks                        Description
   ------------------------------------------------------------------------------- */
#define BITP_CGU_SCBF_STAT_OCLKBF             3                               /* Output Clock 1 Buffer */
#define BITP_CGU_SCBF_STAT_DCLKBF             2                               /* DClock 1 Buffer */
#define BITP_CGU_SCBF_STAT_SCLK1BF            1                               /* System Clock 1 Buffer */
#define BITP_CGU_SCBF_STAT_SCLK0BF            0                               /* System Clock 0 Buffer */
#define BITM_CGU_SCBF_STAT_OCLKBF            (_ADI_MSK(0x00000008,uint32_t))  /* Output Clock 1 Buffer */
#define BITM_CGU_SCBF_STAT_DCLKBF            (_ADI_MSK(0x00000004,uint32_t))  /* DClock 1 Buffer */
#define BITM_CGU_SCBF_STAT_SCLK1BF           (_ADI_MSK(0x00000002,uint32_t))  /* System Clock 1 Buffer */
#define BITM_CGU_SCBF_STAT_SCLK0BF           (_ADI_MSK(0x00000001,uint32_t))  /* System Clock 0 Buffer */

/* -------------------------------------------------------------------------------
        CGU_REVID                            Pos/Masks                        Description
   ------------------------------------------------------------------------------- */
#define BITP_CGU_REVID_MAJOR                  4                               /* Major Version */
#define BITP_CGU_REVID_REV                    0                               /* Incremental Version ID */
#define BITM_CGU_REVID_MAJOR                 (_ADI_MSK(0x000000F0,uint32_t))  /* Major Version */
#define BITM_CGU_REVID_REV                   (_ADI_MSK(0x0000000F,uint32_t))  /* Incremental Version ID */

/* ==================================================
        Clock Distribution Unit Registers
   ================================================== */

/* =========================
        CDU0
   ========================= */
#define REG_CDU0_CFG0                   0x3108F000         /*  CDU0 CDU Configuration */
#define REG_CDU0_CFG1                   0x3108F004         /*  CDU0 CDU Configuration */
#define REG_CDU0_CFG2                   0x3108F008         /*  CDU0 CDU Configuration */
#define REG_CDU0_CFG3                   0x3108F00C         /*  CDU0 CDU Configuration */
#define REG_CDU0_CFG4                   0x3108F010         /*  CDU0 CDU Configuration */
#define REG_CDU0_CFG5                   0x3108F014         /*  CDU0 CDU Configuration */
#define REG_CDU0_CFG6                   0x3108F018         /*  CDU0 CDU Configuration */
#define REG_CDU0_CFG7                   0x3108F01C         /*  CDU0 CDU Configuration */
#define REG_CDU0_CFG8                   0x3108F020         /*  CDU0 CDU Configuration */
#define REG_CDU0_CFG9                   0x3108F024         /*  CDU0 CDU Configuration */
#define REG_CDU0_CFG10                  0x3108F028         /*  CDU0 CDU Configuration */
#define REG_CDU0_CFG11                  0x3108F02C         /*  CDU0 CDU Configuration */
#define REG_CDU0_CFG12                  0x3108F030         /*  CDU0 CDU Configuration */
#define REG_CDU0_STAT                   0x3108F040         /* CDU0 CDU Status */
#define REG_CDU0_CLKINSEL               0x3108F044         /* CDU0 Reg 4 Description */
#define REG_CDU0_REVID                  0x3108F048         /* CDU0 CDU Revision ID */

#endif /* CGUINIT_H_ */
