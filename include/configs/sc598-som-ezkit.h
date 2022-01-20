/*
 * U-boot - Configuration file for sc589 EZ-Kit board
 */

#ifndef __CONFIG_SC598_EZKIT_H
#define __CONFIG_SC598_EZKIT_H

#include <asm/arch/config.h>
#include <linux/sizes.h>

/* Do we have a carrier board attached */
#define ADI_HAVE_CARRIER 1

/* GIC */
#define CONFIG_GICV3
#define GICD_BASE 0x31200000
#define GICR_BASE 0x31240000

/* ARM Timer */
#define COUNTER_FREQUENCY		31250000 //500/16=31.25 MHz

/*
 * Processor Settings
 */

//#define CONFIG_ADI_CPU		"ADSP-SC589-0.1"
#ifdef CONFIG_SC59X_CHAIN_BOOT
# define CONFIG_LOADADDR	0x84000000
# define CONFIG_RSA		/* RSA for FIT authen. */
#else
# define CONFIG_LOADADDR	0x88000000
#endif
#define CONFIG_DTBLOADADDR	"0x84000000"
#define CONFIG_MACH_TYPE	MACH_TYPE_SC594_SOM_EZKIT

//L2 SRAM:
//TEXT:   0x20080000 to 0x2008FFFF (64KB)
//BSS:    0x20090000 to 0x2009FFFF (64KB)
//MALLOC: 0x200A0000 to 0x200AFFFF (64KB)
//STACK:  0x200B0000 to 0x200BFFFF (64KB)
#define CONFIG_SPL_MAX_SIZE		SZ_128K //(overkill)
#ifdef CONFIG_SPL_BUILD
#define CONFIG_SPL_BSS_START_ADDR	0x200A0000
#define CONFIG_SPL_BSS_MAX_SIZE		SZ_64K
#define CONFIG_SYS_SPL_MALLOC_START	(CONFIG_SPL_BSS_START_ADDR + CONFIG_SPL_BSS_MAX_SIZE)
#define CONFIG_SYS_SPL_MALLOC_SIZE	SZ_64K
#define CONFIG_SPL_STACK		(CONFIG_SYS_SPL_MALLOC_START + CONFIG_SYS_SPL_MALLOC_SIZE + CONFIG_SPL_STACK_SIZE)
#define CONFIG_SPL_STACK_SIZE		SZ_64K

//Parameters used for Falcon boot
#define CONFIG_SYS_SPI_ARGS_OFFS   0xE0000    // This is where the DTB should be stored
#define CONFIG_SYS_SPI_ARGS_SIZE   0x10000    // Max size of the DTB
#define CONFIG_SYS_SPI_KERNEL_OFFS 0x100000   // Where the kernel Image should be stored
#define CONFIG_SYS_SPL_ARGS_ADDR   0x84000000 // Where to load the DTB into RAM
#define CONFIG_SYS_LOAD_ADDR       0x90000000 // Where to load the Image into RAM
#define CONFIG_SYS_SPI_KERNEL_SKIP_HEADER
#define CONFIG_SYS_MMCSD_RAW_MODE_KERNEL_SECTOR 4096 // Position of kernel Image in sectors
#define CONFIG_SYS_MMCSD_RAW_MODE_ARGS_SECTOR   2048 // Position of DTB in sectors
#define CONFIG_SYS_MMCSD_RAW_MODE_ARGS_SECTORS  256  // Size of DTB in sectors
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

//#define ADI_VCO_HZ  (CONFIG_CLKIN_HZ * CONFIG_VCO_MULT)
//#define ADI_CCLK_HZ (ADI_VCO_HZ / CONFIG_CCLK_DIV)
//#define ADI_SCLK_HZ (ADI_VCO_HZ / CONFIG_SCLK_DIV)

//#define CONFIG_SYS_TIMERGROUP	TIMER_GROUP
#define CONFIG_SYS_TIMERBASE	TIMER0_CONFIG

/*
 * Memory Settings
 */
#define MEM_ISSI_4Gb_DDR3_800MHZ
#define MEM_DMC0

