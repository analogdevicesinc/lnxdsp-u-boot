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
# define CONFIG_LOADADDR	0xA4000000
# define CONFIG_RSA		/* RSA for FIT authen. */
#else
# define CONFIG_LOADADDR	0xA2000000
#endif
#define CONFIG_DTBLOADADDR	"0xA4000000"
#define CONFIG_MACH_TYPE	MACH_TYPE_SC594_SOM_EZKIT
#define CONFIG_SYS_ARM_CACHE_WRITETHROUGH

#define CONFIG_SYS_BOOTM_LEN 0x1800000

//L2 SRAM:
//TEXT:   0x20080000 to 0x2009FFFF (128 KB)
//BSS:    0x200A0000 to 0x200AFFFF (64  KB)
//MALLOC: 0x200B0000 to 0x200BFFFF (64  KB)
//STACK:  0x200C0000 to 0x200CFFFF (64  KB)
#define CONFIG_SPL_MAX_SIZE		SZ_128K //(overkill)
#ifdef CONFIG_SPL_BUILD
#define CONFIG_SKIP_LOWLEVEL_INIT
#define CONFIG_SPL_BSS_START_ADDR	0x200A0000
#define CONFIG_SPL_BSS_MAX_SIZE		SZ_64K
#define CONFIG_SYS_SPL_MALLOC_START	(CONFIG_SPL_BSS_START_ADDR + CONFIG_SPL_BSS_MAX_SIZE)
#define CONFIG_SYS_SPL_MALLOC_SIZE	SZ_64K
#define CONFIG_SPL_STACK		(CONFIG_SYS_SPL_MALLOC_START + CONFIG_SYS_SPL_MALLOC_SIZE + CONFIG_SPL_STACK_SIZE)
#define CONFIG_SPL_STACK_SIZE		SZ_64K

//Parameters used for Falcon boot
#define CONFIG_SYS_SPI_ARGS_OFFS   0xA0000    // This is where the DTB should be stored
#define CONFIG_SYS_SPI_ARGS_SIZE   0x10000    // Max size of the DTB
#define CONFIG_SYS_SPI_KERNEL_OFFS 0xC0000   // Where the kernel Image should be stored
#define CONFIG_SYS_SPL_ARGS_ADDR   0xA4000000 // Where to load the DTB into RAM
#define CONFIG_SYS_LOAD_ADDR       0xA2000000 // Where to load the Image into RAM
#define CONFIG_SYS_SPI_KERNEL_SKIP_HEADER
#endif

#define CONFIG_SYS_TIMERBASE	TIMER0_CONFIG

/*
 * Memory Settings
 */
#define MEM_IS43TR16512BL
#define MEM_ISSI_8Gb_DDR3_800MHZ
#define MEM_DMC0

#define	CONFIG_NR_DRAM_BANKS		1
#define CONFIG_SYS_SDRAM_BASE	0xA0000000
#define CONFIG_SYS_SDRAM_SIZE	0x20000000
#define CONFIG_SYS_TEXT_BASE	0xB2200000
#ifndef CONFIG_SPL_BUILD
#define CONFIG_SYS_LOAD_ADDR	0x0
#endif
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
#define CONFIG_SC5XX_SPI
#define CONFIG_CMD_SPI
#define CONFIG_ENV_SPI_MAX_HZ	10000000
#define CONFIG_SF_DEFAULT_SPEED	10000000
#define ADI_OSPI_SF_DEFAULT_SPEED CONFIG_CQSPI_REF_CLK / 6 //83.33 MHz STR (Single Transfer Rate)
#define CONFIG_SF_DEFAULT_MODE  SPI_MODE_3

/*
 * Misc Settings
 */
#define CONFIG_SYS_NO_FLASH
#define CONFIG_UART_CONSOLE	0
#define CONFIG_BAUDRATE		115200
#define CONFIG_LINUX_MEMSIZE	"512M"
#define CONFIG_CMD_BOOTZ

#define CONFIG_BOOTCOMMAND	"run ospiboot"
#define INITRAMADDR "0xA8000000"

#define ADI_ENV_SETTINGS \
	"initrd_high=0xFFFFFFFF\0" \
	"fdt_high=0xFFFFFFFF\0" \
	"rfsfile=adsp-sc5xx-minimal-adsp-sc594-som-ezkit.jffs2\0" \
	"dtbsize=0x20000\0" \
	"imagesize=0x600000\0" \
	ADI_INIT_ETHERNET \
	ADI_RAM_BOOT \
	ADI_UPDATE_SPI \
	ADI_OSPI_BOOT \
	ADI_QSPI_BOOT

