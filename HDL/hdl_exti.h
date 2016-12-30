/*
*********************************************************************************************************
*
*	模块名称 : HDL_EXTI模块
*	文件名称 : hdl_exti.h
*	说    明 : stm32中外部中断使用的函数库
*
* 注：此库中只是进行外部中断的初始化工作，并没有将外部中断处理函数加入进来
*
*           stm32中的GPIO外部中断，是以组为一个单位 ，同组间的外部中断同一时间只能有一个
								EXTI0~EXTI4这5个外部中断有着单独的中断响应函数
*								EXTI5_9共用一个中断响应函数		
*								EXTI10_15共用一个中断响应函数
*********************************************************************************************************
*/
#ifndef __HDL_EXTI_H
#define __HDL_EXTI_H


#include "hdl_include.h"
typedef struct
{
	uint8_t port_source;  //中断源触发的端口，可以取值为GPIO_PortSourceGPIOA GPIO_PortSourceGPIOB GPIO_PortSourceGPIOC GPIO_PortSourceGPIOD
                        // GPIO_PortSourceGPIOE GPIO_PortSourceGPIOF  GPIO_PortSourceGPIOG
	uint8_t pin_source ; // 中断源触发 的端口号 GPIO_PinSource0 ~ GPIO_PinSource15
	uint32_t exti_line ; //产生外部中断的中断线
	EXTITrigger_TypeDef  trigger_type; //中断的触发方式，EXTI_Trigger_Rising EXTI_Trigger_Falling EXTI_Trigger_Rising_Falling
	uint8_t exti_irq;   //外部中断号
	uint8_t irq_priority; //此中断的抢占优先级
	
	gpio_t * exti_gpio; //外部中断的GPIO
	
}exti_t;


void hdl_init_exti(exti_t  * t);

#endif