#include <asm-generic/gpio.h>

// @todo migrate other shared spl functionality here

static bool adi_start_uboot_proper = 1;

int spl_start_uboot(void)
{
	return adi_start_uboot_proper;
}

#ifdef CONFIG_ADI_FALCON
void adi_check_pushbuttons(){
	struct gpio_desc *pb0;
	struct gpio_desc *pb1;

	gpio_hog_lookup_name("pushbutton0", &pb0);
	gpio_hog_lookup_name("pushbutton1", &pb1);

	if (!pb0 || !pb1)
		return;

	if (dm_gpio_get_value(pb0) || dm_gpio_get_value(pb1)) {
		adi_start_uboot_proper = 1;

		//Wait until they're released, in case these pins conflict with peripherals (OSPI, etc)
		while(dm_gpio_get_value(pb0));
		while(dm_gpio_get_value(pb1));
	}else{
		adi_start_uboot_proper = 0;
	}
}
#endif
