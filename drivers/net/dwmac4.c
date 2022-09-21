// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * (C) Copyright 2022 - Analog Devices, Inc.
 *
 * Written and/or maintained by Timesys Corporation
 *
 * Contact: Nathan Barrett-Morrison <nathan.morrison@timesys.com>
 * Contact: Greg Malysa <greg.malysa@timesys.com>
 *
 * Designware gmac4+ driver for u-boot.
 * Based on original gmac driver + linux gmac and gmac4 drivers
 *
 * Some caveats:
 * - Currently only really supports full duplex RGMII targets and assumes clocks
 *   and speeds match that in several places (MDIO, phy link configuration, etc)
 */

#include <common.h>
#include <clk.h>
#include <cpu_func.h>
#include <dm.h>
#include <errno.h>
#include <log.h>
#include <miiphy.h>
#include <malloc.h>
#include <net.h>
#include <reset.h>
#include <asm/cache.h>
#include <dm/device_compat.h>
#include <dm/devres.h>
#include <linux/compiler.h>
#include <linux/delay.h>
#include <linux/err.h>
#include <linux/kernel.h>
#include <asm/io.h>
#include <power/regulator.h>
#include <asm-generic/gpio.h>

#include "dwmac4.h"
#include "dwmac4_dma.h"
#include "dwmac4_descs.h"

static int dwmac4_wrback_get_tx_status(struct dwmac4_dev *priv, u32 idx)
{
	struct dwmac4_dma_desc *desc_p = &priv->tx_mac_descrtable[idx];
	u32 tdes1, tdes2, tdes3;

	tdes1 = desc_p->des1;
	tdes2 = desc_p->des2;
	tdes3 = desc_p->des3;
	printf("TX DESCR NUM %d, des1 %x, des2 %x, des3 %x\n", idx, tdes1, tdes2, tdes3);

	/* Get tx owner first */
	if (tdes3 & TDES3_OWN)
		printf("TDES3_OWN SET\n");

	/* Verify tx error by looking at the last segment. */
	if (tdes3 & TDES3_LAST_DESCRIPTOR)
		printf("TDES3_LAST_DESCRIPTOR SET\n");

	if (tdes3 & TDES3_ERROR_SUMMARY) {
		printf("TDES3_ERROR_SUMMARY SET\n");
		if (tdes3 & TDES3_JABBER_TIMEOUT)
			printf("TDES3_JABBER_TIMEOUT SET\n");
		if (tdes3 & TDES3_PACKET_FLUSHED)
			printf("TDES3_PACKET_FLUSHED SET\n");
		if (tdes3 & TDES3_LOSS_CARRIER) {
			printf("TDES3_LOSS_CARRIER SET\n");
		}
		if (tdes3 & TDES3_NO_CARRIER) {
			printf("TDES3_NO_CARRIER SET\n");
		}
		if ((tdes3 & TDES3_LATE_COLLISION) ||
			     (tdes3 & TDES3_EXCESSIVE_COLLISION))
					printf("TDES3_LATE_COLLISION/TDES3_EXCESSIVE_COLLISION SET\n");

		if (tdes3 & TDES3_EXCESSIVE_DEFERRAL)
			printf("TDES3_EXCESSIVE_DEFERRAL SET\n");

		if (tdes3 & TDES3_UNDERFLOW_ERROR)
			printf("TDES3_UNDERFLOW_ERROR SET\n");

		if (tdes3 & TDES3_IP_HDR_ERROR)
			printf("TDES3_IP_HDR_ERROR SET\n");

		if (tdes3 & TDES3_PAYLOAD_ERROR)
			printf("TDES3_PAYLOAD_ERROR SET\n");
	}

	if (tdes3 & TDES3_DEFERRED)
		printf("TDES3_DEFERRED SET\n");

}

