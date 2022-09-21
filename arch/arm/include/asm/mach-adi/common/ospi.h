// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * (C) Copyright 2022 - Analog Devices, Inc.
 *
 * Written and/or maintained by Timesys Corporation
 *
 * Contact: Nathan Barrett-Morrison <nathan.morrison@timesys.com>
 * Contact: Greg Malysa <greg.malysa@timesys.com>
 */

#ifndef OSPI_H
#define OSPI_H

#define BITP_OSPI_CTL_IDLE                       31    // Idle
#define BITP_OSPI_CTL_OPCODEEN                   30    // Dual Byte Opcode Enable
#define BITP_OSPI_CTL_PIPEPHYEN                  25    // Pipeline PHY Mode Enable
#define BITP_OSPI_CTL_DTREN                      24    // Enable DTR Protocol
#define BITP_OSPI_CTL_BAUD                       19    // Master Mode Baud Rate Divisor (2 to 32)
#define BITP_OSPI_CTL_XIPIMM                     18    // Enter XIP Mode Immediately
#define BITP_OSPI_CTL_XIPRD                      17    // Enter XIP Mode on Next Read
#define BITP_OSPI_CTL_AHBADDREN                  16    // Enable AHB Address Remapping
#define BITP_OSPI_CTL_WRPROT                     14    // Write Protect Flash
#define BITP_OSPI_CTL_SSEL                       10    // Peripheral Chip Select Lines
#define BITP_OSPI_CTL_SSELDCODE                   9    // Peripheral Select Decode
#define BITP_OSPI_CTL_DACEN                       7    // Direct Access Controller Enable
#define BITP_OSPI_CTL_RSTCFG                      6    // Reset Configuration
#define BITP_OSPI_CTL_RST                         5    // Reset Pin
#define BITP_OSPI_CTL_HLD                         4    // Hold Pin
#define BITP_OSPI_CTL_PHYEN                       3    // PHY Mode Enable
#define BITP_OSPI_CTL_CPHA                        2    // Clock Phase
#define BITP_OSPI_CTL_CPOL                        1    // Clock Polarity
#define BITP_OSPI_CTL_EN                          0    // Enable
#define BITM_OSPI_CTL_IDLE               0x80000000    // Idle
#define BITM_OSPI_CTL_OPCODEEN           0x40000000    // Dual Byte Opcode Enable
#define BITM_OSPI_CTL_PIPEPHYEN          0x02000000    // Pipeline PHY Mode Enable
#define BITM_OSPI_CTL_DTREN              0x01000000    // Enable DTR Protocol
#define BITM_OSPI_CTL_BAUD               0x00780000    // Master Mode Baud Rate Divisor (2 to 32)
#define BITM_OSPI_CTL_XIPIMM             0x00040000    // Enter XIP Mode Immediately
#define BITM_OSPI_CTL_XIPRD              0x00020000    // Enter XIP Mode on Next Read
#define BITM_OSPI_CTL_AHBADDREN          0x00010000    // Enable AHB Address Remapping
#define BITM_OSPI_CTL_WRPROT             0x00004000    // Write Protect Flash
#define BITM_OSPI_CTL_SSEL               0x00003C00    // Peripheral Chip Select Lines
#define BITM_OSPI_CTL_SSELDCODE          0x00000200    // Peripheral Select Decode
#define BITM_OSPI_CTL_DACEN              0x00000080    // Direct Access Controller Enable
#define BITM_OSPI_CTL_RSTCFG             0x00000040    // Reset Configuration
#define BITM_OSPI_CTL_RST                0x00000020    // Reset Pin
#define BITM_OSPI_CTL_HLD                0x00000010    // Hold Pin
#define BITM_OSPI_CTL_PHYEN              0x00000008    // PHY Mode Enable
#define BITM_OSPI_CTL_CPHA               0x00000004    // Clock Phase
#define BITM_OSPI_CTL_CPOL               0x00000002    // Clock Polarity
#define BITM_OSPI_CTL_EN                 0x00000001    // Enable
#define ENUM_OSPI_CTL_XIPIMM_0           0x00000000    // XIPIMM: If XIP is enabled, it causes the controller to exit XIP mode on the next read instruction
#define ENUM_OSPI_CTL_XIPIMM_1           0x00040000    // XIPIMM: Operate the device in XIP mode immediately
#define ENUM_OSPI_CTL_XIPRD_0            0x00000000    // XIPRD: If XIP is enabled, it cause the controller to exit XIP mode on the next read instruction
#define ENUM_OSPI_CTL_XIPRD_1            0x00020000    // XIPRD: If XIP is disabled, this setting informs the controller that the device is ready to enter XIP on the next read instruction
#define ENUM_OSPI_CTL_SSELDCODE_0        0x00000000    // SSELDCODE: Only one of the 4 selects n_ss_out [3:0] is active
#define ENUM_OSPI_CTL_SSELDCODE_1        0x00000200    // SSELDCODE: Allows external 4:16 decode (n_ss_out = ss)
#define ENUM_OSPI_CTL_DACEN_0            0x00000000    // DACEN: Disable Direct Access Controller once current transfer of the data word is complete
#define ENUM_OSPI_CTL_DACEN_1            0x00000080    // DACEN: Enable Direct Access Controller
#define ENUM_OSPI_CTL_RSTCFG_0           0x00000000    // RSTCFG: Reset Feature on DQ3 Pin of the Device
#define ENUM_OSPI_CTL_RSTCFG_1           0x00000040    // RSTCFG: Reset Feature on Dedicated Pin of the Device (Controlling the 5th bit Influences the Output)
#define ENUM_OSPI_CTL_CPHA_0             0x00000000    // CPHA: SPI clock is active outside the word
#define ENUM_OSPI_CTL_CPHA_1             0x00000004    // CPHA: SPI clock is inactive outside the word
#define ENUM_OSPI_CTL_CPOL_0             0x00000000    // CPOL: OSPI clock is quiescent low
#define ENUM_OSPI_CTL_CPOL_1             0x00000002    // CPOL: OSPI clock is quiescent high
#define ENUM_OSPI_CTL_DIS                0x00000000    // EN: Disable OSPI Module
#define ENUM_OSPI_CTL_EN                 0x00000001    // EN: Enable OSPI Module

