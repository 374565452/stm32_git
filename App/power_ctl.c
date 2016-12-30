#include "power_ctl.h"
#include "gpio.h"
#include "FreeRTOS.h"

#define PowerSetHigh()       GPIO_SetBits(POWER_CTL_GPIO, POWER_CTL_PORT)
#define PowerSetLow()        GPIO_ResetBits(POWER_CTL_GPIO, POWER_CTL_PORT)
#define PowerStatus()        GPIO_ReadOutputDataBit(POWER_CTL_GPIO, POWER_CTL_PORT)
void open_power()
{
	PowerSetHigh();
}

void close_power()
{
	PowerSetLow();
}

void toggle_power()
{
	if(PowerStatus())
	{
		close_power();
	}
	else
	{
		open_power();
	}
}

void init_power_ctl()
{
	init_power_gpio();
}

void power_ctl_task()
{
	while(1)
	{
		toggle_power();
		vTaskDelay(2000); //Ë¯Ãß2S
	}
}

void create_power_ctl_task()
{
	 xTaskCreate( power_ctl_task, "power_task", 40, NULL, 2, NULL );
}