#include <asm/armv7.h>
#define _ADI_MSK_3( mask, smask, type ) ((type)(smask))

#define REG_DMA8_DSCPTR_NXT                  0x310A7000            /*  DMA8 Pointer to Next Initial Descriptor Register */
#define REG_DMA8_ADDRSTART                   0x310A7004            /*  DMA8 Start Address of Current Buffer Register */
#define REG_DMA8_CFG                         0x310A7008            /*  DMA8 Configuration Register */
#define REG_DMA8_XCNT                        0x310A700C            /*  DMA8 Inner Loop Count Start Value Register */
#define REG_DMA8_XMOD                        0x310A7010            /*  DMA8 Inner Loop Address Increment Register */
#define REG_DMA8_YCNT                        0x310A7014            /*  DMA8 Outer Loop Count Start Value (2D only) Register */
#define REG_DMA8_YMOD                        0x310A7018            /*  DMA8 Outer Loop Address Increment (2D only) Register */
#define REG_DMA8_DSCPTR_CUR                  0x310A7024            /*  DMA8 Current Descriptor Pointer Register */
#define REG_DMA8_DSCPTR_PRV                  0x310A7028            /*  DMA8 Previous Initial Descriptor Pointer Register */
#define REG_DMA8_ADDR_CUR                    0x310A702C            /*  DMA8 Current Address Register */
#define REG_DMA8_STAT                        0x310A7030            /*  DMA8 Status Register */
#define REG_DMA8_XCNT_CUR                    0x310A7034            /*  DMA8 Current Count (1D) or Intra-row XCNT (2D) Register */
#define REG_DMA8_YCNT_CUR                    0x310A7038            /*  DMA8 Current Row Count (2D only) Register */
#define REG_DMA8_BWLCNT                      0x310A7040            /*  DMA8 Bandwidth Limit Count Register */
#define REG_DMA8_BWLCNT_CUR                  0x310A7044            /*  DMA8 Bandwidth Limit Count Current Register */
#define REG_DMA8_BWMCNT                      0x310A7048            /*  DMA8 Bandwidth Monitor Count Register */
#define REG_DMA8_BWMCNT_CUR                  0x310A704C            /*  DMA8 Bandwidth Monitor Count Current Register */

#define REG_DMA9_DSCPTR_NXT                  0x310A7080            /*  DMA9 Pointer to Next Initial Descriptor Register */
#define REG_DMA9_ADDRSTART                   0x310A7084            /*  DMA9 Start Address of Current Buffer Register */
#define REG_DMA9_CFG                         0x310A7088            /*  DMA9 Configuration Register */
#define REG_DMA9_XCNT                        0x310A708C            /*  DMA9 Inner Loop Count Start Value Register */
#define REG_DMA9_XMOD                        0x310A7090            /*  DMA9 Inner Loop Address Increment Register */
#define REG_DMA9_YCNT                        0x310A7094            /*  DMA9 Outer Loop Count Start Value (2D only) Register */
#define REG_DMA9_YMOD                        0x310A7098            /*  DMA9 Outer Loop Address Increment (2D only) Register */
#define REG_DMA9_DSCPTR_CUR                  0x310A70A4            /*  DMA9 Current Descriptor Pointer Register */
#define REG_DMA9_DSCPTR_PRV                  0x310A70A8            /*  DMA9 Previous Initial Descriptor Pointer Register */
#define REG_DMA9_ADDR_CUR                    0x310A70AC            /*  DMA9 Current Address Register */
#define REG_DMA9_STAT                        0x310A70B0            /*  DMA9 Status Register */
#define REG_DMA9_XCNT_CUR                    0x310A70B4            /*  DMA9 Current Count (1D) or Intra-row XCNT (2D) Register */
#define REG_DMA9_YCNT_CUR                    0x310A70B8            /*  DMA9 Current Row Count (2D only) Register */
#define REG_DMA9_BWLCNT                      0x310A70C0            /*  DMA9 Bandwidth Limit Count Register */
#define REG_DMA9_BWLCNT_CUR                  0x310A70C4            /*  DMA9 Bandwidth Limit Count Current Register */
#define REG_DMA9_BWMCNT                      0x310A70C8            /*  DMA9 Bandwidth Monitor Count Register */
#define REG_DMA9_BWMCNT_CUR                  0x310A70CC            /*  DMA9 Bandwidth Monitor Count Current Register */

