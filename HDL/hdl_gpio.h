/*
*********************************************************************************************************
*
*	ģ������ : HDL_GPIOģ��
*	�ļ����� : hdl_gpio.h
*	˵    �� :����stm32�ܽŵ�gpio���ܣ�����Ϊ����ܽţ�����Ϊ����ܽ� 
*				 ָ���ܽ�����ߵ�ƽ��ָ���ܽ�����͵�ƽ
*        ������ܽŶ�ȡֵ  
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