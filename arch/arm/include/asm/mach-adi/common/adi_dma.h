#ifndef ADI_DMA
#define ADI_DMA

#define REG_DMA8_DSCPTR_NXT              0x310A7000    // DMA8 Pointer to Next Initial Descriptor Register
#define REG_DMA8_ADDRSTART               0x310A7004    // DMA8 Start Address of Current Buffer Register
#define REG_DMA8_CFG                     0x310A7008    // DMA8 Configuration Register
#define REG_DMA8_XCNT                    0x310A700C    // DMA8 Inner Loop Count Start Value Register
#define REG_DMA8_XMOD                    0x310A7010    // DMA8 Inner Loop Address Increment Register
#define REG_DMA8_YCNT                    0x310A7014    // DMA8 Outer Loop Count Start Value (2D only) Register
#define REG_DMA8_YMOD                    0x310A7018    // DMA8 Outer Loop Address Increment (2D only) Register
#define REG_DMA8_DSCPTR_CUR              0x310A7024    // DMA8 Current Descriptor Pointer Register
#define REG_DMA8_DSCPTR_PRV              0x310A7028    // DMA8 Previous Initial Descriptor Pointer Register
#define REG_DMA8_ADDR_CUR                0x310A702C    // DMA8 Current Address Register
#define REG_DMA8_STAT                    0x310A7030    // DMA8 Status Register
#define REG_DMA8_XCNT_CUR                0x310A7034    // DMA8 Current Count (1D) or Intra-row XCNT (2D) Register
#define REG_DMA8_YCNT_CUR                0x310A7038    // DMA8 Current Row Count (2D only) Register
#define REG_DMA8_BWLCNT                  0x310A7040    // DMA8 Bandwidth Limit Count Register
#define REG_DMA8_BWLCNT_CUR              0x310A7044    // DMA8 Bandwidth Limit Count Current Register
#define REG_DMA8_BWMCNT                  0x310A7048    // DMA8 Bandwidth Monitor Count Register
#define REG_DMA8_BWMCNT_CUR              0x310A704C    // DMA8 Bandwidth Monitor Count Current Register

#define REG_DMA9_DSCPTR_NXT              0x310A7080    // DMA9 Pointer to Next Initial Descriptor Register
#define REG_DMA9_ADDRSTART               0x310A7084    // DMA9 Start Address of Current Buffer Register
#define REG_DMA9_CFG                     0x310A7088    // DMA9 Configuration Register
#define REG_DMA9_XCNT                    0x310A708C    // DMA9 Inner Loop Count Start Value Register
#define REG_DMA9_XMOD                    0x310A7090    // DMA9 Inner Loop Address Increment Register
#define REG_DMA9_YCNT                    0x310A7094    // DMA9 Outer Loop Count Start Value (2D only) Register
#define REG_DMA9_YMOD                    0x310A7098    // DMA9 Outer Loop Address Increment (2D only) Register
#define REG_DMA9_DSCPTR_CUR              0x310A70A4    // DMA9 Current Descriptor Pointer Register
#define REG_DMA9_DSCPTR_PRV              0x310A70A8    // DMA9 Previous Initial Descriptor Pointer Register
#define REG_DMA9_ADDR_CUR                0x310A70AC    // DMA9 Current Address Register
#define REG_DMA9_STAT                    0x310A70B0    // DMA9 Status Register
#define REG_DMA9_XCNT_CUR                0x310A70B4    // DMA9 Current Count (1D) or Intra-row XCNT (2D) Register
#define REG_DMA9_YCNT_CUR                0x310A70B8    // DMA9 Current Row Count (2D only) Register
#define REG_DMA9_BWLCNT                  0x310A70C0    // DMA9 Bandwidth Limit Count Register
#define REG_DMA9_BWLCNT_CUR              0x310A70C4    // DMA9 Bandwidth Limit Count Current Register
#define REG_DMA9_BWMCNT                  0x310A70C8    // DMA9 Bandwidth Monitor Count Register
#define REG_DMA9_BWMCNT_CUR              0x310A70CC    // DMA9 Bandwidth Monitor Count Current Register