#define BITP_DMA_CFG_PDRF                    28            /*  Peripheral Data Request Forward */
#define BITP_DMA_CFG_TWOD                    26            /*  Two Dimension Addressing Enable */
#define BITP_DMA_CFG_DESCIDCPY               25            /*  Descriptor ID Copy Control */
#define BITP_DMA_CFG_TOVEN                   24            /*  Trigger Overrun Error Enable */
#define BITP_DMA_CFG_TRIG                    22            /*  Generate Outgoing Trigger */
#define BITP_DMA_CFG_INT                     20            /*  Generate Interrupt Request */
#define BITP_DMA_CFG_NDSIZE                  16            /*  Next Descriptor Set Size */
#define BITP_DMA_CFG_TWAIT                   15            /*  Wait for Trigger */
#define BITP_DMA_CFG_FLOW                    12            /*  Next Operation */
#define BITP_DMA_CFG_MSIZE                    8            /*  Memory Transfer Word Size */
#define BITP_DMA_CFG_PSIZE                    4            /*  Peripheral Transfer Word Size */
#define BITP_DMA_CFG_CADDR                    3            /*  Use Current Address */
#define BITP_DMA_CFG_SYNC                     2            /*  Synchronize Work Unit Transitions */
#define BITP_DMA_CFG_WNR                      1            /*  Write/Read Channel Direction */
#define BITP_DMA_CFG_EN                       0            /*  DMA Channel Enable */
#define BITM_DMA_CFG_PDRF                    (_ADI_MSK_3(0x10000000,0x10000000UL, uint32_t  ))    /*  Peripheral Data Request Forward */
#define BITM_DMA_CFG_TWOD                    (_ADI_MSK_3(0x04000000,0x04000000UL, uint32_t  ))    /*  Two Dimension Addressing Enable */
#define BITM_DMA_CFG_DESCIDCPY               (_ADI_MSK_3(0x02000000,0x02000000UL, uint32_t  ))    /*  Descriptor ID Copy Control */
#define BITM_DMA_CFG_TOVEN                   (_ADI_MSK_3(0x01000000,0x01000000UL, uint32_t  ))    /*  Trigger Overrun Error Enable */
#define BITM_DMA_CFG_TRIG                    (_ADI_MSK_3(0x00C00000,0x00C00000UL, uint32_t  ))    /*  Generate Outgoing Trigger */
#define BITM_DMA_CFG_INT                     (_ADI_MSK_3(0x00300000,0x00300000UL, uint32_t  ))    /*  Generate Interrupt Request */
#define BITM_DMA_CFG_NDSIZE                  (_ADI_MSK_3(0x00070000,0x00070000UL, uint32_t  ))    /*  Next Descriptor Set Size */
#define BITM_DMA_CFG_TWAIT                   (_ADI_MSK_3(0x00008000,0x00008000UL, uint32_t  ))    /*  Wait for Trigger */
#define BITM_DMA_CFG_FLOW                    (_ADI_MSK_3(0x00007000,0x00007000UL, uint32_t  ))    /*  Next Operation */
#define BITM_DMA_CFG_MSIZE                   (_ADI_MSK_3(0x00000700,0x00000700UL, uint32_t  ))    /*  Memory Transfer Word Size */
#define BITM_DMA_CFG_PSIZE                   (_ADI_MSK_3(0x00000070,0x00000070UL, uint32_t  ))    /*  Peripheral Transfer Word Size */
#define BITM_DMA_CFG_CADDR                   (_ADI_MSK_3(0x00000008,0x00000008UL, uint32_t  ))    /*  Use Current Address */
#define BITM_DMA_CFG_SYNC                    (_ADI_MSK_3(0x00000004,0x00000004UL, uint32_t  ))    /*  Synchronize Work Unit Transitions */
#define BITM_DMA_CFG_WNR                     (_ADI_MSK_3(0x00000002,0x00000002UL, uint32_t  ))    /*  Write/Read Channel Direction */
#define BITM_DMA_CFG_EN                      (_ADI_MSK_3(0x00000001,0x00000001UL, uint32_t  ))    /*  DMA Channel Enable */

