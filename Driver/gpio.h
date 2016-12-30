#ifndef __GPIO_H
#define __GPIO_H

#include "stm32f10x.h"

#define LED_GPIO GPIOC
#define LED_GPIO_PORT GPIO_Pin_14

#define POWER_CTL_GPIO GPIOE
#define POWER_CTL_PORT GPIO_Pin_8

void init_power_gpio();
void init_led_gpio();

#endif