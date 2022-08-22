#include <asm/arch/portmux.h>
#if defined(CONFIG_SC59X_64)
#include <asm/arch/sc59x_64.h>
#elif defined(CONFIG_SC59X)
#include <asm/arch/sc59x.h>
#elif defined(CONFIG_SC58X)
#include <asm/arch/sc58x.h>
#elif defined(CONFIG_SC57X)
#include <asm/arch/sc57x.h>
#endif
#include <asm/io.h>
#include <config.h>
#include "init.h"

void adi_initcode(void){
	adi_initcode_shared();
}