#define BITP_DMA_DSCPTR_NXT_VALUE                 0    // Pointer to Next Descriptor Set
#define BITM_DMA_DSCPTR_NXT_VALUE        0xFFFFFFFF    // Pointer to Next Descriptor Set

#define BITP_DMA_ADDRSTART_VALUE                  0    // Work Unit Address Start Value
#define BITM_DMA_ADDRSTART_VALUE         0xFFFFFFFF    // Work Unit Address Start Value

#define BITP_DMA_CFG_PDRF                        28    // Peripheral Data Request Forward
#define BITP_DMA_CFG_TWOD                        26    // Two Dimension Addressing Enable
#define BITP_DMA_CFG_DESCIDCPY                   25    // Descriptor ID Copy Control
#define BITP_DMA_CFG_TOVEN                       24    // Trigger Overrun Error Enable
#define BITP_DMA_CFG_TRIG                        22    // Generate Outgoing Trigger
#define BITP_DMA_CFG_INT                         20    // Generate Interrupt Request
#define BITP_DMA_CFG_NDSIZE                      16    // Next Descriptor Set Size
#define BITP_DMA_CFG_TWAIT                       15    // Wait for Trigger
#define BITP_DMA_CFG_FLOW                        12    // Next Operation
#define BITP_DMA_CFG_MSIZE                        8    // Memory Transfer Word Size
#define BITP_DMA_CFG_PSIZE                        4    // Peripheral Transfer Word Size
#define BITP_DMA_CFG_CADDR                        3    // Use Current Address
#define BITP_DMA_CFG_SYNC                         2    // Synchronize Work Unit Transitions
#define BITP_DMA_CFG_WNR                          1    // Write/Read Channel Direction
#define BITP_DMA_CFG_EN                           0    // DMA Channel Enable
#define BITM_DMA_CFG_PDRF                0x10000000    // Peripheral Data Request Forward
#define BITM_DMA_CFG_TWOD                0x04000000    // Two Dimension Addressing Enable
#define BITM_DMA_CFG_DESCIDCPY           0x02000000    // Descriptor ID Copy Control
#define BITM_DMA_CFG_TOVEN               0x01000000    // Trigger Overrun Error Enable
#define BITM_DMA_CFG_TRIG                0x00C00000    // Generate Outgoing Trigger
#define BITM_DMA_CFG_INT                 0x00300000    // Generate Interrupt Request
#define BITM_DMA_CFG_NDSIZE              0x00070000    // Next Descriptor Set Size
#define BITM_DMA_CFG_TWAIT               0x00008000    // Wait for Trigger
#define BITM_DMA_CFG_FLOW                0x00007000    // Next Operation
#define BITM_DMA_CFG_MSIZE               0x00000700    // Memory Transfer Word Size
#define BITM_DMA_CFG_PSIZE               0x00000070    // Peripheral Transfer Word Size
#define BITM_DMA_CFG_CADDR               0x00000008    // Use Current Address
#define BITM_DMA_CFG_SYNC                0x00000004    // Synchronize Work Unit Transitions
#define BITM_DMA_CFG_WNR                 0x00000002    // Write/Read Channel Direction
#define BITM_DMA_CFG_EN                  0x00000001    // DMA Channel Enable
#define ENUM_DMA_CFG_PDAT_NOTFWD         0x00000000    // PDRF: Peripheral Data Request Not Forwarded
#define ENUM_DMA_CFG_PDAT_FWD            0x10000000    // PDRF: Peripheral Data Request Forwarded
#define ENUM_DMA_CFG_ADDR1D              0x00000000    // TWOD: One-Dimensional Addressing
#define ENUM_DMA_CFG_ADDR2D              0x04000000    // TWOD: Two-Dimensional Addressing
#define ENUM_DMA_CFG_NO_COPY             0x00000000    // DESCIDCPY: Never Copy
#define ENUM_DMA_CFG_COPY                0x02000000    // DESCIDCPY: Copy on Work Unit Complete
#define ENUM_DMA_CFG_TOV_DIS             0x00000000    // TOVEN: Ignore Trigger Overrun
#define ENUM_DMA_CFG_TOV_EN              0x01000000    // TOVEN: Error on Trigger Overrun
#define ENUM_DMA_CFG_NO_TRIG             0x00000000    // TRIG: Never Assert Trigger
#define ENUM_DMA_CFG_XCNT_TRIG           0x00400000    // TRIG: Trigger When XCNTCUR Reaches 0
#define ENUM_DMA_CFG_YCNT_TRIG           0x00800000    // TRIG: Trigger When YCNTCUR Reaches 0
#define ENUM_DMA_CFG_NO_INT              0x00000000    // INT: Never Assert Interrupt
#define ENUM_DMA_CFG_XCNT_INT            0x00100000    // INT: Interrupt When X Count Expires
#define ENUM_DMA_CFG_YCNT_INT            0x00200000    // INT: Interrupt When Y Count Expires
#define ENUM_DMA_CFG_PERIPH_INT          0x00300000    // INT: Peripheral Interrupt request
#define ENUM_DMA_CFG_FETCH01             0x00000000    // NDSIZE: Fetch One Descriptor Element
#define ENUM_DMA_CFG_FETCH02             0x00010000    // NDSIZE: Fetch Two Descriptor Elements
#define ENUM_DMA_CFG_FETCH03             0x00020000    // NDSIZE: Fetch Three Descriptor Elements
#define ENUM_DMA_CFG_FETCH04             0x00030000    // NDSIZE: Fetch Four Descriptor Elements
#define ENUM_DMA_CFG_FETCH05             0x00040000    // NDSIZE: Fetch Five Descriptor Elements
#define ENUM_DMA_CFG_FETCH06             0x00050000    // NDSIZE: Fetch Six Descriptor Elements
#define ENUM_DMA_CFG_FETCH07             0x00060000    // NDSIZE: Fetch Seven Descriptor Elements
#define ENUM_DMA_CFG_NO_TRGWAIT          0x00000000    // TWAIT: Begin Work Unit Automatically (No Wait)
#define ENUM_DMA_CFG_TRGWAIT             0x00008000    // TWAIT: Wait for Trigger (Halt before Work Unit)
#define ENUM_DMA_CFG_STOP                0x00000000    // FLOW: STOP.
#define ENUM_DMA_CFG_AUTO                0x00001000    // FLOW: AUTO.
#define ENUM_DMA_CFG_DSCLIST             0x00004000    // FLOW: DSCL.
#define ENUM_DMA_CFG_DSCARRAY            0x00005000    // FLOW: DSCA.
#define ENUM_DMA_CFG_DODLIST             0x00006000    // FLOW: Descriptor On-Demand List.
#define ENUM_DMA_CFG_DODARRAY            0x00007000    // FLOW: Descriptor On Demand Array.
#define ENUM_DMA_CFG_MSIZE04             0x00000200    // MSIZE: 4 Bytes
#define ENUM_DMA_CFG_MSIZE08             0x00000300    // MSIZE: 8 Bytes
#define ENUM_DMA_CFG_MSIZE16             0x00000400    // MSIZE: 16 Bytes
#define ENUM_DMA_CFG_MSIZE32             0x00000500    // MSIZE: 32 Bytes
#define ENUM_DMA_CFG_PSIZE04             0x00000020    // PSIZE: 4 Bytes
#define ENUM_DMA_CFG_PSIZE08             0x00000030    // PSIZE: 8 Bytes
#define ENUM_DMA_CFG_LD_STARTADDR        0x00000000    // CADDR: Load Starting Address
#define ENUM_DMA_CFG_LD_CURADDR          0x00000008    // CADDR: Use Current Address
#define ENUM_DMA_CFG_NO_SYNC             0x00000000    // SYNC: No Synchronization
#define ENUM_DMA_CFG_SYNC                0x00000004    // SYNC: Synchronize Channel
#define ENUM_DMA_CFG_READ                0x00000000    // WNR: Transmit (Read from memory)
#define ENUM_DMA_CFG_WRITE               0x00000002    // WNR: Receive (Write to memory)
#define ENUM_DMA_CFG_DIS                 0x00000000    // EN: Disable
#define ENUM_DMA_CFG_EN                  0x00000001    // EN: Enable