#define ADI_INIT_ETHERNET \
	"init_ethernet=mii info; dhcp; setenv serverip ${tftpserverip};\0"

#define ADI_RAM_BOOT \
	"ramboot=run init_ethernet; tftp ${loadaddr} ${ramfile}; tftp ${initramaddr} ${initramfile}; run ramargs; bootm ${loadaddr} ${initramaddr}\0"

#define ADI_UPDATE_SPI_UBOOT_CMD " run update_spi_uboot;"
#define ADI_UPDATE_SPI_RFS_CMD " run update_spi_rfs;"

#if CONFIG_IS_ENABLED(FIT)
	#define ADI_UPDATE_SPI_DTB_CMD ""
	#define ADI_UPDATE_SPI_IMAGE_CMD ""
	#define ADI_UPDATE_SPI_FIT_CMD " run update_spi_fit;"
	#define ADI_SPI_BOOTCMD "sf read ${loadaddr} 0xC0000 ${imagesize}; bootm ${loadaddr};"
#else
	#define ADI_UPDATE_SPI_DTB_CMD " run update_spi_dtb;"
	#define ADI_UPDATE_SPI_IMAGE_CMD " run update_spi_image;"
	#define ADI_UPDATE_SPI_FIT_CMD ""
	#define ADI_SPI_BOOTCMD "sf read ${loadaddr} 0xC0000 ${imagesize}; sf read ${dtbaddr} 0xA0000 ${dtbsize}; bootz ${loadaddr} - ${dtbaddr}"
#endif

#ifndef CONFIG_SPL_OS_BOOT
	#define ADI_UPDATE_SPI_UBOOT \
		"update_spi_uboot=tftp ${loadaddr} ${ubootfile}; sf probe ${sfdev}; sf write ${loadaddr} 0x0 ${filesize};\0"
#else
	#define STAGE_1_FILE "stage1-boot.ldr"
	#define STAGE_2_FILE "stage2-boot.ldr"
	#define ADI_UPDATE_SPI_UBOOT \
		"stage1file=" STAGE_1_FILE "\0" \
		"stage2file=" STAGE_2_FILE "\0" \
		"update_spi_uboot_stage1=tftp ${loadaddr} ${stage1file}; sf probe ${sfdev}; sf write ${loadaddr} 0x0 ${filesize};\0" \
		"update_spi_uboot_stage2=tftp ${loadaddr} ${stage2file}; sf probe ${sfdev}; sf write ${loadaddr} 0x20000 ${filesize};\0" \
		"update_spi_uboot=run update_spi_uboot_stage1; run update_spi_uboot_stage2;\0"
#endif

#ifndef CONFIG_ADI_FALCON
	#define ADI_UPDATE_OSPI_DTB \
	"update_ospi_dtb=tftp ${loadaddr} ${dtbfile}; sf probe 0:0; sf write ${loadaddr} 0xA0000 ${filesize}; setenv dtbsize ${filesize};\0"
#else
	#define ADI_UPDATE_OSPI_DTB \
	"update_ospi_dtb=tftp ${loadaddr} ${dtbfile}; sf probe 0:0; run ospiargs; fdt addr ${loadaddr};  fdt resize 0x10000; fdt boardsetup; fdt chosen; sf write ${loadaddr} 0xA0000 0x10000; setenv dtbsize 0x10000;\0"
#endif

#ifndef CONFIG_SPL_OS_BOOT
	#define ADI_UPDATE_SPI_UBOOT \
		"update_spi_uboot=tftp ${loadaddr} ${ubootfile}; sf probe ${sfdev}; sf write ${loadaddr} 0x0 ${filesize};\0"
#else
	#define STAGE_1_FILE "stage1-boot.ldr"
	#define STAGE_2_FILE "stage2-boot.img"
	#define ADI_UPDATE_SPI_UBOOT \
		"stage1file=" STAGE_1_FILE "\0" \
		"stage2file=" STAGE_2_FILE "\0" \
		"update_spi_uboot_stage1=tftp ${loadaddr} ${stage1file}; sf probe ${sfdev}; sf write ${loadaddr} 0x0 ${filesize};\0" \
		"update_spi_uboot_stage2=tftp ${loadaddr} ${stage2file}; sf probe ${sfdev}; sf write ${loadaddr} 0x20000 ${filesize};\0" \
		"update_spi_uboot=run update_spi_uboot_stage1; run update_spi_uboot_stage2;\0"
