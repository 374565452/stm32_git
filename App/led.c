#include "led.h"

#include "gpio.h"
#include "FreeRTOS.h"

#define ledSetHigh()       GPIO_SetBits(LED_GPIO, LED_GPIO_PORT)
#define ledSetLow()        GPIO_ResetBits(LED_GPIO, LED_GPIO_PORT)
#define ledStatus()        GPIO_ReadOutputDataBit(LED_GPIO, LED_GPIO_PORT)
void led_on()
{
	ledSetHigh();
}

void led_off()
{
	ledSetLow();
}

void toggle_led()
{
	if(ledStatus())
	{
		led_off();
	}
	else
	{
		led_on();
	}
}

void init_led()
{
	init_led_gpio();
}

void led_task()
{
	while(1)
	{
		toggle_led();
		vTaskDelay(1000); //Ë¯Ãß1S
	}
}

void create_led_task()
{
	 xTaskCreate( led_task, "led_task", 40, NULL, 2, NULL );
}