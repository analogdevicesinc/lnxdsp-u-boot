// SPDX-License-Identifier: GPL-2.0+
/*
 * SDHCI ADMA2 helper functions.
 */

#include <common.h>
#include <cpu_func.h>
#include <sdhci.h>
#include <malloc.h>
#include <asm/cache.h>

static void sdhci_adma_desc(struct sdhci_adma_desc *desc,
			    dma_addr_t addr, u16 len, bool end)
{
	u8 attr;

	attr = ADMA_DESC_ATTR_VALID | ADMA_DESC_TRANSFER_DATA;
	if (end)
		attr |= ADMA_DESC_ATTR_END;

	desc->attr = attr;
	desc->len = len;
	desc->reserved = 0;
	desc->addr_lo = lower_32_bits(addr);
#ifdef CONFIG_DMA_ADDR_T_64BIT
	desc->addr_hi = upper_32_bits(addr);
#endif
}

/**
 * sdhci_prepare_adma_table() - Populate the ADMA table
 *
 * @table:	Pointer to the ADMA table
 * @data:	Pointer to MMC data
 * @addr:	DMA address to write to or read from
 *
 * Fill the ADMA table according to the MMC data to read from or write to the
 * given DMA address.
 * Please note, that the table size depends on CONFIG_SYS_MMC_MAX_BLK_COUNT and
 * we don't have to check for overflow.
 */
void sdhci_prepare_adma_table(struct sdhci_adma_desc *table,
			      struct mmc_data *data, dma_addr_t addr)
{
	uint trans_bytes = data->blocksize * data->blocks;
	uint desc_count = DIV_ROUND_UP(trans_bytes, ADMA_MAX_LEN);
	struct sdhci_adma_desc *desc = table;
	int i = desc_count;
#if ADMA_BOUNDARY_SIZE > 0
	int extraEntries = 0;
#endif

	while (--i) {
#if ADMA_BOUNDARY_SIZE > 0
		//Check if this ADMA descriptor is going to cross a boundary
		//If it does cross the boundary, adjust the descriptor so that it does not cross the boundary,
		//by splitting the descriptor into two separate descriptors
		if ((addr & ADMA_BOUNDARY_SIZE) != ((addr + ADMA_MAX_LEN) & ADMA_BOUNDARY_SIZE)) {
			//len = Number of bytes we can transfer before crossing the boundary
			int len = (ADMA_BOUNDARY_SIZE - 1) - (addr & (ADMA_BOUNDARY_SIZE - 1)) + 1;

			//Let the while loop run one more iteration, so that it generates the second half of the split descriptor
			if ((trans_bytes) <= (ADMA_MAX_LEN * MMC_MAX_BLOCK_LEN)) { //Make sure we have room for the extra entry
				i++;
			}
			extraEntries++;

			//Generate and add new descriptor entry
			sdhci_adma_desc(desc, addr, len, false);
			addr += len;
			trans_bytes -= len;
			desc++;
		} else {
#endif
			sdhci_adma_desc(desc, addr, ADMA_MAX_LEN, false);
			addr += ADMA_MAX_LEN;
			trans_bytes -= ADMA_MAX_LEN;
			desc++;
#if ADMA_BOUNDARY_SIZE > 0
		}
#endif
    }

	sdhci_adma_desc(desc, addr, trans_bytes, true);

	flush_cache((dma_addr_t)table,
#if ADMA_BOUNDARY_SIZE > 0
		    ROUND((desc_count + extraEntries) * sizeof(struct sdhci_adma_desc),
			  #else
		    ROUND(desc_count * sizeof(struct sdhci_adma_desc),
#endif
			  ARCH_DMA_MINALIGN));

}

/**
 * sdhci_adma_init() - initialize the ADMA descriptor table
 *
 * Return: pointer to the allocated descriptor table or NULL in case of an
 * error.
 */
struct sdhci_adma_desc *sdhci_adma_init(void)
{
	return memalign(ARCH_DMA_MINALIGN, ADMA_TABLE_SZ);
}