#define BITP_OSPI_DRICTL_DMYRD                   24    // Dummy Read Clock Cycles
#define BITP_OSPI_DRICTL_MODEEN                  20    // Mode Enable
#define BITP_OSPI_DRICTL_DATATRNSFR              16    // Data Transfer Type
#define BITP_OSPI_DRICTL_ADDRTRNSFR              12    // Address Transfer Type
#define BITP_OSPI_DRICTL_DDREN                   10    // DDR Enable
#define BITP_OSPI_DRICTL_INSTRTYP                 8    // Instruction Type
#define BITP_OSPI_DRICTL_OPCODERD                 0    // Read Opcode in Non-XIP Mode
#define BITM_OSPI_DRICTL_DMYRD           0x1F000000    // Dummy Read Clock Cycles
#define BITM_OSPI_DRICTL_MODEEN          0x00100000    // Mode Enable
#define BITM_OSPI_DRICTL_DATATRNSFR      0x00030000    // Data Transfer Type
#define BITM_OSPI_DRICTL_ADDRTRNSFR      0x00003000    // Address Transfer Type
#define BITM_OSPI_DRICTL_DDREN           0x00000400    // DDR Enable
#define BITM_OSPI_DRICTL_INSTRTYP        0x00000300    // Instruction Type
#define BITM_OSPI_DRICTL_OPCODERD        0x000000FF    // Read Opcode in Non-XIP Mode
#define ENUM_OSPI_DRICTL_DATATRNSFR_0    0x00000000    // DATATRNSFR: SIO Mode.
#define ENUM_OSPI_DRICTL_DATATRNSFR_1    0x00010000    // DATATRNSFR: Used for Dual Input/Output instructions.
#define ENUM_OSPI_DRICTL_DATATRNSFR_2    0x00020000    // DATATRNSFR: Used for Quad Input/Output instructions.
#define ENUM_OSPI_DRICTL_DATATRNSFR_3    0x00030000    // DATATRNSFR: Used for Quad Input/Output instructions.
#define ENUM_OSPI_DRICTL_ADDRTRNSFR_0    0x00000000    // ADDRTRNSFR: Addresses can be shifted to the device on DQ0 only.
#define ENUM_OSPI_DRICTL_ADDRTRNSFR_1    0x00001000    // ADDRTRNSFR: Addresses can be shifted to the device on DQ0 and DQ1 only.
#define ENUM_OSPI_DRICTL_ADDRTRNSFR_2    0x00002000    // ADDRTRNSFR: Addresses can be shifted to the device on DQ0, DQ1, DQ2, and DQ3.
#define ENUM_OSPI_DRICTL_ADDRTRNSFR_3    0x00003000    // ADDRTRNSFR: Addresses can be shifted to the device on DQ [7:0]
#define ENUM_OSPI_DRICTL_INSTRTYP_0      0x00000000    // INSTRTYP: Use standard SPI mode.
#define ENUM_OSPI_DRICTL_INSTRTYP_1      0x00000100    // INSTRTYP: Use DIO-SPI mode.
#define ENUM_OSPI_DRICTL_INSTRTYP_2      0x00000200    // INSTRTYP: Use QIO-SPI mode.
#define ENUM_OSPI_DRICTL_INSTRTYP_3      0x00000300    // INSTRTYP: Use OIO-SPI mode.

