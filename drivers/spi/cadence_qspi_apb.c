/*
 * Copyright (C) 2012 Altera Corporation <www.altera.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *  - Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  - Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *  - Neither the name of the Altera Corporation nor the
 *    names of its contributors may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL ALTERA CORPORATION BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <common.h>
#include <log.h>
#include <asm/io.h>
#include <dma.h>
#include <linux/bitops.h>
#include <linux/delay.h>
#include <linux/errno.h>
#include <wait_bit.h>
#include <spi.h>
#include <spi-mem.h>
#include <linux/mtd/spi-nor.h>
#include <malloc.h>
#include "cadence_qspi.h"

#if defined(CONFIG_SC59X) || defined(CONFIG_SC59X_64)
#include <adi/common/ospi.h>
#include <adi/common/adi_mdma.h>
#endif

#define CQSPI_REG_POLL_US			1 /* 1us */
#define CQSPI_REG_RETRY				10000
#define CQSPI_POLL_IDLE_RETRY			3

/* Transfer mode */
#define CQSPI_INST_TYPE_SINGLE			0
#define CQSPI_INST_TYPE_DUAL			1
#define CQSPI_INST_TYPE_QUAD			2
#define CQSPI_INST_TYPE_OCTAL			3

#define CQSPI_STIG_DATA_LEN_MAX			8

#define CQSPI_DUMMY_CLKS_PER_BYTE		8
#define CQSPI_DUMMY_BYTES_MAX			4

/****************************************************************************
 * Controller's configuration and status register (offset from QSPI_BASE)
 ****************************************************************************/
#define	CQSPI_REG_CONFIG			0x00
#define	CQSPI_REG_CONFIG_ENABLE			BIT(0)
#define	CQSPI_REG_CONFIG_CLK_POL		BIT(1)
#define	CQSPI_REG_CONFIG_CLK_PHA		BIT(2)
#define	CQSPI_REG_CONFIG_DIRECT			BIT(7)
#define	CQSPI_REG_CONFIG_DECODE			BIT(9)
#define	CQSPI_REG_CONFIG_XIP_IMM		BIT(18)
#define	CQSPI_REG_CONFIG_CHIPSELECT_LSB		10
#define	CQSPI_REG_CONFIG_BAUD_LSB		19
#define	CQSPI_REG_CONFIG_IDLE_LSB		31
#define	CQSPI_REG_CONFIG_CHIPSELECT_MASK	0xF
#define	CQSPI_REG_CONFIG_BAUD_MASK		0xF

#define	CQSPI_REG_RD_INSTR			0x04
#define	CQSPI_REG_RD_INSTR_OPCODE_LSB		0
#define	CQSPI_REG_RD_INSTR_TYPE_INSTR_LSB	8
#define	CQSPI_REG_RD_INSTR_TYPE_ADDR_LSB	12
#define	CQSPI_REG_RD_INSTR_TYPE_DATA_LSB	16
#define	CQSPI_REG_RD_INSTR_MODE_EN_LSB		20
#define	CQSPI_REG_RD_INSTR_DUMMY_LSB		24
#define	CQSPI_REG_RD_INSTR_TYPE_INSTR_MASK	0x3
#define	CQSPI_REG_RD_INSTR_TYPE_ADDR_MASK	0x3
#define	CQSPI_REG_RD_INSTR_TYPE_DATA_MASK	0x3
#define	CQSPI_REG_RD_INSTR_DUMMY_MASK		0x1F

#define	CQSPI_REG_WR_INSTR			0x08
#define	CQSPI_REG_WR_INSTR_OPCODE_LSB		0
#define	CQSPI_REG_WR_INSTR_TYPE_DATA_LSB	16

#define	CQSPI_REG_DELAY				0x0C
#define	CQSPI_REG_DELAY_TSLCH_LSB		0
#define	CQSPI_REG_DELAY_TCHSH_LSB		8
#define	CQSPI_REG_DELAY_TSD2D_LSB		16
#define	CQSPI_REG_DELAY_TSHSL_LSB		24
#define	CQSPI_REG_DELAY_TSLCH_MASK		0xFF
#define	CQSPI_REG_DELAY_TCHSH_MASK		0xFF
#define	CQSPI_REG_DELAY_TSD2D_MASK		0xFF
#define	CQSPI_REG_DELAY_TSHSL_MASK		0xFF

#define	CQSPI_REG_RD_DATA_CAPTURE		0x10
#define	CQSPI_REG_RD_DATA_CAPTURE_BYPASS	BIT(0)
#define	CQSPI_REG_RD_DATA_CAPTURE_DELAY_LSB	1
#define	CQSPI_REG_RD_DATA_CAPTURE_DELAY_MASK	0xF

#define	CQSPI_REG_SIZE				0x14
#define	CQSPI_REG_SIZE_ADDRESS_LSB		0
#define	CQSPI_REG_SIZE_PAGE_LSB			4
#define	CQSPI_REG_SIZE_BLOCK_LSB		16
#define	CQSPI_REG_SIZE_ADDRESS_MASK		0xF
#define	CQSPI_REG_SIZE_PAGE_MASK		0xFFF
#define	CQSPI_REG_SIZE_BLOCK_MASK		0x3F

#define	CQSPI_REG_SRAMPARTITION			0x18
#define	CQSPI_REG_INDIRECTTRIGGER		0x1C

#define	CQSPI_REG_REMAP				0x24
#define	CQSPI_REG_MODE_BIT			0x28

#define	CQSPI_REG_SDRAMLEVEL			0x2C
#define	CQSPI_REG_SDRAMLEVEL_RD_LSB		0
#define	CQSPI_REG_SDRAMLEVEL_WR_LSB		16
#define	CQSPI_REG_SDRAMLEVEL_RD_MASK		0xFFFF
#define	CQSPI_REG_SDRAMLEVEL_WR_MASK		0xFFFF

#define	CQSPI_REG_IRQSTATUS			0x40
#define	CQSPI_REG_IRQMASK			0x44

#define	CQSPI_REG_INDIRECTRD			0x60
#define	CQSPI_REG_INDIRECTRD_START		BIT(0)
#define	CQSPI_REG_INDIRECTRD_CANCEL		BIT(1)
#define	CQSPI_REG_INDIRECTRD_INPROGRESS		BIT(2)
#define	CQSPI_REG_INDIRECTRD_DONE		BIT(5)

#define	CQSPI_REG_INDIRECTRDWATERMARK		0x64
#define	CQSPI_REG_INDIRECTRDSTARTADDR		0x68
#define	CQSPI_REG_INDIRECTRDBYTES		0x6C

#define	CQSPI_REG_CMDCTRL			0x90
#define	CQSPI_REG_CMDCTRL_EXECUTE		BIT(0)
#define	CQSPI_REG_CMDCTRL_INPROGRESS		BIT(1)
#define	CQSPI_REG_CMDCTRL_DUMMY_LSB		7
#define	CQSPI_REG_CMDCTRL_WR_BYTES_LSB		12
#define	CQSPI_REG_CMDCTRL_WR_EN_LSB		15
#define	CQSPI_REG_CMDCTRL_ADD_BYTES_LSB		16
#define	CQSPI_REG_CMDCTRL_ADDR_EN_LSB		19
#define	CQSPI_REG_CMDCTRL_RD_BYTES_LSB		20
#define	CQSPI_REG_CMDCTRL_RD_EN_LSB		23
#define	CQSPI_REG_CMDCTRL_OPCODE_LSB		24
#define	CQSPI_REG_CMDCTRL_DUMMY_MASK		0x1F
#define	CQSPI_REG_CMDCTRL_WR_BYTES_MASK		0x7
#define	CQSPI_REG_CMDCTRL_ADD_BYTES_MASK	0x3
#define	CQSPI_REG_CMDCTRL_RD_BYTES_MASK		0x7
#define	CQSPI_REG_CMDCTRL_OPCODE_MASK		0xFF

#define	CQSPI_REG_INDIRECTWR			0x70
#define	CQSPI_REG_INDIRECTWR_START		BIT(0)
#define	CQSPI_REG_INDIRECTWR_CANCEL		BIT(1)
#define	CQSPI_REG_INDIRECTWR_INPROGRESS		BIT(2)
#define	CQSPI_REG_INDIRECTWR_DONE		BIT(5)

#define	CQSPI_REG_INDIRECTWRWATERMARK		0x74
#define	CQSPI_REG_INDIRECTWRSTARTADDR		0x78
#define	CQSPI_REG_INDIRECTWRBYTES		0x7C