#define	CONFIG_NR_DRAM_BANKS		1
#define CONFIG_SYS_SDRAM_BASE	0x82000000
#define CONFIG_SYS_SDRAM_SIZE	0x1E000000
#ifndef CONFIG_SPL_BUILD
#define CONFIG_SYS_LOAD_ADDR	0x0
#endif
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
#define CONFIG_DW_PORTS		1
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

//Set this to 1 if you would like to use the maximum SPI speeds for OSPI and will not be using QSPI
#define ADI_USE_MACRONIX_OSPI 0
#define ADI_USE_MACRONIX_OSPI_DTR 0

//Allow U-Boot to dynamically pinmux between the three conflicting ezkit peripherals (OSPI/QSPI/UART)
//This allows U-boot to use all three peripherals
#define ADI_DYNAMIC_OSPI_QSPI_UART_MANAGEMENT

#define CONFIG_CMD_DM
#define CONFIG_CADENCE_QSPI

#define CONFIG_SPI_FLASH
#define CONFIG_SPI_FLASH_MACRONIX
#define CONFIG_SPI_FLASH_ISSI
#define CONFIG_SPI_FLASH_BAR
#define CONFIG_SPI_FLASH_MTD

#define CONFIG_CMD_SF

#define CONFIG_CQSPI_REF_CLK		500000000 // = 500 MHz

#endif

/*
 * SPI - Via Device Tree Support
 */
#define CONFIG_ADI_SPI3_DM
#define CONFIG_SC59X_SPI
#define CONFIG_CMD_SPI

#if ADI_USE_MACRONIX_OSPI_DTR
	//Maximum DTR read speed = 128mbyte / 1.90s = 67.36 mbyte/s
	#define ADI_OSPI_ENV_SPI_MAX_HZ		CONFIG_CQSPI_REF_CLK / 12 //41.66 MHz DTR (Double Transfer Rate)
	#define ADI_OSPI_SF_DEFAULT_SPEED	CONFIG_CQSPI_REF_CLK / 12 //41.66 MHz DTR (Double Transfer Rate)
#else
	//Maximum STR read speed = 128mbyte / 2.17s = 58.98 mbyte/s
	#define ADI_OSPI_ENV_SPI_MAX_HZ		CONFIG_CQSPI_REF_CLK / 6 //83.33 MHz STR (Single Transfer Rate)
	#define ADI_OSPI_SF_DEFAULT_SPEED	CONFIG_CQSPI_REF_CLK / 6 //83.33 MHz STR (Single Transfer Rate)
#endif

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
#define CONFIG_LINUX_MEMSIZE	"496M"
#define CONFIG_CMD_BOOTZ

#if ADI_USE_MACRONIX_OSPI
#define CONFIG_BOOTCOMMAND	"run ospiboot"
#else
#define CONFIG_BOOTCOMMAND	"run qspiboot"
#endif
#define INITRAMADDR "0x85000000"

#define ADI_ENV_SETTINGS \
	"fdt_high=0xFFFFFFFFFFFFFFFF\0" \
	"initrd_high=0xFFFFFFFFFFFFFFFF\0" \
	"rfsfile=adsp-sc5xx-minimal-adsp-sc598-som-ezkit.jffs2\0" \
	"dtbsize=0x20000\0" \
	"imagesize=0xF00000\0" \
	"initramfs_file=initramfs.cpio.gz.uboot\0" \
	ADI_INIT_ETHERNET \
	ADI_RAM_BOOT \
	ADI_OSPI_BOOT \
	ADI_QSPI_BOOT \
	ADI_EMMC_BOOT

#define ADI_INIT_ETHERNET \
	"init_ethernet=mii info; dhcp; setenv serverip ${tftpserverip};\0"

#define ADI_RAM_BOOT \
	"ramboot=run init_ethernet; tftp ${dtbaddr} ${dtbfile}; tftp ${loadaddr} ${ramfile}; tftp ${initramaddr} ${initramfile}; run ramargs; booti ${loadaddr} ${initramaddr} ${dtbaddr}\0"

#ifndef CONFIG_SPL_OS_BOOT
	#define ADI_UPDATE_OSPI_UBOOT \
		"update_ospi_uboot=tftp ${loadaddr} ${ubootfile}; sf probe 0:0; sf write ${loadaddr} 0x0 ${filesize};\0"
