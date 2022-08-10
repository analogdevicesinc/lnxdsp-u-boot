/*
 * Copyright (C) 2013 Analog Devices Inc.
 * Licensed under the GPL-2 or later.
 */

#include <asm/arch/portmux.h>
#include <asm/arch/sc59x.h>
#include <asm/io.h>
#include <config.h>
#include "cguinit.h"
#include "dmcinit.h"

#define CGU_STAT_MASK (BITM_CGU_STAT_PLLEN | BITM_CGU_STAT_PLOCK | \
			BITM_CGU_STAT_CLKSALGN)
#define CGU_STAT_ALGN_LOCK (BITM_CGU_STAT_PLLEN | BITM_CGU_STAT_PLOCK)

static struct CGU_Settings Clock_Dividers0 = {
	CONFIG_CGU0_VCO_MULT,
	CONFIG_CGU0_DF_DIV,
	CONFIG_CGU0_CCLK_DIV,
	CONFIG_CGU0_SCLK_DIV,
	CONFIG_CGU0_SCLK0_DIV,
	CONFIG_CGU0_SCLK1_DIV,
	CONFIG_CGU0_DCLK_DIV,
	CONFIG_CGU0_OCLK_DIV,
	CONFIG_CGU0_DIV_S1SELEX
};

static struct CGU_Settings Clock_Dividers1 = {
	CONFIG_CGU1_VCO_MULT,
	CONFIG_CGU1_DF_DIV,
	CONFIG_CGU1_CCLK_DIV,
	CONFIG_CGU1_SCLK_DIV,
	CONFIG_CGU1_SCLK0_DIV,
	CONFIG_CGU1_SCLK1_DIV,
	CONFIG_CGU1_DCLK_DIV,
	CONFIG_CGU1_OCLK_DIV, 
	CONFIG_CGU1_DIV_S1SELEX
};

__attribute__((always_inline)) static inline void dmcdelay(uint32_t delay)
{
  /* There is no zero-overhead loop on ARM, so assume each iteration takes
   * 4 processor cycles (based on examination of -O3 and -Ofast output).
   */
  static uint32_t i, remainder;
  static float f;

  /* Convert DDR cycles to core clock cycles */
  f = (float)delay * 1.25;
  delay = (uint32_t)(f+0.5);

  /* Round up to multiple of 4 */
  remainder = delay % 4;
  if (remainder != 0u) {
    delay += (4u - remainder);
  }

  for(i=0; i<delay; i+=4) {
    asm("nop");
  }
}