#define BITP_OSPI_DWICTL_DMYWR                   24    // Dummy Write Clock Cycles
#define BITP_OSPI_DWICTL_DATATRNSFR              16    // Data Transfer Type
#define BITP_OSPI_DWICTL_ADDRTRNSFR              12    // Address Transfer Type
#define BITP_OSPI_DWICTL_WELDIS                   8    // WEL Disable
#define BITP_OSPI_DWICTL_OPCODEWR                 0    // Write Opcode
#define BITM_OSPI_DWICTL_DMYWR           0x1F000000    // Dummy Write Clock Cycles
#define BITM_OSPI_DWICTL_DATATRNSFR      0x00030000    // Data Transfer Type
#define BITM_OSPI_DWICTL_ADDRTRNSFR      0x00003000    // Address Transfer Type
#define BITM_OSPI_DWICTL_WELDIS          0x00000100    // WEL Disable
#define BITM_OSPI_DWICTL_OPCODEWR        0x000000FF    // Write Opcode
#define ENUM_OSPI_DWICTL_DATATRNSFR_0    0x00000000    // DATATRNSFR: SIO Mode.
#define ENUM_OSPI_DWICTL_DATATRNSFR_1    0x00010000    // DATATRNSFR: Used for Dual Input/Output instructions.
#define ENUM_OSPI_DWICTL_DATATRNSFR_2    0x00020000    // DATATRNSFR: Used for Octal Input/Output Instructions.
#define ENUM_OSPI_DWICTL_DATATRNSFR_3    0x00030000    // DATATRNSFR: Used for Octal Input/Output instructions.
#define ENUM_OSPI_DWICTL_ADDRTRNSFR_0    0x00000000    // ADDRTRNSFR: Addresses can be shifted to the device on DQ0 only.
#define ENUM_OSPI_DWICTL_ADDRTRNSFR_1    0x00001000    // ADDRTRNSFR: Addresses can be shifted to the device on DQ0 and DQ1.
#define ENUM_OSPI_DWICTL_ADDRTRNSFR_2    0x00002000    // ADDRTRNSFR: Addresses can be shifted to the device on DQ0, DQ1, DQ2, and DQ3.
#define ENUM_OSPI_DWICTL_ADDRTRNSFR_3    0x00003000    // ADDRTRNSFR: Addresses can be shifted to the device on DQ [7:0].

#define BITP_OSPI_DLY_DSRT                       24    // Clock Delay for Chip Select Deassert
#define BITP_OSPI_DLY_DACT                       16    // Clock Delay for Chip Select Deactivation
#define BITP_OSPI_DLY_LSTTRAN                     8    // Clock Delay for Last Transaction
#define BITP_OSPI_DLY_INIT                        0    // Clock Delay
#define BITM_OSPI_DLY_DSRT               0xFF000000    // Clock Delay for Chip Select Deassert
#define BITM_OSPI_DLY_DACT               0x00FF0000    // Clock Delay for Chip Select Deactivation
#define BITM_OSPI_DLY_LSTTRAN            0x0000FF00    // Clock Delay for Last Transaction
#define BITM_OSPI_DLY_INIT               0x000000FF    // Clock Delay

