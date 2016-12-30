/*
*********************************************************************************************************
*
*	模块名称 : HDL_GPIO模块
*	文件名称 : hdl_gpio.h
*	说    明 :设置stm32管脚的gpio功能，设置为输出管脚，设置为输入管脚 
*				 指定管脚输入高电平，指定管脚输出低电平
*        从输入管脚读取值  
*
*
*********************************************************************************************************
*/
#ifndef __HDL_GPIO_H
#define __HDL_GPIO_H

#include "stm32f10x.h"

#define hdl_h(gpiox,port) GPIO_SetBits((GPIO_TypeDef *)(gpiox),(port))

#define hdl_l(gpiox,port) GPIO_ResetBits((GPIO_TypeDef *)(gpiox),(port))

#define hdl_read(gpiox,port) GPIO_ReadInputDataBit((GPIO_TypeDef *)(gpiox),(port))

void hdl_output_gpio(GPIO_TypeDef* gpio,uint16_t port,GPIOMode_TypeDef mode);

void hdl_input_gpio(GPIO_TypeDef* gpio,uint16_t port,GPIOMode_TypeDef mode);

#endif