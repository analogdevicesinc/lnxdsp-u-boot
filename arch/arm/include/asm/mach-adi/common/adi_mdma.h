// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * (C) Copyright 2022 - Analog Devices, Inc.
 *
 * Written and/or maintained by Timesys Corporation
 *
 * Contact: Nathan Barrett-Morrison <nathan.morrison@timesys.com>
 * Contact: Greg Malysa <greg.malysa@timesys.com>
 */

#ifndef ADI_MDMA
#define ADI_MDMA

#include <asm/mach-adi/common/adi_dma.h>

#ifdef CONFIG_SC59X
    #include <asm/armv7.h>
#endif

#define DMA_MIN(a, b)(((a) < (b)) ? (a) : (b))
#define DMA_MAX(a, b)(((a) > (b)) ? (a) : (b))

#define BK_DMA_MDMA_SRC_DEFAULT_CONFIG(PSize, MSize) \
    (BITM_DMA_CFG_EN | (PSize << BITP_DMA_CFG_PSIZE) | (MSize << BITP_DMA_CFG_MSIZE))
#define BK_DMA_MDMA_DST_DEFAULT_CONFIG(PSize, MSize) \
    (BITM_DMA_CFG_EN | BITM_DMA_CFG_WNR | ENUM_DMA_CFG_XCNT_INT | \
    (PSize << BITP_DMA_CFG_PSIZE) | (MSize << BITP_DMA_CFG_MSIZE))

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
}
dma_regs __attribute__((packed));

static u8 dma_get_msize(u32 nByteCount, u32 nAddress) {
    /* Calculate MSIZE, PSIZE, XCNT and XMOD */
    u8 nMsize = 0;
    u32 nValue = nByteCount | nAddress;
    u32 nMask = 0x1;

    for (nMsize = 0; nMsize < 5; nMsize++, nMask <<= 1) {
        if ((nValue & nMask) == nMask) {
            break;
        }
    }

    return nMsize;
}

static u32 memcopy_dma(u32 * data, u32 * flash_source, size_t len) {
    struct dma_regs * mdma_src = (dma_regs *) REG_DMA8_DSCPTR_NXT;
    struct dma_regs * mdma_dest = (dma_regs *) REG_DMA9_DSCPTR_NXT;

    u32 result = 0x00000001; /* Default success */
    u32 ByteCount = (u32) len;

    u8 nSrcMsize;
    u8 nDstMsize;
    u8 nSrcPsize;
    u8 nDstPsize;
    u8 nPSize;
    u32 SrcConfig;
    u32 DstConfig;

    /* guard against zero byte count */
    if (len == 0) {
        printf("MDMA error, zero byte count\n");
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
    mdma_src->ADDRSTART = (u32) flash_source;
    mdma_src->XCNT = ByteCount >> nSrcMsize;
    mdma_src->XMOD = 1 << nSrcMsize;
    mdma_dest->ADDRSTART = (u32) data;
    mdma_dest->XCNT = ByteCount >> nDstMsize;
    mdma_dest->XMOD = 1 << nDstMsize;

#ifdef CONFIG_SC59X
    CP15ISB;
    CP15DSB;
    CP15DMB;
#else
    flush_dcache_range(flash_source, flash_source + len);
#endif

    mdma_dest->CFG = DstConfig;
    mdma_src->CFG = SrcConfig;

#ifdef CONFIG_SC59X
    CP15ISB;
    CP15DSB;
    CP15DMB;
#else
    flush_dcache_range(data, data + len);
#endif

    /* Check for any configuration errors */
    if ((mdma_src->STAT & BITM_DMA_STAT_IRQERR) == BITM_DMA_STAT_IRQERR) {
        printf("mdma_src BITM_DMA_STAT_IRQERR (1)\n");
        result = 0x00000003;
    }
    if ((mdma_dest->STAT & BITM_DMA_STAT_IRQERR) == BITM_DMA_STAT_IRQERR) {
        printf("mdma_dest BITM_DMA_STAT_IRQERR (1)\n");
        result = 0x00000004;
    }

    /* Wait for DMA to complete while checking for a DMA error */
    do {
        if ((mdma_src->STAT & BITM_DMA_STAT_IRQERR) == BITM_DMA_STAT_IRQERR) {
            printf("mdma_src BITM_DMA_STAT_IRQERR (2)\n");
            mdma_src->CFG &= ~1;
            mdma_dest->CFG &= ~1;
            return 0x00000005;
        }
        if ((mdma_dest->STAT & BITM_DMA_STAT_IRQERR) == BITM_DMA_STAT_IRQERR) {
            printf("mdma_dest BITM_DMA_STAT_IRQERR (2)\n");
            mdma_src->CFG &= ~1;
            mdma_dest->CFG &= ~1;
            return 0x00000006;
        }
    } while ((mdma_dest->STAT & BITM_DMA_STAT_IRQDONE) == 0);

    mdma_src->CFG &= ~1;
    mdma_dest->CFG &= ~1;
    return result;
}

#endif
