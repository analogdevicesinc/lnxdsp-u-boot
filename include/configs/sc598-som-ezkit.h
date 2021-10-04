/*
 * U-boot - Configuration file for sc589 EZ-Kit board
 */

#ifndef __CONFIG_SC598_EZKIT_H
#define __CONFIG_SC598_EZKIT_H

#include <asm/arch/config.h>
#include <linux/sizes.h>

#define CONFIG_GICV3
#define GICD_BASE 0x31200000
#define GICR_BASE 0x31240000

/* ARM Timer */
#define COUNTER_FREQUENCY		31250000 //500/16=31.25 MHz

/*
 * Processor Settings
 */
#define CONFIG_ARMV8_SPIN_TABLE

//#define CONFIG_ADI_CPU		"ADSP-SC589-0.1"
#ifdef CONFIG_SC59X_CHAIN_BOOT
# define CONFIG_LOADADDR	0x84000000
# define CONFIG_RSA		/* RSA for FIT authen. */
#else
# define CONFIG_LOADADDR	0xB2000000
#endif
#define CONFIG_DTBLOADADDR	"0x84000000"
#define CONFIG_MACH_TYPE	MACH_TYPE_SC594_SOM_EZKIT

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

//#define ADI_VCO_HZ  (CONFIG_CLKIN_HZ * CONFIG_VCO_MULT)
//#define ADI_CCLK_HZ (ADI_VCO_HZ / CONFIG_CCLK_DIV)
//#define ADI_SCLK_HZ (ADI_VCO_HZ / CONFIG_SCLK_DIV)

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
#define CONFIG_SYS_LOAD_ADDR	0x0
#define CONFIG_SYS_INIT_SP_ADDR (CONFIG_SYS_SDRAM_BASE + 0x3F000)

#define CONFIG_SYS_MONITOR_LEN	0
#define CONFIG_SYS_MALLOC_LEN	(1024*1024*1)

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
#define CONFIG_DTBNAME		"sc598-som-ezkit.dtb"
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

/*
 * USB Settings
 */
//#define CONFIG_MUSB_HCD
//#define CONFIG_USB_SC59X
#define CONFIG_MUSB_TIMEOUT 100000
#define MUSB_HW_VERSION2
#define CONFIG_USB_STORAGE

/*
 * SDH Settings
 */
/*
#define CONFIG_GENERIC_MMC
#define CONFIG_MMC
#define CONFIG_MMC_SPI
#define CONFIG_CMD_MMC_SPI
*/

/* OSPI - Via Device Tree Support
 *
 */
#ifdef CONFIG_OF_CONTROL

#define CONFIG_CMD_DM

#define CONFIG_SPI_FLASH
#define CONFIG_SPI_FLASH_MACRONIX
#define CONFIG_SPI_FLASH_ISSI
#define CONFIG_SPI_FLASH_BAR
#define CONFIG_SPI_FLASH_MTD

#define CONFIG_CMD_SF

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
#define CONFIG_DEBUG_EARLY_SERIAL
//#define CONFIG_SYS_NO_FLASH
#define CONFIG_UART_CONSOLE	0
#define CONFIG_BAUDRATE		57600
//#define CONFIG_UART4_SERIAL
#define CONFIG_LINUX_MEMSIZE	"992M"
#define CONFIG_CMD_BOOTZ

#define CONFIG_BOOTCOMMAND	"run spiboot"
#define INITRAMADDR "0x85000000"

#define ADI_ENV_SETTINGS \
	"fdt_high=0xFFFFFFFFFFFFFFFF\0" \
	"initrd_high=0xFFFFFFFFFFFFFFFF\0" \
	"rfsfile=adsp-sc5xx-minimal-adsp-sc598-som-ezkit.jffs2\0" \
	"dtbsize=0x20000\0" \
	"imagesize=0xF00000\0" \
	"initramfs_file=initramfs.cpio.gz.uboot\0" \
	"ramboot=mii info; dhcp; setenv serverip 192.168.1.239; tftp 0x90080000 Image; tftp 0x84000000 sc598-som-ezkit.dtb; tftp 0x85000000 ${initramfs_file}; run ramargs; booti 0x90080000 0x85000000 0x84000000\0" \
	"update_qspi_sc598=sf probe 2:1; sf erase 0 0x4000000; run update_qspi_uboot; run update_qspi_dtb; run update_qspi_Image; run update_qspi_rfs; sleep 3; saveenv\0" \
	"update_qspi_uboot=tftp ${loadaddr} ${ubootfile}; sf probe 2:1; sf write ${loadaddr} 0x0 ${filesize}\0" \
	"update_qspi_rfs=tftp ${loadaddr} ${rfsfile}; sf probe 2:1; sf write ${loadaddr} 0xFC0000 ${filesize};\0" \
	"update_qspi_Image=tftp ${loadaddr} ${ramfile}; sf probe 2:1; sf write ${loadaddr} 0xC0000 ${filesize}; setenv imagesize ${filesize};\0" \
	"update_qspi_dtb=tftp ${loadaddr} ${dtbfile}; sf probe 2:1; sf write ${loadaddr} 0xA0000 ${filesize}; setenv dtbsize ${filesize};\0"\
	"spiargs=setenv bootargs " ADI_BOOTARGS_SPI "\0" \
	"spiboot=run qspi_boot_sc598\0" \
	"qspi_boot_sc598=run spiargs; sf probe 2:1; sf read ${loadaddr} 0xC0000 ${imagesize}; sf read ${dtbaddr} 0xA0000 ${dtbsize}; booti ${loadaddr} - ${dtbaddr}\0"

#define ADI_BOOTARGS_SPI \
        "root=/dev/mtdblock4 " \
        "rootfstype=jffs2 " \
        "clkin_hz=" __stringify(CONFIG_CLKIN_HZ) " " \
        ADI_BOOTARGS_VIDEO \
        ADI_EARLYPRINTK \
        "console=ttySC" __stringify(CONFIG_UART_CONSOLE) "," \
                        __stringify(CONFIG_BAUDRATE) " "\
        "mem=" CONFIG_LINUX_MEMSIZE

#include <configs/sc_adi_common.h>

#endif
