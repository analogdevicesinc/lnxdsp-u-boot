/*
 * U-boot - Common configuration file for ADI SC serial board
 */

#ifndef __CONFIG_SC_ADI_COMMON_H
#define __CONFIG_SC_ADI_COMMON_H

//#include <generated/autoconf.h>

/*
 * Command Settings
 */
#ifndef _CONFIG_CMD_DEFAULT_H

//# include <config_cmd_default.h>

# ifdef ADI_CMDS_NETWORK
#  define CONFIG_CMD_DHCP
//#  define CONFIG_BOOTP_SUBNETMASK
//#  define CONFIG_BOOTP_GATEWAY
//#  define CONFIG_BOOTP_DNS
#  define CONFIG_BOOTP_NTPSERVER
#  define CONFIG_KEEP_SERVERADDR
#  define CONFIG_CMD_DNS
#  define CONFIG_CMD_PING
//#  define CONFIG_CMD_NET
#  ifdef CONFIG_MII
#   define CONFIG_CMD_MII
#  endif
# else
#  undef CONFIG_CMD_BOOTD
#  undef CONFIG_CMD_NET
#  undef CONFIG_CMD_NFS
# endif
/*
# ifdef CONFIG_MMC
#  define CONFIG_CMD_MMC
#  define CONFIG_CMD_EXT2
#  define CONFIG_CMD_FAT
#  define CONFIG_DOS_PARTITION
# endif
# ifdef CONFIG_MUSB_HCD
#  define CONFIG_CMD_USB
#  define CONFIG_CMD_EXT2
#  define CONFIG_CMD_FAT
#  define CONFIG_DOS_PARTITION
# endif
*/
# if defined(CONFIG_CMD_FAT) && !defined(CONFIG_FS_FAT)
#  define CONFIG_FS_FAT
#  define CONFIG_FS_FAT_MAX_CLUSTSIZE 65536
# endif
# ifdef CONFIG_MMC_SPI
#  define CONFIG_CMD_MMC_SPI
# endif
/*
# ifdef CONFIG_SPI_FLASH
#  define CONFIG_CMD_SF
# endif
*/
# ifdef CONFIG_SYS_I2C
#  define CONFIG_CMD_I2C
# endif
# ifdef CONFIG_SYS_NO_FLASH
#  undef CONFIG_CMD_FLASH
#  undef CONFIG_CMD_IMLS
# endif
# define CONFIG_CMD_CACHE
//# define CONFIG_CMD_ELF
# define CONFIG_CMD_GPIO
# define CONFIG_CMD_REGINFO
# define CONFIG_CMD_STRINGS
# define CONFIG_CMD_MEMTEST
# if defined(CONFIG_SC59X_CHAIN_BOOT) || defined(CONFIG_SC58X_CHAIN_BOOT) || defined(CONFIG_SC57X_CHAIN_BOOT)
#  undef CONFIG_CMD_SAVEENV
# endif
#if defined(CONFIG_SC58X) || defined(CONFIG_SC59X)
//# define CONFIG_CMD_SC58X_OTP
#elif defined(CONFIG_SC57X)
//# define CONFIG_CMD_SC57X_OTP
#endif
#endif

/* Commands */

#define CONFIG_CMDLINE_EDITING	1
#define CONFIG_AUTO_COMPLETE	1
#define CONFIG_LOADS_ECHO	1

/*
 * Debug Settings
 */
#define CONFIG_ENV_OVERWRITE	1
//#define CONFIG_DEBUG_DUMP	1
//#define CONFIG_KALLSYMS		1
#define CONFIG_PANIC_HANG	1


/*
 * Env Settings
 */
#ifndef CONFIG_BOOTDELAY
# if (CONFIG_SC_BOOT_MODE == SC_BOOT_UART)
#  define CONFIG_BOOTDELAY	-1
# else
#  define CONFIG_BOOTDELAY	5
# endif
#endif

