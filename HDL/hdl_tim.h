/*
*********************************************************************************************************
*
*	ģ������ : HDL_TIMģ��
*	�ļ����� : hdl_tim.h
*	˵    �� : TIM1~TIM8��ʱ���ж� Ӳ����ʼ��ģ��
* 
*          tim�ж� ʱ����㹫˾ period /(72MHZ/prescaler)
*********************************************************************************************************
*/
#ifndef __HDL_TIM_H
#define __HDL_TIM_H

#include "hdl_include.h"

typedef struct
{
	TIM_TypeDef* tim_x; //��һ����ʱ����TIM1~Tim8
	uint32_t tim_rcc ; //Timʱ��
	uint8_t tim_rcc_type;//Tim RCCʱ������
	uint16_t period ; //��ʱ��  Ԥ��ֵ
	uint16_t prescaler ;//��ʱ�� Ԥ��Ƶ��
	uint8_t  tim_irq;   //��ʱ���жϺ�
	uint8_t tim_irq_priority; //���жϵ���ռ���ȼ�
}tim_t;

void hdl_init_tim(tim_t *t);

#endif