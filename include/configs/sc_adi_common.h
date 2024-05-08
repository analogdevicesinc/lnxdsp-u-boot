/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * (C) Copyright 2022 - Analog Devices, Inc.
 *
 * Written and/or maintained by Timesys Corporation
 *
 * Contact: Nathan Barrett-Morrison <nathan.morrison@timesys.com>
 * Contact: Greg Malysa <greg.malysa@timesys.com>
 *
 */

#ifndef __CONFIG_SC_ADI_COMMON_H
#define __CONFIG_SC_ADI_COMMON_H

/*
 * Env Settings
 */
#ifdef CONFIG_SC59X_64
	#define ADI_EARLYPRINTK "earlycon=adi_uart,0x31003000 "
	#if !CONFIG_IS_ENABLED(FIT)
		#define IMAGEFILE "Image"
		#define ADI_BOOT_INITRD "booti ${loadaddr} ${initramaddr} ${dtbaddr};"
		#define ADI_BOOT "booti ${loadaddr} - ${dtbaddr};"
	#else
		#define IMAGEFILE "fitImage"
		#define ADI_BOOT_INITRD "bootm ${loadaddr};"
		#define ADI_BOOT "bootm ${loadaddr};"
	#endif
#else
	#define ADI_EARLYPRINTK "earlyprintk=serial,uart0," __stringify(CONFIG_BAUDRATE) " "
	#if !CONFIG_IS_ENABLED(FIT)
		#define IMAGEFILE "zImage"
		#define ADI_BOOT_INITRD "bootz ${loadaddr} ${initramaddr} ${dtbaddr};"
		#define ADI_BOOT "bootz ${loadaddr} - ${dtbaddr};"
	#else
		#define IMAGEFILE "fitImage"
		#define ADI_BOOT_INITRD "bootm ${loadaddr};"
		#define ADI_BOOT "bootm ${loadaddr};"
	#endif
#endif

#define ADI_UPDATE_SPI_UBOOT_CMD " run update_spi_uboot;"
#define ADI_UPDATE_SPI_RFS_CMD " run update_spi_rfs;"

#if CONFIG_IS_ENABLED(FIT)
	#define ADI_UPDATE_SPI_DTB_CMD ""
	#define ADI_UPDATE_SPI_IMAGE_CMD ""
	#define ADI_UPDATE_SPI_FIT_CMD " run update_spi_fit;"
	#define ADI_SPI_BOOTCMD "sf read ${loadaddr} " ADI_IMG_OFFSET " ${imagesize}; bootm ${loadaddr};"
	#define ADI_MMC_LOAD "ext4load mmc 0:1 ${loadaddr} /boot/fitImage;"
	#define ADI_TFTP_DTB ""
	#define ADI_TFTP_INITRD	""
#else
	#define ADI_UPDATE_SPI_DTB_CMD " run update_spi_dtb;"
	#define ADI_UPDATE_SPI_IMAGE_CMD " run update_spi_image;"
	#define ADI_UPDATE_SPI_FIT_CMD ""
	#define ADI_SPI_BOOTCMD "sf read ${loadaddr} " ADI_IMG_OFFSET " ${imagesize}; sf read ${dtbaddr} ${dtbloadaddr} ${dtbsize}; booti ${loadaddr} - ${dtbaddr}"
	#define ADI_MMC_LOAD "ext4load mmc 0:1 ${dtbaddr} /boot/" CONFIG_DTBNAME "; ext4load mmc 0:1 ${loadaddr} /boot/Image;"
	#define ADI_TFTP_DTB "tftp ${dtbaddr} ${tftp_dir_prefix}${dtbfile}; "
	#define ADI_TFTP_INITRD	"tftp ${initramaddr} ${tftp_dir_prefix}${initramfile}; "
#endif

#define ADI_MMC_BOOTCMD "run mmcargs; " ADI_BOOT ""

