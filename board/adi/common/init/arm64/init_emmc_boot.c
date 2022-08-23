#include <asm/arch/portmux.h>
#include <adi/common/sc5xx.h>
#include <asm/io.h>
#include <config.h>
#include "init.h"

#define pREG_EMSI0_SWRST ((uint8_t  *)  0x310C702F)
#define pREG_PADS0_PCFG0 ((uint32_t *)  0x31004604)
#define pREG_PADS0_PCFG1 ((uint32_t *)  0x31004608)

#define ENUM_ROM_BCMD_EMSI_SDRMODE_SDR		0x00004000
#define BITM_ROM_BCMD_EMSI_HISPEEDENABLE	0x00080000u
#define ENUM_ROM_BCMD_EMSI_BUSWID_8BIT		0x00002000
#define BITP_ROM_BCMD_SPIM_DEVICE			0
#define FUNC_ROM_BOOT						0x000000E4

void emmc_boot_trampoline(){
	void * (*adi_rom_Boot)( uint32_t pAddress,
							uint32_t flags,
							int32_t blockCount,
							uint32_t * pHook,
							uint32_t command);
	adi_rom_Boot = FUNC_ROM_BOOT;

	//EMSI -- Reset Controller
	*pREG_EMSI0_SWRST = 0x1;

	//EMSI -- Tuning Support
	*pREG_PADS0_PCFG0 |=(1<<21);
	*pREG_PADS0_PCFG1 |=(4<<28);

	//SDR 8 bit @ 50 MHz
	int BootCmd1 = (0x9<<BITP_ROM_BCMD_SPIM_DEVICE) | ENUM_ROM_BCMD_EMSI_BUSWID_8BIT | BITM_ROM_BCMD_EMSI_HISPEEDENABLE | ENUM_ROM_BCMD_EMSI_SDRMODE_SDR;

	//Boot from LBA-512 on the eMMC
	adi_rom_Boot(512+1, 0, 0, 0, BootCmd1);
}

void adi_initcode(void){
	adi_initcode_shared();
	emmc_boot_trampoline();
}