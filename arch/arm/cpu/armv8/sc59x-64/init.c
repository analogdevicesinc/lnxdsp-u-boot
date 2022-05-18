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
	CONFIG_VCO_MULT,
	0,
	CONFIG_CCLK_DIV,
	CONFIG_SCLK_DIV,
	CONFIG_SCLK0_DIV,
	CONFIG_SCLK1_DIV,
	CONFIG_DCLK_DIV,
	CONFIG_OCLK_DIV,
	CONFIG_DIV_S1SELEX,
	0x30,
};

static struct CGU_Settings Clock_Dividers1 = {
	CONFIG_CGU1_VCO_MULT,
	0,
	CONFIG_CGU1_CCLK_DIV,
	CONFIG_CGU1_SCLK_DIV,
	CONFIG_CGU1_SCLK0_DIV,
	CONFIG_CGU1_SCLK1_DIV,
	CONFIG_CGU1_DCLK_DIV,
	CONFIG_CGU1_OCLK_DIV,
	CONFIG_CGU1_DIV_S1SELEX,
	CONFIG_CGU1_DIV_S0SELEX
};

__attribute__((always_inline)) static inline void dmcdelay(uint32_t delay)
{
  /* There is no zero-overhead loop on ARM, so assume each iteration takes
   * 4 processor cycles (based on examination of -O3 and -Ofast output).
   */
  uint32_t i, remainder;

  /* Convert DDR cycles to core clock cycles */
  float f = (float)delay * 1.25;
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
void Program_Cgu(uint32_t uiCguNo, struct CGU_Settings *pCGU_Settings, bool useExtension0, bool useExtension1)
{
	uint32_t dNewCguDiv = ((OSEL(pCGU_Settings->div_OSEL)) |
					(SYSSEL(pCGU_Settings->div_SYSSEL)) |
					(CSEL(pCGU_Settings->div_CSEL)) |
					(DSEL(pCGU_Settings->div_DSEL)));

	uint32_t cgu_offset = 0x1000 * uiCguNo;
	uint32_t extension = 0;

    //Put PLL in to Bypass Mode
	writel(BITM_CGU_PLLCTL_PLLEN | BITM_CGU_PLLCTL_PLLBPST, CGU0_PLLCTL + cgu_offset);
	while(!(readl(CGU0_STAT + cgu_offset) & BITM_CGU_STAT_PLLBP)) {};

	dmcdelay(1000);

	if(useExtension0){
		dNewCguDiv |= (readl(CGU0_DIV + cgu_offset) & (BITM_CGU_DIV_S0SEL));
	}
	else{
		dNewCguDiv |= (S0SEL(pCGU_Settings->div_S0SEL));
	}

	if(useExtension1){
		dNewCguDiv |= (readl(CGU0_DIV + cgu_offset) & (BITM_CGU_DIV_S1SEL));
	}else{
		dNewCguDiv |= (S1SEL(pCGU_Settings->div_S1SEL));
	}

	writel(dNewCguDiv, CGU0_DIV + cgu_offset);

	dmcdelay(1000);

	if(useExtension1)
		extension |= BITM_CGU_CTL_S1SELEXEN;

	if(useExtension0)
		extension |= BITM_CGU_CTL_S0SELEXEN;

	writel(( extension | MSEL(pCGU_Settings->ctl_MSEL) | DF(pCGU_Settings->ctl_DF)) &
			(~BITM_CGU_CTL_LOCK), CGU0_CTL + cgu_offset);

	if(useExtension0 | useExtension1)
		while(!(readl(CGU0_CTL + cgu_offset) & (BITM_CGU_CTL_S1SELEXEN|BITM_CGU_CTL_S0SELEXEN))) {};


	dmcdelay(1000);

    //Take PLL out of Bypass Mode
	writel(BITM_CGU_PLLCTL_PLLEN | BITM_CGU_PLLCTL_PLLBPCL, CGU0_PLLCTL + cgu_offset);
	while((readl(CGU0_STAT + cgu_offset) & BITM_CGU_STAT_PLLBP)) {};
	while((readl(CGU0_STAT + cgu_offset) & BITM_CGU_STAT_CLKSALGN)) {};

	dmcdelay(1000);

	if(useExtension1 || useExtension0){

	    //Put PLL in to Bypass Mode
		writel(BITM_CGU_PLLCTL_PLLEN | BITM_CGU_PLLCTL_PLLBPST, CGU0_PLLCTL + cgu_offset);
		while(!(readl(CGU0_STAT + cgu_offset) & BITM_CGU_STAT_PLLBP)) {};

		dmcdelay(1000);

		if(useExtension0 | useExtension1)
			writel( S0SELEX(pCGU_Settings->divex_S0SELEX) | S1SELEX(pCGU_Settings->divex_S1SELEX), CGU0_DIVEX + cgu_offset);

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
uint32_t CGU0_write(uint32_t uiClkOutSel, bool useExtension0, bool useExtension1)
{
	Program_Cgu(0, &Clock_Dividers0, useExtension0, useExtension1);
	return 0;
}

__attribute__((always_inline)) static inline
uint32_t CGU1_write(uint32_t uiClkOutSel, bool useExtension0, bool useExtension1)
{
	Program_Cgu(1, &Clock_Dividers1, useExtension0, useExtension1);
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
uint32_t CGU_Init(uint32_t uiCguNo, uint32_t uiClkinsel, uint32_t uiClkoutsel, bool useExtension0, bool useExtension1)
{
	uint32_t result = 0;
	uint32_t status;

	if (uiCguNo == 0) {

		if (readl(CGU0_STAT) & BITM_CGU_STAT_PLLEN)
			writel(BITM_CGU_PLLCTL_PLLEN, CGU0_PLLCTL);

		dmcdelay(1000);

		/* Check if processor is in Active mode */
		if (readl(CGU0_STAT) & BITM_CGU_STAT_PLLBP)
			writel(BITM_CGU_PLLCTL_PLLBPCL, CGU0_PLLCTL);

		dmcdelay(1000);

		CGU0_write(uiClkoutsel, useExtension0, useExtension1);

	} else if (uiCguNo == 1) {

		if (readl(CGU1_STAT) & BITM_CGU_STAT_PLLEN)
			writel(BITM_CGU_PLLCTL_PLLEN, CGU1_PLLCTL);

		dmcdelay(1000);

		/* Check if processor is in Active mode */
		if (readl(CGU1_STAT) & BITM_CGU_STAT_PLLBP)
			writel(BITM_CGU_PLLCTL_PLLBPCL, CGU1_PLLCTL);

		dmcdelay(1000);

		CGU1_write(uiClkoutsel, useExtension0, useExtension1);
	}
	return result;
}


__attribute__((always_inline)) static inline void cgu_init(void)
{
	CGU_Init(0, 0, 0, 0, 1);
	CGU_Init(1, 0, 0, 1, 1);
}


__attribute__((always_inline)) static inline void Enable_VCOby5(uint32_t CGUn, bool Enable)
{
	if(Enable)
	{
		if(CGUn == 0)
		{
			*pREG_PLL_CFG_tst |= 0x40;
		}
		else if(CGUn == 1)
		{
			*pREG_PLL_CFG_tst |= 0x80;
		}
		else
		{
			while(1);
		}
	}
	else
	{

	}
}

__attribute__((always_inline)) static inline void cdu_init(void)
{
	writel((0 << 1) | 0x1, REG_CDU0_CFG0);
	while (readl(REG_CDU0_STAT) & (1 << 0));

	writel((0 << 1) | 0x1, REG_CDU0_CFG1);
	while (readl(REG_CDU0_STAT) & (1 << 1));

	writel((3 << 1) | 0x1, REG_CDU0_CFG2);
	while (readl(REG_CDU0_STAT) & (1 << 2));

	/* DDR - Source from DCLK_1 */
	writel((0 << 1) | 0x1, REG_CDU0_CFG3);
	while (readl(REG_CDU0_STAT) & (1 << 3));

	writel((1 << 1) | 0x1, REG_CDU0_CFG4);
	while (readl(REG_CDU0_STAT) & (1 << 4));

	writel((0 << 1) | 0x1, REG_CDU0_CFG5);
	while (readl(REG_CDU0_STAT) & (1 << 5));

	/* SPI - Source from SCLK0_0 */
	writel((0 << 1) | 0x1, REG_CDU0_CFG6);
	while (readl(REG_CDU0_STAT) & (1 << 6));

	/* Gigabit Ethernet - Source from SCLK0_0 */
	writel((0 << 1) | 0x1, REG_CDU0_CFG7);
	while (readl(REG_CDU0_STAT) & (1 << 7));

	writel((1 << 1) | 0x1, REG_CDU0_CFG8);
	while (readl(REG_CDU0_STAT) & (1 << 8));

	writel((0 << 1) | 0x1, REG_CDU0_CFG9);
	while (readl(REG_CDU0_STAT) & (1 << 9));

	writel((0 << 1) | 0x1, REG_CDU0_CFG10);
	while (readl(REG_CDU0_STAT) & (1 << 10));

	writel((0 << 1) | 0x1, REG_CDU0_CFG12);
	while (readl(REG_CDU0_STAT) & (1 << 12));

	writel((1 << 1) | 0x1, REG_CDU0_CFG13);
	while (readl(REG_CDU0_STAT) & (1 << 13));

	writel((1 << 1) | 0x1, REG_CDU0_CFG14);
	while (readl(REG_CDU0_STAT) & (1 << 14));
}

__attribute__((always_inline)) static inline void
ddr_init(void)
{
	DMC_Config();
}

void initcode_shared(void)
{

# ifdef MEM_DDR3
	adi_dmc_lane_reset(true);
# endif

	cdu_init();
	cgu_init();

# ifdef MEM_DDR3
	adi_config_third_pll(64,2);
	adi_dmc_lane_reset(false);
# endif

	ddr_init();

	// Enable coresight timer
	writel(1, REG_TSGENWR0_CNTCR);

	// Enable non-secure access to SHARC to support remoteproc in Linux
	writel(0, REG_SPU0_SECUREC0);
	writel(0, REG_SPU0_SECUREC1);
	writel(0, REG_SPU0_SECUREC2);

    //Enable board LEDs 7, 9, and 10 (Real Board)
	// - PORTC_01, PORTC_02, PORTC_03
    *(uint32_t*)(0x3100411C) = 0xE;
    *(uint32_t*)(0x3100410C) = 0xE;

}

