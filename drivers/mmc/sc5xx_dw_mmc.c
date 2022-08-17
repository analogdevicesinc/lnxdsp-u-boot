#include <common.h>
#include <dwmmc.h>

#include <asm/mach-adi/common/clock.h>
#include <asm/mach-adi/common/sc5xx.h>

#define DWMMC_MAX_FREQ 52000000
#define DWMMC_MIN_FREQ 400000
#define DWMMC_REG_BASE REG_MSI0_CTL

static struct dwmci_host dwmci_host;

int sc5xx_dwmmc_init(struct bd_info *bis)
{
	int ret;
	struct dwmci_host *host;

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