__attribute__((always_inline)) static inline
void Program_Cgu(uint32_t uiCguNo, struct CGU_Settings *pCGU_Settings, bool useExtension)
{
	static uint32_t dNewCguDiv;
	static uint32_t cgu_offset;

	if(uiCguNo == 0){
		cgu_offset = 0;
	}else{
		cgu_offset = 0x1000;
	}

	dNewCguDiv =  OSEL(pCGU_Settings->div_OSEL);
	dNewCguDiv |= SYSSEL(pCGU_Settings->div_SYSSEL);
	dNewCguDiv |= CSEL(pCGU_Settings->div_CSEL);
	dNewCguDiv |= S0SEL(pCGU_Settings->div_S0SEL);
	dNewCguDiv |= S1SEL(pCGU_Settings->div_S1SEL);
	dNewCguDiv |= DSEL(pCGU_Settings->div_DSEL);

	if(useExtension){
		writel(dNewCguDiv,CGU0_DIV + cgu_offset);
	}else{
		writel(0x4024482, CGU0_DIV + cgu_offset);
	}

	dmcdelay(1000);

    //Put PLL in to Bypass Mode
	writel(BITM_CGU_PLLCTL_PLLEN | BITM_CGU_PLLCTL_PLLBPST, CGU0_PLLCTL + cgu_offset);
	while(!(readl(CGU0_STAT + cgu_offset) & BITM_CGU_STAT_PLLBP)) {};

	dmcdelay(1000);

	if(useExtension){
		writel((BITM_CGU_CTL_S1SELEXEN | MSEL(pCGU_Settings->ctl_MSEL) | DF(pCGU_Settings->ctl_DF)) &
			(~BITM_CGU_CTL_LOCK), CGU0_CTL + cgu_offset);

		while(!(readl(CGU0_CTL + cgu_offset) & BITM_CGU_CTL_S1SELEXEN)) {};

	}else{
		writel((MSEL(pCGU_Settings->ctl_MSEL) | DF(pCGU_Settings->ctl_DF)) &
			(~BITM_CGU_CTL_LOCK), CGU0_CTL + cgu_offset);
	}

	dmcdelay(1000);

    //Take PLL out of Bypass Mode
	writel(BITM_CGU_PLLCTL_PLLEN | BITM_CGU_PLLCTL_PLLBPCL, CGU0_PLLCTL + cgu_offset);
	while((readl(CGU0_STAT + cgu_offset) & BITM_CGU_STAT_PLLBP)) {};
	while((readl(CGU0_STAT + cgu_offset) & BITM_CGU_STAT_CLKSALGN)) {};

	dmcdelay(1000);

	if(useExtension){

	    //Put PLL in to Bypass Mode
		writel(BITM_CGU_PLLCTL_PLLEN | BITM_CGU_PLLCTL_PLLBPST, CGU0_PLLCTL + cgu_offset);
		while(!(readl(CGU0_STAT + cgu_offset) & BITM_CGU_STAT_PLLBP)) {};

		dmcdelay(1000);

		writel(0x60030, CGU0_DIVEX + cgu_offset);

		dmcdelay(1000);

		if(cgu_offset == 0)
			*pREG_CDU0_CLKINSEL = 0;

		dmcdelay(1000);

		writel(BITM_CGU_PLLCTL_PLLEN | BITM_CGU_PLLCTL_PLLBPCL, CGU0_PLLCTL + cgu_offset);
		while((readl(CGU0_STAT + cgu_offset) & BITM_CGU_STAT_PLLBP)) {};
		while((readl(CGU0_STAT + cgu_offset) & BITM_CGU_STAT_CLKSALGN)) {};

		dmcdelay(1000);

	}

}


/* @brief       Performs a CGU write operation.
 * @details	This function performs the write operation to configure new
		frequencies as requested by the user
 * @param [in]  dActionFlags     Specifies the action requested by the user
 * @param [in]  *pSyscontrol     Pointer to the ROM structure
 * @param [in]  *reserved        Reserved for future use
 * @return       Status
 */
__attribute__((always_inline)) static inline
uint32_t CGU0_write(uint32_t uiClkOutSel, bool useExtension)
{
	Program_Cgu(0, &Clock_Dividers0, useExtension);
	return 0;
}

__attribute__((always_inline)) static inline
uint32_t CGU1_write(uint32_t uiClkOutSel, bool useExtension)
{
	Program_Cgu(1, &Clock_Dividers1, useExtension);
	return 0;
}

/* @breif	CGU_Init
 * @details	This function initializes the CGU to configure different clocks
 *		in system.
 * @param [in]	uiCguNo		Instance of CGU, 0 or 1
 * @param [in]	uiClkinsel	CLKIN source for CGU, 0 or 1
 * @param [in]	*pClocks	Pointer to Structure which holds different Clock
 *				multipliers and dividers
 * @param [in]	uiClkoutsel	Selects one of the clock to CLKOUT
 * @return	Success/Error code.
 */
__attribute__((always_inline)) static inline
uint32_t CGU_Init(uint32_t uiCguNo, uint32_t uiClkinsel, uint32_t uiClkoutsel, bool useExtension)
{
	static uint32_t result = 0;

	if (uiCguNo == 0) {

		if (readl(CGU0_STAT) & BITM_CGU_STAT_PLLEN)
			writel(BITM_CGU_PLLCTL_PLLEN, CGU0_PLLCTL);

		dmcdelay(1000);

		/* Check if processor is in Active mode */
		if (readl(CGU0_STAT) & BITM_CGU_STAT_PLLBP)
			writel(BITM_CGU_PLLCTL_PLLBPCL, CGU0_PLLCTL);

		dmcdelay(1000);

		CGU0_write(uiClkoutsel, useExtension);

	} else if (uiCguNo == 1) {

		if (readl(CGU1_STAT) & BITM_CGU_STAT_PLLEN)
			writel(BITM_CGU_PLLCTL_PLLEN, CGU1_PLLCTL);

		dmcdelay(1000);

		/* Check if processor is in Active mode */
		if (readl(CGU1_STAT) & BITM_CGU_STAT_PLLBP)
			writel(BITM_CGU_PLLCTL_PLLBPCL, CGU1_PLLCTL);

		dmcdelay(1000);

		CGU1_write(uiClkoutsel, useExtension);
	}
	return result;
}