#define BITP_OSPI_RDC_DDRDLYRD                   16    // DDR Read Delay
#define BITP_OSPI_RDC_DQSEN                       8    // DQS Enable
#define BITP_OSPI_RDC_SMPLEDG                     5    // Sample Edge Selection
#define BITP_OSPI_RDC_DLYRD                       1    // Read Delay
#define BITM_OSPI_RDC_DDRDLYRD           0x000F0000    // DDR Read Delay
#define BITM_OSPI_RDC_DQSEN              0x00000100    // DQS Enable
#define BITM_OSPI_RDC_SMPLEDG            0x00000020    // Sample Edge Selection
#define BITM_OSPI_RDC_DLYRD              0x0000001E    // Read Delay
#define ENUM_OSPI_RDC_SMPLEDG_0          0x00000000    // SMPLEDG: Data outputs from Flash are sampled on falling edge of the ref_clk.
#define ENUM_OSPI_RDC_SMPLEDG_1          0x00000020    // SMPLEDG: Data outputs from Flash are sampled on rising edge of the ref_clk.

#define BITP_OSPI_DSCTL_BLKSZ                    16    // Number of Bytes per Block
#define BITP_OSPI_DSCTL_PGSZ                      4    // Number of Bytes per Device Page
#define BITP_OSPI_DSCTL_ADDRSZ                    0    // Number of Address Bytes
#define BITM_OSPI_DSCTL_BLKSZ            0x001F0000    // Number of Bytes per Block
#define BITM_OSPI_DSCTL_PGSZ             0x0000FFF0    // Number of Bytes per Device Page
#define BITM_OSPI_DSCTL_ADDRSZ           0x0000000F    // Number of Address Bytes

#define BITP_OSPI_REMAPADDR_OFFSET                0    // Value
#define BITM_OSPI_REMAPADDR_OFFSET       0xFFFFFFFF    // Value

#define BITP_OSPI_MBCTL_MODE                      0    // Mode
#define BITM_OSPI_MBCTL_MODE             0x000000FF    // Mode

#define BITP_OSPI_WCCTL_REPDLY                   24    // Polling Repetition Delay
#define BITP_OSPI_WCCTL_CNT                      16    // Poll Count
#define BITP_OSPI_WCCTL_EXPEN                    15    // Enable Polling Expiration
#define BITP_OSPI_WCCTL_DIS                      14    // Disable Polling
#define BITP_OSPI_WCCTL_POLRTY                   13    // Polling Polarity
#define BITP_OSPI_WCCTL_INDEX                     8    // Polling Index
#define BITP_OSPI_WCCTL_OPCODE                    0    // Opcode
#define BITM_OSPI_WCCTL_REPDLY           0xFF000000    // Polling Repetition Delay
#define BITM_OSPI_WCCTL_CNT              0x00FF0000    // Poll Count
#define BITM_OSPI_WCCTL_EXPEN            0x00008000    // Enable Polling Expiration
#define BITM_OSPI_WCCTL_DIS              0x00004000    // Disable Polling
#define BITM_OSPI_WCCTL_POLRTY           0x00002000    // Polling Polarity
#define BITM_OSPI_WCCTL_INDEX            0x00000700    // Polling Index
#define BITM_OSPI_WCCTL_OPCODE           0x000000FF    // Opcode
#define ENUM_OSPI_WCCTL_POLRTY_0         0x00000000    // POLRTY: Write transfer to the device will be complete if the polled bit is 0.
#define ENUM_OSPI_WCCTL_POLRTY_1         0x00002000    // POLRTY: Write transfer to the device will be complete if the polled bit is 1.

#define BITP_OSPI_POLLEXP_PEC                     0    // Poll Expiration Cycles
#define BITM_OSPI_POLLEXP_PEC            0xFFFFFFFF    // Poll Expiration Cycles

