/*
 * U-boot - Configuration file for sc594 SOM EZ-Kit board
 */

#ifndef __CONFIG_SC594_EZKIT_H
#define __CONFIG_SC594_EZKIT_H

#include <asm/arch/config.h>
#include <linux/sizes.h>

/*
 * Processor Settings
 */
//#define CONFIG_ADI_CPU		"ADSP-SC589-0.1"
#ifdef CONFIG_SC59X_CHAIN_BOOT
# define CONFIG_LOADADDR	0x84000000
# define CONFIG_RSA		/* RSA for FIT authen. */
#else
# define CONFIG_LOADADDR	0x82000000
#endif
#define CONFIG_DTBLOADADDR	"0x84000000"
#define CONFIG_MACH_TYPE	MACH_TYPE_SC594_SOM_EZKIT
#define CONFIG_SYS_ARM_CACHE_WRITETHROUGH

//L2 SRAM:
//TEXT:   0x20080000 to 0x2008FFFF (64KB)
//BSS:    0x20090000 to 0x2009FFFF (64KB)
//MALLOC: 0x200A0000 to 0x200AFFFF (64KB)
//STACK:  0x200B0000 to 0x200BFFFF (64KB)
#define CONFIG_SPL_MAX_SIZE		SZ_256K //(overkill)
#ifdef CONFIG_SPL_BUILD
#define CONFIG_SKIP_LOWLEVEL_INIT
#define CONFIG_SPL_BSS_START_ADDR	0x20090000
#define CONFIG_SPL_BSS_MAX_SIZE		SZ_64K
#define CONFIG_SYS_SPL_MALLOC_START	(CONFIG_SPL_BSS_START_ADDR + CONFIG_SPL_BSS_MAX_SIZE)
#define CONFIG_SYS_SPL_MALLOC_SIZE	SZ_64K
#define CONFIG_SPL_STACK		(CONFIG_SYS_SPL_MALLOC_START + CONFIG_SYS_SPL_MALLOC_SIZE + CONFIG_SPL_STACK_SIZE)
#define CONFIG_SPL_STACK_SIZE		SZ_64K
#endif

/*
 * Clock Settings
 *	CCLK = (CLKIN * VCO_MULT) / CCLK_DIV
 *	SCLK = (CLKIN * VCO_MULT) / SYSCLK_DIV
 *	SCLK0 = SCLK / SCLK0_DIV
 *	SCLK1 = SCLK / SCLK1_DIV
 */
/* CONFIG_CLKIN_HZ is any value in Hz					*/
//#define CONFIG_CLKIN_HZ			(25000000)
/* CLKIN_HALF controls the DF bit in PLL_CTL      0 = CLKIN		*/
/*                                                1 = CLKIN / 2		*/
#define CONFIG_CLKIN_HALF		(0)

#define CONFIG_CGU1_VCO_MULT	 (64)
#define CONFIG_CGU1_CCLK_DIV	 (2)
#define CONFIG_CGU1_SCLK_DIV	 (4)
#define CONFIG_CGU1_SCLK0_DIV	 (4)
#define CONFIG_CGU1_SCLK1_DIV	 (2)
#define CONFIG_CGU1_DCLK_DIV	 (2)
#define CONFIG_CGU1_OCLK_DIV	 (16)
#define CONFIG_CGU1_DIV_S1SELEX  (8)

#define CONFIG_VCO_MULT			 (80)
#define CONFIG_CCLK_DIV			 (2)
#define CONFIG_SCLK_DIV			 (4)
#define CONFIG_SCLK0_DIV		 (4)
#define CONFIG_SCLK1_DIV		 (2)
#define CONFIG_DCLK_DIV			 (2)
#define CONFIG_OCLK_DIV			 (8)
#define CONFIG_DIV_S1SELEX       (6)

#define ADI_VCO_HZ  (CONFIG_CLKIN_HZ * CONFIG_VCO_MULT)
#define ADI_CCLK_HZ (ADI_VCO_HZ / CONFIG_CCLK_DIV)
#define ADI_SCLK_HZ (ADI_VCO_HZ / CONFIG_SCLK_DIV)

//#define CONFIG_SYS_TIMERGROUP	TIMER_GROUP
#define CONFIG_SYS_TIMERBASE	TIMER0_CONFIG

/*
 * Memory Settings
 */
#define MEM_ISSI_8Gb_DDR3_800MHZ
#define MEM_DMC0

#define	CONFIG_NR_DRAM_BANKS		1
#define CONFIG_SYS_SDRAM_BASE	0x82000000
#define CONFIG_SYS_SDRAM_SIZE	0x3E000000
#define CONFIG_SYS_TEXT_BASE	0xB2200000
#define CONFIG_SYS_LOAD_ADDR	0x0
#define CONFIG_SYS_INIT_SP_ADDR (CONFIG_SYS_SDRAM_BASE + 0x3f000)

