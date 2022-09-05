/*
 * U-boot - Configuration file for sc573 EZ-Kit board
 */

#ifndef __CONFIG_SC573_EZKIT_H
#define __CONFIG_SC573_EZKIT_H

#include <asm/mach-adi/common/config.h>

/*
 * Processor Settings
 */
#define CONFIG_ADI_CPU		"ADSP-SC573-0.0"
#ifdef CONFIG_SC57X_CHAIN_BOOT
# define CONFIG_LOADADDR	0x84000000
# define CONFIG_RSA		/* RSA for FIT authen. */
#else
# define CONFIG_LOADADDR	0x82000000
#endif

#define CONFIG_DTBNAME		"sc573-ezkit.dtb"
#define CONFIG_DTBLOADADDR	"0x84000000"
#define CONFIG_MACH_TYPE	MACH_TYPE_SC573_EZKIT

#define CONFIG_SYS_ARM_CACHE_WRITETHROUGH
#define CONFIG_SYS_BOOTM_LEN 0x1800000
#define CONFIG_SYS_TIMERBASE	TIMER0_CONFIG

/*
 * Memory Settings
 */
#define MEM_MT41K128M16JT
#define MEM_DMC0

#define	CONFIG_NR_DRAM_BANKS		1
#define CONFIG_SYS_SDRAM_BASE	0x82000000
#define CONFIG_SYS_SDRAM_SIZE	0xe000000
//#define CONFIG_SYS_TEXT_BASE	0x82200000
#define CONFIG_SYS_LOAD_ADDR	0x0
#define CONFIG_SYS_INIT_SP_ADDR (CONFIG_SYS_SDRAM_BASE + 0x3f000)

//#define CONFIG_SMC_GCTL_VAL	0x00000010
//#define CONFIG_SMC_B0CTL_VAL	0x01007011
//#define CONFIG_SMC_B0TIM_VAL	0x08170977
//#define CONFIG_SMC_B0ETIM_VAL	0x00092231

#define CONFIG_SYS_MONITOR_LEN	(0)
#define CONFIG_SYS_MALLOC_LEN	(1024 * 1024)

/* Reserve 4MB in DRAM for Tlb, Text, Malloc pool, Global data, Stack, etc. */
//#define CONFIG_SYS_MEMTEST_RESERVE_SIZE	(4 * 1024 * 1024)
#define CONFIG_SYS_MEMTEST_START		CONFIG_SYS_SDRAM_BASE
#define CONFIG_SYS_MEMTEST_END			(CONFIG_SYS_SDRAM_BASE + \
				CONFIG_SYS_SDRAM_SIZE - \
				CONFIG_SYS_MEMTEST_RESERVE_SIZE)


/*
 * Network Settings
 */
#define CONFIG_HOSTNAME		"sc57x"
#define CONFIG_DW_ALTDESCRIPTOR
#define CONFIG_DW_AXI_BURST_LEN 16
#define CONFIG_PHY_TI
#define CONFIG_ETHADDR	02:80:ad:20:31:e8

/*
 * USB Settings
 */
#define MUSB_HW_VERSION2
#define CONFIG_USB_STORAGE

/*
 * Misc Settings
 */
#define CONFIG_UART_CONSOLE	0
#define CONFIG_LINUX_MEMSIZE	"224M"

#define CONFIG_BOOTCOMMAND	"run ramboot"
#define INITRAMADDR "0x85000000"

#include <configs/sc_adi_common.h>

#endif