//only applicable for SC594
#if defined CONFIG_SC59X && !CONFIG_SC59X_64
#define ADI_MEM_SIZE	"mem=512M "
#else
#define ADI_MEM_SIZE	""
#endif

#define ADI_BOOTARGS_CONSOLE \
		ADI_EARLYPRINTK \
		"console=ttySC" __stringify(CONFIG_UART_CONSOLE) "," \
				__stringify(CONFIG_BAUDRATE) " " \
				ADI_MEM_SIZE \ 
		"vmalloc=512M "

#define ADI_BOOTARGS_MMC \
		"root=/dev/mmcblk0p1 rw " \
		"rootfstype=ext4 rootwait " \
		ADI_BOOTARGS_CONSOLE

#define ADI_BOOTARGS_SPI \
		"root=/dev/mtdblock4 rw " \
		"rootfstype=jffs2 " \
		ADI_BOOTARGS_CONSOLE

#define ADI_BOOTARGS_NFS	\
		"root=/dev/nfs rw " \
		"nfsroot=${serverip}:/romfs,tcp,nfsvers=3 " \
		ADI_BOOTARGS_CONSOLE

#define CONFIG_BOOTARGS	\
		ADI_BOOTARGS_CONSOLE

#define UBOOT_ENV_FILE "u-boot-" CONFIG_SYS_BOARD ".ldr"

#define ADI_INIT_ETHERNET \
	"init_ethernet=mii info; dhcp; setenv serverip ${tftpserverip};\0"

#define ADI_NFS_BOOT \
	"nfsargs=setenv bootargs " ADI_BOOTARGS_NFS "\0" \
	"nfsboot=" \
		"run init_ethernet; " \
		"tftp ${loadaddr} ${tftp_dir_prefix}${imagefile};" \
		ADI_TFTP_DTB \
		"run nfsargs;" \
		"run addip;" \
		ADI_BOOT \
		"\0"

#define ADI_RAM_BOOT \
	"ramboot=" \
	"run init_ethernet; " \
	"tftp ${loadaddr} ${tftp_dir_prefix}${imagefile}; " \
	ADI_TFTP_DTB \
	ADI_TFTP_INITRD \
	"run ramargs; " \
	ADI_BOOT_INITRD \
	"\0"

#define STAGE_1_FILE "stage1-boot.ldr"
#define STAGE_2_FILE "stage2-boot.ldr"
#define ADI_UPDATE_SPI_UBOOT \
	"stage1file=" STAGE_1_FILE "\0" \
	"stage2file=" STAGE_2_FILE "\0" \
	"update_spi_uboot_stage1=tftp ${loadaddr} ${tftp_dir_prefix}${stage1file}; sf probe ${sfdev}; sf update ${loadaddr} 0x0 ${filesize};\0" \
	"update_spi_uboot_stage2=tftp ${loadaddr} ${tftp_dir_prefix}${stage2file}; sf probe ${sfdev}; sf update ${loadaddr} " ADI_UBOOT_OFFSET " ${filesize};\0" \
	"update_spi_uboot=run update_spi_uboot_stage1; run update_spi_uboot_stage2;\0"

#if CONFIG_ADI_FALCON
	#if CONFIG_IS_ENABLED(FIT)
		#define ADI_UPDATE_SPI_DTB ""
	#else
		#define ADI_UPDATE_SPI_DTB \
		"update_spi_dtb=tftp ${loadaddr} ${tftp_dir_prefix}${dtbfile}; sf probe ${sfdev}; run ${argscmd}; fdt addr ${loadaddr}; fdt resize 0x10000; fdt boardsetup; fdt chosen; sf update ${loadaddr} 0xE0000 0x10000; setenv dtbsize 0x10000;\0"
	#endif