#define BITM_DMA_STAT_TWAIT                  (_ADI_MSK_3(0x00100000,0x00100000UL, uint32_t  ))    /*  Trigger Wait Status */
#define BITM_DMA_STAT_FIFOFILL               (_ADI_MSK_3(0x00070000,0x00070000UL, uint32_t  ))    /*  FIFO Fill Status */
#define BITM_DMA_STAT_MBWID                  (_ADI_MSK_3(0x0000C000,0x0000C000UL, uint32_t  ))    /*  Memory Bus Width */
#define BITM_DMA_STAT_PBWID                  (_ADI_MSK_3(0x00003000,0x00003000UL, uint32_t  ))    /*  Peripheral Bus Width */
#define BITM_DMA_STAT_RUN                    (_ADI_MSK_3(0x00000700,0x00000700UL, uint32_t  ))    /*  Run Status */
#define BITM_DMA_STAT_ERRC                   (_ADI_MSK_3(0x00000070,0x00000070UL, uint32_t  ))    /*  Error Cause */
#define BITM_DMA_STAT_PIRQ                   (_ADI_MSK_3(0x00000004,0x00000004UL, uint32_t  ))    /*  Peripheral Interrupt Request */
#define BITM_DMA_STAT_IRQERR                 (_ADI_MSK_3(0x00000002,0x00000002UL, uint32_t  ))    /*  Error Interrupt Request */
#define BITM_DMA_STAT_IRQDONE                (_ADI_MSK_3(0x00000001,0x00000001UL, uint32_t  ))    /*  Work Unit/Row Done Interrupt */

