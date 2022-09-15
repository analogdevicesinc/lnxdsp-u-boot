/*
 * U-boot - Configuration file for sc589 EZ-Kit board
 */

#ifndef __CONFIG_SC598_EZKIT_H
#define __CONFIG_SC598_EZKIT_H

#include <asm/mach-adi/common/config.h>
#include <linux/sizes.h>
#include <linux/kconfig.h>

/* Do we have a carrier board attached */
#define ADI_HAVE_CARRIER 1

#define CONFIG_SYS_BOOTM_LEN 0x1800000

/* GIC */
#define CONFIG_GICV3
#define GICD_BASE 0x31200000
#define GICR_BASE 0x31240000

/* ARM Timer */
#define COUNTER_FREQUENCY		31250000 //500/16=31.25 MHz

/*
 * Processor Settings
 */
#define CONFIG_LOADADDR	0x96000000
#define CONFIG_DTBLOADADDR	"0x99000000"
#define CONFIG_MACH_TYPE	MACH_TYPE_SC594_SOM_EZKIT

//L2 SRAM:
//TEXT:   0x20080000 to 0x2009FFFF (128 KB)
//BSS:    0x200A0000 to 0x200AFFFF (64  KB)
//MALLOC: 0x200B0000 to 0x200BFFFF (64  KB)
//STACK:  0x200C0000 to 0x200CFFFF (64  KB)
#define CONFIG_SPL_MAX_SIZE		SZ_128K
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
#define CONFIG_SYS_SPI_KERNEL_OFFS 0x160000   // Where the kernel Image should be stored
#define CONFIG_SYS_SPL_ARGS_ADDR   0x99000000 // Where to load the DTB into RAM
#define CONFIG_SYS_SPI_KERNEL_SKIP_HEADER
#define CONFIG_SYS_MMCSD_RAW_MODE_KERNEL_SECTOR 4096 // Position of kernel Image in sectors
#define CONFIG_SYS_MMCSD_RAW_MODE_ARGS_SECTOR   2048 // Position of DTB in sectors
#define CONFIG_SYS_MMCSD_RAW_MODE_ARGS_SECTORS  256  // Size of DTB in sectors

#define CONFIG_SYS_LOAD_ADDR 0x96000000 //Address used to load the FIT image into during Falcon+FIT
#endif

#define CONFIG_SYS_TIMERBASE	TIMER0_CONFIG

/*
 * Memory Settings
 */
#define MEM_IS43TR16512BL
#define MEM_ISSI_4Gb_DDR3_800MHZ
#define MEM_DMC0

#define	CONFIG_NR_DRAM_BANKS		1
#define CONFIG_SYS_SDRAM_BASE	0x90000000
#define CONFIG_SYS_SDRAM_SIZE	0x0e000000
#ifndef CONFIG_SPL_BUILD
#define CONFIG_SYS_LOAD_ADDR	0x0
#endif
#define CONFIG_SYS_INIT_SP_ADDR CONFIG_SYS_TEXT_BASE

#define CONFIG_SYS_MONITOR_LEN	0
#define CONFIG_SYS_MALLOC_LEN	(1024*1024*1)

/*
 * Network Settings
 */
#define CONFIG_DTBNAME		"sc598-som-ezkit.dtb"
#define CONFIG_HOSTNAME		"sc59x"
#define CONFIG_DW_PORTS		1
#define CONFIG_PHY_TI

/*
 * USB Settings
 */
#define MUSB_HW_VERSION2
#define CONFIG_USB_STORAGE

/* OSPI - Via Device Tree Support
 *
 */
//Set this to 1 if you would like to use the maximum SPI speeds for OSPI and will not be using QSPI
#define ADI_USE_MACRONIX_OSPI 0
#define ADI_USE_MACRONIX_OSPI_DTR 0

//Allow U-Boot to dynamically pinmux between the three conflicting ezkit peripherals (OSPI/QSPI/UART)
//This allows U-boot to use all three peripherals
#define ADI_DYNAMIC_OSPI_QSPI_UART_MANAGEMENT

#define CONFIG_CQSPI_REF_CLK		500000000 // = 500 MHz

/*
 * SPI - Via Device Tree Support
 */
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

/*
 * Misc Settings
 */
#define CONFIG_UART_CONSOLE	0
#define ADI_SPI_FIT_OFFSET "0x160000"

#if ADI_USE_MACRONIX_OSPI
#define CONFIG_BOOTCOMMAND	"run ospiboot"
#else
#define CONFIG_BOOTCOMMAND	"run qspiboot"
#endif
#define INITRAMADDR "0x9c000000"

#define ADI_ENV_SETTINGS \
	"fdt_high=0xFFFFFFFFFFFFFFFF\0" \
	"initrd_high=0xFFFFFFFFFFFFFFFF\0" \
	"rfsfile=adsp-sc5xx-minimal-adsp-sc598-som-ezkit.jffs2\0" \
	"dtbsize=0x20000\0" \
	"imagesize=0xF00000\0" \
	"initramfs_file=initramfs.cpio.gz.uboot\0" \
	ADI_INIT_ETHERNET \
	ADI_RAM_BOOT \
	ADI_UPDATE_SPI \
	ADI_OSPI_BOOT \
	ADI_QSPI_BOOT \
	ADI_EMMC_BOOT

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
	#define ADI_SPI_BOOTCMD "sf read ${loadaddr} " ADI_SPI_FIT_OFFSET " ${imagesize}; bootm ${loadaddr};"
	#define ADI_EMMC_LOAD "ext4load mmc 0:1 ${loadaddr} /boot/fitImage;"
	#define ADI_EMMC_BOOTCMD "run emmcargs; bootm;"
