// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (C) 2016 Socionext Inc.
 *   Author: Masahiro Yamada <yamada.masahiro@socionext.com>
 */

#include <common.h>
#include <linux/libfdt.h>
#include <asm/spin_table.h>

unsigned long spin_table_relocate(unsigned long * rsv_addr, unsigned long * rsv_size){
	unsigned long cpu_release_addr;

	//Calculate the offset of the old release addr, relative to rsv_addr
	cpu_release_addr = (unsigned long)&spin_table_cpu_release_addr - *rsv_addr;

	//Shift it by the relocation addr
	cpu_release_addr += CONFIG_ARMV8_SPIN_TABLE_RELOCATE_ADDR;

	//Copy the old spin table into the relocated address
	memcpy(CONFIG_ARMV8_SPIN_TABLE_RELOCATE_ADDR, *rsv_addr, *rsv_size);

	*rsv_addr = CONFIG_ARMV8_SPIN_TABLE_RELOCATE_ADDR;

	return cpu_release_addr;
}

int spin_table_update_dt(void *fdt)
{
	int cpus_offset, offset;
	const char *prop;
	int ret;
	unsigned long rsv_addr = (unsigned long)&spin_table_reserve_begin;
	unsigned long rsv_size = &spin_table_reserve_end -
						&spin_table_reserve_begin;

	cpus_offset = fdt_path_offset(fdt, "/cpus");
	if (cpus_offset < 0)
		return -ENODEV;

	for (offset = fdt_first_subnode(fdt, cpus_offset);
	     offset >= 0;
	     offset = fdt_next_subnode(fdt, offset)) {
		prop = fdt_getprop(fdt, offset, "device_type", NULL);
		if (!prop || strcmp(prop, "cpu"))
			continue;

		/*
		 * In the first loop, we check if every CPU node specifies
		 * spin-table.  Otherwise, just return successfully to not
		 * disturb other methods, like psci.
		 */
		prop = fdt_getprop(fdt, offset, "enable-method", NULL);
		if (!prop || strcmp(prop, "spin-table"))
			return 0;
	}

	for (offset = fdt_first_subnode(fdt, cpus_offset);
	     offset >= 0;
	     offset = fdt_next_subnode(fdt, offset)) {
		prop = fdt_getprop(fdt, offset, "device_type", NULL);
		if (!prop || strcmp(prop, "cpu"))
			continue;

#if defined(CONFIG_ARMV8_SPIN_TABLE_RELOCATE_ADDR)
		ret = fdt_setprop_u64(fdt, offset, "cpu-release-addr",
				spin_table_relocate(&rsv_addr, &rsv_size));
#else
		ret = fdt_setprop_u64(fdt, offset, "cpu-release-addr",
				(unsigned long)&spin_table_cpu_release_addr);
#endif
		if (ret)
			return -ENOSPC;
	}

	ret = fdt_add_mem_rsv(fdt, rsv_addr, rsv_size);
	if (ret)
		return -ENOSPC;

	printf("   Reserved memory region for spin-table: addr=%lx size=%lx\n",
	       rsv_addr, rsv_size);

	return 0;
}