static int dwmac4_wrback_get_rx_status(struct dwmac4_dev *priv, u32 idx)
{
	struct dwmac4_dma_desc *desc_p = &priv->rx_mac_descrtable[idx];
	u32 rdes1, rdes2, rdes3;

	rdes1 = desc_p->des1;
	rdes2 = desc_p->des2;
	rdes3 = desc_p->des3;
	printf("RX DESCR NUM %d, des1 %x, des2 %x, des3 %x\n", idx, rdes1, rdes2, rdes3);

	if (rdes3 & RDES3_OWN)
		printf("RDES3_OWN set\n");

	/* Verify rx error by looking at the last segment. */
	if (rdes3 & RDES3_LAST_DESCRIPTOR)
		printf("RDES3_LAST_DESCRIPTOR set\n");

	if (rdes3 & RDES3_ERROR_SUMMARY) {
		printf("RDES3_ERROR_SUMMARY set\n");

		if (rdes3 & RDES3_GIANT_PACKET)
			printf("RDES3_GIANT_PACKET set\n");

		if (rdes3 & RDES3_OVERFLOW_ERROR)
			printf("RDES3_OVERFLOW_ERROR set\n");

		if (rdes3 & RDES3_RECEIVE_WATCHDOG)
			printf("RDES3_RECEIVE_WATCHDOG set\n");

		if (rdes3 & RDES3_RECEIVE_ERROR)
			printf("RDES3_RECEIVE_ERROR set\n");

		if (rdes3 & RDES3_CRC_ERROR) {
			printf("RDES3_CRC_ERROR set\n");
		}

		if (rdes3 & RDES3_DRIBBLE_ERROR)
			printf("RDES3_DRIBBLE_ERROR set\n");
	}

	if (rdes1 & RDES1_IP_HDR_ERROR)
		printf("RDES1_IP_HDR_ERROR set\n");
	if (rdes1 & RDES1_IP_CSUM_BYPASSED)
		printf("RDES1_IP_CSUM_BYPASSED set\n");
	if (rdes1 & RDES1_IPV4_HEADER)
		printf("RDES1_IPV4_HEADER set\n");
	if (rdes1 & RDES1_IPV6_HEADER)
		printf("RDES1_IPV6_HEADER set\n");

	if (rdes1 & RDES1_PTP_PACKET_TYPE)
		printf("RDES1_PTP_PACKET_TYPE set\n");
	if (rdes1 & RDES1_PTP_VER)
		printf("RDES1_PTP_VER set\n");
	if (rdes1 & RDES1_TIMESTAMP_DROPPED)
		printf("RDES1_TIMESTAMP_DROPPED set\n");

	if (rdes2 & RDES2_SA_FILTER_FAIL) {
		printf("RDES2_SA_FILTER_FAIL set\n");
	}
	if (rdes2 & RDES2_DA_FILTER_FAIL) {
		printf("RDES2_DA_FILTER_FAIL set\n");
	}

	if (rdes2 & RDES2_L3_FILTER_MATCH)
		printf("RDES2_L3_FILTER_MATCH set\n");
	if (rdes2 & RDES2_L4_FILTER_MATCH)
		printf("RDES2_L4_FILTER_MATCH set\n");
	if ((rdes2 & RDES2_L3_L4_FILT_NB_MATCH_MASK)
	    >> RDES2_L3_L4_FILT_NB_MATCH_SHIFT)
		printf("RDES2_L3_L4_FILT_NB_MATCH_MASK set\n");
}

static void show_tx_process_state(u32 status)
{
	u32 state;
	state = (status & DMA_CHAN_STATUS_TEB) >> DMA_CHAN_STATUS_TEB_SHIFT;

	switch (state) {
	case 0:
		printf("- TX (Stopped): Reset or Stop command\n");
		break;
	case 1:
		printf("- TX (Running): Fetching the Tx desc\n");
		break;
	case 2:
		printf("- TX (Running): Waiting for end of tx\n");
		break;
	case 3:
		printf("- TX (Running): Reading the data "
		       "and queuing the data into the Tx buf\n");
		break;
	case 6:
		printf("- TX (Suspended): Tx Buff Underflow "
		       "or an unavailable Transmit descriptor\n");
		break;
	case 7:
		printf("- TX (Running): Closing Tx descriptor\n");
		break;
	default:
		break;
	}
}

static void show_rx_process_state(u32 status)
{
	u32 state;
	state = (status & DMA_CHAN_STATUS_REB) >> DMA_CHAN_STATUS_REB_SHIFT;

	switch (state) {
	case 0:
		printf("- RX (Stopped): Reset or Stop command\n");
		break;
	case 1:
		printf("- RX (Running): Fetching the Rx desc\n");
		break;
	case 2:
		printf("- RX (Running): Checking for end of pkt\n");
		break;
	case 3:
		printf("- RX (Running): Waiting for Rx pkt\n");
		break;
	case 4:
		printf("- RX (Suspended): Unavailable Rx buf\n");
		break;
	case 5:
		printf("- RX (Running): Closing Rx descriptor\n");
		break;
	case 6:
		printf("- RX(Running): Flushing the current frame"
		       " from the Rx buf\n");
		break;
	case 7:
		printf("- RX (Running): Queuing the Rx frame"
		       " from the Rx buf into memory\n");
		break;
	default:
		break;
	}
}

