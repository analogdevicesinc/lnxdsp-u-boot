/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * (C) Copyright 2022 - Analog Devices, Inc.
 *
 * Written and/or maintained by Timesys Corporation
 *
 * Contact: Nathan Barrett-Morrison <nathan.morrison@timesys.com>
 * Contact: Greg Malysa <greg.malysa@timesys.com>
 */

#ifdef CONFIG_SC57X
#define L2_SRAM_BASE                0x20000000
#else
#define L2_SRAM_BASE                0x20080000
#endif

#define TWI0_CLKDIV                 0x31001400    // TWI0 SCL Clock Divider Register
#define TWI1_CLKDIV                 0x31001500    // TWI1 SCL Clock Divider Register
#define TWI2_CLKDIV                 0x31001600    // TWI2 SCL Clock Divider Register

#ifdef CONFIG_SC58X
#define RCU0_CTL                    0x3108B000    // RCU0 Control Register
#define RCU0_STAT                   0x3108B004    // RCU0 Status Register
#define RCU0_CRCTL                  0x3108B008    // RCU0 Core Reset Control Register
#define RCU0_CRSTAT                 0x3108B00C    // RCU0 Core Reset Status Register
#define RCU0_SIDIS                  0x3108B010    // RCU0 System Interface Disable Register
#define RCU0_MSG_SET                0x3108B064    // RCU0 Message Set Bits Register
#else
#define RCU0_CTL                    0x3108C000    // RCU0 Control Register
#define RCU0_STAT                   0x3108C004    // RCU0 Status Register
#define RCU0_CRCTL                  0x3108C008    // RCU0 Core Reset Control Register
#define RCU0_CRSTAT                 0x3108C00C    // RCU0 Core Reset Status Register
#define RCU0_SIDIS                  0x3108C01C    // RCU0 System Interface Disable Register
#define RCU0_MSG_SET                0x3108C070    // RCU0 Message Set Bits Register
#endif

#ifdef CONFIG_SC58X
#define REG_SPU0_CTL                0x3108C000    // SPU0 Control Register
#else
#define REG_SPU0_CTL                0x3108B000    // SPU0 Control Register
#define REG_SPU0_SECUREC0           0x3108B980    // SPU0 Secure Core Registers
#define REG_SPU0_SECUREC1           0x3108B984    // SPU0 Secure Core Registers
#define REG_SPU0_SECUREC2           0x3108B988    // SPU0 Secure Core Registers
#define REG_SPU0_SECURECn(i)        (REG_SPU0_SECUREC0 + (i * 4))
#define REG_SPU0_SECURECn_COUNT              3
#endif

#define CGU0_CTL                    0x3108D000    // CGU0 Control Register
#define CGU0_PLLCTL                 0x3108D004    // CGU0 PLL Control Register
#define CGU0_STAT                   0x3108D008    // CGU0 Status Register
#define CGU0_DIV                    0x3108D00C    // CGU0 Clocks Divisor Register
#define CGU0_CLKOUTSEL              0x3108D010    // CGU0 CLKOUT Select Register
#define CGU0_DIVEX                  0x3108D040    // CGU0 DIV Register Extension

#define CGU1_CTL                    0x3108E000    // CGU1 Control Register
#define CGU1_PLLCTL                 0x3108E004    // CGU1 PLL Control Register
#define CGU1_STAT                   0x3108E008    // CGU1 Status Register

#define BITP_CGU_DIV_OSEL                   22    // OUTCLK Divisor
#define BITP_CGU_DIV_DSEL                   16    // DCLK Divisor
#define BITP_CGU_DIV_S1SEL                  13    // SCLK 1 Divisor
#define BITP_CGU_DIV_SYSSEL                  8    // SYSCLK Divisor
#define BITP_CGU_DIV_S0SEL                   5    // SCLK 0 Divisor
#define BITP_CGU_DIV_CSEL                    0    // CCLK Divisor

#define BITP_CGU_CTL_MSEL                    8    // Multiplier Select
#define BITP_CGU_CTL_DF                      0    // Divide Frequency

#define BITM_CGU_STAT_CLKSALGN      0x00000008
#define BITM_CGU_STAT_PLOCK         0x00000004
#define BITM_CGU_STAT_PLLBP         0x00000002
#define BITM_CGU_STAT_PLLEN         0x00000001