#else
	#if CONFIG_IS_ENABLED(FIT)
		#define ADI_UPDATE_SPI_DTB ""
	#else
		#define ADI_UPDATE_SPI_DTB \
		"update_spi_dtb=tftp ${loadaddr} ${tftp_dir_prefix}${dtbfile}; sf probe ${sfdev}; setexpr dtbloadaddr ${imagesize} + " ADI_IMG_OFFSET "; sf update ${loadaddr} ${dtbloadaddr} ${filesize}; setenv dtbsize ${filesize};\0"
	#endif
#endif

#define ADI_ERASE_SPI \
	"erase_spi=setenv sfdev "  \
		__stringify(CONFIG_SC_BOOT_SPI_BUS) ":" __stringify(CONFIG_SC_BOOT_SPI_SSEL) "; " \
		"setenv sfsize " ADI_SPI_SIZE "; sf probe ${sfdev}; sf erase 0 ${sfsize};\0"

#define ADI_UPDATE_SPI \
	"start_update_spi=run init_ethernet; sf probe ${sfdev};" \
	ADI_UPDATE_SPI_UBOOT_CMD \
	ADI_UPDATE_SPI_IMAGE_CMD \
	ADI_UPDATE_SPI_FIT_CMD \
	ADI_UPDATE_SPI_DTB_CMD \
	ADI_UPDATE_SPI_RFS_CMD \
	" sleep 3; saveenv\0" \
	ADI_UPDATE_SPI_UBOOT \
	ADI_UPDATE_SPI_DTB \
	"update_spi_image=tftp ${loadaddr} ${tftp_dir_prefix}${imagefile}; sf probe ${sfdev}; sf update ${loadaddr} " ADI_IMG_OFFSET " ${filesize}; setenv imagesize ${filesize};\0" \
	"update_spi_fit=tftp ${loadaddr} ${tftp_dir_prefix}${imagefile}; sf probe ${sfdev}; sf update ${loadaddr} " ADI_IMG_OFFSET " ${filesize}; setenv imagesize ${filesize};\0" \
	"update_spi_rfs=tftp ${loadaddr} ${tftp_dir_prefix}${jffs2file}; sf probe ${sfdev}; sf update ${loadaddr} " ADI_RFS_OFFSET " ${filesize};\0"

#define ADI_UPDATE_SPI_UBOOT_ONLY \
	"start_update_spi_uboot_only=run init_ethernet; sf probe ${sfdev};" \
	ADI_UPDATE_SPI_UBOOT_CMD \
	" sleep 3; saveenv\0" \
	ADI_UPDATE_SPI_UBOOT \

#define ADI_ERASE_OSPI \
	"erase_ospi=setenv sfdev "  \
		__stringify(CONFIG_SC_BOOT_OSPI_BUS) ":" __stringify(CONFIG_SC_BOOT_OSPI_SSEL) "; " \
		"setenv sfsize " ADI_OSPI_SIZE "; sf probe ${sfdev}; sf erase 0 ${sfsize};\0"

#define ADI_OSPI_BOOT \
	"update_ospi=setenv sfdev " \
		__stringify(CONFIG_SC_BOOT_OSPI_BUS) ":" __stringify(CONFIG_SC_BOOT_OSPI_SSEL) "; " \
		"setenv sfsize " ADI_OSPI_SIZE "; setenv bootcmd \'run ospiboot\'; setenv argscmd ospiargs; run start_update_spi;\0" \
	"update_ospi_uboot_only=setenv sfdev " \
		__stringify(CONFIG_SC_BOOT_OSPI_BUS) ":" __stringify(CONFIG_SC_BOOT_OSPI_SSEL) "; run start_update_spi_uboot_only;\0" \
	"ospiargs=setenv bootargs " ADI_BOOTARGS_SPI "\0" \
	"ospi_boot=run ospiargs; sf probe ${sfdev};" ADI_SPI_BOOTCMD "\0" \
	"ospiboot=run ospi_boot\0"