#else
	#define UBOOT_SPL_FILE "u-boot-" CONFIG_SYS_BOARD ".ldr"
	#define UBOOT_PROPER_FILE "u-boot-" CONFIG_SYS_BOARD ".img"
	#define ADI_UPDATE_OSPI_UBOOT \
		"ubootsplfile=" UBOOT_SPL_FILE "\0" \
		"ubootproperfile=" UBOOT_PROPER_FILE "\0" \
		"update_ospi_uboot_spl=tftp ${loadaddr} ${ubootsplfile}; sf probe 0:0; sf write ${loadaddr} 0x0 ${filesize};\0" \
		"update_ospi_uboot_proper=tftp ${loadaddr} ${ubootproperfile}; sf probe 0:0; sf write ${loadaddr} 0x20000 ${filesize};\0" \
		"update_ospi_uboot=run update_ospi_uboot_spl; run update_ospi_uboot_proper;\0"
#endif

#ifndef CONFIG_ADI_FALCON
	#define ADI_UPDATE_OSPI_DTB \
	"update_ospi_dtb=tftp ${loadaddr} ${dtbfile}; sf probe 0:0; sf write ${loadaddr} 0xE0000 ${filesize}; setenv dtbsize ${filesize};\0"
#else
	#define ADI_UPDATE_OSPI_DTB \
	"update_ospi_dtb=tftp ${loadaddr} ${dtbfile}; sf probe 0:0; run ospiargs; fdt addr ${loadaddr}; fdt resize 0x10000; fdt boardsetup; fdt chosen; sf write ${loadaddr} 0xE0000 0x10000; setenv dtbsize 0x10000;\0"
#endif

#define ADI_OSPI_BOOT \
	"update_ospi_sc598=run init_ethernet; sf probe 0:0; sf erase 0 0x4000000; run update_ospi_uboot; run update_ospi_dtb; run update_ospi_Image; run update_ospi_rfs; setenv bootcmd \'run ospiboot\'; sleep 3; saveenv\0" \
	ADI_UPDATE_OSPI_UBOOT \
	"update_ospi_rfs=tftp ${loadaddr} ${rfsfile}; sf probe 0:0; sf write ${loadaddr} 0x1000000 ${filesize};\0" \
	"update_ospi_Image=tftp ${loadaddr} ${ramfile}; sf probe 0:0; sf write ${loadaddr} 0x100000 ${filesize}; setenv imagesize ${filesize};\0" \
	ADI_UPDATE_OSPI_DTB \
	"ospiargs=setenv bootargs " ADI_BOOTARGS_OSPI "\0" \
	"ospi_boot_sc598=run ospiargs; sf probe 0:0; sf read ${loadaddr} 0x100000 ${imagesize}; sf read ${dtbaddr} 0xE0000 ${dtbsize}; booti ${loadaddr} - ${dtbaddr}\0" \
	"ospiboot=run ospi_boot_sc598\0"

#ifndef CONFIG_SPL_OS_BOOT
	#define ADI_UPDATE_QSPI_UBOOT \
		"update_ospi_uboot=tftp ${loadaddr} ${ubootfile}; sf probe 2:1; sf write ${loadaddr} 0x0 ${filesize};\0"
#else
	#define UBOOT_SPL_FILE "u-boot-spl-" CONFIG_SYS_BOARD ".ldr"
	#define UBOOT_PROPER_FILE "u-boot-proper-" CONFIG_SYS_BOARD ".img"
	#define ADI_UPDATE_QSPI_UBOOT \
		"ubootsplfile=" UBOOT_SPL_FILE "\0" \
		"ubootproperfile=" UBOOT_PROPER_FILE "\0" \
		"update_qspi_uboot_spl=tftp ${loadaddr} ${ubootsplfile}; sf probe 2:1; sf write ${loadaddr} 0x0 ${filesize};\0" \
		"update_qspi_uboot_proper=tftp ${loadaddr} ${ubootproperfile}; sf probe 2:1; sf write ${loadaddr} 0x20000 ${filesize};\0" \
		"update_qspi_uboot=run update_qspi_uboot_spl; run update_qspi_uboot_proper;\0"
