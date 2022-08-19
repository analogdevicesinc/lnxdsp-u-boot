/*
 * Copyright (C) 2013 Analog Devices Inc.
 * Licensed under the GPL-2 or later.
 */

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
#include "cguinit.h"
#include "dmc/dmcinit.h"
#include "init.h"

#define CGU_STAT_MASK (BITM_CGU_STAT_PLLEN | BITM_CGU_STAT_PLOCK | \
            BITM_CGU_STAT_CLKSALGN)
#define CGU_STAT_ALGN_LOCK (BITM_CGU_STAT_PLLEN | BITM_CGU_STAT_PLOCK)

static inline
void Program_Cgu(uint32_t uiCguNo, struct CGU_Settings *pCGU_Settings, bool useExtension0, bool useExtension1)
{
    uint32_t dNewCguDiv;
    uint32_t cgu_offset = 0x1000 * uiCguNo;
    uint32_t extension = 0;

    dNewCguDiv =  OSEL(pCGU_Settings->div_OSEL);
    dNewCguDiv |= SYSSEL(pCGU_Settings->div_SYSSEL);
    dNewCguDiv |= CSEL(pCGU_Settings->div_CSEL);
    dNewCguDiv |= DSEL(pCGU_Settings->div_DSEL);
    dNewCguDiv &= ~BITM_CGU_DIV_LOCK;

    //Put PLL in to Bypass Mode
    writel(BITM_CGU_PLLCTL_PLLEN | BITM_CGU_PLLCTL_PLLBPST, CGU0_PLLCTL + cgu_offset);
    while(!(readl(CGU0_STAT + cgu_offset) & BITM_CGU_STAT_PLLBP)) {};

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

    while (!((readl(CGU0_STAT + cgu_offset) & CGU_STAT_MASK) ==
         CGU_STAT_ALGN_LOCK));

    dmcdelay(1000);

    writel(dNewCguDiv & (~BITM_CGU_DIV_ALGN) & (~BITM_CGU_DIV_UPDT),
        CGU0_DIV + cgu_offset);

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

#if defined(CONFIG_SC59X)
        writel( S1SELEX(pCGU_Settings->divex_S1SELEX) | 0x30, CGU0_DIVEX + cgu_offset);
#endif

#if defined(CONFIG_SC59X_64)
        writel( S0SELEX(pCGU_Settings->divex_S0SELEX) | S1SELEX(pCGU_Settings->divex_S1SELEX), CGU0_DIVEX + cgu_offset);
#endif

        dmcdelay(1000);

#if defined(CONFIG_SC59X) || defined(CONFIG_SC59X_64)
        if(cgu_offset == 0)
            writel(0, REG_CDU0_CLKINSEL);
#endif

        dmcdelay(1000);

        writel(BITM_CGU_PLLCTL_PLLEN | BITM_CGU_PLLCTL_PLLBPCL, CGU0_PLLCTL + cgu_offset);
        while((readl(CGU0_STAT + cgu_offset) & BITM_CGU_STAT_PLLBP)) {};
        while((readl(CGU0_STAT + cgu_offset) & BITM_CGU_STAT_CLKSALGN)) {};

        dmcdelay(1000);

    }
}

/* @brief       Performs a CGU write operation.
 * @details    This function performs the write operation to configure new
        frequencies as requested by the user
 * @param [in]  dActionFlags     Specifies the action requested by the user
 * @param [in]  *pSyscontrol     Pointer to the ROM structure
 * @param [in]  *reserved        Reserved for future use
 * @return       Status
 */
static inline
uint32_t CGU0_write(struct CGU_Settings *pCGU_Settings, uint32_t uiClkOutSel, bool useExtension0, bool useExtension1)
{

    Program_Cgu(0, pCGU_Settings, useExtension0, useExtension1);

#ifndef CONFIG_SC59X
    writel(uiClkOutSel, CGU0_CLKOUTSEL);
#endif

    return 0;
}

static inline
uint32_t CGU1_write(struct CGU_Settings *pCGU_Settings, uint32_t uiClkOutSel, bool useExtension0, bool useExtension1)
{

    Program_Cgu(1, pCGU_Settings, useExtension0, useExtension1);

    return 0;
}