#define CONFIG_SMC_GCTL_VAL	0x00000010
#define CONFIG_SMC_B0CTL_VAL	0x01007011
#define CONFIG_SMC_B0TIM_VAL	0x08170977
#define CONFIG_SMC_B0ETIM_VAL	0x00092231

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
#define ADI_CMDS_NETWORK
#define CONFIG_NETCONSOLE
#define CONFIG_NET_MULTI
#define CONFIG_DTBNAME		"sc594-som-ezkit.dtb"
#define CONFIG_HOSTNAME		"sc59x"
#define CONFIG_DESIGNWARE_ETH
#define CONFIG_DW_PORTS		2
//#define CONFIG_DW_AUTONEG
#define CONFIG_DW_ALTDESCRIPTOR
#define CONFIG_DW_AXI_BURST_LEN 16
#define CONFIG_MII
//#define CONFIG_PHYLIB
#define CONFIG_PHY_TI
#define CONFIG_ETHADDR	02:80:ad:20:31:e8
#define CONFIG_ETH1ADDR	02:80:ad:20:31:e9

/*
 * I2C Settings
 */
#define CONFIG_SYS_I2C
//#define CONFIG_SYS_I2C_ADI
#define CONFIG_SYS_MAX_I2C_BUS 3

/* OSPI - Via Device Tree Support
 *
 */
#ifdef CONFIG_OF_CONTROL

#define CONFIG_CMD_DM
#define CONFIG_CADENCE_QSPI

#define CONFIG_SPI_FLASH
#define CONFIG_SPI_FLASH_MACRONIX
#define CONFIG_SPI_FLASH_ISSI
#define CONFIG_SPI_FLASH_BAR
#define CONFIG_SPI_FLASH_MTD

#define CONFIG_CQSPI_REF_CLK		500000000
//#define CONFIG_CQSPI_DECODER		0
#define CONFIG_CMD_SF
//#define CONFIG_CMD_SPI

#endif

/*
 * SPI - Via Device Tree Support
 */
#define CONFIG_ADI_SPI3_DM
#define CONFIG_SC59X_SPI
#define CONFIG_CMD_SPI
#define CONFIG_ENV_SPI_MAX_HZ	10000000
#define CONFIG_SF_DEFAULT_SPEED	10000000
#define CONFIG_SF_DEFAULT_MODE  SPI_MODE_3

/*
 * Misc Settings
 */
//#define CONFIG_DEBUG_EARLY_SERIAL
//#define CONFIG_SYS_NO_FLASH
#define CONFIG_UART_CONSOLE	0
#define CONFIG_BAUDRATE		57600
//#define CONFIG_UART4_SERIAL
#define CONFIG_LINUX_MEMSIZE	"992M"
#define CONFIG_CMD_BOOTZ

#define CONFIG_BOOTCOMMAND	"run spiboot"
#define INITRAMADDR "0x85000000"

#define ADI_ENV_SETTINGS \
	"fdt_high=0xFFFFFFFF\0" \
	"rfsfile=adsp-sc5xx-minimal-adsp-sc594-som-ezkit.jffs2\0" \
	"dtbsize=0x20000\0" \
	"zimagesize=0x600000\0" \
	"update_ospi_sc594=sf probe 0:0; sf erase 0 0x2000000; run update_ospi_uboot; run update_ospi_dtb; run update_ospi_zImage; run update_ospi_rfs; sleep 3; saveenv\0" \
	"update_ospi_uboot=tftp ${loadaddr} ${ubootfile}; sf probe 0:0; sf write ${loadaddr} 0x0 ${filesize}\0" \
	"update_ospi_rfs=tftp ${loadaddr} ${rfsfile}; sf probe 0:0; sf write ${loadaddr} 0x6C0000 ${filesize};\0" \
	"update_ospi_zImage=tftp ${loadaddr} ${ramfile}; sf probe 0:0; sf write ${loadaddr} 0xC0000 ${filesize}; setenv zimagesize ${filesize};\0" \
	"update_ospi_dtb=tftp ${loadaddr} ${dtbfile}; sf probe 0:0; sf write ${loadaddr} 0xA0000 ${filesize}; setenv dtbsize ${filesize};\0"\
	"spiargs=setenv bootargs " ADI_BOOTARGS_SPI "\0" \
	"spiboot=run ospi_boot_sc594\0" \
	"ospi_boot_sc594=run spiargs; sf probe 0:0; sf read ${loadaddr} 0xC0000 ${zimagesize}; sf read ${dtbaddr} 0xA0000 ${dtbsize}; bootz ${loadaddr} - ${dtbaddr}\0"

#define ADI_BOOTARGS_SPI \
        "root=/dev/mtdblock4 " \
        "rootfstype=jffs2 " \
        "clkin_hz=" __stringify(CONFIG_CLKIN_HZ) " " \
        ADI_BOOTARGS_VIDEO \
        "earlyprintk=serial,uart0,57600 " \
        "console=ttySC" __stringify(CONFIG_UART_CONSOLE) "," \
                        __stringify(CONFIG_BAUDRATE) " "\
        "mem=" CONFIG_LINUX_MEMSIZE

#include <configs/sc_adi_common.h>

#endif