#define BITP_DMA_XCNT_VALUE                       0    // Work Unit Inner Loop Counter Start Value
#define BITM_DMA_XCNT_VALUE              0xFFFFFFFF    // Work Unit Inner Loop Counter Start Value

#define BITP_DMA_XMOD_VALUE                       0    // Inner Loop Address Increment in Bytes
#define BITM_DMA_XMOD_VALUE              0xFFFFFFFF    // Inner Loop Address Increment in Bytes

#define BITP_DMA_YCNT_VALUE                       0    // Work Unit Inner Loop Counter Current Value
#define BITM_DMA_YCNT_VALUE              0xFFFFFFFF    // Work Unit Inner Loop Counter Current Value

#define BITP_DMA_YMOD_VALUE                       0    // Outer Loop Address Increment in Bytes
#define BITM_DMA_YMOD_VALUE              0xFFFFFFFF    // Outer Loop Address Increment in Bytes

#define BITP_DMA_DSCPTR_CUR_VALUE                 0    // Pointer for Current Descriptor Element
#define BITM_DMA_DSCPTR_CUR_VALUE        0xFFFFFFFF    // Pointer for Current Descriptor Element

#define BITP_DMA_DSCPTR_PRV_DESCPPREV             2    // Descriptor Pointer for Previous Element
#define BITP_DMA_DSCPTR_PRV_PDPO                  0    // Previous Descriptor Pointer Overrun
#define BITM_DMA_DSCPTR_PRV_DESCPPREV    0xFFFFFFFC    // Descriptor Pointer for Previous Element
#define BITM_DMA_DSCPTR_PRV_PDPO         0x00000001    // Previous Descriptor Pointer Overrun

