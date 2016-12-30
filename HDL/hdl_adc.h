/*
*********************************************************************************************************
*
*	ģ������ : HDL_ADCģ��
*	�ļ����� : hdl_adc.h
*	˵    �� : stm32��Ƭ���е�adc�������з�װ�Ŀ⣬Ŀǰֻ�ǽ�ADC1���з�װ�������ݵĴ���ͨ��DMA��ʽ������
*            
* ע��    ����STM32�ֲ��ϣ�ֻ��ADC1��ADC3��DMA���ܣ���ADC2ת�������ݿ���ͨ��˫ADCģʽ������ADC1��DMAͨ������
*
*         Ŀǰ��û����Ŀû�нӴ���ADC2���Ƚ�ADC1���ܽ��з�װһ��
*
*********************************************************************************************************
*/
#ifndef  __HDL_ADC_H
#define  __HDL_ADC_H

#include "hdl_include.h"


typedef struct 
{
	
	ADC_TypeDef* adcx;  //ȡֵΪADC1,ADC2,ADC3
	volatile uint16_t * adc_covert_value; //DMA��ʽ����ת����Ż���
	uint8_t value_len ; //ת�������ݳ���  ���Ϊ16��ֻ��ADCֻ֧��16·�Ĳ��� 1-16
	uint32_t adc_rcc ; //adcʱ�� RCC_APB2Periph_ADC1
	//uint32_t dma_rcc ; //DMAʱ�� RCC_AHBPeriph_DMA1
	uint8_t channel_len; //����ת����ADCͨ����Ŀ
	void(*adc_set_channel_time)(); //����adc������ͨ������������ڵĺ���ָ��
	gpio_t * gpio; //����adc����ʱ�� �ܽ�
}adc_t;

void hdl_init_adc(adc_t * t);

#endif