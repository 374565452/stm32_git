/*
*********************************************************************************************************
*
*	模块名称 : HDL_TIM模块
*	文件名称 : hdl_tim.h
*	说    明 : TIM1~TIM8定时器中断 硬件初始化模块
* 
*          tim中断 时间计算公司 period /(72MHZ/prescaler)
*********************************************************************************************************
*/
#ifndef __HDL_TIM_H
#define __HDL_TIM_H

#include "hdl_include.h"

typedef struct
{
	TIM_TypeDef* tim_x; //哪一个定时器，TIM1~Tim8
	uint32_t tim_rcc ; //Tim时钟
	uint8_t tim_rcc_type;//Tim RCC时钟类型
	uint16_t period ; //定时器  预设值
	uint16_t prescaler ;//定时器 预分频数
	uint8_t  tim_irq;   //定时器中断号
	uint8_t tim_irq_priority; //此中断的抢占优先级
}tim_t;

void hdl_init_tim(tim_t *t);

#endif