#include <common.h>
#include <dwmmc.h>

#if defined(CONFIG_SC59X)
#include <asm/arch-sc59x/clock.h>
#include <asm/arch-sc59x/portmux.h>
#elif defined(CONFIG_SC58X)
#include <asm/arch-sc58x/clock.h>
#include <asm/arch-sc58x/portmux.h>
#elif defined(CONFIG_SC57X)
#include <asm/arch-sc57x/clock.h>
#include <asm/arch-sc57x/portmux.h>
#endif
#include <adi/common/sc5xx.h>

#define DWMMC_MAX_FREQ 52000000
#define DWMMC_MIN_FREQ 400000
#define DWMMC_REG_BASE REG_MSI0_CTL

#define PORTMUX_PINS \
	{ P_MSI0_D0, P_MSI0_D1, P_MSI0_D2, P_MSI0_D3, \
	  P_MSI0_CMD, P_MSI0_CLK, P_MSI0_CDb, 0 }

static struct dwmci_host dwmci_host;

int sc5xx_dwmmc_init(struct bd_info *bis)
{
	int ret;
	const unsigned short pins[] = PORTMUX_PINS;
	struct dwmci_host *host;

	/*initialize pin mux in sc58x*/
	ret = peripheral_request_list(pins, "sc5xx_sdh");

	host = &dwmci_host;
	host->name = "SC5XX SDH";
	host->caps = MMC_MODE_DDR_52MHz;
	host->ioaddr = (void *)DWMMC_REG_BASE;
	host->buswidth = CONFIG_SC5XX_BUS_WIDTH;
	host->bus_hz = get_sclk0();

	ret = add_dwmci(host, DWMMC_MAX_FREQ, DWMMC_MIN_FREQ);
	if (ret < 0) {
		printf("add_dwmci failed\n");
		return -1;
	}
	return 0;
}
