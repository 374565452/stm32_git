/*
*********************************************************************************************************
*
*	模块名称 : HDL_IWDG模块
*	文件名称 : hdl_iwdg.h
*	说    明 :stm32独立看门狗驱动程序
*
*           独立看门狗溢出时间运算公式
						Tout=((4*2^prer)*rlr)/40; 
						其中prer为预分频值，取值范围为0~7。例：IWDG_Prescaler_32采用32分频，其值应为3。4*2^3=32
						rlr为看门狗重装载值
*********************************************************************************************************
*/
#ifndef __HDL_IWDG_H
#define __HDL_IWDG_H
#include "hdl_include.h"

void hdl_init_iwdg(uint16_t rlr);

void hdl_feed_dog();

#endif