#ifdef CONFIG_SC59X_64
# define ADI_EARLYPRINTK "earlycon=adi_uart,0x31003000 "
# if !CONFIG_IS_ENABLED(FIT)
#  define IMAGEFILE "Image"
#  define IMAGEFILE_RAM "Image"
#  define ADI_BOOT_INITRD "booti ${loadaddr} ${initramaddr} ${dtbaddr};"
#  define ADI_BOOT "booti ${loadaddr} - ${dtbaddr};"
# else
#  define IMAGEFILE "fitImage"
#  define IMAGEFILE_RAM "fitImage"
#  define ADI_BOOT_INITRD "bootm ${loadaddr} ${initramaddr};"
#  define ADI_BOOT "bootm ${loadaddr};"
# endif
#else
# define ADI_EARLYPRINTK "earlyprintk=serial,uart0," __stringify(CONFIG_BAUDRATE) " "
# if !CONFIG_IS_ENABLED(FIT)
#  define IMAGEFILE "zImage"
#  define IMAGEFILE_RAM "zImage"
#  define ADI_BOOT_INITRD "bootz ${loadaddr} ${initramaddr} ${dtbaddr};"
#  define ADI_BOOT "bootz ${loadaddr} - ${dtbaddr};"
# else
#  define IMAGEFILE "fitImage"
#  define IMAGEFILE_RAM "fitImage"
#  define ADI_BOOT_INITRD "bootm ${loadaddr} ${initramaddr};"
#  define ADI_BOOT "bootm ${loadaddr};"
# endif
#endif

#if !CONFIG_IS_ENABLED(FIT)
	#define ADI_MMC_DTB "ext2load mmc 0:1 ${dtbaddr} /boot/${dtbfile};"
	#define ADI_TFTP_DTB "tftp ${dtbaddr} ${dtbfile};"
#else
	#define ADI_MMC_DTB ""
	#define ADI_TFTP_DTB ""
#endif

//only applicable for SC594
#if defined CONFIG_SC59X && !CONFIG_SC59X_64
#define ADI_MEM_SIZE	"mem=512M "
#else
#define ADI_MEM_SIZE	""
#endif

#ifdef CONFIG_VIDEO
# define ADI_BOOTARGS_VIDEO "console=tty0 " ADI_MEM_SIZE " "
#else
# define ADI_BOOTARGS_VIDEO ""
#endif

#define ADI_BOOTARGS_ROOT_NAND "/dev/mtdblock2 rw"
#define ADI_BOOTARGS_ROOT_SDCARD    "/dev/mmcblk0p1 rw"

#define ADI_BOOTARGS_SDCARD	\
	"root=" ADI_BOOTARGS_ROOT_SDCARD " " \
	ADI_BOOTARGS_VIDEO \
	ADI_EARLYPRINTK \
	"console=ttySC" __stringify(CONFIG_UART_CONSOLE) "," \
			__stringify(CONFIG_BAUDRATE) " "

#define ADI_BOOTARGS_NFS	\
	"root=/dev/nfs rw " \
	"nfsroot=${serverip}:${rootpath},tcp,nfsvers=3 " \
	ADI_BOOTARGS_VIDEO \
	ADI_EARLYPRINTK \
	"console=ttySC" __stringify(CONFIG_UART_CONSOLE) "," \
			__stringify(CONFIG_BAUDRATE) " "

#define CONFIG_BOOTARGS	\
	"root=" ADI_BOOTARGS_ROOT_NAND " " \
	"rootfstype=jffs2 " \
	ADI_BOOTARGS_VIDEO \
	ADI_EARLYPRINTK \
	"console=ttySC" __stringify(CONFIG_UART_CONSOLE) "," \
			__stringify(CONFIG_BAUDRATE) " "

#if defined(CONFIG_CMD_NET)
# define UBOOT_ENV_FILE "u-boot-" CONFIG_SYS_BOARD ".ldr"
# if (CONFIG_SC_BOOT_MODE == SC_BOOT_SPI_MASTER)
#  ifndef CONFIG_SPI_IMG_SIZE
//#   define CONFIG_SPI_IMG_SIZE 0x80000
#  endif
#  define UBOOT_ENV_UPDATE \
		"sf probe " __stringify(CONFIG_SC_BOOT_SPI_BUS) ":" \
		__stringify(CONFIG_SC_BOOT_SPI_SSEL) ";" \
		"sf erase 0 " __stringify(CONFIG_SPI_IMG_SIZE) ";" \
		"sf write ${loadaddr} 0 ${filesize}"