#define REG_DMC0_PHY_CTL0           0x31071000    // DMC0 PHY Control 0 Register
#define REG_DMC0_PHY_CTL1           0x31071004    // DMC0 PHY Control 1 Register
#define REG_DMC0_PHY_CTL2           0x31071008    // DMC0 PHY Control 2 Register
#define REG_DMC0_PHY_CTL3           0x3107100c    // DMC0 PHY Control 3 Register
#define REG_DMC0_PHY_CTL4           0x31071010    // DMC0 PHY Control 4 Register
#define REG_DMC0_CAL_PADCTL0        0x31071034    // DMC0 CALIBRATION PAD CTL 0 Register
#define REG_DMC0_CAL_PADCTL2        0x3107103C    // DMC0 CALIBRATION PAD CTL2 Register

#define REG_DMC0_CTL                0x31070004    // DMC0 Control Register
#define REG_DMC0_STAT               0x31070008    // DMC0 Status Register
#define REG_DMC0_CFG                0x31070040    // DMC0 Configuration Register
#define REG_DMC0_TR0                0x31070044    // DMC0 Timing 0 Register
#define REG_DMC0_TR1                0x31070048    // DMC0 Timing 1 Register
#define REG_DMC0_TR2                0x3107004C    // DMC0 Timing 2 Register
#define REG_DMC0_MR                 0x31070060    // DMC0 Shadow MR Register (DDR3)
#define REG_DMC0_EMR1               0x31070064    // DMC0 Shadow EMR1 Register
#define REG_DMC0_EMR2               0x31070068    // DMC0 Shadow EMR2 Register
#define REG_DMC0_EMR3               0x3107006C
#define REG_DMC0_DLLCTL             0x31070080    // DMC0 DLL Control Register
#define REG_DMC0_DT_CALIB_ADDR      0x31070090    // DMC0 Data Calibration Address Register
#define REG_DMC0_CPHY_CTL           0x310701C0    // DMC0 Controller to PHY Interface Register

#define REG_DMC0_DDR_LANE0_CTL0     0x31071000    // DMC0 Data Lane 0 Control Register 0
#define REG_DMC0_DDR_LANE0_CTL1     0x31071004    // DMC0 Data Lane 0 Control Register 1
#define REG_DMC0_DDR_LANE1_CTL0     0x3107100C    // DMC0 Data Lane 1 Control Register 0
#define REG_DMC0_DDR_LANE1_CTL1     0x31071010    // DMC0 Data Lane 1 Control Register 1
#define REG_DMC0_DDR_ROOT_CTL       0x31071018    // DMC0 DDR ROOT Module Control Register
#define REG_DMC0_DDR_ZQ_CTL0        0x31071034    // DMC0 DDR Calibration Control Register 0
#define REG_DMC0_DDR_ZQ_CTL1        0x31071038    // DMC0 DDR Calibration Control Register 1
#define REG_DMC0_DDR_ZQ_CTL2        0x3107103C    // DMC0 DDR Calibration Control Register 2
#define REG_DMC0_DDR_CA_CTL         0x31071068    // DMC0 DDR CA Lane Control Register

#define REG_DMC0_DDR_SCRATCH_2      0x31071074
#define REG_DMC0_DDR_SCRATCH_3      0x31071078
#define REG_DMC0_DDR_SCRATCH_6      0x31071084
#define REG_DMC0_DDR_SCRATCH_7      0x31071088

#define REG_DMC0_DDR_SCRATCH_STAT0  0x3107107C
#define REG_DMC0_DDR_SCRATCH_STAT1  0x31071080

#define REG_DMC1_PHY_CTL0           0x31074000    // DMC1 PHY Control 0 Register
#define REG_DMC1_PHY_CTL1           0x31074004    // DMC1 PHY Control 1 Register
#define REG_DMC1_PHY_CTL2           0x31074008    // DMC1 PHY Control 2 Register
#define REG_DMC1_PHY_CTL3           0x3107400C    // DMC1 PHY Control 3 Register
#define REG_DMC1_PHY_CTL4           0x31074010    // DMC1 PHY Control 4 Register
#define REG_DMC1_CAL_PADCTL0        0x31074034    // DMC1 CALIBRATION PAD CTL 0 Register
#define REG_DMC1_CAL_PADCTL2        0x3107403C    // DMC1 CALIBRATION PAD CTL2 Register