int dwmac4_dma_interrupt_status_register(void __iomem *ioaddr, u32 chan)
{
	u32 intr_status = readl(ioaddr + DMA_CHAN_STATUS(chan));

	printf("interrupt status: 0x%x\n", intr_status);

	/* ABNORMAL interrupts */
	if (intr_status & DMA_CHAN_STATUS_AIS) {
		printf("DMA_CHAN_STATUS_AIS SET\n");
		if (intr_status & DMA_CHAN_STATUS_RBU)
			printf("DMA_CHAN_STATUS_RBU SET\n");
		if (intr_status & DMA_CHAN_STATUS_RPS)
			printf("DMA_CHAN_STATUS_RPS SET\n");
		if (intr_status & DMA_CHAN_STATUS_RWT)
			printf("DMA_CHAN_STATUS_RWT SET\n");
		if (intr_status & DMA_CHAN_STATUS_ETI)
			printf("DMA_CHAN_STATUS_ETI SET\n");
		if (intr_status & DMA_CHAN_STATUS_TPS) {
			printf("DMA_CHAN_STATUS_TPS SET\n");
		}
		if (intr_status & DMA_CHAN_STATUS_FBE) {
			printf("DMA_CHAN_STATUS_FBE SET\n");
		}
	}

	/* TX/RX NORMAL interrupts */
	if (intr_status & DMA_CHAN_STATUS_NIS) {
			printf("DMA_CHAN_STATUS_NIS SET\n");
		if (intr_status & DMA_CHAN_STATUS_RI) {
			printf("DMA_CHAN_STATUS_RI SET\n");
		}
		if (intr_status & (DMA_CHAN_STATUS_TI |
					  DMA_CHAN_STATUS_TBU)) {
			printf("DMA_CHAN_STATUS_TI or DMA_CHAN_STATUS_TBU SET\n");
		}
		if (intr_status & DMA_CHAN_STATUS_ERI)
			printf("DMA_CHAN_STATUS_ERI SET\n");
	}

	intr_status = readl(ioaddr + DMA_STATUS);
	printf("DMA interrupt status is: 0x%x\n", intr_status);

	intr_status = readl(ioaddr + DMA_DEBUG_STATUS_0);
	show_rx_process_state(intr_status);
	show_tx_process_state(intr_status);
}

static int dw_mdio_read(struct mii_dev *bus, int addr, int devad, int reg)
{
	struct dwmac4_dev *priv = (struct dwmac4_dev *) bus->priv;
	ulong start;
	u32 miiaddr;
	int timeout = CONFIG_MDIO_TIMEOUT;

	miiaddr = ((addr << MII_ADDR_SHIFT) & MII_ADDR_MASK) |
		((reg << MII_REG_SHIFT) & MII_REG_MASK);

	writel(miiaddr | MII_CLKRANGE_150_250M | MII_BUSY | MII_GMAC4_READ,
		priv->ioaddr + GMAC_MDIO_ADDR);

	start = get_timer(0);
	while (get_timer(start) < timeout) {
		if (!(readl(priv->ioaddr + GMAC_MDIO_ADDR) & MII_BUSY))
			return readl(priv->ioaddr + GMAC_MDIO_DATA);
		udelay(10);
	};

	return -ETIMEDOUT;
}

static int dw_mdio_write(struct mii_dev *bus, int addr, int devad, int reg,
			u16 val)
{
	struct dwmac4_dev *priv = (struct dwmac4_dev *) bus->priv;
	ulong start;
	u32 miiaddr;
	int ret = -ETIMEDOUT, timeout = CONFIG_MDIO_TIMEOUT;

	writel(val, priv->ioaddr + GMAC_MDIO_DATA);
	miiaddr = ((addr << MII_ADDR_SHIFT) & MII_ADDR_MASK) |
		((reg << MII_REG_SHIFT) & MII_REG_MASK);

	writel(miiaddr | MII_CLKRANGE_150_250M | MII_BUSY | MII_GMAC4_WRITE,
		priv->ioaddr + GMAC_MDIO_ADDR);

	start = get_timer(0);
	while (get_timer(start) < timeout) {
		if (!(readl(priv->ioaddr + GMAC_MDIO_ADDR) & MII_BUSY)) {
			ret = 0;
			break;
		}
		udelay(10);
	};

	return ret;
}

#if defined(CONFIG_DM_ETH) && CONFIG_IS_ENABLED(DM_GPIO)
static int dw_mdio_reset(struct mii_dev *bus)
{
	struct dwmac4_dev *priv = bus->priv;
	int ret;

	if (!dm_gpio_is_valid(&priv->reset_gpio))
		return 0;

	/* reset the phy */
	ret = dm_gpio_set_value(&priv->reset_gpio, 0);
	if (ret)
		return ret;

	udelay(priv->reset_delays[0]);

	ret = dm_gpio_set_value(&priv->reset_gpio, 1);
	if (ret)
		return ret;

	udelay(priv->reset_delays[1]);

	ret = dm_gpio_set_value(&priv->reset_gpio, 0);
	if (ret)
		return ret;

	udelay(priv->reset_delays[2]);

	return 0;
}
#endif

static int dw_mdio_init(const char *name, struct dwmac4_dev *priv)
{
	struct mii_dev *bus = mdio_alloc();

	if (!bus) {
		printf("Failed to allocate MDIO bus\n");
		return -ENOMEM;
	}

	bus->read = dw_mdio_read;
	bus->write = dw_mdio_write;
	snprintf(bus->name, sizeof(bus->name), "%s", name);
#if defined(CONFIG_DM_ETH) && CONFIG_IS_ENABLED(DM_GPIO)
	bus->reset = dw_mdio_reset;
#endif

	bus->priv = (void *) priv;

	return mdio_register(bus);
}