#define	CQSPI_REG_CMDADDRESS			0x94
#define	CQSPI_REG_CMDREADDATALOWER		0xA0
#define	CQSPI_REG_CMDREADDATAUPPER		0xA4
#define	CQSPI_REG_CMDWRITEDATALOWER		0xA8
#define	CQSPI_REG_CMDWRITEDATAUPPER		0xAC

#define CQSPI_REG_OE_LOWER 0xE0

#define CQSPI_REG_IS_IDLE(base)					\
	((readl(base + CQSPI_REG_CONFIG) >>		\
		CQSPI_REG_CONFIG_IDLE_LSB) & 0x1)

#define CQSPI_GET_RD_SRAM_LEVEL(reg_base)			\
	(((readl(reg_base + CQSPI_REG_SDRAMLEVEL)) >>	\
	CQSPI_REG_SDRAMLEVEL_RD_LSB) & CQSPI_REG_SDRAMLEVEL_RD_MASK)

#define CQSPI_GET_WR_SRAM_LEVEL(reg_base)			\
	(((readl(reg_base + CQSPI_REG_SDRAMLEVEL)) >>	\
	CQSPI_REG_SDRAMLEVEL_WR_LSB) & CQSPI_REG_SDRAMLEVEL_WR_MASK)

void cadence_qspi_apb_controller_enable(void *reg_base)
{
	unsigned int reg;
	reg = readl(reg_base + CQSPI_REG_CONFIG);
	reg |= CQSPI_REG_CONFIG_ENABLE;
	writel(reg, reg_base + CQSPI_REG_CONFIG);
}

void cadence_qspi_apb_controller_disable(void *reg_base)
{
	unsigned int reg;
	reg = readl(reg_base + CQSPI_REG_CONFIG);
	reg &= ~CQSPI_REG_CONFIG_ENABLE;
	writel(reg, reg_base + CQSPI_REG_CONFIG);
}

void cadence_qspi_apb_dac_mode_enable(void *reg_base)
{
	unsigned int reg;

	reg = readl(reg_base + CQSPI_REG_CONFIG);
	reg |= CQSPI_REG_CONFIG_DIRECT;
	writel(reg, reg_base + CQSPI_REG_CONFIG);
}

/* Return 1 if idle, otherwise return 0 (busy). */
static unsigned int cadence_qspi_wait_idle(void *reg_base)
{
	unsigned int start, count = 0;
	/* timeout in unit of ms */
	unsigned int timeout = 5000;

	start = get_timer(0);
	for ( ; get_timer(start) < timeout ; ) {
		if (CQSPI_REG_IS_IDLE(reg_base))
			count++;
		else
			count = 0;
		/*
		 * Ensure the QSPI controller is in true idle state after
		 * reading back the same idle status consecutively
		 */
		if (count >= CQSPI_POLL_IDLE_RETRY)
			return 1;
	}

	/* Timeout, still in busy mode. */
	printf("QSPI: QSPI is still busy after poll for %d times.\n",
	       CQSPI_REG_RETRY);
	return 0;
}

void cadence_qspi_apb_readdata_capture(void *reg_base,
				unsigned int bypass, unsigned int delay)
{
	unsigned int reg;
	cadence_qspi_apb_controller_disable(reg_base);

	reg = readl(reg_base + CQSPI_REG_RD_DATA_CAPTURE);

	if (bypass)
		reg |= CQSPI_REG_RD_DATA_CAPTURE_BYPASS;
	else
		reg &= ~CQSPI_REG_RD_DATA_CAPTURE_BYPASS;

	reg &= ~(CQSPI_REG_RD_DATA_CAPTURE_DELAY_MASK
		<< CQSPI_REG_RD_DATA_CAPTURE_DELAY_LSB);

	reg |= (delay & CQSPI_REG_RD_DATA_CAPTURE_DELAY_MASK)
		<< CQSPI_REG_RD_DATA_CAPTURE_DELAY_LSB;

	writel(reg, reg_base + CQSPI_REG_RD_DATA_CAPTURE);

	cadence_qspi_apb_controller_enable(reg_base);
}

void cadence_qspi_apb_config_baudrate_div(void *reg_base,
	unsigned int ref_clk_hz, unsigned int sclk_hz)
{
	unsigned int reg;
	unsigned int div;

	cadence_qspi_apb_controller_disable(reg_base);
	reg = readl(reg_base + CQSPI_REG_CONFIG);
	reg &= ~(CQSPI_REG_CONFIG_BAUD_MASK << CQSPI_REG_CONFIG_BAUD_LSB);

	/*
	 * The baud_div field in the config reg is 4 bits, and the ref clock is
	 * divided by 2 * (baud_div + 1). Round up the divider to ensure the
	 * SPI clock rate is less than or equal to the requested clock rate.
	 */
	div = DIV_ROUND_UP(ref_clk_hz, sclk_hz * 2) - 1;

	/* ensure the baud rate doesn't exceed the max value */
	if (div > CQSPI_REG_CONFIG_BAUD_MASK)
		div = CQSPI_REG_CONFIG_BAUD_MASK;

	debug("%s: ref_clk %dHz sclk %dHz Div 0x%x, actual %dHz\n", __func__,
	      ref_clk_hz, sclk_hz, div, ref_clk_hz / (2 * (div + 1)));

	reg |= (div << CQSPI_REG_CONFIG_BAUD_LSB);
	writel(reg, reg_base + CQSPI_REG_CONFIG);

	cadence_qspi_apb_controller_enable(reg_base);
}

void cadence_qspi_apb_set_clk_mode(void *reg_base, uint mode)
{
	unsigned int reg;
	struct cadence_spi_platdata *plat = cadence_get_plat();

	cadence_qspi_apb_controller_disable(reg_base);
	reg = readl(reg_base + CQSPI_REG_CONFIG);
	reg &= ~(CQSPI_REG_CONFIG_CLK_POL | CQSPI_REG_CONFIG_CLK_PHA);

	//On the MX66, DTR mode works best with CPOL=0 and CPHA=0
	if(!plat->use_dtr){
		if (mode & SPI_CPOL)
			reg |= CQSPI_REG_CONFIG_CLK_POL;
		if (mode & SPI_CPHA)
			reg |= CQSPI_REG_CONFIG_CLK_PHA;
	}

	writel(reg, reg_base + CQSPI_REG_CONFIG);

	cadence_qspi_apb_controller_enable(reg_base);
}

void cadence_qspi_apb_chipselect(void *reg_base,
	unsigned int chip_select, unsigned int decoder_enable)
{
	unsigned int reg;

	cadence_qspi_apb_controller_disable(reg_base);

	debug("%s : chipselect %d decode %d\n", __func__, chip_select,
	      decoder_enable);

	reg = readl(reg_base + CQSPI_REG_CONFIG);
	/* docoder */
	if (decoder_enable) {
		reg |= CQSPI_REG_CONFIG_DECODE;
	} else {
		reg &= ~CQSPI_REG_CONFIG_DECODE;
		/* Convert CS if without decoder.
		 * CS0 to 4b'1110
		 * CS1 to 4b'1101
		 * CS2 to 4b'1011
		 * CS3 to 4b'0111
		 */
		chip_select = 0xF & ~(1 << chip_select);
	}

	reg &= ~(CQSPI_REG_CONFIG_CHIPSELECT_MASK
			<< CQSPI_REG_CONFIG_CHIPSELECT_LSB);
	reg |= (chip_select & CQSPI_REG_CONFIG_CHIPSELECT_MASK)
			<< CQSPI_REG_CONFIG_CHIPSELECT_LSB;
	writel(reg, reg_base + CQSPI_REG_CONFIG);

	cadence_qspi_apb_controller_enable(reg_base);
}

