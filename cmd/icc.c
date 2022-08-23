/*
 * cmd_icc.c - inter core communication interface for sc5xx
 *
 * Copyright (c) 2015 Analog Devices Inc.
 *
 * Licensed under the GPL-2 or later.
 */

#include <config.h>
#include <common.h>
#include <command.h>
#include <asm/mach-adi/common/sc5xx.h>
#include <asm/io.h>
#include <linux/delay.h>

static void icc_queue_l2_init(void)
{
	memset((void *)L2_SRAM_BASE, 0, 0x1000);
}

void icc_enable_core(int coreid)
{
	icc_queue_l2_init();

	/* clear CRSTAT bit for given coreid */
	writel(1 << coreid, RCU0_CRSTAT);

	if (!(readl(RCU0_CRCTL) & 1 << coreid)) {
		/* disable the system interface */
		writel(readl(RCU0_SIDIS) | 1 << (coreid - 1), RCU0_SIDIS);
		/* Anomaly 36-10-0005: SISTAT doesn't acknowledge set status */
		/* while (readl(RCU0_SISTAT) & 1 << (coreid - 1) != 1); */
		udelay(50);

		/* put core in reset */
		writel(readl(RCU0_CRCTL) | 1 << coreid, RCU0_CRCTL);
		/* reenable the system interface */
		writel(readl(RCU0_SIDIS) & ~(1 << (coreid - 1)), RCU0_SIDIS);
		/* Anomaly 36-10-0005: SISTAT doesn't acknowledge set status */
		/* while (readl(RCU0_SISTAT) & 1 << (coreid - 1) != 0); */
		udelay(50);
	}

	/* move core out of reset */
	writel(readl(RCU0_CRCTL) & ~(1 << coreid), RCU0_CRCTL);
	/* clear CRSTAT bit for given coreid */
	writel(1 << coreid, RCU0_CRSTAT);
	/* notify CCES */
	writel(1 << (18 + coreid), RCU0_MSG_SET);
}

void icc_disable_core(int coreid)
{
	if (readl(RCU0_CRCTL) & 1 << coreid) {
		printf("Core %d is already disabled.\n", coreid);
		return;
	}

	/* clear CRSTAT bit for given coreid */
	writel(1 << coreid, RCU0_CRSTAT);
	/* disable the system interface */
	writel(readl(RCU0_SIDIS) | 1 << (coreid - 1), RCU0_SIDIS);
	/* Anomaly 36-10-0005: SISTAT doesn't acknowledge set status */
	/* while (readl(RCU0_SISTAT) & 1 << (coreid - 1) != 1); */
	udelay(50);

	/* put core in reset */
	writel(readl(RCU0_CRCTL) | 1 << coreid, RCU0_CRCTL);
	/* reenable the system interface */
	writel(readl(RCU0_SIDIS) & ~(1 << (coreid - 1)), RCU0_SIDIS);
	/* Anomaly 36-10-0005: SISTAT doesn't acknowledge set status */
	/* while (readl(RCU0_SISTAT) & 1 << (coreid - 1) != 0); */
	udelay(50);

	/* clear CRSTAT bit for given coreid */
	writel(1 << coreid, RCU0_CRSTAT);
}

//int do_icc(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
int do_icc(struct cmd_tbl *cmdtp, int flag, int argc, char * const argv[])
{
	char *cmd;
	int coreid;

	if (argc < 3)
		return CMD_RET_USAGE;

	coreid = simple_strtoul(argv[2], NULL, 16);
	if (coreid < 1 || coreid > 2) {
		printf("Only core 1 and 2 are valid!\n");
		return CMD_RET_USAGE;
	}

	cmd = argv[1];
	if (!strcmp(cmd, "enable"))
		icc_enable_core(coreid);
	else if (!strcmp(cmd, "disable"))
		icc_disable_core(coreid);
	else
		return CMD_RET_USAGE;

	return CMD_RET_SUCCESS;
}

U_BOOT_CMD(
	icc, 3, 0, do_icc,
	"Inter core communication interface",
	"icc enable <coreid>\n"
	" - enable coreid\n"
	"icc disable <coreid>\n"
	" - disable coreid\n"
);