static void tx_descs_init(struct dwmac4_dev *priv)
{
	char *txbuffs = &priv->txbuffs[0];
	struct dwmac4_dma_desc *desc_table_p = &priv->tx_mac_descrtable[0];
	struct dwmac4_dma_desc *desc_p;
	u32 idx;
	u32 val;

	val = readl(priv->ioaddr + DMA_CHAN_TX_CONTROL(0));
	val |= DMA_TXPBL << DMA_CONTROL_PBL_SHIFT;
	val |= DMA_CONTROL_OSP;
	writel(val, priv->ioaddr + DMA_CHAN_TX_CONTROL(0));

	for (idx = 0; idx < CONFIG_TX_DESCR_NUM; idx++) {
		desc_p = &desc_table_p[idx];
		desc_p->dmamac_addr = (ulong)&txbuffs[idx * CONFIG_ETH_BUFSIZE];
		desc_p->des1 = 0;
		desc_p->des2 = 0;
		desc_p->des3 = 0;
	}

	/* Flush all Tx buffer descriptors at once */
	flush_dcache_range((ulong)priv->tx_mac_descrtable,
			   (ulong)priv->tx_mac_descrtable +
			   sizeof(priv->tx_mac_descrtable));

	writel((ulong)&desc_table_p[0], priv->ioaddr + DMA_CHAN_TX_BASE_ADDR(0));
	writel((ulong)&desc_table_p[0], priv->ioaddr + DMA_CHAN_TX_END_ADDR(0));
	writel(CONFIG_TX_DESCR_NUM-1, priv->ioaddr + DMA_CHAN_TX_RING_LEN(0));
	priv->tx_currdescnum = 0;
}

static void rx_descs_init(struct dwmac4_dev *priv)
{
	char *rxbuffs = &priv->rxbuffs[0];
	struct dwmac4_dma_desc *desc_table_p = &priv->rx_mac_descrtable[0];
	struct dwmac4_dma_desc *desc_p;
	u32 idx;
	u32 val;

	/* Before passing buffers to GMAC we need to make sure zeros
	 * written there right after "priv" structure allocation were
	 * flushed into RAM.
	 * Otherwise there's a chance to get some of them flushed in RAM when
	 * GMAC is already pushing data to RAM via DMA. This way incoming from
	 * GMAC data will be corrupted. */
	flush_dcache_range((ulong)rxbuffs, (ulong)rxbuffs + RX_TOTAL_BUFSIZE);

	// Set the buffer size that applies to all rx dma descriptors in channel 0
	val = readl(priv->ioaddr + DMA_CHAN_RX_CONTROL(0));
	val &= ~DMA_RBSZ_MASK;
	val |= CONFIG_ETH_BUFSIZE << DMA_RBSZ_SHIFT;
	val |= DMA_RXPBL << DMA_CONTROL_PBL_SHIFT;
	writel(val, priv->ioaddr + DMA_CHAN_RX_CONTROL(0));

	for (idx = 0; idx < CONFIG_RX_DESCR_NUM; idx++) {
		desc_p = &desc_table_p[idx];
		desc_p->dmamac_addr = (ulong)&rxbuffs[idx * CONFIG_ETH_BUFSIZE];
		desc_p->des1 = 0;
		desc_p->des2 = 0;
		desc_p->des3 = RDES3_OWN | RDES3_BUFFER1_VALID_ADDR;
	}

	/* Flush all Rx buffer descriptors at once */
	flush_dcache_range((ulong)priv->rx_mac_descrtable,
			   (ulong)priv->rx_mac_descrtable +
			   sizeof(priv->rx_mac_descrtable));

	writel((ulong)desc_table_p, priv->ioaddr + DMA_CHAN_RX_BASE_ADDR(0));
	writel((ulong)(desc_table_p + CONFIG_RX_DESCR_NUM),
		priv->ioaddr + DMA_CHAN_RX_END_ADDR(0));
	writel(CONFIG_RX_DESCR_NUM-1, priv->ioaddr + DMA_CHAN_RX_RING_LEN(0));
}

static int _dw_write_hwaddr(struct dwmac4_dev *priv, u8 *mac_id)
{
	u32 macid_lo, macid_hi;

	macid_lo = mac_id[0] + (mac_id[1] << 8) + (mac_id[2] << 16) +
		   (mac_id[3] << 24);
	macid_hi = mac_id[4] + (mac_id[5] << 8);

	writel(macid_hi, priv->ioaddr + GMAC_ADDR_HIGH(0));
	writel(macid_lo, priv->ioaddr + GMAC_ADDR_LOW(0));

	return 0;
}

static int dw_adjust_link(struct dwmac4_dev *priv, struct phy_device *phydev)
{
	u32 conf = readl(priv->ioaddr + GMAC_CONFIG);

	if (!phydev->link) {
		printf("%s: No link.\n", phydev->dev->name);
		return 0;
	}

	if (phydev->speed != 1000)
		conf |= GMAC_CONFIG_PS;
	else
		conf &= ~GMAC_CONFIG_PS;

	if (phydev->speed == 100)
		conf |= GMAC_CONFIG_FES;
	else
		conf &= GMAC_CONFIG_FES;

	if (phydev->duplex)
		conf |= GMAC_CONFIG_DM;
	else
		conf &= ~GMAC_CONFIG_DM;

	writel(conf, priv->ioaddr + GMAC_CONFIG);

	printf("Speed: %d, %s duplex%s\n", phydev->speed,
	       (phydev->duplex) ? "full" : "half",
	       (phydev->port == PORT_FIBRE) ? ", fiber mode" : "");

	return 0;
}