# else
#  define UBOOT_ENV_UPDATE
# endif
# ifdef CONFIG_NETCONSOLE
#  define NETCONSOLE_ENV \
	"nc=" \
		"setenv ncip ${serverip};" \
		"setenv stdin nc;" \
		"setenv stdout nc;" \
		"setenv stderr nc" \
		"\0"
# else
#  define NETCONSOLE_ENV
# endif
# define NETWORK_ENV_SETTINGS \
	NETCONSOLE_ENV \
	\
	"ubootfile=" UBOOT_ENV_FILE "\0" \
	"update=" \
		"tftp ${loadaddr} ${ubootfile};" \
		UBOOT_ENV_UPDATE \
		"\0" \
	"addip=setenv bootargs ${bootargs} " \
		"ip=${ipaddr}:${serverip}:${gatewayip}:${netmask}:" \
		   "${hostname}:eth0:off" \
		"\0" \
	\
	"imagefile=" IMAGEFILE "\0" \
	"ramfile=" IMAGEFILE_RAM "\0" \
	"initramfile=ramdisk.cpio.xz.u-boot\0" \
	"initramaddr=" INITRAMADDR "\0" \
	"dtbfile=" CONFIG_DTBNAME "\0" \
	"dtbaddr=" CONFIG_DTBLOADADDR "\0" \
	"sdcardargs=setenv bootargs " ADI_BOOTARGS_SDCARD "\0" \
	"ramargs=setenv bootargs " CONFIG_BOOTARGS "\0" \
	"nfsargs=setenv bootargs " ADI_BOOTARGS_NFS "\0" \
	"ramboot_emmc=" \
		"mmc rescan;" \
		"mmc dev 0 0;" \
		"ext2load mmc 0:1 ${loadaddr} /boot/${ramfile};" \
		ADI_MMC_DTB \
		"ext2load mmc 0:1 ${initramaddr} /boot/${initramfile};" \
		"run sdcardargs;" \
		"run addip;" \
		ADI_BOOT_INITRD \
		"\0" \
	\
	"ramboot=" \
		"tftp ${loadaddr} ${ramfile};" \
		ADI_TFTP_DTB \
		"tftp ${initramaddr} ${initramfile};" \
		"run ramargs;" \
		"run addip;" \
		ADI_BOOT_INITRD \
		"\0" \
	\
	"norboot=" \
		"tftp ${loadaddr} ${ramfile};" \
		ADI_TFTP_DTB \
		"run ramargs;" \
		"run addip;" \
		ADI_BOOT \
		"\0" \
	\
	"sdcardboot=" \
		"mmc rescan;" \
		"mmc dev 0 0;" \
		"ext2load mmc 0:1 ${loadaddr} /boot/${ramfile};" \
		ADI_MMC_DTB \
		"run sdcardargs;" \
		ADI_BOOT \
		"\0" \
	\
	"nfsfile=" IMAGEFILE "\0" \
	"nfsboot=" \
		"tftp ${loadaddr} ${nfsfile};" \
		ADI_TFTP_DTB \
		"run nfsargs;" \
		"run addip;" \
		ADI_BOOT \
		"\0"
#else
# define NETWORK_ENV_SETTINGS
#endif

#define CONFIG_ENV_OVERWRITE    1

#ifndef BOARD_ENV_SETTINGS
# define BOARD_ENV_SETTINGS
#endif

#if ! (defined(CONFIG_SC59X) | defined (CONFIG_SC59X_64))
	#define ADI_ENV_SETTINGS "\0"
#endif

#define ETH0ADDR "02:80:ad:20:31:e8"
#define ETH1ADDR "02:80:ad:20:31:e9"

#define CONFIG_EXTRA_ENV_SETTINGS \
	NETWORK_ENV_SETTINGS \
	BOARD_ENV_SETTINGS \
	"ethaddr=" ETH0ADDR "\0" \
	ADI_ENV_SETTINGS

/*
 * Boot Paramter Settings
 */
#define CONFIG_CMDLINE_TAG              1       /* enable passing of ATAGs */
//#define CONFIG_OF_LIBFDT                /* Device Tree support */
#define CONFIG_SETUP_MEMORY_TAGS        1
//#define CONFIG_SYS_GENERIC_BOARD

/*
 * Network Settings
 */
