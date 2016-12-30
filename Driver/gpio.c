#include "gpio.h"

void init_power_gpio()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE , ENABLE);
	
	GPIO_InitStructure.GPIO_Pin	= POWER_CTL_PORT;        
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 	 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	 
  GPIO_Init(POWER_CTL_GPIO, &GPIO_InitStructure); 
}

void init_led_gpio()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC , ENABLE);
	
	GPIO_InitStructure.GPIO_Pin	= LED_GPIO_PORT;        
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 	 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	 
  GPIO_Init(LED_GPIO, &GPIO_InitStructure); 
}