void cadence_qspi_apb_delay(void *reg_base,
	unsigned int ref_clk, unsigned int sclk_hz,
	unsigned int tshsl_ns, unsigned int tsd2d_ns,
	unsigned int tchsh_ns, unsigned int tslch_ns)
{
	unsigned int ref_clk_ns;
	unsigned int sclk_ns;
	unsigned int tshsl, tchsh, tslch, tsd2d;
	unsigned int reg;

	cadence_qspi_apb_controller_disable(reg_base);

	/* Convert to ns. */
	ref_clk_ns = DIV_ROUND_UP(1000000000, ref_clk);

	/* Convert to ns. */
	sclk_ns = DIV_ROUND_UP(1000000000, sclk_hz);

	/* The controller adds additional delay to that programmed in the reg */
	if (tshsl_ns >= sclk_ns + ref_clk_ns)
		tshsl_ns -= sclk_ns + ref_clk_ns;
	if (tchsh_ns >= sclk_ns + 3 * ref_clk_ns)
		tchsh_ns -= sclk_ns + 3 * ref_clk_ns;
	tshsl = DIV_ROUND_UP(tshsl_ns, ref_clk_ns);
	tchsh = DIV_ROUND_UP(tchsh_ns, ref_clk_ns);
	tslch = DIV_ROUND_UP(tslch_ns, ref_clk_ns);
	tsd2d = DIV_ROUND_UP(tsd2d_ns, ref_clk_ns);

	reg = ((tshsl & CQSPI_REG_DELAY_TSHSL_MASK)
			<< CQSPI_REG_DELAY_TSHSL_LSB);
	reg |= ((tchsh & CQSPI_REG_DELAY_TCHSH_MASK)
			<< CQSPI_REG_DELAY_TCHSH_LSB);
	reg |= ((tslch & CQSPI_REG_DELAY_TSLCH_MASK)
			<< CQSPI_REG_DELAY_TSLCH_LSB);
	reg |= ((tsd2d & CQSPI_REG_DELAY_TSD2D_MASK)
			<< CQSPI_REG_DELAY_TSD2D_LSB);
	writel(reg, reg_base + CQSPI_REG_DELAY);

	cadence_qspi_apb_controller_enable(reg_base);
}

void cadence_qspi_apb_controller_init(struct cadence_spi_platdata *plat)
{
	unsigned reg;

	cadence_qspi_apb_controller_disable(plat->regbase);

	/* Configure the device size and address bytes */
	reg = readl(plat->regbase + CQSPI_REG_SIZE);
	/* Clear the previous value */
	reg &= ~(CQSPI_REG_SIZE_PAGE_MASK << CQSPI_REG_SIZE_PAGE_LSB);
	reg &= ~(CQSPI_REG_SIZE_BLOCK_MASK << CQSPI_REG_SIZE_BLOCK_LSB);
	reg |= (plat->page_size << CQSPI_REG_SIZE_PAGE_LSB);
	reg |= (plat->block_size << CQSPI_REG_SIZE_BLOCK_LSB);
	writel(reg, plat->regbase + CQSPI_REG_SIZE);

	/* Configure the remap address register, no remap */
	writel(0, plat->regbase + CQSPI_REG_REMAP);

	/* Indirect mode configurations */
	writel(plat->fifo_depth / 2, plat->regbase + CQSPI_REG_SRAMPARTITION);

	/* Disable all interrupts */
	writel(0, plat->regbase + CQSPI_REG_IRQMASK);

	cadence_qspi_apb_controller_enable(plat->regbase);
}

static int cadence_qspi_apb_exec_flash_cmd(void *reg_base,
	unsigned int reg)
{
	unsigned int retry = CQSPI_REG_RETRY;

	/* Write the CMDCTRL without start execution. */
	writel(reg, reg_base + CQSPI_REG_CMDCTRL);
	/* Start execute */
	reg |= CQSPI_REG_CMDCTRL_EXECUTE;
	writel(reg, reg_base + CQSPI_REG_CMDCTRL);

	while (retry--) {
		reg = readl(reg_base + CQSPI_REG_CMDCTRL);
		if ((reg & CQSPI_REG_CMDCTRL_INPROGRESS) == 0)
			break;
		udelay(1);
	}

	if (!retry) {
		printf("QSPI: flash command execution timeout\n");
		return -EIO;
	}

	/* Polling QSPI idle status. */
	if (!cadence_qspi_wait_idle(reg_base))
		return -EIO;

	return 0;
}

/* For command RDID, RDSR. */
int cadence_qspi_apb_command_read(void *reg_base, const struct spi_mem_op *op)
{
	unsigned int reg, curVal;
	unsigned int read_len;
	int status;
	unsigned int rxlen = op->data.nbytes;
	void *rxbuf = op->data.buf.in;
	static char tempBuf[8];
	unsigned int op2;
	u8 dummy;
	int i;
	struct cadence_spi_platdata *plat = cadence_get_plat();

	if (rxlen > CQSPI_STIG_DATA_LEN_MAX || !rxbuf) {
		printf("QSPI: Invalid input arguments rxlen %u\n", rxlen);
		return -EINVAL;
	}

	reg = op->cmd.opcode << CQSPI_REG_CMDCTRL_OPCODE_LSB;

	reg |= (0x1 << CQSPI_REG_CMDCTRL_RD_EN_LSB);

	// Set Dummy cycles as per datasheet
	if (op->dummy.nbytes && plat->cadenceMode != CADENCE_OSPI_MODE)
		dummy = (op->dummy.nbytes * 8 / op->dummy.buswidth);
	else
		dummy = plat->stig_read_dummy;

	reg |= ((dummy << BITP_OSPI_FCCTL_DMY) & BITM_OSPI_FCCTL_DMY);

	//Handle reading while in OPI mode
	//Handle reading with an address in SPI mode
	if(plat->cadenceMode == CADENCE_OSPI_MODE){

		if(plat->use_dtr){
			rxlen *= 2;
			if(rxlen > 8) // 8 bytes maximum
				rxlen = 8;

			rxbuf = tempBuf;
		}

		if(plat->id != 0x195a9d){ //SC594, IS25LX256

			// Set read instruction type to OPI
			curVal = readl(reg_base + CQSPI_REG_RD_INSTR);
			curVal |= (3UL << BITP_OSPI_DRICTL_INSTRTYP);
			curVal |= (3UL << BITP_OSPI_DRICTL_ADDRTRNSFR);
			curVal |= (3UL << BITP_OSPI_DRICTL_DATATRNSFR);
			writel(curVal, reg_base + CQSPI_REG_RD_INSTR);

			// Enable 4 address bytes
			reg |= (0x1 << CQSPI_REG_CMDCTRL_ADDR_EN_LSB);
			reg |= ((3) << CQSPI_REG_CMDCTRL_ADD_BYTES_LSB);
			//writel(op->addr.val, reg_base + CQSPI_REG_CMDADDRESS);
		}

		// Configure the dual opcode
		curVal = readl(reg_base + CQSPI_REG_OE_LOWER);
		curVal &= ~BITM_OSPI_OE_LWR_XSTIGBYT;
		if(plat->use_opcode2){
			switch(op->cmd.opcode){
				default:
					if(plat->use_opcode2_invert){
						op2 = 0xFF - op->cmd.opcode;
					}else{
						op2 = op->cmd.opcode;
					}				
					break;
			}
			curVal |= ((uint32_t)(op2) << BITP_OSPI_OE_LWR_XSTIGBYT) & BITM_OSPI_OE_LWR_XSTIGBYT;
		}
		writel(curVal, reg_base + CQSPI_REG_OE_LOWER);
	}else if(op->addr.nbytes){
		reg |= (0x1 << CQSPI_REG_CMDCTRL_ADDR_EN_LSB);
		reg |= ((op->addr.nbytes-1) << CQSPI_REG_CMDCTRL_ADD_BYTES_LSB);
		writel(op->addr.val, reg_base + CQSPI_REG_CMDADDRESS);
	}

	/* 0 means 1 byte. */
	reg |= (((rxlen - 1) & CQSPI_REG_CMDCTRL_RD_BYTES_MASK)
		<< CQSPI_REG_CMDCTRL_RD_BYTES_LSB);
	status = cadence_qspi_apb_exec_flash_cmd(reg_base, reg);
	if (status != 0)
		return status;

	reg = readl(reg_base + CQSPI_REG_CMDREADDATALOWER);

	/* Put the read value into rx_buf */
	read_len = (rxlen > 4) ? 4 : rxlen;
	memcpy(rxbuf, &reg, read_len);
	rxbuf += read_len;

	if (rxlen > 4) {
		reg = readl(reg_base + CQSPI_REG_CMDREADDATAUPPER);

		read_len = rxlen - read_len;
		memcpy(rxbuf, &reg, read_len);
	}

	if(plat->cadenceMode == CADENCE_OSPI_MODE && plat->use_dtr){
		for(i = 0; i < rxlen/2; i++){
			((char*)op->data.buf.in)[i] = ((char*)tempBuf)[i*2];
		}
	}

	return 0;
}

