/*
*********************************************************************************************************
*
*	模块名称 : HDL_ADC模块
*	文件名称 : hdl_adc.h
*	说    明 : stm32单片机中的adc采样进行封装的库，目前只是将ADC1进行封装，其数据的传送通过DMA方式来传输
*            
* 注：    根据STM32手册上，只有ADC1与ADC3有DMA功能，由ADC2转换的数据可以通过双ADC模式，利用ADC1的DMA通道传输
*
*         目前还没有项目没有接触到ADC2，先将ADC1功能进行封装一下
*
*********************************************************************************************************
*/
#ifndef  __HDL_ADC_H
#define  __HDL_ADC_H

#include "hdl_include.h"


typedef struct 
{
	
	ADC_TypeDef* adcx;  //取值为ADC1,ADC2,ADC3
	volatile uint16_t * adc_covert_value; //DMA方式数据转换存放缓冲
	uint8_t value_len ; //转换的数据长度  最大为16，只是ADC只支持16路的采样 1-16
	uint32_t adc_rcc ; //adc时钟 RCC_APB2Periph_ADC1
	//uint32_t dma_rcc ; //DMA时钟 RCC_AHBPeriph_DMA1
	uint8_t channel_len; //连续转换的ADC通道数目
	void(*adc_set_channel_time)(); //设置adc采样的通道，与采样周期的函数指针
	gpio_t * gpio; //进行adc采样时的 管脚
}adc_t;

void hdl_init_adc(adc_t * t);

#endif