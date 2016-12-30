/*
*********************************************************************************************************
*
*	模块名称 : HDL_INCLUDE模块
*	文件名称 : hdl_include.h
*	说    明 : 各个头文件包括文件
*			  不需要#include 每个模块的 h 文件
*				 HDL是指Hardware Driver Library（硬件驱动库）	
*
*
*********************************************************************************************************
*/
#ifndef __HDL_INCLUDE_H
#define __HDL_INCLUDE_H	

#include "stm32f10x.h"
#include "hdl_rcc.h"
#include "hdl_uart.h"
#include "hdl_gpio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hdl_flash.h"
#include "hdl_iwdg.h"
#include "hdl_i2c.h"


typedef struct
{
	GPIO_TypeDef * GPIO_x; //端口类型 A,B,C,D,E
	uint16_t GPIO_Pin;     //端口号
	uint32_t gpio_rcc; //GPIO端口的RCC时钟
}gpio_t;


#endif