/* For commands: WRSR, WREN, WRDI, CHIP_ERASE, BE, etc. */
int cadence_qspi_apb_command_write(void *reg_base, const struct spi_mem_op *op)
{
	unsigned int reg = 0, curVal;
	unsigned int wr_data;
	unsigned int wr_len;
	unsigned int txlen = op->data.nbytes;
	const void *txbuf = op->data.buf.out;
	u32 addr;
	unsigned int op1, op2;
	struct cadence_spi_platdata *plat = cadence_get_plat();
	op1 = op->cmd.opcode;

	/* Reorder address to SPI bus order if only transferring address */
	if (!txlen) {
		addr = cpu_to_be32(op->addr.val);
		if (op->addr.nbytes == 3)
			addr >>= 8;
		txbuf = &addr;
		txlen = op->addr.nbytes;
	}else if(op->addr.nbytes){
		reg |= (0x1 << CQSPI_REG_CMDCTRL_ADDR_EN_LSB);
		reg |= ((op->addr.nbytes-1) << CQSPI_REG_CMDCTRL_ADD_BYTES_LSB);
		writel(op->addr.val, reg_base + CQSPI_REG_CMDADDRESS);
	}

	if(plat->cadenceMode == CADENCE_OSPI_MODE){

		if(plat->id != 0x195a9d){ //SC594, IS25LX256
			// Set read instruction type to OPI
			curVal = readl(reg_base + CQSPI_REG_RD_INSTR);
			curVal |= (3UL << BITP_OSPI_DRICTL_INSTRTYP);
			curVal &= ~(3UL << BITP_OSPI_DRICTL_ADDRTRNSFR);
			curVal &= ~(3UL << BITP_OSPI_DRICTL_DATATRNSFR);
			writel(curVal, reg_base + CQSPI_REG_RD_INSTR);

			// Set write instruction type to OPI
			curVal = readl(reg_base + CQSPI_REG_WR_INSTR);
			curVal |= (3UL << BITP_OSPI_DWICTL_ADDRTRNSFR) |
					  (3UL << BITP_OSPI_DWICTL_DATATRNSFR);
			writel(curVal, reg_base + CQSPI_REG_WR_INSTR);
		}

		// Configure the dual opcode
		curVal = readl(reg_base + CQSPI_REG_OE_LOWER);
		curVal &= ~BITM_OSPI_OE_LWR_XSTIGBYT;
		if(plat->use_opcode2){
			switch(op->cmd.opcode){
				case 0xD8: //Block Erases
				case 0xDC:
					op1 = 0xDC;
					op2 = 0x23;
					break;
				case 0xB7: //Enter/Exit 4 byte mode --
				case 0xE9: //not necessary if we're in OPI already
					return 0;
				default:
					if(plat->use_opcode2_invert){
						op2 = 0xFF - op->cmd.opcode;
					}else{
						op2 = op->cmd.opcode;
					}
					break;
			}
		}
		curVal = ((uint32_t)(op2) << BITP_OSPI_OE_LWR_XSTIGBYT) & BITM_OSPI_OE_LWR_XSTIGBYT;
		writel(curVal, reg_base + CQSPI_REG_OE_LOWER);
	}

	if (txlen > CQSPI_STIG_DATA_LEN_MAX) {
		printf("QSPI: Invalid input arguments txlen %u\n", txlen);
		return -EINVAL;
	}

	reg |= op1 << CQSPI_REG_CMDCTRL_OPCODE_LSB;

	if (txlen) {
		/* writing data = yes */
		reg |= (0x1 << CQSPI_REG_CMDCTRL_WR_EN_LSB);
		reg |= ((txlen - 1) & CQSPI_REG_CMDCTRL_WR_BYTES_MASK)
			<< CQSPI_REG_CMDCTRL_WR_BYTES_LSB;

		wr_len = txlen > 4 ? 4 : txlen;
		memcpy(&wr_data, txbuf, wr_len);
		writel(wr_data, reg_base +
			CQSPI_REG_CMDWRITEDATALOWER);

		if (txlen > 4) {
			txbuf += wr_len;
			wr_len = txlen - wr_len;
			memcpy(&wr_data, txbuf, wr_len);
			writel(wr_data, reg_base +
				CQSPI_REG_CMDWRITEDATAUPPER);
		}
	}

	/* Execute the command */
	return cadence_qspi_apb_exec_flash_cmd(reg_base, reg);
}

/* Opcode + Address (3/4 bytes) + dummy bytes (0-4 bytes) */
int cadence_qspi_apb_read_setup(struct cadence_spi_platdata *plat,
				const struct spi_mem_op *op)
{
	unsigned int reg;
	unsigned int rd_reg;
	unsigned int dummy_clk;
	unsigned int dummy_bytes = op->dummy.nbytes;

	/* Setup the indirect trigger address */
	writel(plat->trigger_address,
	       plat->regbase + CQSPI_REG_INDIRECTTRIGGER);

	/* Configure the opcode */
	rd_reg = op->cmd.opcode << CQSPI_REG_RD_INSTR_OPCODE_LSB;

	if (op->data.buswidth == 8)
		/* Instruction and address at DQ0, data at DQ0-7. */
		rd_reg |= CQSPI_INST_TYPE_OCTAL << CQSPI_REG_RD_INSTR_TYPE_DATA_LSB;
	else if (op->data.buswidth == 4)
		/* Instruction and address at DQ0, data at DQ0-3. */
		rd_reg |= CQSPI_INST_TYPE_QUAD << CQSPI_REG_RD_INSTR_TYPE_DATA_LSB;

	writel(op->addr.val, plat->regbase + CQSPI_REG_INDIRECTRDSTARTADDR);

	if (dummy_bytes) {
		if (dummy_bytes > CQSPI_DUMMY_BYTES_MAX)
			dummy_bytes = CQSPI_DUMMY_BYTES_MAX;

		/* Convert to clock cycles. */
		dummy_clk = dummy_bytes * CQSPI_DUMMY_CLKS_PER_BYTE;

		if (dummy_clk)
			rd_reg |= (dummy_clk & CQSPI_REG_RD_INSTR_DUMMY_MASK)
				<< CQSPI_REG_RD_INSTR_DUMMY_LSB;
	}

	writel(rd_reg, plat->regbase + CQSPI_REG_RD_INSTR);

	/* set device size */
	reg = readl(plat->regbase + CQSPI_REG_SIZE);
	reg &= ~CQSPI_REG_SIZE_ADDRESS_MASK;
	reg |= (op->addr.nbytes - 1);
	writel(reg, plat->regbase + CQSPI_REG_SIZE);
	return 0;
}

static u32 cadence_qspi_get_rd_sram_level(struct cadence_spi_platdata *plat)
{
	u32 reg = readl(plat->regbase + CQSPI_REG_SDRAMLEVEL);
	reg >>= CQSPI_REG_SDRAMLEVEL_RD_LSB;
	return reg & CQSPI_REG_SDRAMLEVEL_RD_MASK;
}

static int cadence_qspi_wait_for_data(struct cadence_spi_platdata *plat)
{
	unsigned int timeout = 10000;
	u32 reg;

	while (timeout--) {
		reg = cadence_qspi_get_rd_sram_level(plat);
		if (reg)
			return reg;
		udelay(1);
	}

	return -ETIMEDOUT;
}

