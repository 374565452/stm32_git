/*
*********************************************************************************************************
*
*	ģ������ : M_UTILģ��
*	�ļ����� : m_util.h
*	˵    �� : ���õĹ����ຯ�������ڴ��ļ��н���ͳһ����
*
*
*********************************************************************************************************
*/
#ifndef __M_UTIL_H
#define __M_UTIL_H

#include "hdl_include.h"

#define  M_PRINTF_EN  1 //����printf��ӡ����

#if M_PRINTF_EN ==1
   #define DEBUG_UART USART3
#endif

//����У��ͣ��޷���У��ͣ�
uint8_t m_check_unsigned_sum(uint8_t * buffer, uint32_t len);

//stm32�������λ���ߺ���
void m_system_reset();

#endif