#define ENUM_DMA_CFG_PDAT_NOTFWD             (_ADI_MSK_3(0x00000000,0x00000000UL, uint32_t  ))    /*  PDRF: Peripheral Data Request Not Forwarded */
#define ENUM_DMA_CFG_PDAT_FWD                (_ADI_MSK_3(0x10000000,0x10000000UL, uint32_t  ))    /*  PDRF: Peripheral Data Request Forwarded */
#define ENUM_DMA_CFG_ADDR1D                  (_ADI_MSK_3(0x00000000,0x00000000UL, uint32_t  ))    /*  TWOD: One-Dimensional Addressing */
#define ENUM_DMA_CFG_ADDR2D                  (_ADI_MSK_3(0x04000000,0x04000000UL, uint32_t  ))    /*  TWOD: Two-Dimensional Addressing */
#define ENUM_DMA_CFG_NO_COPY                 (_ADI_MSK_3(0x00000000,0x00000000UL, uint32_t  ))    /*  DESCIDCPY: Never Copy */
#define ENUM_DMA_CFG_COPY                    (_ADI_MSK_3(0x02000000,0x02000000UL, uint32_t  ))    /*  DESCIDCPY: Copy on Work Unit Complete */
#define ENUM_DMA_CFG_TOV_DIS                 (_ADI_MSK_3(0x00000000,0x00000000UL, uint32_t  ))    /*  TOVEN: Ignore Trigger Overrun */
#define ENUM_DMA_CFG_TOV_EN                  (_ADI_MSK_3(0x01000000,0x01000000UL, uint32_t  ))    /*  TOVEN: Error on Trigger Overrun */
#define ENUM_DMA_CFG_NO_TRIG                 (_ADI_MSK_3(0x00000000,0x00000000UL, uint32_t  ))    /*  TRIG: Never Assert Trigger */
#define ENUM_DMA_CFG_XCNT_TRIG               (_ADI_MSK_3(0x00400000,0x00400000UL, uint32_t  ))    /*  TRIG: Trigger When XCNTCUR Reaches 0 */
#define ENUM_DMA_CFG_YCNT_TRIG               (_ADI_MSK_3(0x00800000,0x00800000UL, uint32_t  ))    /*  TRIG: Trigger When YCNTCUR Reaches 0 */
#define ENUM_DMA_CFG_NO_INT                  (_ADI_MSK_3(0x00000000,0x00000000UL, uint32_t  ))    /*  INT: Never Assert Interrupt */
#define ENUM_DMA_CFG_XCNT_INT                (_ADI_MSK_3(0x00100000,0x00100000UL, uint32_t  ))    /*  INT: Interrupt When X Count Expires */
#define ENUM_DMA_CFG_YCNT_INT                (_ADI_MSK_3(0x00200000,0x00200000UL, uint32_t  ))    /*  INT: Interrupt When Y Count Expires */
#define ENUM_DMA_CFG_PERIPH_INT              (_ADI_MSK_3(0x00300000,0x00300000UL, uint32_t  ))    /*  INT: Peripheral Interrupt request */
#define ENUM_DMA_CFG_FETCH01                 (_ADI_MSK_3(0x00000000,0x00000000UL, uint32_t  ))    /*  NDSIZE: Fetch One Descriptor Element */
#define ENUM_DMA_CFG_FETCH02                 (_ADI_MSK_3(0x00010000,0x00010000UL, uint32_t  ))    /*  NDSIZE: Fetch Two Descriptor Elements */
#define ENUM_DMA_CFG_FETCH03                 (_ADI_MSK_3(0x00020000,0x00020000UL, uint32_t  ))    /*  NDSIZE: Fetch Three Descriptor Elements */
#define ENUM_DMA_CFG_FETCH04                 (_ADI_MSK_3(0x00030000,0x00030000UL, uint32_t  ))    /*  NDSIZE: Fetch Four Descriptor Elements */
#define ENUM_DMA_CFG_FETCH05                 (_ADI_MSK_3(0x00040000,0x00040000UL, uint32_t  ))    /*  NDSIZE: Fetch Five Descriptor Elements */
#define ENUM_DMA_CFG_FETCH06                 (_ADI_MSK_3(0x00050000,0x00050000UL, uint32_t  ))    /*  NDSIZE: Fetch Six Descriptor Elements */
#define ENUM_DMA_CFG_FETCH07                 (_ADI_MSK_3(0x00060000,0x00060000UL, uint32_t  ))    /*  NDSIZE: Fetch Seven Descriptor Elements */
#define ENUM_DMA_CFG_NO_TRGWAIT              (_ADI_MSK_3(0x00000000,0x00000000UL, uint32_t  ))    /*  TWAIT: Begin Work Unit Automatically (No Wait) */
#define ENUM_DMA_CFG_TRGWAIT                 (_ADI_MSK_3(0x00008000,0x00008000UL, uint32_t  ))    /*  TWAIT: Wait for Trigger (Halt before Work Unit) */
#define ENUM_DMA_CFG_STOP                    (_ADI_MSK_3(0x00000000,0x00000000UL, uint32_t  ))    /*  FLOW: STOP. */
#define ENUM_DMA_CFG_AUTO                    (_ADI_MSK_3(0x00001000,0x00001000UL, uint32_t  ))    /*  FLOW: AUTO. */
#define ENUM_DMA_CFG_DSCLIST                 (_ADI_MSK_3(0x00004000,0x00004000UL, uint32_t  ))    /*  FLOW: DSCL. */
#define ENUM_DMA_CFG_DSCARRAY                (_ADI_MSK_3(0x00005000,0x00005000UL, uint32_t  ))    /*  FLOW: DSCA. */
#define ENUM_DMA_CFG_DODLIST                 (_ADI_MSK_3(0x00006000,0x00006000UL, uint32_t  ))    /*  FLOW: Descriptor On-Demand List. */
#define ENUM_DMA_CFG_DODARRAY                (_ADI_MSK_3(0x00007000,0x00007000UL, uint32_t  ))    /*  FLOW: Descriptor On Demand Array. */
#define ENUM_DMA_CFG_MSIZE04                 (_ADI_MSK_3(0x00000200,0x00000200UL, uint32_t  ))    /*  MSIZE: 4 Bytes */
#define ENUM_DMA_CFG_MSIZE08                 (_ADI_MSK_3(0x00000300,0x00000300UL, uint32_t  ))    /*  MSIZE: 8 Bytes */
#define ENUM_DMA_CFG_MSIZE16                 (_ADI_MSK_3(0x00000400,0x00000400UL, uint32_t  ))    /*  MSIZE: 16 Bytes */
#define ENUM_DMA_CFG_MSIZE32                 (_ADI_MSK_3(0x00000500,0x00000500UL, uint32_t  ))    /*  MSIZE: 32 Bytes */
#define ENUM_DMA_CFG_PSIZE04                 (_ADI_MSK_3(0x00000020,0x00000020UL, uint32_t  ))    /*  PSIZE: 4 Bytes */
#define ENUM_DMA_CFG_PSIZE08                 (_ADI_MSK_3(0x00000030,0x00000030UL, uint32_t  ))    /*  PSIZE: 8 Bytes */
#define ENUM_DMA_CFG_LD_STARTADDR            (_ADI_MSK_3(0x00000000,0x00000000UL, uint32_t  ))    /*  CADDR: Load Starting Address */
#define ENUM_DMA_CFG_LD_CURADDR              (_ADI_MSK_3(0x00000008,0x00000008UL, uint32_t  ))    /*  CADDR: Use Current Address */
#define ENUM_DMA_CFG_NO_SYNC                 (_ADI_MSK_3(0x00000000,0x00000000UL, uint32_t  ))    /*  SYNC: No Synchronization */
#define ENUM_DMA_CFG_SYNC                    (_ADI_MSK_3(0x00000004,0x00000004UL, uint32_t  ))    /*  SYNC: Synchronize Channel */
#define ENUM_DMA_CFG_READ                    (_ADI_MSK_3(0x00000000,0x00000000UL, uint32_t  ))    /*  WNR: Transmit (Read from memory) */
#define ENUM_DMA_CFG_WRITE                   (_ADI_MSK_3(0x00000002,0x00000002UL, uint32_t  ))    /*  WNR: Receive (Write to memory) */
#define ENUM_DMA_CFG_DIS                     (_ADI_MSK_3(0x00000000,0x00000000UL, uint32_t  ))    /*  EN: Disable */
#define ENUM_DMA_CFG_EN                      (_ADI_MSK_3(0x00000001,0x00000001UL, uint32_t  ))    /*  EN: Enable */

