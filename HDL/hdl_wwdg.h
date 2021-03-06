/*
*********************************************************************************************************
*
*	模块名称 : HDL_WWDG模块
*	文件名称 : hdl_wwdg.h
*	说    明 :stm32窗口看门狗驱动程序
*
*           窗口看门狗有上窗口与下窗口，下窗口固定值为0x40 上窗口最大值为0x7F 
						所有窗口值应该设置在0x40~0x7F之间
*           窗口看门狗溢出时间计算公式
						分频值  (PCLK1 (48M)/4096)/8=1464HZ ~683us  （此处按照8分频来计算的）
						计算器范围设置为0x40~0x7F
						计数个数为0x7F-0x40=63
						最大喂狗时间为683us*63 = ~430ms
*********************************************************************************************************
*/
#ifndef __HDL_WWDG_H
#define __HDL_WWDG_H

#include "hdl_include.h"

void hdl_init_wwdg(uint8_t tr,uint8_t wr);


#endif