static int
cadence_qspi_apb_indirect_read_execute(struct cadence_spi_platdata *plat,
				       unsigned int n_rx, u8 *rxbuf)
{
	unsigned int remaining = n_rx;
	unsigned int bytes_to_read = 0;
	int ret;

	writel(n_rx, plat->regbase + CQSPI_REG_INDIRECTRDBYTES);

	/* Start the indirect read transfer */
	writel(CQSPI_REG_INDIRECTRD_START,
	       plat->regbase + CQSPI_REG_INDIRECTRD);

	while (remaining > 0) {
		ret = cadence_qspi_wait_for_data(plat);
		if (ret < 0) {
			printf("Indirect write timed out (%i)\n", ret);
			goto failrd;
		}

		bytes_to_read = ret;

		while (bytes_to_read != 0) {
			bytes_to_read *= plat->fifo_width;
			bytes_to_read = bytes_to_read > remaining ?
					remaining : bytes_to_read;
			/*
			 * Handle non-4-byte aligned access to avoid
			 * data abort.
			 */
			if (((uintptr_t)rxbuf % 4) || (bytes_to_read % 4))
				readsb(plat->ahbbase, rxbuf, bytes_to_read);
			else
				readsl(plat->ahbbase, rxbuf,
				       bytes_to_read >> 2);
			rxbuf += bytes_to_read;
			remaining -= bytes_to_read;
			bytes_to_read = cadence_qspi_get_rd_sram_level(plat);
		}
	}

	/* Check indirect done status */
	ret = wait_for_bit_le32(plat->regbase + CQSPI_REG_INDIRECTRD,
				CQSPI_REG_INDIRECTRD_DONE, 1, 10, 0);
	if (ret) {
		printf("Indirect read completion error (%i)\n", ret);
		goto failrd;
	}

	/* Clear indirect completion status */
	writel(CQSPI_REG_INDIRECTRD_DONE,
	       plat->regbase + CQSPI_REG_INDIRECTRD);

	return 0;

failrd:
	/* Cancel the indirect read */
	writel(CQSPI_REG_INDIRECTRD_CANCEL,
	       plat->regbase + CQSPI_REG_INDIRECTRD);
	return ret;
}

int cadence_qspi_apb_read_execute(struct cadence_spi_platdata *plat,
				  const struct spi_mem_op *op)
{
	u64 from = op->addr.val;
	void *buf = op->data.buf.in;
	size_t len = op->data.nbytes;

	if (plat->use_dac_mode && (from + len < plat->ahbsize)) {
		if (len < 256 ||
		    dma_memcpy(buf, plat->ahbbase + from, len) < 0) {
			memcpy_fromio(buf, plat->ahbbase + from, len);
		}
		if (!cadence_qspi_wait_idle(plat->regbase))
			return -EIO;
		return 0;
	}

	return cadence_qspi_apb_indirect_read_execute(plat, len, buf);
}

/* Opcode + Address (3/4 bytes) */
int cadence_qspi_apb_write_setup(struct cadence_spi_platdata *plat,
				 const struct spi_mem_op *op)
{
	unsigned int reg;

	/* Setup the indirect trigger address */
	writel(plat->trigger_address,
	       plat->regbase + CQSPI_REG_INDIRECTTRIGGER);

	/* Configure the opcode */
	reg = op->cmd.opcode << CQSPI_REG_WR_INSTR_OPCODE_LSB;
	writel(reg, plat->regbase + CQSPI_REG_WR_INSTR);

	writel(op->addr.val, plat->regbase + CQSPI_REG_INDIRECTWRSTARTADDR);

	reg = readl(plat->regbase + CQSPI_REG_SIZE);
	reg &= ~CQSPI_REG_SIZE_ADDRESS_MASK;
	reg |= (op->addr.nbytes - 1);
	writel(reg, plat->regbase + CQSPI_REG_SIZE);
	return 0;
}

static int
cadence_qspi_apb_indirect_write_execute(struct cadence_spi_platdata *plat,
					unsigned int n_tx, const u8 *txbuf)
{
	unsigned int page_size = plat->page_size;
	unsigned int remaining = n_tx;
	const u8 *bb_txbuf = txbuf;
	void *bounce_buf = NULL;
	unsigned int write_bytes;
	int ret;

	/*
	 * Use bounce buffer for non 32 bit aligned txbuf to avoid data
	 * aborts
	 */
	if ((uintptr_t)txbuf % 4) {
		bounce_buf = malloc(n_tx);
		if (!bounce_buf)
			return -ENOMEM;
		memcpy(bounce_buf, txbuf, n_tx);
		bb_txbuf = bounce_buf;
	}

	/* Configure the indirect read transfer bytes */
	writel(n_tx, plat->regbase + CQSPI_REG_INDIRECTWRBYTES);

	/* Start the indirect write transfer */
	writel(CQSPI_REG_INDIRECTWR_START,
	       plat->regbase + CQSPI_REG_INDIRECTWR);

	while (remaining > 0) {
		write_bytes = remaining > page_size ? page_size : remaining;
		writesl(plat->ahbbase, bb_txbuf, write_bytes >> 2);
		if (write_bytes % 4)
			writesb(plat->ahbbase,
				bb_txbuf + rounddown(write_bytes, 4),
				write_bytes % 4);

		ret = wait_for_bit_le32(plat->regbase + CQSPI_REG_SDRAMLEVEL,
					CQSPI_REG_SDRAMLEVEL_WR_MASK <<
					CQSPI_REG_SDRAMLEVEL_WR_LSB, 0, 10, 0);
		if (ret) {
			printf("Indirect write timed out (%i)\n", ret);
			goto failwr;
		}

		bb_txbuf += write_bytes;
		remaining -= write_bytes;
	}

	/* Check indirect done status */
	ret = wait_for_bit_le32(plat->regbase + CQSPI_REG_INDIRECTWR,
				CQSPI_REG_INDIRECTWR_DONE, 1, 10, 0);
	if (ret) {
		printf("Indirect write completion error (%i)\n", ret);
		goto failwr;
	}

	/* Clear indirect completion status */
	writel(CQSPI_REG_INDIRECTWR_DONE,
	       plat->regbase + CQSPI_REG_INDIRECTWR);
	if (bounce_buf)
		free(bounce_buf);
	return 0;

failwr:
	/* Cancel the indirect write */
	writel(CQSPI_REG_INDIRECTWR_CANCEL,
	       plat->regbase + CQSPI_REG_INDIRECTWR);
	if (bounce_buf)
		free(bounce_buf);
	return ret;
}

int cadence_qspi_apb_write_execute(struct cadence_spi_platdata *plat,
				   const struct spi_mem_op *op)
{
	u32 to = op->addr.val;
	const void *buf = op->data.buf.out;
	size_t len = op->data.nbytes;

	if (plat->use_dac_mode && (to + len < plat->ahbsize)) {
		memcpy_toio(plat->ahbbase + to, buf, len);
		if (!cadence_qspi_wait_idle(plat->regbase))
			return -EIO;
		return 0;
	}

	return cadence_qspi_apb_indirect_write_execute(plat, len, buf);
}

void cadence_qspi_apb_enter_xip(void *reg_base, char xip_dummy)
{
	unsigned int reg;

	/* enter XiP mode immediately and enable direct mode */
	reg = readl(reg_base + CQSPI_REG_CONFIG);
	reg |= CQSPI_REG_CONFIG_ENABLE;
	reg |= CQSPI_REG_CONFIG_DIRECT;
	reg |= CQSPI_REG_CONFIG_XIP_IMM;
	writel(reg, reg_base + CQSPI_REG_CONFIG);

	/* keep the XiP mode */
	writel(xip_dummy, reg_base + CQSPI_REG_MODE_BIT);

	/* Enable mode bit at devrd */
	reg = readl(reg_base + CQSPI_REG_RD_INSTR);
	reg |= (1 << CQSPI_REG_RD_INSTR_MODE_EN_LSB);
	writel(reg, reg_base + CQSPI_REG_RD_INSTR);
}

#if defined(CONFIG_SC59X) || defined (CONFIG_SC59X_64)

#define SCB5_SPI2_OSPI_REMAP 0x30400000
#define OSPI0_MMAP_ADDRESS 0x60000000

#define ADI_OCTAL_USE_DMA

#ifdef CONFIG_SPL_BUILD
void cadence_ospi_append_chipinfo(char * bootcmd){
	static char currentMode[32];
	struct cadence_spi_platdata *plat;
	char * bootargs;

	plat = cadence_get_plat();

	if(plat->cadenceMode == CADENCE_OSPI_MODE){
		if(plat->use_dtr){
			sprintf(currentMode, "dtr\0");
		}else{
			sprintf(currentMode, "str\0");
		}
	}else{
		sprintf(currentMode, "spi\0");
	}

	sprintf(bootcmd+strlen(bootcmd), " cadence-quadspi.ospi_id=%d cadence-quadspi.ospi_mode=%s\0",
										plat->id, currentMode);
}
#else
void cadence_ospi_append_chipinfo(){
	static char newArgs[2048];
	static char currentMode[32];
	struct cadence_spi_platdata *plat;
	char * bootargs;

	plat = cadence_get_plat();

	bootargs = env_get("bootargs") & 0xFFFFFFFF;

	if (bootargs && (bootargs[0] != '\0')) {
		if(plat->cadenceMode == CADENCE_OSPI_MODE){
			if(plat->use_dtr){
				sprintf(currentMode, "dtr\0");
			}else{
				sprintf(currentMode, "str\0");
			}
		}else{
			sprintf(currentMode, "spi\0");
		}

		snprintf(newArgs, sizeof(newArgs), "%s cadence-quadspi.ospi_id=%d cadence-quadspi.ospi_mode=%s\0", bootargs,
											plat->id, currentMode);
		env_set("bootargs", newArgs);
	}
}
#endif

