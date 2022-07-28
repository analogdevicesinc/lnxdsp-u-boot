/*
 * Copyright (C) 2013 Analog Devices Inc.
 * Licensed under the GPL-2 or later.
 */

#include <asm/arch/portmux.h>
#include <asm/arch/sc58x.h>
#include <asm/io.h>
#include <config.h>
#include "cguinit.h"
#include "dmcinit.h"

#define SPI_BASE	0x60000000
#define CGU_STAT_MASK (BITM_CGU_STAT_PLLEN | BITM_CGU_STAT_PLOCK | \
			BITM_CGU_STAT_CLKSALGN)
#define CGU_STAT_ALGN_LOCK (BITM_CGU_STAT_PLLEN | BITM_CGU_STAT_PLOCK)

static inline
void Program_Cgu(uint32_t uiCguNo, struct CGU_Settings *pCGU_Settings)
{
	uint32_t dNewCguDiv = ((OSEL(pCGU_Settings->div_OSEL)) |
					(SYSSEL(pCGU_Settings->div_SYSSEL)) |
					(CSEL(pCGU_Settings->div_CSEL)) |
					(S0SEL(pCGU_Settings->div_S0SEL))|
					(S1SEL(pCGU_Settings->div_S1SEL))|
					(DSEL(pCGU_Settings->div_DSEL)))&
					(~BITM_CGU_DIV_LOCK);
	uint32_t cgu_offset = 0x1000 * uiCguNo;

	//Put PLL in to Bypass Mode
	writel(BITM_CGU_PLLCTL_PLLEN | BITM_CGU_PLLCTL_PLLBPST, CGU0_PLLCTL + cgu_offset);
	while(!(readl(CGU0_STAT + cgu_offset) & BITM_CGU_STAT_PLLBP)) {};

	while (!((readl(CGU0_STAT + cgu_offset) & CGU_STAT_MASK) ==
		 CGU_STAT_ALGN_LOCK));

	writel(dNewCguDiv & (~BITM_CGU_DIV_ALGN) & (~BITM_CGU_DIV_UPDT),
		CGU0_DIV + cgu_offset);
		/* Clear the Align and Update bit, since CTL is also written */
	writel((MSEL(pCGU_Settings->ctl_MSEL) | DF(pCGU_Settings->ctl_DF)) &
		(~BITM_CGU_CTL_LOCK), CGU0_CTL + cgu_offset);


	//Take PLL out of Bypass Mode
	writel(BITM_CGU_PLLCTL_PLLEN | BITM_CGU_PLLCTL_PLLBPCL, CGU0_PLLCTL + cgu_offset);
	while((readl(CGU0_STAT + cgu_offset) & BITM_CGU_STAT_PLLBP)) {};
	while((readl(CGU0_STAT + cgu_offset) & BITM_CGU_STAT_CLKSALGN)) {};
}

/* @brief       Performs a CGU write operation.
 * @details	This function performs the write operation to configure new
		frequencies as requested by the user
 * @param [in]  dActionFlags     Specifies the action requested by the user
 * @param [in]  *pSyscontrol     Pointer to the ROM structure
 * @param [in]  *reserved        Reserved for future use
 * @return       Status
 */
static inline
uint32_t CGU0_write(struct CGU_Settings *pCGU_Settings, uint32_t uiClkOutSel)
{

	Program_Cgu(0, pCGU_Settings);

	writel(uiClkOutSel, CGU0_CLKOUTSEL);

	return 0;
}

static inline
uint32_t CGU1_write(struct CGU_Settings *pCGU_Settings, uint32_t uiClkOutSel)
{

	Program_Cgu(1, pCGU_Settings);

	return 0;
}


/* @brief	Active_To_Fullon
 * @details	This function changes the Operating mode from Active to FullOn
 * @param [in]	uiCGU_ID	CGU number 0 or 1
 * @return	None
 */