static void _dw_eth_halt(struct dwmac4_dev *priv)
{
	u32 val;

	val = readl(priv->ioaddr + GMAC_CONFIG);
	val &= ~(GMAC_CONFIG_RE | GMAC_CONFIG_TE);
	writel(val, priv->ioaddr + GMAC_CONFIG);

	val = readl(priv->ioaddr + DMA_CHAN_TX_CONTROL(0));
	val &= ~DMA_CONTROL_ST;
	writel(val, priv->ioaddr + DMA_CHAN_TX_CONTROL(0));

	val = readl(priv->ioaddr + DMA_CHAN_RX_CONTROL(0));
	val &= ~DMA_CONTROL_SR;
	writel(val, priv->ioaddr + DMA_CHAN_RX_CONTROL(0));

	phy_shutdown(priv->phydev);
}

static int designware_eth_init(struct dwmac4_dev *priv, u8 *enetaddr)
{
	unsigned int start;
	u32 val;
	int ret;

	val = readl(priv->ioaddr + DMA_BUS_MODE);
	val |= DMA_BUS_MODE_SFT_RESET;
	writel(val, priv->ioaddr + DMA_BUS_MODE);

	start = get_timer(0);
	while (readl(priv->ioaddr + DMA_BUS_MODE) & DMA_BUS_MODE_SFT_RESET) {
		if (get_timer(start) >= CONFIG_MACRESET_TIMEOUT) {
			printf("DMA reset timeout\n");
			return -ETIMEDOUT;
		}

		mdelay(100);
	};

	val = readl(priv->ioaddr + DMA_SYS_BUS_MODE);
	val |= DMA_SYS_BUS_AAL;
	val |= DMA_BURST_LEN_DEFAULT;
	val |= DMA_AXI_WR_OSR_LMT | DMA_AXI_RD_OSR_LMT;
	writel(val, priv->ioaddr + DMA_SYS_BUS_MODE);

	rx_descs_init(priv);
	tx_descs_init(priv);

	/* could program PBL bit in DMA Channel Control register here */

	/*
	 * Soft reset above clears HW address registers.
	 * So we have to set it here once again.
	 */
	_dw_write_hwaddr(priv, enetaddr);

	/* default configuration for gmac control, some updates happen later */
	val = readl(priv->ioaddr + GMAC_CONFIG);
	val |= GMAC_CONFIG_JD | GMAC_CONFIG_BE | GMAC_CONFIG_DCRS | GMAC_CONFIG_JE;
	writel(val, priv->ioaddr + GMAC_CONFIG);

	/* enable reception of all packets in packet filter */
	val = GMAC_PACKET_FILTER_RA;
	writel(val, priv->ioaddr + GMAC_PACKET_FILTER);

	/* Do not touch tx flow control */

	/* @todo support threshold mode DMA and configuration */

	/* @todo change burst length on AXI bus per platdata (above we enabled all) */

	/* Start up the PHY */
	ret = phy_startup(priv->phydev);
	if (ret) {
		printf("Could not initialize PHY %s\n",
		       priv->phydev->dev->name);
		return ret;
	}

	ret = dw_adjust_link(priv, priv->phydev);
	if (ret)
		return ret;

	/* Configure MTL registers to enable RX and TX queue 0 */
	/* @todo configurable queue size; we're using 4kb to be safe but hardware typically
	 * has around 16 kb minimum for the MTL queue, to be broken up per queue */
	val = readl(priv->ioaddr + MTL_CHAN_RX_OP_MODE(0));
	val |= MTL_OP_MODE_RSF;
	val &= ~MTL_OP_MODE_RQS_MASK;
	val |= ((4096 / 256) - 1) << MTL_OP_MODE_RQS_SHIFT;
	val |= MTL_OP_MODE_RX_FUP | MTL_OP_MODE_RX_FEP;
	writel(val, priv->ioaddr + MTL_CHAN_RX_OP_MODE(0));

	writel(GMAC_RXQCTRL0_RXQ0EN_DCB, priv->ioaddr + GMAC_RXQ_CTRL0);

	val = readl(priv->ioaddr + MTL_CHAN_TX_OP_MODE(0));
	val |= MTL_OP_MODE_TSF;
	val &= ~MTL_OP_MODE_TXQEN_MASK;
	val |= MTL_OP_MODE_TXQEN;
	val &= ~MTL_OP_MODE_TQS_MASK;
	val |= ((4096 / 256) - 1) << MTL_OP_MODE_TQS_SHIFT;
	writel(val, priv->ioaddr + MTL_CHAN_TX_OP_MODE(0));

	return 0;
}

int designware_eth_enable(struct dwmac4_dev *priv)
{
	u32 val;

	if (!priv->phydev->link)
		return -EIO;

	val = readl(priv->ioaddr + GMAC_CONFIG);
	val |= GMAC_CONFIG_RE | GMAC_CONFIG_TE;
	writel(val, priv->ioaddr + GMAC_CONFIG);

	val = readl(priv->ioaddr + DMA_CHAN_TX_CONTROL(0));
	val |= DMA_CONTROL_ST;
	writel(val, priv->ioaddr + DMA_CHAN_TX_CONTROL(0));

	val = readl(priv->ioaddr + DMA_CHAN_RX_CONTROL(0));
	val |= DMA_CONTROL_SR;
	writel(val, priv->ioaddr + DMA_CHAN_RX_CONTROL(0));

	return 0;
}

