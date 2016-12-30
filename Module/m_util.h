/*
*********************************************************************************************************
*
*	模块名称 : M_UTIL模块
*	文件名称 : m_util.h
*	说    明 : 常用的工具类函数定义在此文件中进行统一处理
*
*
*********************************************************************************************************
*/
#ifndef __M_UTIL_H
#define __M_UTIL_H

#include "hdl_include.h"

#define  M_PRINTF_EN  1 //开启printf打印功能

#if M_PRINTF_EN ==1
   #define DEBUG_UART USART3
#endif

//计算校验和（无符号校验和）
uint8_t m_check_unsigned_sum(uint8_t * buffer, uint32_t len);

//stm32中软件复位工具函数
void m_system_reset();

#endif