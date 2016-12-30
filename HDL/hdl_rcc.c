/*
*********************************************************************************************************
*
*	ģ������ : HDL_RCCģ��
*	�ļ����� : hdl_rcc.c
* ��    �� ��v1.0
*	˵    �� : ��stm32ϵͳ��������RCCʱ�ӽ��м򵥷�װ�������ظ�ʹ��ϵͳ���еĺ���
*				    ����ϵͳ�������Ϊ�ϲ�Ӧ��ͳһ�ṩ�ӿ�
*
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    2016-11-28        ��ʽ����
*   V1.1    2016-12-7            �޸Ĵ˺���������һ��rccTypeѡ�У�������APB2����APB1����Ϊ����ֱ��ͨ��ֵ���Ƚϣ���
                                 ������ɴ����ڵ���USART3ʱ�����ִ���һֱ�������
*
*
*********************************************************************************************************
*/

#include "hdl_rcc.h"


/*
*********************************************************************************************************
*	�� �� ��: hdl_set_rcc
*	����˵��: ���ø�������ģ�飬���ܹܽ�rccʱ�ӣ�
*	��    ��: rcc��Ҫ���õ�ʱ�����ͣ��ο�stm32������ָʾ��ʱ������
						newState״̬���ͣ�ֵΪDISABLE��ENABLE
						rccTypeΪʱ������ѡ�� 1ΪAPB1  2ΪAPB2
*	�� �� ֵ: ��
* ע��UART4��UART5��ʱ�Ӻ�׺ΪUART4��UART5��������USART4��USART5
*********************************************************************************************************
*/
	void hdl_set_rcc(uint32_t rcc,FunctionalState newState ,uint8_t rccType)
	{
		if(rccType==1)
		{
			RCC_APB1PeriphClockCmd(rcc,newState);
		}
		else if(rccType==2)
		{
			RCC_APB2PeriphClockCmd(rcc,newState);
		}
//		if( rcc==RCC_APB2Periph_AFIO  || rcc==RCC_APB2Periph_GPIOA ||rcc==RCC_APB2Periph_GPIOB ||
//				rcc==RCC_APB2Periph_GPIOC ||rcc==RCC_APB2Periph_GPIOD ||rcc==RCC_APB2Periph_GPIOE ||
//				rcc==RCC_APB2Periph_GPIOF ||rcc==RCC_APB2Periph_GPIOG ||rcc==RCC_APB2Periph_ADC1 ||
//				rcc==RCC_APB2Periph_ADC2 ||rcc==RCC_APB2Periph_TIM1 ||rcc==RCC_APB2Periph_SPI1 ||
//				rcc==RCC_APB2Periph_TIM8 ||rcc==RCC_APB2Periph_USART1 ||rcc==RCC_APB2Periph_ADC3 ||
//				rcc==RCC_APB2Periph_TIM15  ||rcc==RCC_APB2Periph_TIM16 ||rcc==RCC_APB2Periph_TIM17 ||
//				rcc==RCC_APB2Periph_TIM9 ||rcc==RCC_APB2Periph_TIM10 ||rcc==RCC_APB2Periph_TIM11 )
//		{
//			RCC_APB2PeriphClockCmd(rcc,newState);
//		}
//		else if (rcc==RCC_APB1Periph_TIM2  || rcc==RCC_APB1Periph_TIM3 ||rcc==RCC_APB1Periph_TIM4 ||
//				rcc==RCC_APB1Periph_TIM5 ||rcc==RCC_APB1Periph_TIM6 ||rcc==RCC_APB1Periph_TIM7 ||
//				rcc==RCC_APB1Periph_WWDG ||rcc==RCC_APB1Periph_SPI2 ||rcc==RCC_APB1Periph_SPI3 ||
//				rcc==RCC_APB1Periph_USART2 ||rcc==RCC_APB1Periph_USART3 ||rcc==RCC_APB1Periph_UART4 ||
//				rcc==RCC_APB1Periph_UART5 ||rcc==RCC_APB1Periph_I2C1 ||rcc==RCC_APB1Periph_I2C2 ||
//				rcc==RCC_APB1Periph_USB  ||rcc==RCC_APB1Periph_CAN1 ||rcc==RCC_APB1Periph_BKP ||
//				rcc==RCC_APB1Periph_PWR ||rcc==RCC_APB1Periph_DAC ||rcc==RCC_APB1Periph_CEC ||
//				rcc==RCC_APB1Periph_TIM12 ||rcc==RCC_APB1Periph_TIM13 ||rcc==RCC_APB1Periph_TIM14)
//		{
//			RCC_APB1PeriphClockCmd(rcc,newState);
//		}
		
	}