#define BITP_OSPI_ISTAT_STIGREQ                  14    // STIG Request Completion Interrupt
#define BITP_OSPI_ISTAT_POLEXP                   13    // Poll Expiration Cycles
#define BITP_OSPI_ISTAT_ILLACCES                  5    // Illegal AHB Access Detected
#define BITP_OSPI_ISTAT_WRPROT                    4    // Write to Protected Area is Attempted and Rejected
#define BITP_OSPI_ISTAT_UNDRFLW                   1    // Underflow Detection
#define BITM_OSPI_ISTAT_STIGREQ          0x00004000    // STIG Request Completion Interrupt
#define BITM_OSPI_ISTAT_POLEXP           0x00002000    // Poll Expiration Cycles
#define BITM_OSPI_ISTAT_ILLACCES         0x00000020    // Illegal AHB Access Detected
#define BITM_OSPI_ISTAT_WRPROT           0x00000010    // Write to Protected Area is Attempted and Rejected
#define BITM_OSPI_ISTAT_UNDRFLW          0x00000002    // Underflow Detection
#define ENUM_OSPI_ISTAT_UNDRFLW_0        0x00000000    // UNDRFLW: No underflow is detected.
#define ENUM_OSPI_ISTAT_UNDRFLW_1        0x00000002    // UNDRFLW: underflow is detected and an attempt to transfer data is made when the small TX FIFO is empty.

#define BITP_OSPI_IMSK_STIGREQ_MSK               14    // STIG Request Completion Mask
#define BITP_OSPI_IMSK_POLEXP_MSK                13    // Polling Expiration Detected Mask
#define BITP_OSPI_IMSK_ILLACCES_MSK               5    // Illegal Access Detected Mask
#define BITP_OSPI_IMSK_WRPROT_MSK                 4    // Protected Area Write Attempt Mask
#define BITP_OSPI_IMSK_UNDRFLW_MSK                1    // Underflow Detected Mask
#define BITP_OSPI_IMSK_MODEFAIL_MSK               0    // Mode M Failure Mask
#define BITM_OSPI_IMSK_STIGREQ_MSK       0x00004000    // STIG Request Completion Mask
#define BITM_OSPI_IMSK_POLEXP_MSK        0x00002000    // Polling Expiration Detected Mask
#define BITM_OSPI_IMSK_ILLACCES_MSK      0x00000020    // Illegal Access Detected Mask
#define BITM_OSPI_IMSK_WRPROT_MSK        0x00000010    // Protected Area Write Attempt Mask
#define BITM_OSPI_IMSK_UNDRFLW_MSK       0x00000002    // Underflow Detected Mask
#define BITM_OSPI_IMSK_MODEFAIL_MSK      0x00000001    // Mode M Failure Mask

#define BITP_OSPI_WRPROT_LWR_SUBSECT              0    // Subsector
#define BITM_OSPI_WRPROT_LWR_SUBSECT     0xFFFFFFFF    // Subsector

#define BITP_OSPI_WRPROT_UP_SUBSECT               0    // Subsector
#define BITM_OSPI_WRPROT_UP_SUBSECT      0xFFFFFFFF    // Subsector

#define BITP_OSPI_WRPROT_CTL_EN                   1    // Write Protection Enable Bit
#define BITP_OSPI_WRPROT_CTL_INV                  0    // Write Protection Inversion Bit
#define BITM_OSPI_WRPROT_CTL_EN          0x00000002    // Write Protection Enable Bit
#define BITM_OSPI_WRPROT_CTL_INV         0x00000001    // Write Protection Inversion Bit

#define BITP_OSPI_FCMCTL_BNKADDR                 20    // Memory Bank Address
#define BITP_OSPI_FCMCTL_RDSZ                    16    // Number of STIG Bytes
#define BITP_OSPI_FCMCTL_BNKDATA                  8    // Last Requested Data from STIG Memory Bank
#define BITP_OSPI_FCMCTL_BNKREQ                   1    // Memory Bank Data Request in Progress
#define BITP_OSPI_FCMCTL_TRIGREQ                  0    // Trigger Memory Bank Data Request
#define BITM_OSPI_FCMCTL_BNKADDR         0x1FF00000    // Memory Bank Address
#define BITM_OSPI_FCMCTL_RDSZ            0x00070000    // Number of STIG Bytes
#define BITM_OSPI_FCMCTL_BNKDATA         0x0000FF00    // Last Requested Data from STIG Memory Bank
#define BITM_OSPI_FCMCTL_BNKREQ          0x00000002    // Memory Bank Data Request in Progress
#define BITM_OSPI_FCMCTL_TRIGREQ         0x00000001    // Trigger Memory Bank Data Request