static inline
void Active_To_Fullon(unsigned int uiCGU_ID)
{
	if (uiCGU_ID == 0) {
		uint32_t oldCGU0_STAT;

		while (1) {
			oldCGU0_STAT = readl(CGU0_STAT);
			if ((oldCGU0_STAT & BITM_CGU_STAT_PLLEN) &&
				(oldCGU0_STAT & BITM_CGU_STAT_PLLBP))
				break;
		}

		writel(BITM_CGU_PLLCTL_PLLBPCL, CGU0_PLLCTL);

		while (1) {
			oldCGU0_STAT = readl(CGU0_STAT);
			if ((oldCGU0_STAT & BITM_CGU_STAT_PLLEN) &&
				~(oldCGU0_STAT & BITM_CGU_STAT_PLLBP) &&
				~(oldCGU0_STAT & BITM_CGU_STAT_CLKSALGN))
				break;
		}
	}

	if (uiCGU_ID == 1)
	{
		uint32_t oldCGU0_STAT1;

		while (1) {
			oldCGU0_STAT1 = readl(CGU1_STAT);
			if ((oldCGU0_STAT1 & BITM_CGU_STAT_PLLEN) &&
				(oldCGU0_STAT1 & BITM_CGU_STAT_PLLBP))
				break;
		}

		writel(BITM_CGU_PLLCTL_PLLBPCL, CGU1_PLLCTL);

		while (1) {
			oldCGU0_STAT1 = readl(CGU1_STAT);
			if ((oldCGU0_STAT1 & BITM_CGU_STAT_PLLEN) &&
				~(oldCGU0_STAT1 & BITM_CGU_STAT_PLLBP) &&
				~(oldCGU0_STAT1 & BITM_CGU_STAT_CLKSALGN))
				break;
        }
    }
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
static inline
uint32_t CGU_Init(uint32_t uiCguNo, uint32_t uiClkinsel,
			struct CGU_Settings *pClocks, uint32_t uiClkoutsel)
{
	uint32_t result = 0;

	if (uiCguNo == 0) {
		/* Check if processor is in Active mode */
		if (readl(CGU0_STAT) & BITM_CGU_STAT_PLLBP)
			Active_To_Fullon(0);

		CGU0_write(pClocks, uiClkoutsel);
	} else if (uiCguNo == 1) {
		/* Check if processor is in Active mode */
		if(readl(CGU1_STAT) & BITM_CGU_STAT_PLLBP)
			Active_To_Fullon(1);

		CGU1_write(pClocks, uiClkoutsel);
	}
	return result;
}

static inline void cgu_init(void)
{
	struct CGU_Settings Clock_Diverders0 = { CONFIG_VCO_MULT,
						0,
						CONFIG_CCLK_DIV,
						CONFIG_SCLK_DIV,
						CONFIG_SCLK0_DIV,
						CONFIG_SCLK1_DIV,
						CONFIG_DCLK_DIV,
						CONFIG_OCLK_DIV };
#ifdef CONFIG_CLOCK_SPEED_500MHZ
	/* Configure CGU1 for DMC clock to max frequence: 450MHz */
	struct CGU_Settings Clock_Diverders1 = { CONFIG_CGU1_VCO_MULT,
						0,
						CONFIG_CGU1_CCLK_DIV,
						CONFIG_CGU1_SCLK_DIV,
						CONFIG_CGU1_SCLK0_DIV,
						CONFIG_CGU1_SCLK1_DIV,
						CONFIG_CGU1_DCLK_DIV,
						CONFIG_CGU1_OCLK_DIV };
#else
	struct CGU_Settings Clock_Diverders1 = {5,0,1,2,2,2,2,3};
#endif
	CGU_Init(0,0,&Clock_Diverders0, CLKO7);
	CGU_Init(1,0,&Clock_Diverders1, 0);
}

static inline void cdu_init(void)
{
#ifdef CONFIG_CLOCK_SPEED_500MHZ
	/* Configure GigEth/RGMII clock source from SCLK1_0 (125MHz) */
	writel((0 << 1) | 0x1, REG_CDU0_CFG7);
	while (readl(REG_CDU0_STAT) & (1 << 7));
	/* Configure DMC clock source from DCLK1: 450MHz(max frequence) */
	writel((1 << 1) | 0x1, REG_CDU0_CFG3);
	while (readl(REG_CDU0_STAT) & (1 << 3));
#else
	/* Configure GigEth/RGMII clock source from CCLK0_1 (125MHz) */
	writel((2 << 1) | 0x1, REG_CDU0_CFG7);
	while (readl(REG_CDU0_STAT) & (1 << 7));
#endif
}


#define REG_SPU0_SECUREC0              0x3108C980
#define REG_SPU0_SECUREC1              0x3108C984
#define REG_SPU0_SECUREC2              0x3108C988
#define REG_SPU0_SECUREP_START         0x3108CA00
#define REG_SPU0_SECUREP_END           0x3108CD24

#define REG_ARMPMU0_PMCR               0x31121E04
#define REG_ARMPMU0_PMUSERENR          0x31121E08
#define REG_ARMPMU0_PMLAR              0x31121FB0

__attribute__((always_inline)) static inline void
ddr_init(void)
{
	DMC_Config();
}

void initcode(void)
{
	u32 i;

# ifdef MEM_DDR3
	writel(0x800, REG_DMC0_PHY_CTL0);
	writel(0x800, REG_DMC1_PHY_CTL0);
# endif
	cdu_init();
	cgu_init();

# ifdef MEM_DDR3
	writel(0x0, REG_DMC0_PHY_CTL0);
	writel(0x0, REG_DMC1_PHY_CTL0);
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

}