int cadence_qspi_setup_octal_read(struct cadence_spi_platdata *plat){
	unsigned int curVal;

	// Clear the contents of the Read instruction register
	writel(0, plat->regbase + CQSPI_REG_RD_INSTR);

	// Clear the DDREN in read instruction control
	curVal = readl(plat->regbase + CQSPI_REG_RD_INSTR);
	if(plat->cadenceMode == CADENCE_OSPI_MODE && plat->use_dtr)
		curVal |= BITM_OSPI_DRICTL_DDREN;
	else
		curVal &= ~(BITM_OSPI_DRICTL_DDREN);
	writel(curVal, plat->regbase + CQSPI_REG_RD_INSTR);

	// Clear the DTR mode bits in read instruction
	curVal = readl(plat->regbase + CQSPI_REG_CONFIG);
	if(plat->cadenceMode == CADENCE_OSPI_MODE && plat->use_dtr)
		curVal |= BITM_OSPI_CTL_DTREN;
	else
		curVal &= ~(BITM_OSPI_CTL_DTREN);
	writel(curVal, plat->regbase + CQSPI_REG_CONFIG);

	/* enable DAC controller */
	curVal = readl(plat->regbase + CQSPI_REG_CONFIG);
	curVal |= BITM_OSPI_CTL_DACEN;
	writel(curVal, plat->regbase + CQSPI_REG_CONFIG);
}

int cadence_qspi_setup_octal_write(struct cadence_spi_platdata *plat){
	unsigned int curVal;

	/* Configure the address mode of flash */
	curVal = readl(plat->regbase + CQSPI_REG_SIZE);
	curVal &= ~BITM_OSPI_DSCTL_ADDRSZ;
	curVal |= (((uint32_t)(4U)-1U) << BITP_OSPI_DSCTL_ADDRSZ) & BITM_OSPI_DSCTL_ADDRSZ;
	writel(curVal, plat->regbase + CQSPI_REG_SIZE);

	/* Configure the modedata value */
	curVal = readl(plat->regbase + CQSPI_REG_MODE_BIT);
	curVal &= ~BITM_OSPI_MBCTL_MODE;
	curVal |= ((uint32_t)(0) << BITP_OSPI_MBCTL_MODE) & BITM_OSPI_MBCTL_MODE;
	writel(curVal, plat->regbase + CQSPI_REG_MODE_BIT);

	/* enable DAC controller */
	curVal = readl(plat->regbase + CQSPI_REG_CONFIG);
	curVal |= BITM_OSPI_CTL_DACEN;
	writel(curVal, plat->regbase + CQSPI_REG_CONFIG);

	/* configure the WEL values to use non-inverted opcodes*/
	if (plat->use_opcode2) {
		// By default WEL inverts the WREN opcode, fix it in the non-inverted case
		if (!plat->use_opcode2_invert) {
			u32 wel = (SPINOR_OP_WREN << 24) | (SPINOR_OP_WREN << 16);
			writel(0x06060000, plat->regbase + 0xe4);
		}
	}
}

int cadence_qspi_setup_octal(struct cadence_spi_platdata *plat){
	unsigned int reg;
	unsigned int curVal;

	//On the MX66, DTR mode works best with CPOL=0 and CPHA=0
	if(plat->use_dtr){
		curVal = readl(plat->regbase + CQSPI_REG_CONFIG);
		curVal &= ~(CQSPI_REG_CONFIG_CLK_POL | CQSPI_REG_CONFIG_CLK_PHA);
		writel(curVal, plat->regbase + CQSPI_REG_CONFIG);
	}

	if(plat->use_opcode2){ 
		/* Make sure the dual op-code is enabled */
		curVal = readl(plat->regbase + CQSPI_REG_CONFIG);
		curVal |= BITM_OSPI_CTL_OPCODEEN;
		writel(curVal, plat->regbase + CQSPI_REG_CONFIG);
	}else{
		/* Make sure the dual op-code is not enabled */
		curVal = readl(plat->regbase + CQSPI_REG_CONFIG);
		curVal &= ~BITM_OSPI_CTL_OPCODEEN;
		writel(curVal, plat->regbase + CQSPI_REG_CONFIG);
	}

	if(plat->use_dtr){
		curVal = readl(plat->regbase + CQSPI_REG_RD_DATA_CAPTURE);
		curVal &= ~BITM_OSPI_RDC_DLYRD;
		curVal &= ~BITM_OSPI_RDC_DDRDLYRD;
		curVal |= CQSPI_REG_RD_DATA_CAPTURE_BYPASS;
		curVal |= plat->dly_rd << BITP_OSPI_RDC_DLYRD;
		curVal |= plat->ddr_dly_rd << BITP_OSPI_RDC_DDRDLYRD;
		curVal = writel(curVal, plat->regbase + CQSPI_REG_RD_DATA_CAPTURE);
	}

	/* Configure the address mode of flash */
	curVal = readl(plat->regbase + CQSPI_REG_SIZE);
	curVal &= ~BITM_OSPI_DSCTL_ADDRSZ;
	curVal |= (((uint32_t)(4U)-1U) << BITP_OSPI_DSCTL_ADDRSZ) & BITM_OSPI_DSCTL_ADDRSZ;
	writel(curVal, plat->regbase + CQSPI_REG_SIZE);

	// Configure the modedata value
	curVal = readl(plat->regbase + CQSPI_REG_MODE_BIT);
	curVal &= ~BITM_OSPI_MBCTL_MODE;
	curVal |= ((uint32_t)(0) << BITP_OSPI_MBCTL_MODE) & BITM_OSPI_MBCTL_MODE;
	writel(curVal, plat->regbase + CQSPI_REG_MODE_BIT);

	//Disable automatic polling in WCCTL and erase opcode
	writel(1 << BITP_OSPI_WCCTL_DIS, plat->regbase + 0x38);
}

int cadence_enter_octal_is25(struct cadence_spi_platdata *plat, struct spi_mem_op * wren,
								struct spi_mem_op * opTemp, char * id_spi){
	char data[1] = {0xff};
	opTemp->cmd.opcode = 0x81;
	data[0] = 0xC7;
	opTemp->data.buf.out = data;
	opTemp->data.nbytes = 1;
	cadence_qspi_apb_command_write(plat->regbase, opTemp);

	//TODO: Read back and check for errors, like cadence_enter_octal_mx66() function

	plat->cadenceMode = CADENCE_OSPI_MODE;
	cadence_qspi_setup_octal(plat);
	cadence_qspi_setup_octal_read(plat);
	cadence_qspi_setup_octal_write(plat);
	return 0;
}