#ifdef CONFIG_CMD_NET
# define CONFIG_NETMASK         255.255.255.0
# ifndef CONFIG_IPADDR
#  define CONFIG_IPADDR         192.168.0.15
#  define CONFIG_GATEWAYIP      192.168.0.1
#  define CONFIG_SERVERIP       192.168.0.2
# endif
# ifndef CONFIG_ROOTPATH
#  define CONFIG_ROOTPATH       "/romfs"
# endif
# ifdef CONFIG_CMD_DHCP
#  ifndef CONFIG_SYS_AUTOLOAD
#   define CONFIG_SYS_AUTOLOAD "no"
#  endif
# endif
//# define CONFIG_IP_DEFRAG
# define CONFIG_NET_RETRY_COUNT 20
#endif


/*
 * I2C Settings
 */
#ifdef CONFIG_SYS_I2C
# ifndef CONFIG_SYS_I2C_SPEED
#  define CONFIG_SYS_I2C_SPEED 100000
# endif
# ifndef CONFIG_SYS_I2C_SLAVE
#  define CONFIG_SYS_I2C_SLAVE 0
# endif
#endif


/*
 * SPI Settings
 */
#ifdef CONFIG_SPI_FLASH_ALL
# define CONFIG_SPI_FLASH_ATMEL
# define CONFIG_SPI_FLASH_EON
# define CONFIG_SPI_FLASH_MACRONIX
# define CONFIG_SPI_FLASH_SPANSION
# define CONFIG_SPI_FLASH_SST
# define CONFIG_SPI_FLASH_STMICRO
# define CONFIG_SPI_FLASH_WINBOND
#endif
#ifndef CONFIG_SPI_MM_BASE
//# define CONFIG_SPI_MM_BASE      0x60000000
//# define CONFIG_SYS_FLASH_BASE   0x60000000
#endif
#ifndef CONFIG_SPI_MM_SIZE
//# define CONFIG_SPI_MM_SIZE      0x20000000
#endif


/*
 * Env Storage Settings
 */
//TODO: Delete this whole block
#if !(defined(CONFIG_SC59X) || defined(CONFIG_SC58X) || defined(CONFIG_SC59X_64))
	// #define CONFIG_ENV_OFFSET       0x10000
	// #define CONFIG_ENV_SIZE         0x2000
	// #define CONFIG_ENV_SECT_SIZE    0x10000
	/* We need envcrc to embed the env into LDRs */
	#ifdef CONFIG_ENV_IS_EMBEDDED_IN_LDR
	# define CONFIG_BUILD_ENVCRC
	#endif
	#define CONFIG_ENV_ADDR         0
#endif

/*
 * Misc Settings
 */

#define ADI_VCO_HZ (CONFIG_CLKIN_HZ * CONFIG_CGU0_VCO_MULT)
#define ADI_CCLK_HZ (ADI_VCO_HZ / CONFIG_CGU0_CCLK_DIV)
#define ADI_SCLK_HZ (ADI_VCO_HZ / CONFIG_CGU0_SCLK_DIV)

#define CONFIG_SYS_HZ			1000
//#define CONFIG_SYS_LONGHELP		/* undef to save memory */
//#define CONFIG_SYS_HUSH_PARSER		/* use "hush" command parser */
//#define CONFIG_SYS_PROMPT		"sc # "
#define CONFIG_SYS_CBSIZE		512	/* Console I/O Buffer Size */
#define CONFIG_SYS_PBSIZE		(CONFIG_SYS_CBSIZE + sizeof(CONFIG_SYS_PROMPT) + 16)
#define CONFIG_SYS_MAXARGS		16
#define CONFIG_BOARD_EARLY_INIT_F
//#define CONFIG_CMD_MEMORY
#define CONFIG_MISC_INIT_R
//#define CONFIG_ADI_GPIO2
#define CONFIG_SOFT_SWITCH
//#define CONFIG_ARCH_HEADER_IN_MACH
//#define CONFIG_DISPLAY_CPUINFO
//#define CONFIG_BUILD_LDR

#ifdef CONFIG_SC5XX_DWMMC
# ifndef CONFIG_SC5XX_BUS_WIDTH
//#  define CONFIG_SC5XX_BUS_WIDTH   4
# endif
#endif

#ifdef CONFIG_ADI_WATCHDOG
#ifndef CONFIG_SPL_BUILD
# define CONFIG_HW_WATCHDOG
#endif
#endif

#define CONFIG_RSA

#endif
