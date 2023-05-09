/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * (C) Copyright 2022 - Analog Devices, Inc.
 *
 * Written and/or maintained by Timesys Corporation
 *
 * Contact: Nathan Barrett-Morrison <nathan.morrison@timesys.com>
 * Contact: Greg Malysa <greg.malysa@timesys.com>
 */

#ifndef RCU_H
#define RCU_H

#define BITP_RCU_CTL_LOCK                      31    // Lock
#define BITP_RCU_CTL_CRSTMSKSEL                10    // Core Reset System Reset Mask Select
#define BITP_RCU_CTL_CRSTREQEN                  9    // Core Reset Request Enabled
#define BITP_RCU_CTL_SRSTREQEN                  8    // System Reset Request Enabled
#define BITP_RCU_CTL_RSTOUTDSRT                 2    // Reset Out Deassert
#define BITP_RCU_CTL_RSTOUTASRT                 1    // Reset Out Assert
#define BITP_RCU_CTL_SYSRST                     0    // System Reset
#define BITM_RCU_CTL_LOCK              0x80000000    // Lock
#define BITM_RCU_CTL_CRSTMSKSEL        0x00000400    // Core Reset System Reset Mask Select
#define BITM_RCU_CTL_CRSTREQEN         0x00000200    // Core Reset Request Enabled
#define BITM_RCU_CTL_SRSTREQEN         0x00000100    // System Reset Request Enabled
#define BITM_RCU_CTL_RSTOUTDSRT        0x00000004    // Reset Out Deassert
#define BITM_RCU_CTL_RSTOUTASRT        0x00000002    // Reset Out Assert
#define BITM_RCU_CTL_SYSRST            0x00000001    // System Reset

#define BITP_RCU_STAT_RSTOUTERR                18    // Reset Out Error
#define BITP_RCU_STAT_LWERR                    17    // Lock Write Error
#define BITP_RCU_STAT_ADDRERR                  16    // Address Error
#define BITP_RCU_STAT_BMODE                     8    // Boot Mode
#define BITP_RCU_STAT_RSTOUT                    5    // Reset Out Status
#define BITP_RCU_STAT_SWRST                     3    // Software Reset
#define BITP_RCU_STAT_SSRST                     2    // System Source Reset
#define BITP_RCU_STAT_HWRST                     0    // Hardware Reset
#define BITM_RCU_STAT_RSTOUTERR        0x00040000    // Reset Out Error
#define BITM_RCU_STAT_LWERR            0x00020000    // Lock Write Error
#define BITM_RCU_STAT_ADDRERR          0x00010000    // Address Error
#define BITM_RCU_STAT_BMODE            0x00000F00    // Boot Mode
#define BITM_RCU_STAT_RSTOUT           0x00000020    // Reset Out Status
#define BITM_RCU_STAT_SWRST            0x00000008    // Software Reset
#define BITM_RCU_STAT_SSRST            0x00000004    // System Source Reset
#define BITM_RCU_STAT_HWRST            0x00000001    // Hardware Reset

#define BITP_RCU_CRCTL_LOCK                    31    // Lock
#define BITP_RCU_CRCTL_CR0                      0    // Core Reset Outputs
#define BITP_RCU_CRCTL_CR1                      1    // Core Reset Outputs
#define BITP_RCU_CRCTL_CR2                      2    // Core Reset Outputs
#define BITP_RCU_CRCTL_CR3                      3    // Core Reset Outputs
#define BITM_RCU_CRCTL_LOCK            0x80000000    // Lock
#define BITM_RCU_CRCTL_CR0             0x00000001    // Core Reset Outputs
#define BITM_RCU_CRCTL_CR1             0x00000002    // Core Reset Outputs
#define BITM_RCU_CRCTL_CR2             0x00000004    // Core Reset Outputs
#define BITM_RCU_CRCTL_CR3             0x00000008    // Core Reset Outputs

#define BITP_RCU_CRSTAT_CR0                     0    // Core Reset Outputs
#define BITP_RCU_CRSTAT_CR1                     1    // Core Reset Outputs
#define BITP_RCU_CRSTAT_CR2                     2    // Core Reset Outputs
#define BITP_RCU_CRSTAT_CR3                     3    // Core Reset Outputs
#define BITM_RCU_CRSTAT_CR0            0x00000001    // Core Reset Outputs
#define BITM_RCU_CRSTAT_CR1            0x00000002    // Core Reset Outputs
#define BITM_RCU_CRSTAT_CR2            0x00000004    // Core Reset Outputs
#define BITM_RCU_CRSTAT_CR3            0x00000008    // Core Reset Outputs

