/*
*********************************************************************************************************
*
*	ģ������ : HDL_EXTIģ��
*	�ļ����� : hdl_exti.h
*	˵    �� : stm32���ⲿ�ж�ʹ�õĺ�����
*
* ע���˿���ֻ�ǽ����ⲿ�жϵĳ�ʼ����������û�н��ⲿ�жϴ������������
*
*           stm32�е�GPIO�ⲿ�жϣ�������Ϊһ����λ ��ͬ�����ⲿ�ж�ͬһʱ��ֻ����һ��
								EXTI0~EXTI4��5���ⲿ�ж����ŵ������ж���Ӧ����
*								EXTI5_9����һ���ж���Ӧ����		
*								EXTI10_15����һ���ж���Ӧ����
*********************************************************************************************************
*/
#ifndef __HDL_EXTI_H
#define __HDL_EXTI_H


#include "hdl_include.h"
typedef struct
{
	uint8_t port_source;  //�ж�Դ�����Ķ˿ڣ�����ȡֵΪGPIO_PortSourceGPIOA GPIO_PortSourceGPIOB GPIO_PortSourceGPIOC GPIO_PortSourceGPIOD
                        // GPIO_PortSourceGPIOE GPIO_PortSourceGPIOF  GPIO_PortSourceGPIOG
	uint8_t pin_source ; // �ж�Դ���� �Ķ˿ں� GPIO_PinSource0 ~ GPIO_PinSource15
	uint32_t exti_line ; //�����ⲿ�жϵ��ж���
	EXTITrigger_TypeDef  trigger_type; //�жϵĴ�����ʽ��EXTI_Trigger_Rising EXTI_Trigger_Falling EXTI_Trigger_Rising_Falling
	uint8_t exti_irq;   //�ⲿ�жϺ�
	uint8_t irq_priority; //���жϵ���ռ���ȼ�
	
	gpio_t * exti_gpio; //�ⲿ�жϵ�GPIO
	
}exti_t;


void hdl_init_exti(exti_t  * t);

#endif