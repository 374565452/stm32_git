/*
*********************************************************************************************************
*
*	ģ������ : HDL_GPIOģ��
*	�ļ����� : hdl_gpio.c
* ��    �� ��v1.0
*	˵    �� : ��stm32ϵͳ��������gpio���ܹܽŽ��м򵥷�װ������Ӧ�ò�Ƶ�����ÿ��к���
*
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    2016-11-28        ��ʽ����
*
*
*********************************************************************************************************
*/

#include "hdl_gpio.h"

static void hdl_set_gpio(GPIO_TypeDef* GPIOx,uint16_t port,GPIOMode_TypeDef mode)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE , ENABLE);
	
	GPIO_InitStructure.GPIO_Pin	= port;        
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 	 
  GPIO_InitStructure.GPIO_Mode = mode;	 
  GPIO_Init(GPIOx, &GPIO_InitStructure); 
}

/*
*********************************************************************************************************
*	�� �� ��: hdl_output_gpio_
*	����˵��: ����ָ���ܽ�Ϊ����ܽ�
*	��    ��: GPIOXΪ�ܽ�a-g��portΪ0-15 ��mode�ܽ��������
*	�� �� ֵ: ��
* 
*********************************************************************************************************
*/
void hdl_output_gpio(GPIO_TypeDef* gpio,uint16_t port,GPIOMode_TypeDef mode)
{
	hdl_set_gpio(gpio,port,mode);
}
/*
*********************************************************************************************************
*	�� �� ��: hdl_set_rcc
*	����˵��: ����ָ���ܽ�Ϊ����ܽ�
*	��    ��: GPIOXΪ�ܽ�a-g��portΪ0-15 ��mode�ܽ��������
						
*	�� �� ֵ: ��
*
*********************************************************************************************************
*/
void hdl_input_gpio(GPIO_TypeDef* gpio,uint16_t port,GPIOMode_TypeDef mode)
{
	hdl_set_gpio(gpio,port,mode);
}