__attribute__((always_inline)) static inline void cgu_init(void)
{
	CGU_Init(0, 0, 0, 1);
	CGU_Init(1, 0, 0, 0);
}

#define CONFIGURE_CDU0(a,b,c) \
	writel(a, b); \
	while (readl(REG_CDU0_STAT) & (1 << c));

__attribute__((always_inline)) static inline void cdu_init(void)
{
	CONFIGURE_CDU0(CONFIG_CDU0_CLKO0, REG_CDU0_CFG0, 0);
	CONFIGURE_CDU0(CONFIG_CDU0_CLKO1, REG_CDU0_CFG1, 1);
	CONFIGURE_CDU0(CONFIG_CDU0_CLKO2, REG_CDU0_CFG2, 2);
	CONFIGURE_CDU0(CONFIG_CDU0_CLKO3, REG_CDU0_CFG3, 3);
	CONFIGURE_CDU0(CONFIG_CDU0_CLKO4, REG_CDU0_CFG4, 4);
	CONFIGURE_CDU0(CONFIG_CDU0_CLKO5, REG_CDU0_CFG5, 5);
	CONFIGURE_CDU0(CONFIG_CDU0_CLKO6, REG_CDU0_CFG6, 6);
	CONFIGURE_CDU0(CONFIG_CDU0_CLKO7, REG_CDU0_CFG7, 7);
	CONFIGURE_CDU0(CONFIG_CDU0_CLKO8, REG_CDU0_CFG8, 8);
	CONFIGURE_CDU0(CONFIG_CDU0_CLKO9, REG_CDU0_CFG9, 9);
	CONFIGURE_CDU0(CONFIG_CDU0_CLKO10, REG_CDU0_CFG10, 10);
	CONFIGURE_CDU0(CONFIG_CDU0_CLKO12, REG_CDU0_CFG12, 12);
}

__attribute__((always_inline)) static inline void
ddr_init(void)
{
	DMC_Config();
}

//static volatile int i = 0;

#define REG_SPU0_SECUREC0              0x3108B980
#define REG_SPU0_SECUREC1              0x3108B984
#define REG_SPU0_SECUREC2              0x3108B988
#define REG_SPU0_SECUREP_START         0x3108BA00
#define REG_SPU0_SECUREP_END           0x3108BD24

#define REG_ARMPMU0_PMCR               0x31121E04
#define REG_ARMPMU0_PMUSERENR          0x31121E08
#define REG_ARMPMU0_PMLAR              0x31121FB0

void initcode_shared(void)
{
	u32 i;

# ifdef MEM_DDR3
	adi_dmc_lane_reset(true);
# endif

	cdu_init();
	cgu_init();

# ifdef MEM_DDR3
	adi_dmc_lane_reset(false);
# endif

	ddr_init();

	// Enable non-secure access to SHARC to support remoteproc in Linux
	writel(0, REG_SPU0_SECUREC0);
	writel(0, REG_SPU0_SECUREC1);
	writel(0, REG_SPU0_SECUREC2);

	// Configure PMU for non-secure operation
	writel(readl(REG_ARMPMU0_PMUSERENR) | 0x01, REG_ARMPMU0_PMUSERENR);
	writel(0xc5acce55, REG_ARMPMU0_PMLAR);
	writel(readl(REG_ARMPMU0_PMCR) | (1 << 1), REG_ARMPMU0_PMCR);

	for(i = REG_SPU0_SECUREP_START; i <= REG_SPU0_SECUREP_END; i+=4){
		writel(0, i);
	}

        //Disable board LEDs 7, 9, and 10
//      *(uint32_t*)(0x31004114) = 0xE;

        //Enable board LEDs 7, 9, and 10
        *(uint32_t*)(0x3100411C) = 0xE;
        *(uint32_t*)(0x3100410C) = 0xE;

//        *(volatile uint32_t*)(0x20000010) = 0;
//        while(*(volatile uint32_t*)(0x20000010) == 0){
//                i++;
//        }
}