/* @brief    Active_To_Fullon
 * @details    This function changes the Operating mode from Active to FullOn
 * @param [in]    uiCGU_ID    CGU number 0 or 1
 * @return    None
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

/* @breif    CGU_Init
 * @details    This function initializes the CGU to configure different clocks
 *        in system.
 * @param [in]    uiCguNo        Instance of CGU, 0 or 1
 * @param [in]    uiClkinsel    CLKIN source for CGU, 0 or 1
 * @param [in]    *pClocks    Pointer to Structure which holds different Clock
 *                multipliers and dividers
 * @param [in]    uiClkoutsel    Selects one of the clock to CLKOUT
 * @return    Success/Error code.
 */
static inline
uint32_t CGU_Init(uint32_t uiCguNo, uint32_t uiClkinsel,
            struct CGU_Settings *pClocks, uint32_t uiClkoutsel, bool useExtension0, bool useExtension1)
{
    uint32_t result = 0;

    if (uiCguNo == 0) {

#if defined(CONFIG_SC59X) || defined(CONFIG_SC59X_64)
        if (readl(CGU0_STAT) & BITM_CGU_STAT_PLLEN)
            writel(BITM_CGU_PLLCTL_PLLEN, CGU0_PLLCTL);

        dmcdelay(1000);
#endif

        /* Check if processor is in Active mode */
        if (readl(CGU0_STAT) & BITM_CGU_STAT_PLLBP)
            Active_To_Fullon(0);

#if defined(CONFIG_SC59X) || defined(CONFIG_SC59X_64)
        dmcdelay(1000);
#endif

        CGU0_write(pClocks, uiClkoutsel, useExtension0, useExtension1);
    } else if (uiCguNo == 1) {

#if defined(CONFIG_SC59X) || defined(CONFIG_SC59X_64)
        if (readl(CGU1_STAT) & BITM_CGU_STAT_PLLEN)
            writel(BITM_CGU_PLLCTL_PLLEN, CGU1_PLLCTL);

        dmcdelay(1000);
#endif

        /* Check if processor is in Active mode */
        if(readl(CGU1_STAT) & BITM_CGU_STAT_PLLBP)
            Active_To_Fullon(1);

#if defined(CONFIG_SC59X) || defined(CONFIG_SC59X_64)
        dmcdelay(1000);
#endif

        CGU1_write(pClocks, uiClkoutsel, useExtension0, useExtension1);
    }
    return result;
}

static inline void cgu_init(void)
{
    struct CGU_Settings Clock_Dividers0 = { CONFIG_CGU0_VCO_MULT,
                        CONFIG_CGU0_DF_DIV,
                        CONFIG_CGU0_CCLK_DIV,
                        CONFIG_CGU0_SCLK_DIV,
                        CONFIG_CGU0_SCLK0_DIV,
                        CONFIG_CGU0_SCLK1_DIV,
                        CONFIG_CGU0_DCLK_DIV,
                        CONFIG_CGU0_OCLK_DIV };
    struct CGU_Settings Clock_Dividers1 = { CONFIG_CGU1_VCO_MULT,
                        CONFIG_CGU1_DF_DIV,
                        CONFIG_CGU1_CCLK_DIV,
                        CONFIG_CGU1_SCLK_DIV,
                        CONFIG_CGU1_SCLK0_DIV,
                        CONFIG_CGU1_SCLK1_DIV,
                        CONFIG_CGU1_DCLK_DIV,
                        CONFIG_CGU1_OCLK_DIV };

#if defined(CONFIG_SC59X) || defined(CONFIG_SC59X_64)
    Clock_Dividers0.divex_S1SELEX = CONFIG_CGU0_DIV_S1SELEX;
    Clock_Dividers1.divex_S1SELEX = CONFIG_CGU1_DIV_S1SELEX;
#endif

#if defined(CONFIG_SC59X_64)
    Clock_Dividers0.divex_S0SELEX = 0x30;
    Clock_Dividers1.divex_S0SELEX = CONFIG_CGU1_DIV_S0SELEX;
#endif

#if defined(CONFIG_SC59X_64)
    CGU_Init(0, 0, &Clock_Dividers0, 0, 0, 1);
    CGU_Init(1, 0, &Clock_Dividers1, 0, 1, 1);
#elif defined(CONFIG_SC59X)
    CGU_Init(0, 0, &Clock_Dividers0, 0, 0, 1);
    CGU_Init(1, 0, &Clock_Dividers1, 0, 0, 0);
#else
    CGU_Init(0, 0, &Clock_Dividers0, CLKO7, 0, 0);
    CGU_Init(1, 0, &Clock_Dividers1, 0, 0, 0);
#endif
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
#if defined(CONFIG_SC59X) || defined(CONFIG_SC59X_64)
    CONFIGURE_CDU0(CONFIG_CDU0_CLKO10, REG_CDU0_CFG10, 10);
    CONFIGURE_CDU0(CONFIG_CDU0_CLKO12, REG_CDU0_CFG12, 12);
#endif
#ifdef CONFIG_SC59X_64
    CONFIGURE_CDU0(CONFIG_CDU0_CLKO13, REG_CDU0_CFG13, 13);
    CONFIGURE_CDU0(CONFIG_CDU0_CLKO14, REG_CDU0_CFG14, 14);
#endif
}