#define BITP_OSPI_FCCTL_OPCODE                   24    // Command Opcode
#define BITP_OSPI_FCCTL_RDEN                     23    // Read Data Enable
#define BITP_OSPI_FCCTL_RDSZ                     20    // Number of Read Data Bytes
#define BITP_OSPI_FCCTL_ADDREN                   19    // Command Address Enable
#define BITP_OSPI_FCCTL_MODEEN                   18    // Mode Bit Enable
#define BITP_OSPI_FCCTL_ADDRSZ                   16    // Number of Address Bytes
#define BITP_OSPI_FCCTL_WREN                     15    // Write Data Enable
#define BITP_OSPI_FCCTL_WRSZ                     12    // Number of Write Data Bytes
#define BITP_OSPI_FCCTL_DMY                       7    // Number of Dummy Cycles
#define BITP_OSPI_FCCTL_STIGBNKEN                 2    // STIG Memory Bank Enable
#define BITP_OSPI_FCCTL_STAT                      1    // Command Execution in Progress
#define BITP_OSPI_FCCTL_EXE                       0    // Execute the STIG Command
#define BITM_OSPI_FCCTL_OPCODE           0xFF000000    // Command Opcode
#define BITM_OSPI_FCCTL_RDEN             0x00800000    // Read Data Enable
#define BITM_OSPI_FCCTL_RDSZ             0x00700000    // Number of Read Data Bytes
#define BITM_OSPI_FCCTL_ADDREN           0x00080000    // Command Address Enable
#define BITM_OSPI_FCCTL_MODEEN           0x00040000    // Mode Bit Enable
#define BITM_OSPI_FCCTL_ADDRSZ           0x00030000    // Number of Address Bytes
#define BITM_OSPI_FCCTL_WREN             0x00008000    // Write Data Enable
#define BITM_OSPI_FCCTL_WRSZ             0x00007000    // Number of Write Data Bytes
#define BITM_OSPI_FCCTL_DMY              0x00000F80    // Number of Dummy Cycles
#define BITM_OSPI_FCCTL_STIGBNKEN        0x00000004    // STIG Memory Bank Enable
#define BITM_OSPI_FCCTL_STAT             0x00000002    // Command Execution in Progress
#define BITM_OSPI_FCCTL_EXE              0x00000001    // Execute the STIG Command
#define ENUM_OSPI_FCCTL_OPCODE_0         0x00000000    // OPCODE: Command opcode, command address, command dummy bytes, and command data are serially transferred.
#define ENUM_OSPI_FCCTL_OPCODE_1         0x01000000    // OPCODE: Command opcode, command address, command dummy bytes, and command data are transferred in parallel using DQ0 and DQ1 pins.
#define ENUM_OSPI_FCCTL_OPCODE_2         0x02000000    // OPCODE: Command opcode, command address, command dummy bytes, and command data are transferred in parallel using DQ0 , DQ1, and DQ2 pins.
#define ENUM_OSPI_FCCTL_OPCODE_3         0x03000000    // OPCODE: Reserved
#define ENUM_OSPI_FCCTL_ADDRSZ_0         0x00000000    // ADDRSZ: 1 address byte
#define ENUM_OSPI_FCCTL_ADDRSZ_1         0x00010000    // ADDRSZ: 2 address bytes
#define ENUM_OSPI_FCCTL_ADDRSZ_2         0x00020000    // ADDRSZ: 3 address bytes
#define ENUM_OSPI_FCCTL_ADDRSZ_3         0x00030000    // ADDRSZ: 4 address bytes

#define BITP_OSPI_FCA_ADDR                        0    // Command Address
#define BITM_OSPI_FCA_ADDR               0xFFFFFFFF    // Command Address

#define BITP_OSPI_FCRD_LWR_DATA                   0    // Data
#define BITM_OSPI_FCRD_LWR_DATA          0xFFFFFFFF    // Data

#define BITP_OSPI_FCRD_UP_DATA                    0    // Data
#define BITM_OSPI_FCRD_UP_DATA           0xFFFFFFFF    // Data

#define BITP_OSPI_FCWD_LWR_DATA                   0    // Command Write Data Lower Byte
#define BITM_OSPI_FCWD_LWR_DATA          0xFFFFFFFF    // Command Write Data Lower Byte