int cadence_enter_octal_mx66(struct cadence_spi_platdata *plat, struct spi_mem_op * wren,
								struct spi_mem_op * opTemp, char * id_spi){

	char id_opi[4] = {0xff, 0xff, 0xff, 0x00};
	char conf_reg2[1] = {0xff};
	unsigned int curVal;

	//Check if the read control register is already configured for OPI mode
	curVal = readl(plat->regbase + CQSPI_REG_RD_INSTR);
	curVal &= (3UL << BITP_OSPI_DRICTL_INSTRTYP) |
			  (3UL << BITP_OSPI_DRICTL_ADDRTRNSFR) |
			  (3UL << BITP_OSPI_DRICTL_DATATRNSFR);

	if(!curVal){
		//Enter Octal Mode - Write Configuration Register 2
		if(plat->use_dtr)
			conf_reg2[0] = 0x02;
		else
			conf_reg2[0] = 0x01;
		opTemp->cmd.opcode = 0x72;
		opTemp->addr.val = 0x0;
		opTemp->data.nbytes = 1;
		opTemp->addr.nbytes = 4;
		opTemp->data.buf.out = conf_reg2;
		udelay(100000);
		cadence_qspi_apb_command_write(plat->regbase, wren);
		udelay(100000);
		cadence_qspi_apb_command_write(plat->regbase, opTemp);
		udelay(100000);
	}

	plat->cadenceMode = CADENCE_OSPI_MODE;
	cadence_qspi_setup_octal(plat);
	cadence_qspi_setup_octal_read(plat);
	cadence_qspi_setup_octal_write(plat);

	//RDID - OPI Mode		
	opTemp->cmd.opcode = SPINOR_OP_RDID;
	opTemp->addr.val = 0x0;
	opTemp->addr.nbytes = 4;
	opTemp->data.nbytes = 3;
	opTemp->data.buf.out = id_opi;
	cadence_qspi_apb_command_read(plat->regbase, opTemp);
	if(plat->use_dtr)
		printf("Read ID via 8x OPI+DTR: %x %x %x\n", id_opi[0], id_opi[1], id_opi[2]);
	else
		printf("Read ID via 8x OPI+STR: %x %x %x\n", id_opi[0], id_opi[1], id_opi[2]);

	if(strncmp(id_spi, id_opi, 3) != 0){
		printf("\tError: ID Mismatch!\n");
		return -1;
	}else{
		printf("\tSuccess: ID Match!\n");
	}

	//Read Configuration Register 2 @ 0x0
	opTemp->cmd.opcode = 0x71;
	opTemp->addr.val = 0x0;
	opTemp->addr.nbytes = 4;
	opTemp->data.nbytes = 1;
	opTemp->data.buf.out = conf_reg2;
	conf_reg2[0] = 0xff;
	cadence_qspi_apb_command_read(plat->regbase, opTemp);
	printf("Configuration Register 2(0x0): %x\n", conf_reg2[0]);

	if(plat->use_dtr){
		if(conf_reg2[0] != 2){
			printf("\tError: Not in DTR OPI Mode!\n");
			return -1;
		}else{
			printf("\tSuccess: In DTR OPI Mode!\n");
		}
	}else{
		if(conf_reg2[0] != 1){
			printf("\tError: Not in STR OPI Mode!\n");
			return -1;
		}else{
			printf("\tSuccess: In STR OPI Mode!\n");
		}
	}
	return 0;
}

int cadence_configure_opi_mode(struct cadence_spi_platdata *plat){

	struct spi_mem_op wren;
	char id_spi[4] = {0xff, 0xff, 0xff, 0x00};
	char id_opi[4] = {0xff, 0xff, 0xff, 0x00};
	struct spi_mem_op opTemp = SPI_MEM_OP(SPI_MEM_OP_CMD(SPINOR_OP_RDID, 1),
		SPI_MEM_OP_NO_ADDR,
		SPI_MEM_OP_NO_DUMMY,
		SPI_MEM_OP_DATA_IN(3, NULL, 1));
	int curVal;

	//WREN
	wren.data.nbytes = 0;
	wren.addr.nbytes = 0;
	wren.cmd.opcode = SPINOR_OP_WREN;

	//RDID - SPI Mode
	opTemp.data.buf.out = id_spi;

	//Check if the read control register is already configured for OPI mode
	curVal = readl(plat->regbase + CQSPI_REG_RD_INSTR);
	curVal &= (3UL << BITP_OSPI_DRICTL_INSTRTYP) |
			  (3UL << BITP_OSPI_DRICTL_ADDRTRNSFR) |
			  (3UL << BITP_OSPI_DRICTL_DATATRNSFR);

	if(curVal){
		memcpy(id_spi, 0x20000000, 4); //Read back the ID from SRAM
	}else{
		cadence_qspi_apb_command_read(plat->regbase, &opTemp);
		printf("Read ID via 1x SPI: %x %x %x\n", id_spi[0], id_spi[1], id_spi[2]);
		memcpy(0x20000000, id_spi, 4); //Store the ID at the beginning of SRAM
	}

	plat->id = *(uint32_t*)id_spi;

	switch( *(uint32_t*)id_spi ){
		case 0x3b85c2: //MX66LM1G45
			plat->use_opcode2 = 1;
			plat->use_opcode2_invert = 1;
			plat->stig_read_dummy = 4;
			plat->read_dummy = 20;
			plat->write_dummy = 0;
			plat->write_opcode = SPINOR_OP_PP_4B;
			#if ADI_USE_MACRONIX_OSPI_DTR
				plat->use_dtr = 1;
			#else
				plat->use_dtr = 0;
			#endif
			plat->dly_rd = 0xE;
			plat->ddr_dly_rd = 0x2;

			if(plat->use_dtr){
				plat->read_opcode = 0xEE;
				printf("Configure MX66LM1G45: OPI DTR = on\n");
			}else{
				plat->read_opcode = 0xEC;
				printf("Configure MX66LM1G45: OPI STR = on\n");
			}
			return cadence_enter_octal_mx66(plat, &wren, &opTemp, id_spi);
			break;
		case 0x195a9d: //IS25LX256
			plat->use_opcode2 = 0;
			plat->use_opcode2_invert = 0;
			plat->stig_read_dummy = 0;
			plat->read_dummy = 8;
			plat->write_dummy = 0;
			plat->read_opcode = 0x8B;
			plat->write_opcode = 0x82;
			printf("Configure IS25LX256: Set VCR to Octal DDR without DQS\r\n");
			return cadence_enter_octal_is25(plat, &wren, &opTemp, id_spi);
			break;
		default:
			plat->cadenceMode = CADENCE_SPI_MODE;
			plat->use_opcode2 = 0;
			plat->use_opcode2_invert = 0;
			plat->stig_read_dummy = 0;
			plat->read_dummy = 0;
			plat->write_dummy = 0;
			plat->read_opcode = SPINOR_OP_READ_4B;
			plat->write_opcode = SPINOR_OP_PP_4B;
			printf("Unrecognized SPI device, cannot switch into octal mode... proceeding in 1x mode\n");
			return 0;
	}
}

int cadence_qspi_enter_octal(struct cadence_spi_platdata *plat){
	return cadence_configure_opi_mode(plat);
}