__attribute__((always_inline)) static inline void
ddr_init(void)
{
    DMC_Config();
}

void initcode_shared(void)
{
    u32 i;

#ifdef MEM_DDR3
    #if defined(CONFIG_SC59X) || defined(CONFIG_SC59X_64)
        adi_dmc_lane_reset(true, 0);
    #else
        #ifdef MEM_DMC0
            writel(0x800, REG_DMC0_PHY_CTL0);
        #endif
        #ifdef MEM_DMC1
            writel(0x800, REG_DMC1_PHY_CTL0);
        #endif
    #endif
#endif

    cdu_init();
    cgu_init();

#ifdef MEM_DDR3
    #if defined(CONFIG_SC59X) || defined(CONFIG_SC59X_64)
        #ifdef CONFIG_SC59X_64
            adi_config_third_pll(64,2);
        #endif
        adi_dmc_lane_reset(false, 0);
    #else
        #ifdef MEM_DMC0
            writel(0x0, REG_DMC0_PHY_CTL0);
        #endif
        #ifdef MEM_DMC1
            writel(0x0, REG_DMC1_PHY_CTL0);
        #endif
    #endif
#endif

    ddr_init();

#ifdef CONFIG_SC59X_64
    // Enable coresight timer
    writel(1, REG_TSGENWR0_CNTCR);
#endif

    // Enable non-secure access to SHARC to support remoteproc in Linux
    writel(0, REG_SPU0_SECUREC0);
    writel(0, REG_SPU0_SECUREC1);
    writel(0, REG_SPU0_SECUREC2);

#ifdef CONFIG_SC59X_64
    //Do not rerun preboot routine --
    // Without this, hardware resets triggered by RCU0_CTL:SYSRST
    // lead to a deadlock somewhere in the boot ROM
    writel(0x200, REG_RCU0_BCODE);
#endif

#ifndef CONFIG_SC59X_64
    // Configure PMU for non-secure operation
    writel(readl(REG_ARMPMU0_PMUSERENR) | 0x01, REG_ARMPMU0_PMUSERENR);
    writel(0xc5acce55, REG_ARMPMU0_PMLAR);
    writel(readl(REG_ARMPMU0_PMCR) | (1 << 1), REG_ARMPMU0_PMCR);
#endif

#if defined(CONFIG_SC59X) || defined(CONFIG_SC59X_64)
    //Enable board LEDs 7, 9, and 10
    *(uint32_t*)(0x3100411C) = 0xE;
    *(uint32_t*)(0x3100410C) = 0xE;
#endif

#if ! (defined(CONFIG_SC57X) || defined(CONFIG_SC59X_64))
    for(i = REG_SPU0_SECUREP_START; i <= REG_SPU0_SECUREP_END; i+=4){
        writel(0, i);
    }
#endif

}
