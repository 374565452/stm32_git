/*
*********************************************************************************************************
*
*	ģ������ : HDL_INCLUDEģ��
*	�ļ����� : hdl_include.h
*	˵    �� : ����ͷ�ļ������ļ�
*			  ����Ҫ#include ÿ��ģ��� h �ļ�
*				 HDL��ָHardware Driver Library��Ӳ�������⣩	
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
	GPIO_TypeDef * GPIO_x; //�˿����� A,B,C,D,E
	uint16_t GPIO_Pin;     //�˿ں�
	uint32_t gpio_rcc; //GPIO�˿ڵ�RCCʱ��
}gpio_t;


#endif