#endif

#ifndef CONFIG_ADI_FALCON
	#define ADI_UPDATE_QSPI_DTB \
	"update_qspi_dtb=tftp ${loadaddr} ${dtbfile}; sf probe 2:1; sf write ${loadaddr} 0xE0000 ${filesize}; setenv dtbsize ${filesize};\0"
#else
	#define ADI_UPDATE_QSPI_DTB \
	"update_qspi_dtb=tftp ${loadaddr} ${dtbfile}; sf probe 2:1; run qspiargs; fdt addr ${loadaddr}; fdt resize 0x10000; fdt boardsetup; fdt chosen; sf write ${loadaddr} 0xE0000 0x10000; setenv dtbsize 0x10000;\0"
#endif

#define ADI_QSPI_BOOT \
	"update_qspi_sc598=run init_ethernet; sf probe 2:1; sf erase 0 0x4000000; run update_qspi_uboot; run update_qspi_dtb; run update_qspi_Image; run update_qspi_rfs; setenv bootcmd \'run qspiboot\'; sleep 3; saveenv\0" \
	ADI_UPDATE_QSPI_UBOOT \
	"update_qspi_rfs=tftp ${loadaddr} ${rfsfile}; sf probe 2:1; sf write ${loadaddr} 0x1000000 ${filesize};\0" \
	"update_qspi_Image=tftp ${loadaddr} ${ramfile}; sf probe 2:1; sf write ${loadaddr} 0x100000 ${filesize}; setenv imagesize ${filesize};\0" \
	ADI_UPDATE_QSPI_DTB \
	"qspiargs=setenv bootargs " ADI_BOOTARGS_QSPI "\0" \
	"qspi_boot_sc598=run qspiargs; sf probe 2:1; sf read ${loadaddr} 0x100000 ${imagesize}; sf read ${dtbaddr} 0xE0000 ${dtbsize}; booti ${loadaddr} - ${dtbaddr}\0" \
	"qspiboot=run qspi_boot_sc598\0"

#define ADI_EMMC_BOOT \
	"update_emmc_sc598=setenv bootcmd \'run emmcboot\'; saveenv; run ramboot\0" \
	"emmcargs=setenv bootargs " ADI_BOOTARGS_EMMC "\0" \
	"emmcload=ext4load mmc 0:1 ${dtbaddr} /boot/sc598-som-ezkit.dtb; ext4load mmc 0:1 ${loadaddr} /boot/Image;\0" \
	"emmc_boot=run emmcargs; booti ${loadaddr} - ${dtbaddr}\0" \
	"emmc_boot_sc598=run emmcload; run emmc_boot\0" \
	"emmcboot=run emmc_boot_sc598\0" \
	"emmc_setup_falcon=run emmcargs; mmc read ${loadaddr} 0x800 0x100; fdt addr ${loadaddr}; fdt resize 0x10000; fdt boardsetup; fdt chosen; mmc erase 0x800 0x400; mmc write ${loadaddr} 0x800 0x100\0" \

#define ADI_BOOTARGS_EMMC \
        "root=/dev/mmcblk0p1 " \
        "rootfstype=ext4 rootwait " \
        "clkin_hz=" __stringify(CONFIG_CLKIN_HZ) " " \
        ADI_BOOTARGS_VIDEO \
        ADI_EARLYPRINTK \
        "console=ttySC" __stringify(CONFIG_UART_CONSOLE) "," \
                        __stringify(CONFIG_BAUDRATE) " "\
        "mem=" CONFIG_LINUX_MEMSIZE

#define ADI_BOOTARGS_QSPI \
        "root=/dev/mtdblock4 " \
        "rootfstype=jffs2 " \
        "clkin_hz=" __stringify(CONFIG_CLKIN_HZ) " " \
        ADI_BOOTARGS_VIDEO \
        ADI_EARLYPRINTK \
        "console=ttySC" __stringify(CONFIG_UART_CONSOLE) "," \
                        __stringify(CONFIG_BAUDRATE) " "\
        "mem=" CONFIG_LINUX_MEMSIZE

#define ADI_BOOTARGS_OSPI \
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