#define DMA_MIN(a,b) (((a)<(b))?(a):(b))
#define DMA_MAX(a,b) (((a)>(b))?(a):(b))

#define BK_DMA_MDMA_SRC_DEFAULT_CONFIG(PSize, MSize) (BITM_DMA_CFG_EN|(PSize << BITP_DMA_CFG_PSIZE)|(MSize << BITP_DMA_CFG_MSIZE))
#define BK_DMA_MDMA_DST_DEFAULT_CONFIG(PSize, MSize) (BITM_DMA_CFG_EN|BITM_DMA_CFG_WNR|ENUM_DMA_CFG_XCNT_INT|(PSize << BITP_DMA_CFG_PSIZE)|(MSize << BITP_DMA_CFG_MSIZE))

#define MAX_DMA_PSIZE 2

typedef struct dma_regs {
	volatile u32 DSCPTR_NXT;
	volatile u32 ADDRSTART;
	volatile u32 CFG;
	volatile u32 XCNT;
	volatile u32 XMOD;
	volatile u32 YCNT;
	volatile u32 YMOD;
        volatile u32 reserved0;
        volatile u32 reserved1;
	volatile u32 DSCPTR_CUR;
	volatile u32 DSCPTR_PRV;
	volatile u32 ADDR_CUR;
	volatile u32 STAT;
	volatile u32 XCNT_CUR;
	volatile u32 YCNT_CUR;
        volatile u32 reserved3;
	volatile u32 BWLCNT;
	volatile u32 BWLCNT_CUR;
	volatile u32 BWMCNT;
	volatile u32 BWMCNT_CUR;
}dma_regs;