#define BITP_DMA_ADDR_CUR_VALUE                   0    // Work Unit Current Address Value
#define BITM_DMA_ADDR_CUR_VALUE          0xFFFFFFFF    // Work Unit Current Address Value

#define BITP_DMA_STAT_TWAIT                      20    // Trigger Wait Status
#define BITP_DMA_STAT_FIFOFILL                   16    // FIFO Fill Status
#define BITP_DMA_STAT_MBWID                      14    // Memory Bus Width
#define BITP_DMA_STAT_PBWID                      12    // Peripheral Bus Width
#define BITP_DMA_STAT_RUN                         8    // Run Status
#define BITP_DMA_STAT_ERRC                        4    // Error Cause
#define BITP_DMA_STAT_PIRQ                        2    // Peripheral Interrupt Request
#define BITP_DMA_STAT_IRQERR                      1    // Error Interrupt Request
#define BITP_DMA_STAT_IRQDONE                     0    // Work Unit/Row Done Interrupt
#define BITM_DMA_STAT_TWAIT              0x00100000    // Trigger Wait Status
#define BITM_DMA_STAT_FIFOFILL           0x00070000    // FIFO Fill Status
#define BITM_DMA_STAT_MBWID              0x0000C000    // Memory Bus Width
#define BITM_DMA_STAT_PBWID              0x00003000    // Peripheral Bus Width
#define BITM_DMA_STAT_RUN                0x00000700    // Run Status
#define BITM_DMA_STAT_ERRC               0x00000070    // Error Cause
#define BITM_DMA_STAT_PIRQ               0x00000004    // Peripheral Interrupt Request
#define BITM_DMA_STAT_IRQERR             0x00000002    // Error Interrupt Request
#define BITM_DMA_STAT_IRQDONE            0x00000001    // Work Unit/Row Done Interrupt
#define ENUM_DMA_STAT_NOTRIGRX           0x00000000    // TWAIT: No Trigger Received
#define ENUM_DMA_STAT_TRIGRX             0x00100000    // TWAIT: Trigger Received
#define ENUM_DMA_STAT_FIFOEMPTY          0x00000000    // FIFOFILL: Empty
#define ENUM_DMA_STAT_FIFO25             0x00010000    // FIFOFILL: Empty < FIFO = 1/4 Full
#define ENUM_DMA_STAT_FIFO50             0x00020000    // FIFOFILL: 1/4 Full < FIFO = 1/2 Full
#define ENUM_DMA_STAT_FIFO75             0x00030000    // FIFOFILL: 1/2 Full < FIFO = 3/4 Full
#define ENUM_DMA_STAT_FIFONEARFULL       0x00040000    // FIFOFILL: 3/4 Full < FIFO = Full
#define ENUM_DMA_STAT_FIFOFULL           0x00070000    // FIFOFILL: Full
#define ENUM_DMA_STAT_MBUS02             0x00000000    // MBWID: 2 Bytes
#define ENUM_DMA_STAT_MBUS04             0x00004000    // MBWID: 4 Bytes
#define ENUM_DMA_STAT_MBUS08             0x00008000    // MBWID: 8 Bytes
#define ENUM_DMA_STAT_MBUS16             0x0000C000    // MBWID: 16 Bytes
#define ENUM_DMA_STAT_PBUS01             0x00000000    // PBWID: 1 Byte
#define ENUM_DMA_STAT_PBUS02             0x00001000    // PBWID: 2 Bytes
#define ENUM_DMA_STAT_PBUS04             0x00002000    // PBWID: 4 Bytes
#define ENUM_DMA_STAT_PBUS08             0x00003000    // PBWID: 8 Bytes
#define ENUM_DMA_STAT_STOPPED            0x00000000    // RUN: Idle/Stop State
#define ENUM_DMA_STAT_DSCFETCH           0x00000100    // RUN: Descriptor Fetch
#define ENUM_DMA_STAT_DATAXFER           0x00000200    // RUN: Data Transfer
#define ENUM_DMA_STAT_TRGWAIT            0x00000300    // RUN: Waiting for Trigger
#define ENUM_DMA_STAT_ACKWAIT            0x00000400    // RUN: Waiting for Write ACK/FIFO Drain to Peripheral
#define ENUM_DMA_STAT_CFGERR             0x00000000    // ERRC: Configuration Error
#define ENUM_DMA_STAT_ILLWRERR           0x00000010    // ERRC: Illegal Write Occurred While Channel Running
#define ENUM_DMA_STAT_ALGNERR            0x00000020    // ERRC: Address Alignment Error
#define ENUM_DMA_STAT_MEMERR             0x00000030    // ERRC: Memory Access or Fabric Error
#define ENUM_DMA_STAT_TRGOVERR           0x00000050    // ERRC: Trigger Overrun
#define ENUM_DMA_STAT_BWMONERR           0x00000060    // ERRC: Bandwidth Monitor Error
#define ENUM_DMA_STAT_NO_PIRQ            0x00000000    // PIRQ: No Interrupt request
#define ENUM_DMA_STAT_PIRQ               0x00000004    // PIRQ: Interrupt Request signaled by peripheral
#define ENUM_DMA_STAT_NO_IRQERR          0x00000000    // IRQERR: No Error
#define ENUM_DMA_STAT_IRQERR             0x00000002    // IRQERR: Error Occurred
#define ENUM_DMA_STAT_NO_IRQ             0x00000000    // IRQDONE: Inactive
#define ENUM_DMA_STAT_IRQDONE            0x00000001    // IRQDONE: Active