#define ETH_ZLEN	60

static int _dw_eth_send(struct dwmac4_dev *priv, void *packet, int length)
{
	u32 desc_num = priv->tx_currdescnum;
	struct dwmac4_dma_desc *desc_p = &priv->tx_mac_descrtable[desc_num];
	ulong desc_start = (ulong)desc_p;
	ulong desc_end = desc_start +
		roundup(sizeof(*desc_p), ARCH_DMA_MINALIGN);
	ulong data_start = desc_p->dmamac_addr;
	ulong data_end = data_start + roundup(length, ARCH_DMA_MINALIGN);

	invalidate_dcache_range(desc_start, desc_end);

	/* Check if the descriptor is owned by CPU */
	if (desc_p->des2 & TDES3_OWN) {
		printf("CPU not owner of tx frame\n");
		return -EPERM;
	}

	memcpy((void *)data_start, packet, length);
	if (length < ETH_ZLEN) {
		memset(&((char *)data_start)[length], 0, ETH_ZLEN - length);
		length = ETH_ZLEN;
	}

	/* Flush data to be sent */
	flush_dcache_range(data_start, data_end);

	desc_p->des2 = length & TDES2_BUFFER1_SIZE_MASK;
	desc_p->des3 = TDES3_FIRST_DESCRIPTOR | TDES3_LAST_DESCRIPTOR | TDES3_OWN |
		(length & TDES3_PACKET_SIZE_MASK);

	/* Flush modified buffer descriptor */
	flush_dcache_range(desc_start, desc_end);

	/* Test the wrap-around condition. */
	if (++desc_num >= CONFIG_TX_DESCR_NUM)
		desc_num = 0;

	priv->tx_currdescnum = desc_num;

	/* Start the transmission */
	writel((ulong)&priv->tx_mac_descrtable[desc_num],
		priv->ioaddr + DMA_CHAN_TX_END_ADDR(0));

	return 0;
}

static int _dw_eth_recv(struct dwmac4_dev *priv, uchar **packetp)
{
	u32 status, desc_num;
	int length = -EAGAIN;

	/* In ring mode we start at the beginning of the ring and iterate to find ready
	 * descriptors, typically we will reuse index 0 a bunch of times */
	for (desc_num = 0; desc_num < CONFIG_RX_DESCR_NUM; ++desc_num) {
		struct dwmac4_dma_desc *desc_p = &priv->rx_mac_descrtable[desc_num];
		ulong desc_start = (ulong)desc_p;
		ulong desc_end = desc_start +
			roundup(sizeof(*desc_p), ARCH_DMA_MINALIGN);
		ulong data_start = desc_p->dmamac_addr;
		ulong data_end;

		/* Invalidate entire buffer descriptor */
		invalidate_dcache_range(desc_start, desc_end);

		status = desc_p->des3;

		/* Check  if the owner is the CPU */
		if (!(status & RDES3_OWN)) {
			priv->rx_currdescnum = desc_num;

			length = status & RDES3_PACKET_SIZE_MASK;

			/* Invalidate received data */
			data_end = data_start + roundup(length, ARCH_DMA_MINALIGN);
			invalidate_dcache_range(data_start, data_end);
			*packetp = (uchar *)(ulong)desc_p->dmamac_addr;
		}
	}

	return length;
}

static int _dw_free_pkt(struct dwmac4_dev *priv)
{
	u32 desc_num = priv->rx_currdescnum;
	struct dwmac4_dma_desc *desc_p = &priv->rx_mac_descrtable[desc_num];
	ulong desc_start = (ulong)desc_p;
	ulong desc_end = desc_start +
		roundup(sizeof(*desc_p), ARCH_DMA_MINALIGN);

	/*
	 * Reinitialize current descriptor
	 */
	desc_p->dmamac_addr = (ulong)&priv->rxbuffs[desc_num * CONFIG_ETH_BUFSIZE];
	desc_p->des1 = 0;
	desc_p->des2 = 0;
	desc_p->des3 = RDES3_OWN | RDES3_BUFFER1_VALID_ADDR;

	/* Flush only status field - others weren't changed */
	flush_dcache_range(desc_start, desc_end);

	/* let hardware know the ring has been updated, it will start at offset 0 again */
	writel((ulong)(priv->rx_mac_descrtable + CONFIG_RX_DESCR_NUM),
		priv->ioaddr + DMA_CHAN_RX_END_ADDR(0));

	return 0;
}

static int dw_phy_init(struct dwmac4_dev *priv, void *dev)
{
	struct phy_device *phydev;
	int phy_addr = -1, ret;

#ifdef CONFIG_PHY_ADDR
	phy_addr = CONFIG_PHY_ADDR;
#endif

	phydev = phy_connect(priv->bus, phy_addr, dev, priv->interface);
	if (!phydev)
		return -ENODEV;

	phydev->supported &= PHY_GBIT_FEATURES;
	if (priv->max_speed) {
		ret = phy_set_supported(phydev, priv->max_speed);
		if (ret)
			return ret;
	}
	phydev->advertising = phydev->supported;

	priv->phydev = phydev;
	phy_config(phydev);

	return 0;
}