static u8 dma_get_msize(u32 nByteCount, u32 nAddress)
{
  /* Calculate MSIZE, PSIZE, XCNT and XMOD */
  u8  nMsize = 0;
  u32 nValue = nByteCount | nAddress;
  u32 nMask  = 0x1;


  for (nMsize = 0; nMsize < 5; nMsize++, nMask <<= 1) {
    if ((nValue & nMask) == nMask) {
      break;
    }
  }

  return nMsize;
}

static u32 memcopy_dma(void *data, void * flash_source, size_t len)
{
  struct dma_regs * mdma_src= (dma_regs *)REG_DMA8_DSCPTR_NXT;
  struct dma_regs * mdma_dest= (dma_regs *)REG_DMA9_DSCPTR_NXT;

  u32 result = 0x00000001; /* Default success */
  u32 ByteCount = (u32) len;

  u8 nSrcMsize ;
  u8 nDstMsize ;
  u8 nSrcPsize ;
  u8 nDstPsize ;
  u8 nPSize ;
  u32 SrcConfig ;
  u32 DstConfig ;

  /* guard against zero byte count */
  if (len == 0) {
    return 0x00000002;
  }

  /* Clear DMA status */
  mdma_src->STAT = (BITM_DMA_STAT_IRQDONE | BITM_DMA_STAT_IRQERR | BITM_DMA_STAT_PIRQ);
  mdma_dest->STAT = (BITM_DMA_STAT_IRQDONE | BITM_DMA_STAT_IRQERR | BITM_DMA_STAT_PIRQ);

  /* Calculate MSIZE, PSIZE, XCNT and XMOD */
  nSrcMsize = dma_get_msize(ByteCount, (u32) flash_source);
  nDstMsize = dma_get_msize(ByteCount, (u32) data);
  nSrcPsize = DMA_MIN(nSrcMsize, MAX_DMA_PSIZE);
  nDstPsize = DMA_MIN(nDstMsize, MAX_DMA_PSIZE);
  nPSize = DMA_MIN(nSrcPsize, nDstPsize);

  SrcConfig = BK_DMA_MDMA_SRC_DEFAULT_CONFIG(nPSize, nSrcMsize);
  DstConfig = BK_DMA_MDMA_DST_DEFAULT_CONFIG(nPSize, nDstMsize);

  /* Load the DMA descriptors */
  mdma_src->ADDRSTART = (u32)flash_source;
  mdma_src->XCNT = ByteCount >> nSrcMsize;
  mdma_src->XMOD = 1 << nSrcMsize;
  mdma_dest->ADDRSTART = (u32)data;
  mdma_dest->XCNT = ByteCount >> nDstMsize;
  mdma_dest->XMOD = 1 << nDstMsize;

  CP15ISB;
  CP15DSB;
  CP15DMB;

  mdma_dest->CFG = DstConfig;
  mdma_src->CFG = SrcConfig;

  CP15ISB;
  CP15DSB;
  CP15DMB;

  /* Check for any configuration errors */
  if ((mdma_src->STAT & BITM_DMA_STAT_IRQERR) == BITM_DMA_STAT_IRQERR) {
    result = 0x00000003;
  }
  if ((mdma_dest->STAT & BITM_DMA_STAT_IRQERR) == BITM_DMA_STAT_IRQERR) {
    result = 0x00000004;
  }

      /* Wait for DMA to complete while checking for a DMA error */
      do {
        if ((mdma_src->STAT & BITM_DMA_STAT_IRQERR) == BITM_DMA_STAT_IRQERR) {
          mdma_src->CFG &= ~1;
          mdma_dest->CFG &= ~1;
          return 0x00000005;
        }
        if ((mdma_dest->STAT & BITM_DMA_STAT_IRQERR) == BITM_DMA_STAT_IRQERR) {
          mdma_src->CFG &= ~1;
          mdma_dest->CFG &= ~1;
          return 0x00000006;
        }
      }while ((mdma_dest->STAT & BITM_DMA_STAT_IRQDONE) == 0);

      mdma_src->CFG &= ~1;
      mdma_dest->CFG &= ~1;
      return result;
}