#define BITP_OSPI_FCWD_UP_DATA                    0    // Command Write Data Upper Byte
#define BITM_OSPI_FCWD_UP_DATA           0xFFFFFFFF    // Command Write Data Upper Byte

#define BITP_OSPI_POLSTAT_DMY                    16    // Number of Dummy Cycles for Auto Polling
#define BITP_OSPI_POLSTAT_STAT                    8    // Device Status Valid
#define BITP_OSPI_POLSTAT_DEVSTAT                 0    // Defines Actual Status Register of Device
#define BITM_OSPI_POLSTAT_DMY            0x000F0000    // Number of Dummy Cycles for Auto Polling
#define BITM_OSPI_POLSTAT_STAT           0x00000100    // Device Status Valid
#define BITM_OSPI_POLSTAT_DEVSTAT        0x000000FF    // Defines Actual Status Register of Device

#define BITP_OSPI_PHYCTL_RESYNC                  31    // Re-synchronization Delay
#define BITP_OSPI_PHYCTL_RST                     30    // DLL Reset
#define BITP_OSPI_PHYCTL_RXBYP                   29    // Receive DLL Bypass
#define BITP_OSPI_PHYCTL_RSVRD2                  23    // Reserved
#define BITP_OSPI_PHYCTL_TXDLY                   16    // Transmit DLL Delay
#define BITP_OSPI_PHYCTL_RSVRD1                   7    // Reserved
#define BITP_OSPI_PHYCTL_RXDLY                    0    // Receive DLL Delay
#define BITM_OSPI_PHYCTL_RESYNC          0x80000000    // Re-synchronization Delay
#define BITM_OSPI_PHYCTL_RST             0x40000000    // DLL Reset
#define BITM_OSPI_PHYCTL_RXBYP           0x20000000    // Receive DLL Bypass
#define BITM_OSPI_PHYCTL_RSVRD2          0x1F800000    // Reserved
#define BITM_OSPI_PHYCTL_TXDLY           0x007F0000    // Transmit DLL Delay
#define BITM_OSPI_PHYCTL_RSVRD1          0x0000FF80    // Reserved
#define BITM_OSPI_PHYCTL_RXDLY           0x0000007F    // Receive DLL Delay

#define BITP_OSPI_PHYMCTL_RSVRD3                 25    // Reserved
#define BITP_OSPI_PHYMCTL_LCK                    24    // PHY Master Lock Mode
#define BITP_OSPI_PHYMCTL_BYPCTL                 23    // Bypass Mode Control
#define BITP_OSPI_PHYMCTL_PDSEL                  20    // PHY Master Phase Detect Selector
#define BITP_OSPI_PHYMCTL_RSVRD2                 19    // Reserved
#define BITP_OSPI_PHYMCTL_IND                    16    // Increment/Decrement Indication
#define BITP_OSPI_PHYMCTL_RSVRD1                  7    // Reserved
#define BITP_OSPI_PHYMCTL_INITDLY                 0    // Initial Delay
#define BITM_OSPI_PHYMCTL_RSVRD3         0xFE000000    // Reserved
#define BITM_OSPI_PHYMCTL_LCK            0x01000000    // PHY Master Lock Mode
#define BITM_OSPI_PHYMCTL_BYPCTL         0x00800000    // Bypass Mode Control
#define BITM_OSPI_PHYMCTL_PDSEL          0x00700000    // PHY Master Phase Detect Selector
#define BITM_OSPI_PHYMCTL_RSVRD2         0x00080000    // Reserved
#define BITM_OSPI_PHYMCTL_IND            0x00070000    // Increment/Decrement Indication
#define BITM_OSPI_PHYMCTL_RSVRD1         0x0000FF80    // Reserved
#define BITM_OSPI_PHYMCTL_INITDLY        0x0000007F    // Initial Delay

