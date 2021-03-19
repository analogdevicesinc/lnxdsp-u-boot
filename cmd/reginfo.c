// SPDX-License-Identifier: GPL-2.0+
/*
 * (C) Copyright 2000
 * Subodh Nijsure, SkyStream Networks, snijsure@skystream.com
 */

#include <common.h>
#include <command.h>
#ifdef CONFIG_PPC
#include <asm/ppc.h>
#endif

static int do_reginfo(struct cmd_tbl *cmdtp, int flag, int argc,
		      char *const argv[])
{
	return 0;
}

 /**************************************************/

U_BOOT_CMD(
	reginfo,	2,	1,	do_reginfo,
	"print register information",
	""
);