#ifndef CONFIG_DM_ETH
static int dw_eth_init(struct eth_device *dev, struct bd_info *bis)
{
	int ret;

	ret = designware_eth_init(dev->priv, dev->enetaddr);
	if (!ret)
		ret = designware_eth_enable(dev->priv);

	return ret;
}

static int dw_eth_send(struct eth_device *dev, void *packet, int length)
{
	return _dw_eth_send(dev->priv, packet, length);
}

static int dw_eth_recv(struct eth_device *dev)
{
	uchar *packet;
	int length;

	length = _dw_eth_recv(dev->priv, &packet);
	while (length != -EAGAIN) {
		net_process_received_packet(packet, length);
		_dw_free_pkt(dev->priv);
		length = _dw_eth_recv(dev->priv, &packet);
	}

	return 0;
}

static void dw_eth_halt(struct eth_device *dev)
{
	return _dw_eth_halt(dev->priv);
}

static int dw_write_hwaddr(struct eth_device *dev)
{
	return _dw_write_hwaddr(dev->priv, dev->enetaddr);
}

int dwmac4_initialize(ulong base_addr, u32 interface)
{
	struct eth_device *dev;
	struct dwmac4_dev *priv;

	dev = (struct eth_device *) malloc(sizeof(*dev));
	if (!dev)
		return -ENOMEM;

	priv = (struct dwmac4_dev *) malloc(sizeof(*priv));
	if (!priv) {
		free(dev);
		return -ENOMEM;
	}

	if ((phys_addr_t)priv + sizeof(*priv) > (1ULL << 32)) {
		printf("dwmac4: buffers are outside DMA memory\n");
		free(priv);
		free(dev);
		return -EINVAL;
	}

	memset(dev, 0, sizeof(*dev));
	memset(priv, 0, sizeof(*priv));

	sprintf(dev->name, "dwmac4.%lx", base_addr);
	dev->iobase = (int)base_addr;
	dev->priv = priv;

	priv->dev = dev;
	priv->ioaddr = base_addr;

	dev->init = dw_eth_init;
	dev->send = dw_eth_send;
	dev->recv = dw_eth_recv;
	dev->halt = dw_eth_halt;
	dev->write_hwaddr = dw_write_hwaddr;

	eth_register(dev);

	priv->interface = interface;

	dw_mdio_init(dev->name, priv);
	priv->bus = miiphy_get_dev_by_name(dev->name);

	return dw_phy_init(priv, dev);
}
#endif

#ifdef CONFIG_DM_ETH
static int designware_eth_start(struct udevice *dev)
{
	struct eth_pdata *pdata = dev_get_platdata(dev);
	struct dwmac4_dev *priv = dev_get_priv(dev);
	int ret;

	ret = designware_eth_init(priv, pdata->enetaddr);
	if (ret)
		return ret;
	ret = designware_eth_enable(priv);
	if (ret)
		return ret;

	return 0;
}

static int designware_eth_send(struct udevice *dev, void *packet, int length)
{
	struct dwmac4_dev *priv = dev_get_priv(dev);

	return _dw_eth_send(priv, packet, length);
}

static int designware_eth_recv(struct udevice *dev, int flags, uchar **packetp)
{
	struct dwmac4_dev *priv = dev_get_priv(dev);

	return _dw_eth_recv(priv, packetp);
}

static int designware_eth_free_pkt(struct udevice *dev, uchar *packet, int length)
{
	struct dwmac4_dev *priv = dev_get_priv(dev);

	return _dw_free_pkt(priv);
}

static void designware_eth_stop(struct udevice *dev)
{
	struct dwmac4_dev *priv = dev_get_priv(dev);

	return _dw_eth_halt(priv);
}

static int designware_eth_write_hwaddr(struct udevice *dev)
{
	struct eth_pdata *pdata = dev_get_platdata(dev);
	struct dwmac4_dev *priv = dev_get_priv(dev);

	return _dw_write_hwaddr(priv, pdata->enetaddr);
}

static int designware_eth_bind(struct udevice *dev)
{
	return 0;
}