#else
	#define ADI_UPDATE_SPI_DTB_CMD " run update_spi_dtb;"
	#define ADI_UPDATE_SPI_IMAGE_CMD " run update_spi_image;"
	#define ADI_UPDATE_SPI_FIT_CMD ""
	#define ADI_SPI_BOOTCMD "sf read ${loadaddr} 0x100000 ${imagesize}; sf read ${dtbaddr} 0xE0000 ${dtbsize}; booti ${loadaddr} - ${dtbaddr}"
	#define ADI_EMMC_LOAD "ext4load mmc 0:1 ${dtbaddr} /boot/sc598-som-ezkit.dtb; ext4load mmc 0:1 ${loadaddr} /boot/Image;"
	#define ADI_EMMC_BOOTCMD "run emmcargs; booti ${loadaddr} - ${dtbaddr};"
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

#if CONFIG_ADI_FALCON
	#if CONFIG_IS_ENABLED(FIT)
		#define ADI_UPDATE_SPI_DTB ""
	#else
		#define ADI_UPDATE_SPI_DTB \
		"update_spi_dtb=tftp ${loadaddr} ${dtbfile}; sf probe ${sfdev}; run ${argscmd}; fdt addr ${loadaddr}; fdt resize 0x10000; fdt boardsetup; fdt chosen; sf write ${loadaddr} 0xE0000 0x10000; setenv dtbsize 0x10000;\0"
	#endif
#else
	#if CONFIG_IS_ENABLED(FIT)
		#define ADI_UPDATE_SPI_DTB ""
	#else
		#define ADI_UPDATE_SPI_DTB \
		"update_spi_dtb=tftp ${loadaddr} ${dtbfile}; sf probe ${sfdev}; sf write ${loadaddr} 0xE0000 0x10000; setenv dtbsize 0x10000;\0"
	#endif
#endif

#define ADI_UPDATE_SPI \
	"update_spi_sc598=run init_ethernet; sf probe ${sfdev}; sf erase 0 ${sfsize};" \
	ADI_UPDATE_SPI_UBOOT_CMD \
	ADI_UPDATE_SPI_DTB_CMD \
	ADI_UPDATE_SPI_IMAGE_CMD \
	ADI_UPDATE_SPI_FIT_CMD \
	ADI_UPDATE_SPI_RFS_CMD \
	" sleep 3; saveenv\0" \
	ADI_UPDATE_SPI_UBOOT \
	ADI_UPDATE_SPI_DTB \
	"update_spi_image=tftp ${loadaddr} ${imagefile}; sf probe ${sfdev}; sf write ${loadaddr} 0x100000 ${filesize}; setenv imagesize ${filesize};\0" \
	"update_spi_fit=tftp ${loadaddr} ${imagefile}; sf probe ${sfdev}; sf write ${loadaddr} " ADI_SPI_FIT_OFFSET " ${filesize}; setenv imagesize ${filesize};\0" \
	"update_spi_rfs=tftp ${loadaddr} ${rfsfile}; sf probe ${sfdev}; sf write ${loadaddr} 0x1000000 ${filesize};\0"

#define ADI_OSPI_BOOT \
	"update_ospi_sc598=setenv sfdev 0:0; setenv sfsize 0x4000000; setenv bootcmd \'run ospiboot\'; setenv argscmd ospiargs; run update_spi_sc598;\0" \
	"ospiargs=setenv bootargs " ADI_BOOTARGS_SPI "\0" \
	"ospi_boot_sc598=run ospiargs; sf probe ${sfdev};" ADI_SPI_BOOTCMD "\0" \
	"ospiboot=run ospi_boot_sc598\0"

#define ADI_QSPI_BOOT \
	"update_qspi_sc598=setenv sfdev 2:1; setenv sfsize 0x4000000; setenv bootcmd \'run qspiboot\'; setenv argscmd qspiargs; run update_spi_sc598;\0" \
	"qspiargs=setenv bootargs " ADI_BOOTARGS_SPI "\0" \
	"qspi_boot_sc598=run qspiargs; sf probe ${sfdev};" ADI_SPI_BOOTCMD "\0" \
	"qspiboot=run qspi_boot_sc598\0"

#define ADI_EMMC_BOOT \
	"update_emmc_sc598=setenv bootcmd \'run emmcboot\'; saveenv; run ramboot\0" \
	"emmcargs=setenv bootargs " ADI_BOOTARGS_EMMC "\0" \
	"emmcload=" ADI_EMMC_LOAD "\0" \
	"emmc_boot=" ADI_EMMC_BOOTCMD "\0" \
	"emmc_boot_sc598=run emmcload; run emmc_boot\0" \
	"emmcboot=run emmc_boot_sc598\0" \
	"emmc_setup_falcon=run emmcargs; mmc read ${loadaddr} 0x800 0x100; fdt addr ${loadaddr}; fdt resize 0x10000; fdt boardsetup; fdt chosen; mmc erase 0x800 0x400; mmc write ${loadaddr} 0x800 0x100\0" \

#define ADI_BOOTARGS_EMMC \
        "root=/dev/mmcblk0p1 " \
        "rootfstype=ext4 rootwait " \
        ADI_EARLYPRINTK \
        "console=ttySC" __stringify(CONFIG_UART_CONSOLE) "," \
                        __stringify(CONFIG_BAUDRATE) " "

#define ADI_BOOTARGS_SPI \
        "root=/dev/mtdblock4 " \
        "rootfstype=jffs2 " \
        ADI_EARLYPRINTK \
        "console=ttySC" __stringify(CONFIG_UART_CONSOLE) "," \
                        __stringify(CONFIG_BAUDRATE) " "

#include <configs/sc_adi_common.h>

#endif