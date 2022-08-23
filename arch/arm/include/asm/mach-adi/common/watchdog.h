/*
 * Watchdog Masks
 */

#ifndef __SC5XX_WATCHDOG__
#define __SC5XX_WATCHDOG__

#define WDOG_CTL WDOG0_CTL
#define WDOG_CNT WDOG0_CNT
#define WDOG_STAT WDOG0_STAT
#define WDOG_WIN WDOG0_WIN

/* Watchdog Timer WDOG_CTL Register Masks */
#define WDEN			0x0010	/* enable watchdog */
#define WDRO			0x8000	/* watchdog rolled over latch */
#define WDDIS			0x0AD0  /* disable watchdog */
#define WDRO			0x8000  /* watchdog rolled over latch */

#endif
