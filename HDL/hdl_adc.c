/*
*********************************************************************************************************
*
*	模块名称 : HDL_ADC模块
*	文件名称 : hdl_adc.c
*	说    明 : stm32单片机中的adc采样进行封装的库，目前只是将ADC1进行封装，其数据的传送通过DMA方式来传输
*            
* 注：    根据STM32手册上，只有ADC1与ADC3有DMA功能，由ADC2转换的数据可以通过双ADC模式，利用ADC1的DMA通道传输
*
*         采集到的数据直接放在adc_covert_value中
*
*
*         目前还没有项目没有接触到ADC2，先将ADC1功能进行封装一下
*      特别提醒，在进行ADC功能时，使用的IO管脚，要设置为模拟输入 GPIO_Mode_AIN
*      2016-12-2 初步完成时，并没有加上ADC采样管脚的定义，后面可以加上，可以直接加在adc_t 结构上
								 channel_len通道的长度，则定义出此采集管脚的个数，所以可以将管脚配置起来统一管理
			 2016-12-2下午的时候，加上管脚定义与管脚初始化操作
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2016-12-2        正式发布
*
*********************************************************************************************************
*/
#include "hdl_adc.h"

//配置ADC1时需要一个数据存储地址，下面就是这个值
#define ADC1_DR_Address    ((u32)0x4001244C) //其实这个地址就是ADC1->DR的地址

static void hdl_adc_var(adc_t * t);

static void hdl_dma_var(adc_t * t);

static void hdl_adc_rcc(adc_t * t);

static void hdl_adc_gpio(adc_t * t);

/*
*********************************************************************************************************
*	函 数 名: hdl_init_adc
*	功能说明: adc功能硬件初始化操作，在此函数中执行
*	形    参: adc_t * t 封装adc参数的结构体变量
*	返 回 值: 无
* 
*********************************************************************************************************
*/
void hdl_init_adc(adc_t * t)
{
	hdl_adc_rcc(t);
	hdl_adc_gpio(t);
	hdl_dma_var(t);
	hdl_adc_var(t);
}
/*
*********************************************************************************************************
*	函 数 名: hdl_adc_var
*	功能说明: adc参数初始化
*	形    参: adc_t * t 封装adc参数的结构体变量
*	返 回 值: 无
* 
*********************************************************************************************************
*/
static void hdl_adc_var(adc_t * t)
{
	ADC_InitTypeDef ADC_InitStructure;
	ADC_DeInit(t->adcx); 
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; //ADC的工作模式为独立模式
  ADC_InitStructure.ADC_ScanConvMode = ENABLE; //使能扫描 
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; //ADC转换工作在连续模式
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; //有软件控制转换
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; //转换数据右对齐
  ADC_InitStructure.ADC_NbrOfChannel = t->channel_len; //进行规则转换的ADC通道 的数目 1-16
  ADC_Init(t->adcx, &ADC_InitStructure);

  // ADC1 regular channel11 configuration 
	//这里直接调用外部的函数来进行ADC信道的选择，以及设置采样时间，配置采集后的数据放置
	//在哪个DMA数据存储器位置上
	if(t->adc_set_channel_time != NULL)
	{
		t->adc_set_channel_time();
	}
  //ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 1, ADC_SampleTime_239Cycles5);
  //ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 2, ADC_SampleTime_55Cycles5);
  // Enable ADC1 DMA 
  ADC_DMACmd(t->adcx, ENABLE); //使能ADC1的DMA功能
  //Enable ADC1 
  ADC_Cmd(t->adcx, ENABLE); //使能ADC1
  // Enable ADC1 reset calibaration register    
  ADC_ResetCalibration(t->adcx); //重置ADC1校准寄存器
  // Check the end of ADC1 reset calibration register 
  while(ADC_GetResetCalibrationStatus(t->adcx)); //等待ADC1校准完成
  // Start ADC1 calibaration 
  ADC_StartCalibration(t->adcx); //开始ADC校准
  // Check the end of ADC1 calibration 
  while(ADC_GetCalibrationStatus(t->adcx)); //等待ADC校准完成
  // Start ADC1 Software Conversion 
  ADC_SoftwareStartConvCmd(t->adcx, ENABLE); //使能ADC1软件进行转换工作
}
/*
*********************************************************************************************************
*	函 数 名: hdl_dma_var
*	功能说明: adc中进行数据存储时，使用DMA配置
*	形    参: adc_t * t 封装adc参数的结构体变量
*	返 回 值: 无
* 
*********************************************************************************************************
*/
static void hdl_dma_var(adc_t * t)
{
	DMA_InitTypeDef DMA_InitStructure;
	DMA_DeInit(DMA1_Channel1);
	//DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address; //定义DMA通道外设基地址 可以设置一个基地址
	DMA_InitStructure.DMA_PeripheralBaseAddr =(u32)(&(t->adcx->DR));
  DMA_InitStructure.DMA_MemoryBaseAddr = (u32)(t->adc_covert_value); //DMA通道存储器地址
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; //指定外设为源地址
  DMA_InitStructure.DMA_BufferSize = t->value_len; //DMA缓冲区大小 
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //外部寄存器地址不变
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //当前存储器地址不变
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //外设数据宽度为16位
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; //外设存储器宽度为16位
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; //DMA通道 操作模式为环形缓冲模式
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;  //DMA通道 优先级高
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; //禁止DMA通道 存储器到存储器传输
  DMA_Init(DMA1_Channel1, &DMA_InitStructure); //初始化DMA通道1
	DMA_Cmd(DMA1_Channel1, ENABLE); //使能通道1
}

/*
*********************************************************************************************************
*	函 数 名: hdl_adc_rcc
*	功能说明: adc时钟配置
*	形    参: adc_t * t 封装adc参数的结构体变量
*	返 回 值: 无
* 
*********************************************************************************************************
*/
static void hdl_adc_rcc(adc_t * t)
{
	//如果要设置DMA1的时钟，只能通过下面的函数来设置
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	//hdl_set_rcc(t->dma_rcc ,ENABLE);
	hdl_set_rcc(t->adc_rcc,ENABLE,2);
}

/*
*********************************************************************************************************
*	函 数 名: hdl_adc_gpio
*	功能说明: adc功能使用时，进行采样的管脚定义
*	形    参: adc_t * t 封装adc参数的结构体变量
*	返 回 值: 无
* 
*********************************************************************************************************
*/
static void hdl_adc_gpio(adc_t * t)
{
	uint8_t i =0;
	//因为这里面的采样管脚不可能是一个，所以要用数组的方式来获取gpio
	for(i=0;i<t->channel_len ;i ++)
	{
		hdl_set_rcc(t->gpio[i].gpio_rcc ,ENABLE,2);
		hdl_input_gpio(t->gpio[i].GPIO_x,t->gpio[i].GPIO_Pin,GPIO_Mode_AIN);
	}
}