#define ADI_SPI_BOOT \
	"update_spi=setenv sfdev " \
		__stringify(CONFIG_SC_BOOT_SPI_BUS) ":" __stringify(CONFIG_SC_BOOT_SPI_SSEL) "; " \
		"setenv sfsize " ADI_SPI_SIZE "; setenv bootcmd \'run spiboot\'; setenv argscmd spiargs; run start_update_spi;\0" \
	"update_spi_uboot_only=setenv sfdev " \
		__stringify(CONFIG_SC_BOOT_SPI_BUS) ":" __stringify(CONFIG_SC_BOOT_SPI_SSEL) "; run start_update_spi_uboot_only;\0" \
	"spiargs=setenv bootargs " ADI_BOOTARGS_SPI "\0" \
	"spi_boot=run spiargs; sf probe ${sfdev};" ADI_SPI_BOOTCMD "\0" \
	"spiboot=run spi_boot\0"

#define ADI_MMC_BOOT \
	"update_mmc=setenv bootcmd \'run mmcboot\'; saveenv; setenv imagefile fitImage-emmc-tools; run ramboot\0" \
	"mmcargs=setenv bootargs " ADI_BOOTARGS_MMC "\0" \
	"mmcload=" ADI_MMC_LOAD "\0" \
	"mmc_boot=" ADI_MMC_BOOTCMD "\0" \
	"mmcboot=mmc rescan; run mmcload; run mmc_boot\0" \
	"mmc_setup_falcon=run mmcargs; mmc read ${loadaddr} 0x800 0x100; fdt addr ${loadaddr}; fdt resize 0x10000; fdt boardsetup; fdt chosen; mmc erase 0x800 0x400; mmc write ${loadaddr} 0x800 0x100\0" \

#define ETH0ADDR "02:80:ad:20:31:e8"
#define ETH1ADDR "02:80:ad:20:31:e9"

#define CFG_EXTRA_ENV_SETTINGS \
	"autoload=no\0" \
	"ubootfile=" UBOOT_ENV_FILE "\0" \
	"addip=setenv bootargs ${bootargs} " \
		"ip=${ipaddr}:${serverip}:${gatewayip}:${netmask}:" \
		   "${hostname}:eth0:off" \
		"\0" \
	\
	"imagefile=" IMAGEFILE "\0" \
	"initramfile=adsp-sc5xx-ramdisk-adsp-" CONFIG_SYS_BOARD ".cpio.xz.u-boot\0" \
	"jffs2file=adsp-sc5xx-" ADI_JFFS2_FILE "-adsp-" CONFIG_SYS_BOARD ".rootfs.jffs2\0" \
	"initramaddr=" INITRAMADDR "\0" \
	"dtbfile=" CONFIG_DTBNAME "\0" \
	"dtbaddr=" CONFIG_DTBLOADADDR "\0" \
	"loadaddr="	__stringify(CONFIG_LOADADDR) "\0" \
	"ramargs=setenv bootargs " CONFIG_BOOTARGS "\0" \
	"ethaddr=" ETH0ADDR "\0" \
	ADI_ENV_SETTINGS

/*
 * Boot Parameter Settings
 */
#define CONFIG_CMDLINE_TAG              1       /* enable passing of ATAGs */
#define CONFIG_SETUP_MEMORY_TAGS        1

/*
 * Network Settings
 */
#ifdef CONFIG_CMD_NET
	#define CONFIG_NETMASK         255.255.255.0
	#ifndef CONFIG_IPADDR
		#define CONFIG_IPADDR         192.168.0.15
		#define CONFIG_GATEWAYIP      192.168.0.1
		#define CONFIG_SERVERIP       192.168.0.2
	#endif
	#ifndef CONFIG_ROOTPATH
		#define CONFIG_ROOTPATH       "/romfs"
	#endif
	#define CONFIG_SYS_AUTOLOAD "no"
	#define CONFIG_NET_RETRY_COUNT 20
#endif

/*
 * Misc Settings
 */
#define CONFIG_SYS_CBSIZE		512	/* Console I/O Buffer Size */

#endif