#define BITP_RCU_SRRQSTAT_SRRQ                  0    // System Reset Triggered by System Reset Request [n]
#define BITM_RCU_SRRQSTAT_SRRQ         0x000000FF    // System Reset Triggered by System Reset Request [n]

#define BITP_RCU_SIDIS_LOCK                    31    // Lock
#define BITP_RCU_SIDIS_SI0                      0    // System Interface Disable Request [2:0]
#define BITP_RCU_SIDIS_SI1                      1    // System Interface Disable Request [2:0]
#define BITP_RCU_SIDIS_SI2                      2    // System Interface Disable Request [2:0]
#define BITM_RCU_SIDIS_LOCK            0x80000000    // Lock
#define BITM_RCU_SIDIS_SI0             0x00000001    // System Interface Disable Request [2:0]
#define BITM_RCU_SIDIS_SI1             0x00000002    // System Interface Disable Request [2:0]
#define BITM_RCU_SIDIS_SI2             0x00000004    // System Interface Disable Request [2:0]

#define BITP_RCU_SISTAT_SI0                     0    // System Interface Disable Acknowledge [2:0]
#define BITP_RCU_SISTAT_SI1                     1    // System Interface Disable Acknowledge [2:0]
#define BITP_RCU_SISTAT_SI2                     2    // System Interface Disable Acknowledge [2:0]
#define BITM_RCU_SISTAT_SI0            0x00000001    // System Interface Disable Acknowledge [2:0]
#define BITM_RCU_SISTAT_SI1            0x00000002    // System Interface Disable Acknowledge [2:0]
#define BITM_RCU_SISTAT_SI2            0x00000004    // System Interface Disable Acknowledge [2:0]

#define BITP_RCU_SVECT_LCK_LOCK                31    // Lock
#define BITP_RCU_SVECT_LCK_SVECT0               0    // Lock SVECTn Registers
#define BITP_RCU_SVECT_LCK_SVECT1               1    // Lock SVECTn Registers
#define BITP_RCU_SVECT_LCK_SVECT2               2    // Lock SVECTn Registers
#define BITM_RCU_SVECT_LCK_LOCK        0x80000000    // Lock
#define BITM_RCU_SVECT_LCK_SVECT0      0x00000001    // Lock SVECTn Registers
#define BITM_RCU_SVECT_LCK_SVECT1      0x00000002    // Lock SVECTn Registers
#define BITM_RCU_SVECT_LCK_SVECT2      0x00000004    // Lock SVECTn Registers

#define BITP_RCU_BCODE_LOCK                    31    // Lock
#define BITP_RCU_BCODE_IDLEONENTRY             13    // Idle On Entry
#define BITP_RCU_BCODE_NOL2CONFIG              12    // No L2 Configuration
#define BITP_RCU_BCODE_NOHOOK                  10    // No Hook
#define BITP_RCU_BCODE_NOPREBOOT                9    // No Preboot
#define BITP_RCU_BCODE_NOFAULTS                 6    // No Faults
#define BITP_RCU_BCODE_NOCACHE                  5    // No Cache
#define BITP_RCU_BCODE_NOMEMINIT                4    // No Memory Initialization
#define BITP_RCU_BCODE_HBTOVW                   3    // Execute Wakeup
#define BITP_RCU_BCODE_HALT                     2    // Halt
#define BITP_RCU_BCODE_NOVECTINIT               1    // No Vector Initialize
#define BITP_RCU_BCODE_NOKERNEL                 0    // No Boot Kernel
#define BITM_RCU_BCODE_LOCK            0x80000000    // Lock
#define BITM_RCU_BCODE_IDLEONENTRY     0x00002000    // Idle On Entry
#define BITM_RCU_BCODE_NOL2CONFIG      0x00001000    // No L2 Configuration
#define BITM_RCU_BCODE_NOHOOK          0x00000400    // No Hook
#define BITM_RCU_BCODE_NOPREBOOT       0x00000200    // No Preboot
#define BITM_RCU_BCODE_NOFAULTS        0x00000040    // No Faults
#define BITM_RCU_BCODE_NOCACHE         0x00000020    // No Cache
#define BITM_RCU_BCODE_NOMEMINIT       0x00000010    // No Memory Initialization
#define BITM_RCU_BCODE_HBTOVW          0x00000008    // Execute Wakeup
#define BITM_RCU_BCODE_HALT            0x00000004    // Halt
#define BITM_RCU_BCODE_NOVECTINIT      0x00000002    // No Vector Initialize
#define BITM_RCU_BCODE_NOKERNEL        0x00000001    // No Boot Kernel