int designware_eth_probe(struct udevice *dev)
{
	struct eth_pdata *pdata = dev_get_platdata(dev);
	struct dwmac4_dev *priv = dev_get_priv(dev);
	int ret, err;
	struct reset_ctl_bulk reset_bulk;
#ifdef CONFIG_CLK
	int i, clock_nb;

	priv->clock_count = 0;
	clock_nb = dev_count_phandle_with_args(dev, "clocks", "#clock-cells");
	if (clock_nb > 0) {
		priv->clocks = devm_kcalloc(dev, clock_nb, sizeof(struct clk),
					    GFP_KERNEL);
		if (!priv->clocks)
			return -ENOMEM;

		for (i = 0; i < clock_nb; i++) {
			err = clk_get_by_index(dev, i, &priv->clocks[i]);
			if (err < 0)
				break;

			err = clk_enable(&priv->clocks[i]);
			if (err && err != -ENOSYS && err != -ENOTSUPP) {
				pr_err("failed to enable clock %d\n", i);
				clk_free(&priv->clocks[i]);
				goto clk_err;
			}
			priv->clock_count++;
		}
	} else if (clock_nb != -ENOENT) {
		pr_err("failed to get clock phandle(%d)\n", clock_nb);
		return clock_nb;
	}
#endif

#if defined(CONFIG_DM_REGULATOR)
	struct udevice *phy_supply;

	ret = device_get_supply_regulator(dev, "phy-supply",
					  &phy_supply);
	if (ret) {
		debug("%s: No phy supply\n", dev->name);
	} else {
		ret = regulator_set_enable(phy_supply, true);
		if (ret) {
			puts("Error enabling phy supply\n");
			return ret;
		}
	}
#endif

	ret = reset_get_bulk(dev, &reset_bulk);
	if (ret)
		dev_warn(dev, "Can't get reset: %d\n", ret);
	else
		reset_deassert_bulk(&reset_bulk);

	debug("%s, iobase=%x, priv=%p\n", __func__, pdata->iobase, priv);
	priv->ioaddr = pdata->iobase;
	priv->interface = pdata->phy_interface;
	priv->max_speed = pdata->max_speed;

	ret = dw_mdio_init(dev->name, priv);
	if (ret) {
		err = ret;
		goto mdio_err;
	}
	priv->bus = miiphy_get_dev_by_name(dev->name);

	ret = dw_phy_init(priv, dev);
	debug("%s, ret=%d\n", __func__, ret);
	if (!ret)
		return 0;

	/* continue here for cleanup if no PHY found */
	err = ret;
	mdio_unregister(priv->bus);
	mdio_free(priv->bus);
mdio_err:

#ifdef CONFIG_CLK
clk_err:
	ret = clk_release_all(priv->clocks, priv->clock_count);
	if (ret)
		pr_err("failed to disable all clocks\n");

#endif
	return err;
}

static int designware_eth_remove(struct udevice *dev)
{
	struct dwmac4_dev *priv = dev_get_priv(dev);

	free(priv->phydev);
	mdio_unregister(priv->bus);
	mdio_free(priv->bus);

#ifdef CONFIG_CLK
	return clk_release_all(priv->clocks, priv->clock_count);
#else
	return 0;
#endif
}

const struct eth_ops designware_eth_ops = {
	.start			= designware_eth_start,
	.send			= designware_eth_send,
	.recv			= designware_eth_recv,
	.free_pkt		= designware_eth_free_pkt,
	.stop			= designware_eth_stop,
	.write_hwaddr		= designware_eth_write_hwaddr,
};

static int designware_eth_ofdata_to_platdata(struct udevice *dev)
{
#if CONFIG_IS_ENABLED(DM_GPIO)
	struct dwmac4_dev *priv = dev_get_priv(dev);
#endif
	struct eth_pdata *pdata = dev_get_platdata(dev);
	const char *phy_mode;
#if CONFIG_IS_ENABLED(DM_GPIO)
	int reset_flags = GPIOD_IS_OUT;
#endif
	int ret = 0;

	pdata->iobase = dev_read_addr(dev);
	pdata->phy_interface = -1;
	phy_mode = dev_read_string(dev, "phy-mode");
	if (phy_mode)
		pdata->phy_interface = phy_get_interface_by_name(phy_mode);
	if (pdata->phy_interface == -1) {
		debug("%s: Invalid PHY interface '%s'\n", __func__, phy_mode);
		return -EINVAL;
	}

	pdata->max_speed = dev_read_u32_default(dev, "max-speed", 0);

#if CONFIG_IS_ENABLED(DM_GPIO)
	if (dev_read_bool(dev, "snps,reset-active-low"))
		reset_flags |= GPIOD_ACTIVE_LOW;

	ret = gpio_request_by_name(dev, "snps,reset-gpio", 0,
		&priv->reset_gpio, reset_flags);
	if (ret == 0) {
		ret = dev_read_u32_array(dev, "snps,reset-delays-us",
					 priv->reset_delays, 3);
	} else if (ret == -ENOENT) {
		ret = 0;
	}
#endif

	return ret;
}

static const struct udevice_id designware_eth_ids[] = {
	{ .compatible = "snps,dwmac-5.20" },
	{ }
};

U_BOOT_DRIVER(eth_designware) = {
	.name	= "eth_dwmac4",
	.id	= UCLASS_ETH,
	.of_match = designware_eth_ids,
	.ofdata_to_platdata = designware_eth_ofdata_to_platdata,
	.bind	= designware_eth_bind,
	.probe	= designware_eth_probe,
	.remove	= designware_eth_remove,
	.ops	= &designware_eth_ops,
	.priv_auto_alloc_size = sizeof(struct dwmac4_dev),
	.platdata_auto_alloc_size = sizeof(struct eth_pdata),
	.flags = DM_FLAG_ALLOC_PRIV_DMA,
};

#endif