#define BITP_OSPI_DLLOB_LWR_INCSTAT              24    // State of Cumulative DLL Lock Inc Register
#define BITP_OSPI_DLLOB_LWR_DECSTAT              16    // State of Cumulative DLL Lock Dec Register
#define BITP_OSPI_DLLOB_LWR_LOCK_LB              15    // Loop Back Lock
#define BITP_OSPI_DLLOB_LWR_LWRLOCK               8    // DLL Observable Lower Lock Value
#define BITP_OSPI_DLLOB_LWR_UNLOCK                3    // DLL Observable Lower Unlock Counter
#define BITP_OSPI_DLLOB_LWR_LOCK                  1    // Mode in Which DLL is Locked
#define BITP_OSPI_DLLOB_LWR_STAT                  0    // Indicates DLL Status
#define BITM_OSPI_DLLOB_LWR_INCSTAT      0xFF000000    // State of Cumulative DLL Lock Inc Register
#define BITM_OSPI_DLLOB_LWR_DECSTAT      0x00FF0000    // State of Cumulative DLL Lock Dec Register
#define BITM_OSPI_DLLOB_LWR_LOCK_LB      0x00008000    // Loop Back Lock
#define BITM_OSPI_DLLOB_LWR_LWRLOCK      0x00007F00    // DLL Observable Lower Lock Value
#define BITM_OSPI_DLLOB_LWR_UNLOCK       0x000000F8    // DLL Observable Lower Unlock Counter
#define BITM_OSPI_DLLOB_LWR_LOCK         0x00000006    // Mode in Which DLL is Locked
#define BITM_OSPI_DLLOB_LWR_STAT         0x00000001    // Indicates DLL Status

#define BITP_OSPI_DLLOB_UP_RSVRD2                23    // Reserved
#define BITP_OSPI_DLLOB_UP_TXOP                  16    // DLL Observable Upper Transmit Decoder Output
#define BITP_OSPI_DLLOB_UP_RSVRD1                 7    // Reserved
#define BITP_OSPI_DLLOB_UP_RXOP                   0    // DLL Observable  Upper Receive Decoder Output
#define BITM_OSPI_DLLOB_UP_RSVRD2        0xFF800000    // Reserved
#define BITM_OSPI_DLLOB_UP_TXOP          0x007F0000    // DLL Observable Upper Transmit Decoder Output
#define BITM_OSPI_DLLOB_UP_RSVRD1        0x0000FF80    // Reserved
#define BITM_OSPI_DLLOB_UP_RXOP          0x0000007F    // DLL Observable  Upper Receive Decoder Output

#define BITP_OSPI_OE_LWR_XRDBYT                  24    // Supplement Byte of Any Read Opcode
#define BITP_OSPI_OE_LWR_XWRBYT                  16    // Supplement Byte of Any Write Opcode
#define BITP_OSPI_OE_LWR_XPOLBYT                  8    // Supplement Byte of Any Polling Opcode
#define BITP_OSPI_OE_LWR_XSTIGBYT                 0    // Supplement Byte of Any STIG Opcode
#define BITM_OSPI_OE_LWR_XRDBYT          0xFF000000    // Supplement Byte of Any Read Opcode
#define BITM_OSPI_OE_LWR_XWRBYT          0x00FF0000    // Supplement Byte of Any Write Opcode
#define BITM_OSPI_OE_LWR_XPOLBYT         0x0000FF00    // Supplement Byte of Any Polling Opcode
#define BITM_OSPI_OE_LWR_XSTIGBYT        0x000000FF    // Supplement Byte of Any STIG Opcode

#define BITP_OSPI_OE_UP_FSTBYTWEL                24    // First Byte of Any WEL Opcode
#define BITP_OSPI_OE_UP_XBYTWEL                  16    // Supplement Byte of Any WEL Opcode
#define BITM_OSPI_OE_UP_FSTBYTWEL        0xFF000000    // First Byte of Any WEL Opcode
#define BITM_OSPI_OE_UP_XBYTWEL          0x00FF0000    // Supplement Byte of Any WEL Opcode

#define BITP_OSPI_MODID_PATCH                    24    // Fix/Patch Number
#define BITP_OSPI_MODID_REVID                     8    // Module/Revision ID Number
#define BITP_OSPI_MODID_CFGID                     0    // Configuration ID
#define BITM_OSPI_MODID_PATCH            0xFF000000    // Fix/Patch Number
#define BITM_OSPI_MODID_REVID            0x00FFFF00    // Module/Revision ID Number
#define BITM_OSPI_MODID_CFGID            0x00000003    // Configuration ID

#endif