#define REG_DMC1_CTL                0x31073004    // DMC1 Control Register
#define REG_DMC1_STAT               0x31073008    // DMC1 Status Register
#define REG_DMC1_CFG                0x31073040    // DMC1 Configuration Register
#define REG_DMC1_TR0                0x31073044    // DMC1 Timing 0 Register
#define REG_DMC1_TR1                0x31073048    // DMC1 Timing 1 Register
#define REG_DMC1_TR2                0x3107304C    // DMC1 Timing 2 Register
#define REG_DMC1_MR                 0x31073060    // DMC1 Shadow MR Register (DDR3)
#define REG_DMC1_EMR1               0x31073064    // DMC1 Shadow EMR1 Register
#define REG_DMC1_EMR2               0x31073068    // DMC1 Shadow EMR2 Register
#define REG_DMC1_EMR3               0x3107306C
#define REG_DMC1_DLLCTL             0x31073080    // DMC1 DLL Control Register
#define REG_DMC1_DT_CALIB_ADDR      0x31073090    // DMC1 Data Calibration Address Register
#define REG_DMC1_CPHY_CTL           0x310731C0    // DMC1 Controller to PHY Interface Register

#define REG_DMC1_DDR_LANE0_CTL0     0x31074000    // DMC1 Data Lane 0 Control Register 0
#define REG_DMC1_DDR_LANE0_CTL1     0x31074004    // DMC1 Data Lane 0 Control Register 1
#define REG_DMC1_DDR_LANE1_CTL0     0x3107400C    // DMC1 Data Lane 1 Control Register 0
#define REG_DMC1_DDR_LANE1_CTL1     0x31074010    // DMC1 Data Lane 1 Control Register 1
#define REG_DMC1_DDR_ROOT_CTL       0x31074018    // DMC1 DDR ROOT Module Control Register
#define REG_DMC1_DDR_ZQ_CTL0        0x31074034    // DMC1 DDR Calibration Control Register 0
#define REG_DMC1_DDR_ZQ_CTL1        0x31074038    // DMC1 DDR Calibration Control Register 1
#define REG_DMC1_DDR_ZQ_CTL2        0x3107403C    // DMC1 DDR Calibration Control Register 2
#define REG_DMC1_DDR_CA_CTL         0x31074068    // DMC1 DDR CA Lane Control Register

#define REG_DMC1_DDR_SCRATCH_2      0x31074074
#define REG_DMC1_DDR_SCRATCH_3      0x31074078
#define REG_DMC1_DDR_SCRATCH_6      0x31074084
#define REG_DMC1_DDR_SCRATCH_7      0x31074088

#define REG_DMC1_DDR_SCRATCH_STAT0  0x3107407C
#define REG_DMC1_DDR_SCRATCH_STAT1  0x31074080

#define REG_MISC_REG10_tst_addr     0x310A902C

#define REG_USB0_FADDR              0x310C1000    // USB0 Function Address Register
#define REG_USB0_DMA_IRQ            0x310C1200    // USB0 DMA Interrupt Register
#define REG_USB0_VBUS_CTL           0x310C1380    // USB0 VBUS Control Register
#define REG_USB0_PHY_CTL            0x310C1394    // USB0 PHY Control Register
#define REG_USB0_PLL_OSC            0x310C1398    // USB0 PLL and Oscillator Control Register
#define REG_USB0_UTMI_CTL           0x310C139C

#define BITP_USB_DMA_CTL_IE                  3    // DMA Interrupt Enable
#define BITP_USB_DMA_CTL_DIR                 1    // DMA Transfer Direction
#define BITP_USB_DMA_CTL_EN                  0    // DMA Enable


#if defined(CONFIG_SC59X) || defined(CONFIG_SC59X_64)
#define REG_PADS0_PCFG0             0x31004604    // PADS0 Peripheral Configuration0 Register
#else
#define REG_PADS0_PCFG0             0x31004404    // PADS0 Peripheral Configuration0 Register
#endif
