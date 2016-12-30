/*
*********************************************************************************************************
*
*	ģ������ : HDL_EXTIģ��
*	�ļ����� : hdl_exti.c
*	˵    �� : stm32���ⲿ�ж�ʹ�õĺ�����
*
* ע���˿���ֻ�ǽ����ⲿ�жϵĳ�ʼ����������û�н��ⲿ�жϴ������������
*
*           stm32�е�GPIO�ⲿ�жϣ�������Ϊһ����λ ��ͬ�����ⲿ�ж�ͬһʱ��ֻ����һ��
								EXTI0~EXTI4��5���ⲿ�ж����ŵ������ж���Ӧ����
*								EXTI5_9����һ���ж���Ӧ����		
*								EXTI10_15����һ���ж���Ӧ����
*
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    2016-12-5           ��ʽ����
*
*********************************************************************************************************
*/
#include "hdl_exti.h"


static void hdl_exti_nvic(exti_t * t);
static void hdl_exti(exti_t *t);
static void hdl_exti_gpio(exti_t *t);

/*
*********************************************************************************************************
*	�� �� ��: hdl_init_exti
*	����˵��: �ⲿ�жϵ���Ӧ��ʼ������
*	��    ��: exti * t ��װexti�����Ľṹ�����
*	�� �� ֵ: ��
* 
*********************************************************************************************************
*/
void hdl_init_exti(exti_t  * t)
{
	hdl_set_rcc(RCC_APB2Periph_AFIO,ENABLE ,2); //Ҫ����RCC_APB2Periph_AFIO ���RCCΪENABLE
	hdl_exti_gpio(t);
	hdl_exti(t);
	hdl_exti_nvic(t);
}
/*
*********************************************************************************************************
*	�� �� ��: hdl_init_exti
*	����˵��: �ⲿ�жϵ���Ӧ��ʼ������  ,�ⲿ�жϵ���Ӧ��ʼ������
*	��    ��: exti * t ��װexti�����Ľṹ�����
*	�� �� ֵ: ��
* 
*********************************************************************************************************
*/
static void hdl_exti_nvic(exti_t * t)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //������ⲿͳһ���ã�������FreeRTOSһ����ͳһΪ   NVIC_PriorityGroup_4
	//NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;  
	NVIC_InitStructure.NVIC_IRQChannel = t->exti_irq;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = t->irq_priority; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;        
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                                   
  NVIC_Init(&NVIC_InitStructure);
}

/*
*********************************************************************************************************
*	�� �� ��: hdl_init_exti
*	����˵��: �ⲿ�жϵ���Ӧ��ʼ������  exti�ⲿ�ж���Ӧ��������
*	��    ��: exti * t ��װexti�����Ľṹ�����
*	�� �� ֵ: ��
* 
*********************************************************************************************************
*/
static void hdl_exti(exti_t *t)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	GPIO_EXTILineConfig(t->port_source , t->pin_source);
	EXTI_InitStructure.EXTI_Line = t->exti_line; 
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	//EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_Trigger = t->trigger_type;
	EXTI_InitStructure.EXTI_LineCmd	= ENABLE;
	EXTI_Init(&EXTI_InitStructure);
}
/*
*********************************************************************************************************
*	�� �� ��: hdl_init_exti
*	����˵��: �ⲿ�жϵ���Ӧ��ʼ������
*	��    ��: exti * t ��װexti�����Ľṹ�����   exti�ⲿ��gpio��������
*	�� �� ֵ: ��
* 
* ע�� �˴���gpio��������Ϊ��������ģʽ�����Ը����Լ���ҵ���������趨��ģʽ
*********************************************************************************************************
*/
static void hdl_exti_gpio(exti_t *t)
{
	hdl_set_rcc(t->exti_gpio->gpio_rcc,ENABLE,2);
	hdl_input_gpio(t->exti_gpio->GPIO_x,t->exti_gpio->GPIO_Pin,GPIO_Mode_IPU);
}