#define BITP_RCU_SVECT0_VALUE                   0    // Core 0 Reset Vector
#define BITM_RCU_SVECT0_VALUE          0xFFFFFFFF    // Core 0 Reset Vector

#define BITP_RCU_SVECT1_VALUE                   0    // Reset Vector
#define BITM_RCU_SVECT1_VALUE          0xFFFFFFFF    // Reset Vector

#define BITP_RCU_SVECT2_VALUE                   0    // Reset Vector
#define BITM_RCU_SVECT2_VALUE          0xFFFFFFFF    // Reset Vector

#define BITP_RCU_MSG_CALLERR                   31    // Call Error Flag
#define BITP_RCU_MSG_CALLBACK                  30    // Callback Call Flag
#define BITP_RCU_MSG_CALLINIT                  29    // Call Initcode Flag
#define BITP_RCU_MSG_CALLAPP                   28    // Call Application Flag
#define BITP_RCU_MSG_HALTONERR                 27    // Halt on Error Call
#define BITP_RCU_MSG_HALTONCALL                26    // Halt on Callback Call
#define BITP_RCU_MSG_HALTONINIT                25    // Halt on Initcode Call
#define BITP_RCU_MSG_HALTONAPP                 24    // Halt on Application Call
#define BITP_RCU_MSG_L3INIT                    23    // L3 Initialized
#define BITP_RCU_MSG_L2INIT                    22    // L2 Initialized
#define BITP_RCU_MSG_SECINIT                   21    // SEC Initialized
#define BITP_RCU_MSG_C2ACTIVATE                20    // Core 2 Activated
#define BITP_RCU_MSG_C1ACTIVATE                19    // Core 1 Activated
#define BITP_RCU_MSG_C2L1INIT                  18    // Core 2 L1 Initialized
#define BITP_RCU_MSG_C1L1INIT                  17    // Core 1 L1 Initialized
#define BITP_RCU_MSG_C0L1INIT                  16    // Core 0 L1 Initialized
#define BITP_RCU_MSG_C2IDLE                    10    // Core 2 Idle
#define BITP_RCU_MSG_C1IDLE                     9    // Core 1 Idle
#define BITP_RCU_MSG_C0IDLE                     8    // Core 0 Idle
#define BITP_RCU_MSG_ERRCODE                    0    // ROM Error Code
#define BITM_RCU_MSG_CALLERR           0x80000000    // Call Error Flag
#define BITM_RCU_MSG_CALLBACK          0x40000000    // Callback Call Flag
#define BITM_RCU_MSG_CALLINIT          0x20000000    // Call Initcode Flag
#define BITM_RCU_MSG_CALLAPP           0x10000000    // Call Application Flag
#define BITM_RCU_MSG_HALTONERR         0x08000000    // Halt on Error Call
#define BITM_RCU_MSG_HALTONCALL        0x04000000    // Halt on Callback Call
#define BITM_RCU_MSG_HALTONINIT        0x02000000    // Halt on Initcode Call
#define BITM_RCU_MSG_HALTONAPP         0x01000000    // Halt on Application Call
#define BITM_RCU_MSG_L3INIT            0x00800000    // L3 Initialized
#define BITM_RCU_MSG_L2INIT            0x00400000    // L2 Initialized
#define BITM_RCU_MSG_SECINIT           0x00200000    // SEC Initialized
#define BITM_RCU_MSG_C2ACTIVATE        0x00100000    // Core 2 Activated
#define BITM_RCU_MSG_C1ACTIVATE        0x00080000    // Core 1 Activated
#define BITM_RCU_MSG_C2L1INIT          0x00040000    // Core 2 L1 Initialized
#define BITM_RCU_MSG_C1L1INIT          0x00020000    // Core 1 L1 Initialized
#define BITM_RCU_MSG_C0L1INIT          0x00010000    // Core 0 L1 Initialized
#define BITM_RCU_MSG_C2IDLE            0x00000400    // Core 2 Idle
#define BITM_RCU_MSG_C1IDLE            0x00000200    // Core 1 Idle
#define BITM_RCU_MSG_C0IDLE            0x00000100    // Core 0 Idle
#define BITM_RCU_MSG_ERRCODE           0x000000FF    // ROM Error Code

#endif