int cadence_qspi_direct_read(struct cadence_spi_platdata *plat,
				   const struct spi_mem_op *op)
{
	unsigned int curVal;
	unsigned int addr_value = op->addr.val;
	unsigned int rxlen = op->data.nbytes;
	u8 *rxbuf = op->data.buf.in;

	if(plat->use_dtr){
		if(rxlen % 2)
			rxlen++;
	}

	*(uint32_t*)SCB5_SPI2_OSPI_REMAP = 0x1U;

	cadence_qspi_apb_controller_enable(plat->regbase);

	// Configure the read opcode
	curVal = readl(plat->regbase + CQSPI_REG_RD_INSTR);
	if(plat->cadenceMode == CADENCE_OSPI_MODE){
		curVal |= (((uint32_t)(plat->read_opcode) << BITP_OSPI_DRICTL_OPCODERD) & BITM_OSPI_DRICTL_OPCODERD) |
				  (((uint32_t)(plat->read_dummy) << BITP_OSPI_DRICTL_DMYRD) & BITM_OSPI_DRICTL_DMYRD);
				  //(((uint32_t)(1) << BITP_OSPI_DRICTL_MODEEN) & BITM_OSPI_DRICTL_MODEEN);
	}else{
		curVal |= (((uint32_t)(plat->read_opcode) << BITP_OSPI_DRICTL_OPCODERD) & BITM_OSPI_DRICTL_OPCODERD) |
				  (((uint32_t)(plat->read_dummy) << BITP_OSPI_DRICTL_DMYRD) & BITM_OSPI_DRICTL_DMYRD);
				  //(((uint32_t)(1) << BITP_OSPI_DRICTL_MODEEN) & BITM_OSPI_DRICTL_MODEEN);
	}
	writel(curVal, plat->regbase + CQSPI_REG_RD_INSTR);

	if(plat->use_opcode2){
		// Configure the dual read opcode
		curVal = readl(plat->regbase + CQSPI_REG_OE_LOWER); 
		curVal &= ~BITM_OSPI_OE_LWR_XRDBYT;
		if(plat->use_opcode2_invert){
			curVal |= ((uint32_t)(0xFF-plat->read_opcode) << BITP_OSPI_OE_LWR_XRDBYT) & BITM_OSPI_OE_LWR_XRDBYT;
		}else{
			curVal |= ((uint32_t)(plat->read_opcode) << BITP_OSPI_OE_LWR_XRDBYT) & BITM_OSPI_OE_LWR_XRDBYT;
		}
		writel(curVal, plat->regbase + CQSPI_REG_OE_LOWER);
	}

	//Configure the read control register for OPI mode
	curVal = readl(plat->regbase + CQSPI_REG_RD_INSTR);
	if(plat->cadenceMode == CADENCE_OSPI_MODE){
		// OSPI Octal Mode
		curVal |= (3UL << BITP_OSPI_DRICTL_INSTRTYP) |
				  (3UL << BITP_OSPI_DRICTL_ADDRTRNSFR) |
				  (3UL << BITP_OSPI_DRICTL_DATATRNSFR);
	}else{
		// OSPI Single Mode
		curVal |= (0UL << BITP_OSPI_DRICTL_INSTRTYP) |
				  (0UL << BITP_OSPI_DRICTL_ADDRTRNSFR) |
				  (0UL << BITP_OSPI_DRICTL_DATATRNSFR);
	}
	writel(curVal, plat->regbase + CQSPI_REG_RD_INSTR);

	if(plat->cadenceMode == CADENCE_OSPI_MODE && plat->id == 0x195a9d){ //SC594, IS25LX256
		//Octal address and instruction are broken on IS25LX256 at the moment
		curVal = readl(plat->regbase + CQSPI_REG_RD_INSTR);
		curVal &= ~(3UL << BITP_OSPI_DRICTL_INSTRTYP);
		curVal &= ~(3UL << BITP_OSPI_DRICTL_ADDRTRNSFR);
		writel(curVal, plat->regbase + CQSPI_REG_RD_INSTR);
	}

#ifdef ADI_OCTAL_USE_DMA
	memcopy_dma(rxbuf, OSPI0_MMAP_ADDRESS+addr_value, rxlen);
#else
	/* Perform the transfer */
	uint32_t count = 0;
	uint64_t *pReadBuffer = rxbuf;
	uint64_t *pFlashAddress = (uint64_t *)(OSPI0_MMAP_ADDRESS+addr_value);
	uint8_t *pReadBuffer8;
	uint8_t *pFlashAddress8;

	for (count = 0U; count < (rxlen / 8); count++)
	{
		*(uint64_t*)pReadBuffer++ = *(uint64_t*)pFlashAddress++;
	}

	if(rxlen % 8){
		pReadBuffer8 = (uint8_t*)pReadBuffer;
		pFlashAddress8 = (uint8_t*)pFlashAddress;

		for (count = 0U; count < (rxlen % 8); count++)
		{
			*(uint8_t*)pReadBuffer8++ = *(uint8_t*)pFlashAddress8++;
		}
	}
#endif

	/* Make sure the OSPI controller is no longer busy */
	int nCount = 4U;
	while(nCount-- > 0U){
		while( ! ((readl(plat->regbase + CQSPI_REG_CONFIG) & BITM_OSPI_CTL_IDLE) >> BITP_OSPI_CTL_IDLE) );
	}

	return 0;
}

int cadence_qspi_direct_write(struct cadence_spi_platdata *plat,
				   const struct spi_mem_op *op)
{
	unsigned int reg;
	unsigned int curVal;
	unsigned int addr_value = op->addr.val;
	unsigned int txlen = op->data.nbytes;
	u8 *txbuf = op->data.buf.out;
	u8 *txbufTemp;
	bool needFreeBuf = false;

	if(plat->use_dtr){
		if(txlen % 2){
			txbufTemp = txbuf;
			txbuf = malloc(txlen+1);
			memcpy(txbuf, txbufTemp, txlen);
			txbuf[txlen] = 0xFF;
			txlen++;
			needFreeBuf = true;
		}
	}

	*(uint32_t*)SCB5_SPI2_OSPI_REMAP = 0x1U;

	cadence_qspi_apb_controller_enable(plat->regbase);

	/* Clear the contents of the Write instruction register */
	writel(0, plat->regbase + CQSPI_REG_WR_INSTR);

	/* Configure the opcode */
	curVal = readl(plat->regbase + CQSPI_REG_WR_INSTR);
	if(plat->cadenceMode == CADENCE_OSPI_MODE){
		curVal |= (((uint32_t)(plat->write_opcode) << BITP_OSPI_DWICTL_OPCODEWR) & BITM_OSPI_DWICTL_OPCODEWR) |
				  (((uint32_t)(plat->write_dummy) << BITP_OSPI_DWICTL_DMYWR) & BITM_OSPI_DWICTL_DMYWR) |
				  (((uint32_t)(0) << BITP_OSPI_DWICTL_WELDIS) & BITM_OSPI_DWICTL_WELDIS);
	}else{
		curVal |= (((uint32_t)(plat->write_opcode) << BITP_OSPI_DWICTL_OPCODEWR) & BITM_OSPI_DWICTL_OPCODEWR) |
				  (((uint32_t)(plat->write_dummy) << BITP_OSPI_DWICTL_DMYWR) & BITM_OSPI_DWICTL_DMYWR) |
				  (((uint32_t)(0) << BITP_OSPI_DWICTL_WELDIS) & BITM_OSPI_DWICTL_WELDIS);
	}
	writel(curVal, plat->regbase + CQSPI_REG_WR_INSTR);

	if(plat->use_opcode2){
		/* Configure the dual opcode */
		curVal = readl(plat->regbase + CQSPI_REG_OE_LOWER); 
		curVal &= ~BITM_OSPI_OE_LWR_XWRBYT;
		if(plat->use_opcode2_invert){
			curVal |= ((uint32_t)(0xFF-plat->write_opcode) << BITP_OSPI_OE_LWR_XWRBYT) & BITM_OSPI_OE_LWR_XWRBYT;
		}else{
			curVal |= ((uint32_t)(plat->write_opcode) << BITP_OSPI_OE_LWR_XWRBYT) & BITM_OSPI_OE_LWR_XWRBYT;
		}
		writel(curVal, plat->regbase + CQSPI_REG_OE_LOWER);
	}

	curVal = readl(plat->regbase + CQSPI_REG_WR_INSTR);
	if(plat->cadenceMode == CADENCE_OSPI_MODE){
		/* OSPI Octal Mode */
		curVal |= (3UL << BITP_OSPI_DWICTL_ADDRTRNSFR) |
				  (3UL << BITP_OSPI_DWICTL_DATATRNSFR);
	}else{
		/* OSPI Single Mode */
		curVal |= (0UL << BITP_OSPI_DWICTL_ADDRTRNSFR) |
				  (0UL << BITP_OSPI_DWICTL_DATATRNSFR);
	}
	writel(curVal, plat->regbase + CQSPI_REG_WR_INSTR);

	if(plat->cadenceMode == CADENCE_OSPI_MODE && plat->id == 0x195a9d){ //SC594, IS25LX256
		//Octal address is broken on IS25LX256 at the moment
		curVal = readl(plat->regbase + CQSPI_REG_WR_INSTR);
		curVal &= ~((3UL << BITP_OSPI_DRICTL_ADDRTRNSFR));
		writel(curVal, plat->regbase + CQSPI_REG_WR_INSTR);
	}

#ifdef ADI_OCTAL_USE_DMA
	memcopy_dma(OSPI0_MMAP_ADDRESS+addr_value, txbuf, txlen);
#else
	/* Perform the transfer */
	uint32_t count = 0;
	uint64_t *pWriteBuffer = txbuf;
	uint64_t *pFlashAddress = (uint64_t *)(OSPI0_MMAP_ADDRESS+addr_value);
	uint8_t *pWriteBuffer8;
	uint8_t *pFlashAddress8;

	for (count = 0U; count < (txlen / 8); count++)
	{
		*(uint64_t*)pFlashAddress++ = *(uint64_t*)pWriteBuffer++;
	}

	if(txlen % 8){
		pWriteBuffer8 = (uint8_t*)pWriteBuffer;
		pFlashAddress8 = (uint8_t*)pFlashAddress;

		for (count = 0U; count < (txlen % 8); count++)
		{
			*(uint8_t*)pFlashAddress8++ = *(uint8_t*)pWriteBuffer8++;
		}
	}
#endif

	/* Make sure the OSPI controller is no longer busy */
	int nCount = 4U;
	while(nCount-- > 0U){
		while( ! ((readl(plat->regbase + CQSPI_REG_CONFIG) & BITM_OSPI_CTL_IDLE) >> BITP_OSPI_CTL_IDLE) );
	}

	if(needFreeBuf){
		free(txbuf);
		txbuf = txbufTemp;
		txlen--;
	}

	return 0;
}

#endif
