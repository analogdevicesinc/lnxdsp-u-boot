/*
 * Copyright (C) 2013 Analog Devices Inc.
 * Licensed under the GPL-2 or later.
 */

/* =========================
        TIMER
   ========================= */
#define TIMER_GROUP                 0x31018004
#define TIMER0_CONFIG               0x31018060

/* =========================
	L2 SRAM
   ========================= */
#define L2_SRAM_BASE                0x20080000


/* =========================
        TWI MMR base
   ========================= */
#define TWI0_CLKDIV                 0x31001400         /* TWI0 SCL Clock Divider Register */
#define TWI1_CLKDIV                 0x31001500         /* TWI1 SCL Clock Divider Register */
#define TWI2_CLKDIV                 0x31001600         /* TWI2 SCL Clock Divider Register */


/* =========================
        UART MMR base
   ========================= */
#define UART0_REVID                 0x31003000         /* UART0 Revision ID Register */
#define UART1_REVID                 0x31003400         /* UART1 Revision ID Register */
#define UART2_REVID                 0x31003800         /* UART2 Revision ID Register */

/* =========================
        SEC
   ========================= */

/* ----------------------------------------------------
       SEC Global Register Definitions
   ---------------------------------------------------- */
#define SEC0_GCTL                   0x31089000         /* SEC0 Global Control Register */

/* ----------------------------------------------------
       SEC Source Interface (SSI) Register Definitions
   ---------------------------------------------------- */
#define SEC0_SCTL0                  0x31089800         /* SEC0 Source Control Register n */

/* ----------------------------------------------------
       SEC Fault Management Interface (SFI) Register Definitions
   ---------------------------------------------------- */
#define SEC0_FCTL                   0x31089010         /* SEC0 Fault Control Register */


/* =========================
        WDOG0
   ========================= */
#define WDOG0_CTL                   0x31008000         /* Control Register */
#define WDOG0_CNT                   0x31008004         /* Count Register */
#define WDOG0_STAT                  0x31008008         /* Status Register */


/* =========================
        EMAC MMR base
   ========================= */
#define REG_EMAC0_MACCFG                0x31040000         /* EMAC0 MAC Configuration Register */
#define REG_EMAC1_MACCFG                0x31042000         /* EMAC1 MAC Configuration Register */


/* =========================
	MSI MMR base
   ========================= */
#define REG_MSI0_CTL                    0x31010000         /* MSI0 Contorl register */

/* ==================================================
        Serial Peripheral Interface MMR base
   ================================================== */
#define SPI0_REGBASE                  0x3102E000
#define SPI1_REGBASE                  0x3102F000
#define SPI2_REGBASE                  0x31030000


/* =========================
        RCU0
   ========================= */
#define RCU0_CTL                    0x3108C000         /* RCU0 Control Register */
#define RCU0_STAT                   0x3108C004         /* RCU0 Status Register */
#define RCU0_CRCTL                  0x3108C008         /* RCU0 Core Reset Control Register */
#define RCU0_CRSTAT                 0x3108C00C         /* RCU0 Core Reset Status Register */
#define RCU0_SIDIS                  0x3108C01C         /* RCU0 System Interface Disable Register */
#define RCU0_MSG_SET                0x3108C070         /* RCU0 Message Set Bits Register */


/* =========================
        SPU MMR base
   ========================= */
#define REG_SPU0_CTL                    0x3108B000         /* SPU0 Control Register */


/* =========================
        CGU0
   ========================= */
#define CGU0_CTL                    0x3108D000         /* CGU0 Control Register */
#define CGU0_PLLCTL                 0x3108D004         /* CGU0 PLL Control Register */
#define CGU0_STAT                   0x3108D008         /* CGU0 Status Register */
#define CGU0_DIV                    0x3108D00C         /* CGU0 Clocks Divisor Register */
#define CGU0_CLKOUTSEL              0x3108D010         /* CGU0 CLKOUT Select Register */
#define CGU0_DIVEX                  0x3108D040         /* CGU0 DIV Register Extension */

/* =========================
        CGU1
   ========================= */
#define CGU1_CTL                    0x3108E000         /* CGU1 Control Register */
#define CGU1_PLLCTL                 0x3108E004         /* CGU1 PLL Control Register */
#define CGU1_STAT                   0x3108E008         /* CGU1 Status Register */

#define BITP_CGU_DIV_OSEL                    22            //  OUTCLK Divisor
#define BITP_CGU_DIV_DSEL                    16            //  DCLK Divisor
#define BITP_CGU_DIV_S1SEL                   13            //  SCLK 1 Divisor
#define BITP_CGU_DIV_SYSSEL                   8            //  SYSCLK Divisor
#define BITP_CGU_DIV_S0SEL                    5            //  SCLK 0 Divisor
#define BITP_CGU_DIV_CSEL                     0            //  CCLK Divisor

#define BITP_CGU_CTL_MSEL                     8            //  Multiplier Select
#define BITP_CGU_CTL_DF                       0            //  Divide Frequency

/* ---------------------------------------------------
        STAT                                 Masks
   --------------------------------------------------- */
#define BITM_CGU_STAT_CLKSALGN               0x00000008
#define BITM_CGU_STAT_PLOCK                  0x00000004
#define BITM_CGU_STAT_PLLBP                  0x00000002
#define BITM_CGU_STAT_PLLEN                  0x00000001

/* =========================
        USB0
   ========================= */
#define REG_USB0_FADDR                  0x310C1000         /* USB0 Function Address Register */
#define REG_USB0_DMA_IRQ                0x310C1200         /* USB0 DMA Interrupt Register */
#define REG_USB0_VBUS_CTL               0x310C1380         /* USB0 VBUS Control Register */
#define REG_USB0_PHY_CTL                0x310C1394         /* USB0 PHY Control Register */
#define REG_USB0_PLL_OSC                0x310C1398         /* USB0 PLL and Oscillator Control Register */
#define REG_USB0_UTMI_CTL               0x310C139C

/* --------------------------------------------------------------------------
        USB_DMA_CTL                          Pos/Masks           Description
   -------------------------------------------------------------------------- */
#define BITP_USB_DMA_CTL_IE                   3            /* DMA Interrupt Enable */
#define BITP_USB_DMA_CTL_DIR                  1            /* DMA Transfer Direction */
#define BITP_USB_DMA_CTL_EN                   0            /* DMA Enable */


/* =========================
        PADS0
   ========================= */
#define REG_PADS0_PCFG0                 0x31004604        /* PADS0 Peripheral Configuration0 Register */