#endif

#if CONFIG_ADI_FALCON
	#if CONFIG_IS_ENABLED(FIT)
		#define ADI_UPDATE_SPI_DTB ""
	#else
		#define ADI_UPDATE_SPI_DTB \
		"update_spi_dtb=tftp ${loadaddr} ${dtbfile}; sf probe ${sfdev}; run ${argscmd}; fdt addr ${loadaddr}; fdt resize 0x10000; fdt boardsetup; fdt chosen; sf write ${loadaddr} 0xA0000 0x10000; setenv dtbsize 0x10000;\0"
	#endif
#else
	#if CONFIG_IS_ENABLED(FIT)
		#define ADI_UPDATE_SPI_DTB ""
	#else
		#define ADI_UPDATE_SPI_DTB \
		"update_spi_dtb=tftp ${loadaddr} ${dtbfile}; sf probe ${sfdev}; sf write ${loadaddr} 0xA0000 0x10000; setenv dtbsize 0x10000;\0"
	#endif
#endif

#define ADI_OSPI_BOOT \
	"update_ospi_sc594=setenv sfdev 0:0; setenv sfsize 0x2000000; setenv bootcmd \'run ospiboot\'; setenv argscmd ospiargs; run update_spi_sc594;\0" \
	"ospiargs=setenv bootargs " ADI_BOOTARGS_SPI "\0" \
	"ospi_boot_sc594=run ospiargs; sf probe ${sfdev};" ADI_SPI_BOOTCMD "\0" \
	"ospiboot=run ospi_boot_sc594\0"

#define ADI_QSPI_BOOT \
	"update_qspi_sc594=setenv sfdev 2:1; setenv sfsize 0x4000000; setenv bootcmd \'run qspiboot\'; setenv argscmd qspiargs; run update_spi_sc594;\0" \
	"qspiargs=setenv bootargs " ADI_BOOTARGS_SPI "\0" \
	"qspi_boot_sc594=run qspiargs; sf probe ${sfdev};" ADI_SPI_BOOTCMD "\0" \
	"qspiboot=run qspi_boot_sc594\0"

#define ADI_BOOTARGS_SPI \
        "root=/dev/mtdblock4 " \
        "rootfstype=jffs2 " \
        "clkin_hz=" __stringify(CONFIG_CLKIN_HZ) " " \
        ADI_BOOTARGS_VIDEO \
        "earlyprintk=serial,uart" __stringify(CONFIG_UART_CONSOLE) "," \
                        __stringify(CONFIG_BAUDRATE) " " \
        "console=ttySC" __stringify(CONFIG_UART_CONSOLE) "," \
                         __stringify(CONFIG_BAUDRATE) " "\
        "mem=" CONFIG_LINUX_MEMSIZE " " \
        "vmalloc=512M"

#define ADI_UPDATE_SPI \
	"update_spi_sc594=run init_ethernet; sf probe ${sfdev}; sf erase 0 ${sfsize};" \
	ADI_UPDATE_SPI_UBOOT_CMD \
	ADI_UPDATE_SPI_DTB_CMD \
	ADI_UPDATE_SPI_IMAGE_CMD \
	ADI_UPDATE_SPI_FIT_CMD \
	ADI_UPDATE_SPI_RFS_CMD \
	" sleep 3; saveenv\0" \
	ADI_UPDATE_SPI_UBOOT \
	ADI_UPDATE_SPI_DTB \
	"update_spi_image=tftp ${loadaddr} ${imagefile}; sf probe ${sfdev}; sf write ${loadaddr} 0xC0000 ${filesize}; setenv imagesize ${filesize};\0" \
	"update_spi_fit=tftp ${loadaddr} ${imagefile}; sf probe ${sfdev}; sf write ${loadaddr} 0xC0000 ${filesize}; setenv imagesize ${filesize};\0" \
	"update_spi_rfs=tftp ${loadaddr} ${rfsfile}; sf probe ${sfdev}; sf write ${loadaddr} 0x6C0000 ${filesize};\0"


#include <configs/sc_adi_common.h>

#endif
