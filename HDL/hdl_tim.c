/*
*********************************************************************************************************
*
*	ģ������ : HDL_TIMģ��
*	�ļ����� : hdl_tim.c
*	˵    �� : TIM1~TIM8��ʱ���ж� Ӳ����ʼ��ģ��
*
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    2016-12-9           ��ʽ����
*
*********************************************************************************************************
*/
#include "hdl_tim.h"

static void hdl_tim_rcc(tim_t *t);
static void hdl_tim_nvic(tim_t *t);
static void hdl_tim(tim_t * t);

/*
*********************************************************************************************************
*	�� �� ��: hdl_init_tim
*	����˵��: ��ʱ���жϳ�ʼ������
*	��    ��: tim_t * t ��װtim_t�����Ľṹ�����
*	�� �� ֵ: ��
* 
*********************************************************************************************************
*/
void hdl_init_tim(tim_t *t)
{
	hdl_tim_rcc(t);
	hdl_tim(t);
	hdl_tim_nvic(t);
}
/*
*********************************************************************************************************
*	�� �� ��: hdl_init_tim
*	����˵��: ��ʱ���жϳ�ʼ������ ʱ�ӳ�ʼ��
*	��    ��: tim_t * t ��װtim_t�����Ľṹ�����
*	�� �� ֵ: ��
* 
*********************************************************************************************************
*/
static void hdl_tim_rcc(tim_t *t)
{
	hdl_set_rcc(t->tim_rcc,ENABLE,t->tim_rcc_type);
}
/*
*********************************************************************************************************
*	�� �� ��: hdl_init_tim
*	����˵��: ��ʱ���жϳ�ʼ������ ��ʱ���ж��������
*	��    ��: tim_t * t ��װtim_t�����Ľṹ�����
*	�� �� ֵ: ��
* 
*********************************************************************************************************
*/
static void hdl_tim_nvic(tim_t *t)
{
	NVIC_InitTypeDef NVIC_InitStructure;    
      
  NVIC_InitStructure.NVIC_IRQChannel = t->tim_irq;    
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = t->tim_irq_priority;    
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;    
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;    
  NVIC_Init(&NVIC_InitStructure);    
}
/*
*********************************************************************************************************
*	�� �� ��: hdl_init_tim
*	����˵��: ��ʱ���жϳ�ʼ������  �ڲ�tim�ṹ��������
*	��    ��: tim_t * t ��װtim_t�����Ľṹ�����
*	�� �� ֵ: ��
* 
*********************************************************************************************************
*/
static void hdl_tim(tim_t * t)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_DeInit(t->tim_x); //��������TIMΪȱʡֵ
	//TIM_InternalClockConfig(TIM2);
	TIM_TimeBaseStructure.TIM_Prescaler = t->prescaler - 1; //����Tim��Ԥ��Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //���ü�����ģʽΪ���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period = t->period - 1; //����Tim��Ԥ����ֵ
	TIM_TimeBaseInit(t->tim_x,&TIM_TimeBaseStructure);
	
	TIM_ClearFlag(t->tim_x, TIM_FLAG_Update); //�����ʱ�����жϱ�ʶ
	//��ֵ ����˼ �ǣ��ⲿ�Ƿ�����޸��Ѿ����úõ�periodֵ��
	//������жϷ������У�����������periodֵ�ģ���Ҫ����ΪENABLE
	TIM_ARRPreloadConfig(t->tim_x, DISABLE); //�˴�����Ϊ��ֹ��������ARRԤװ�ػ�����
	TIM_ITConfig(t->tim_x,TIM_IT_Update,ENABLE); //ʹ���ж�
		/* TIMx enable counter */
	TIM_Cmd(t->tim_x, ENABLE);
}