#define BITP_DMA_XCNT_CUR_VALUE                   0    // Work Unit Outer Loop Counter Start Value
#define BITM_DMA_XCNT_CUR_VALUE          0xFFFFFFFF    // Work Unit Outer Loop Counter Start Value

#define BITP_DMA_YCNT_CUR_VALUE                   0    // Work Unit Outer Loop Counter Current Value
#define BITM_DMA_YCNT_CUR_VALUE          0xFFFFFFFF    // Work Unit Outer Loop Counter Current Value

#define BITP_DMA_BWLCNT_VALUE                     0    // Bandwidth Limit Count
#define BITM_DMA_BWLCNT_VALUE            0x0000FFFF    // Bandwidth Limit Count

#define BITP_DMA_BWLCNT_CUR_VALUE                 0    // Bandwidth Limit Count Current
#define BITM_DMA_BWLCNT_CUR_VALUE        0x0000FFFF    // Bandwidth Limit Count Current

#define BITP_DMA_BWMCNT_VALUE                     0    // Bandwidth Monitor Count
#define BITM_DMA_BWMCNT_VALUE            0xFFFFFFFF    // Bandwidth Monitor Count

#define BITP_DMA_BWMCNT_CUR_VALUE                 0    // Bandwidth Monitor Count Current
#define BITM_DMA_BWMCNT_CUR_VALUE        0xFFFFFFFF    // Bandwidth Monitor Count Current

#endif