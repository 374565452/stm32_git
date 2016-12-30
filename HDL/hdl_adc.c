/*
*********************************************************************************************************
*
*	ģ������ : HDL_ADCģ��
*	�ļ����� : hdl_adc.c
*	˵    �� : stm32��Ƭ���е�adc�������з�װ�Ŀ⣬Ŀǰֻ�ǽ�ADC1���з�װ�������ݵĴ���ͨ��DMA��ʽ������
*            
* ע��    ����STM32�ֲ��ϣ�ֻ��ADC1��ADC3��DMA���ܣ���ADC2ת�������ݿ���ͨ��˫ADCģʽ������ADC1��DMAͨ������
*
*         �ɼ���������ֱ�ӷ���adc_covert_value��
*
*
*         Ŀǰ��û����Ŀû�нӴ���ADC2���Ƚ�ADC1���ܽ��з�װһ��
*      �ر����ѣ��ڽ���ADC����ʱ��ʹ�õ�IO�ܽţ�Ҫ����Ϊģ������ GPIO_Mode_AIN
*      2016-12-2 �������ʱ����û�м���ADC�����ܽŵĶ��壬������Լ��ϣ�����ֱ�Ӽ���adc_t �ṹ��
								 channel_lenͨ���ĳ��ȣ�������˲ɼ��ܽŵĸ��������Կ��Խ��ܽ���������ͳһ����
			 2016-12-2�����ʱ�򣬼��ϹܽŶ�����ܽų�ʼ������
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    2016-12-2        ��ʽ����
*
*********************************************************************************************************
*/
#include "hdl_adc.h"

//����ADC1ʱ��Ҫһ�����ݴ洢��ַ������������ֵ
#define ADC1_DR_Address    ((u32)0x4001244C) //��ʵ�����ַ����ADC1->DR�ĵ�ַ

static void hdl_adc_var(adc_t * t);

static void hdl_dma_var(adc_t * t);

static void hdl_adc_rcc(adc_t * t);

static void hdl_adc_gpio(adc_t * t);

/*
*********************************************************************************************************
*	�� �� ��: hdl_init_adc
*	����˵��: adc����Ӳ����ʼ���������ڴ˺�����ִ��
*	��    ��: adc_t * t ��װadc�����Ľṹ�����
*	�� �� ֵ: ��
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
*	�� �� ��: hdl_adc_var
*	����˵��: adc������ʼ��
*	��    ��: adc_t * t ��װadc�����Ľṹ�����
*	�� �� ֵ: ��
* 
*********************************************************************************************************
*/
static void hdl_adc_var(adc_t * t)
{
	ADC_InitTypeDef ADC_InitStructure;
	ADC_DeInit(t->adcx); 
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; //ADC�Ĺ���ģʽΪ����ģʽ
  ADC_InitStructure.ADC_ScanConvMode = ENABLE; //ʹ��ɨ�� 
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; //ADCת������������ģʽ
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; //���������ת��
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; //ת�������Ҷ���
  ADC_InitStructure.ADC_NbrOfChannel = t->channel_len; //���й���ת����ADCͨ�� ����Ŀ 1-16
  ADC_Init(t->adcx, &ADC_InitStructure);

  // ADC1 regular channel11 configuration 
	//����ֱ�ӵ����ⲿ�ĺ���������ADC�ŵ���ѡ���Լ����ò���ʱ�䣬���òɼ�������ݷ���
	//���ĸ�DMA���ݴ洢��λ����
	if(t->adc_set_channel_time != NULL)
	{
		t->adc_set_channel_time();
	}
  //ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 1, ADC_SampleTime_239Cycles5);
  //ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 2, ADC_SampleTime_55Cycles5);
  // Enable ADC1 DMA 
  ADC_DMACmd(t->adcx, ENABLE); //ʹ��ADC1��DMA����
  //Enable ADC1 
  ADC_Cmd(t->adcx, ENABLE); //ʹ��ADC1
  // Enable ADC1 reset calibaration register    
  ADC_ResetCalibration(t->adcx); //����ADC1У׼�Ĵ���
  // Check the end of ADC1 reset calibration register 
  while(ADC_GetResetCalibrationStatus(t->adcx)); //�ȴ�ADC1У׼���
  // Start ADC1 calibaration 
  ADC_StartCalibration(t->adcx); //��ʼADCУ׼
  // Check the end of ADC1 calibration 
  while(ADC_GetCalibrationStatus(t->adcx)); //�ȴ�ADCУ׼���
  // Start ADC1 Software Conversion 
  ADC_SoftwareStartConvCmd(t->adcx, ENABLE); //ʹ��ADC1�������ת������
}
/*
*********************************************************************************************************
*	�� �� ��: hdl_dma_var
*	����˵��: adc�н������ݴ洢ʱ��ʹ��DMA����
*	��    ��: adc_t * t ��װadc�����Ľṹ�����
*	�� �� ֵ: ��
* 
*********************************************************************************************************
*/
static void hdl_dma_var(adc_t * t)
{
	DMA_InitTypeDef DMA_InitStructure;
	DMA_DeInit(DMA1_Channel1);
	//DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address; //����DMAͨ���������ַ ��������һ������ַ
	DMA_InitStructure.DMA_PeripheralBaseAddr =(u32)(&(t->adcx->DR));
  DMA_InitStructure.DMA_MemoryBaseAddr = (u32)(t->adc_covert_value); //DMAͨ���洢����ַ
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; //ָ������ΪԴ��ַ
  DMA_InitStructure.DMA_BufferSize = t->value_len; //DMA��������С 
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //�ⲿ�Ĵ�����ַ����
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //��ǰ�洢����ַ����
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //�������ݿ��Ϊ16λ
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; //����洢�����Ϊ16λ
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; //DMAͨ�� ����ģʽΪ���λ���ģʽ
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;  //DMAͨ�� ���ȼ���
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; //��ֹDMAͨ�� �洢�����洢������
  DMA_Init(DMA1_Channel1, &DMA_InitStructure); //��ʼ��DMAͨ��1
	DMA_Cmd(DMA1_Channel1, ENABLE); //ʹ��ͨ��1
}

/*
*********************************************************************************************************
*	�� �� ��: hdl_adc_rcc
*	����˵��: adcʱ������
*	��    ��: adc_t * t ��װadc�����Ľṹ�����
*	�� �� ֵ: ��
* 
*********************************************************************************************************
*/
static void hdl_adc_rcc(adc_t * t)
{
	//���Ҫ����DMA1��ʱ�ӣ�ֻ��ͨ������ĺ���������
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	//hdl_set_rcc(t->dma_rcc ,ENABLE);
	hdl_set_rcc(t->adc_rcc,ENABLE,2);
}

/*
*********************************************************************************************************
*	�� �� ��: hdl_adc_gpio
*	����˵��: adc����ʹ��ʱ�����в����ĹܽŶ���
*	��    ��: adc_t * t ��װadc�����Ľṹ�����
*	�� �� ֵ: ��
* 
*********************************************************************************************************
*/
static void hdl_adc_gpio(adc_t * t)
{
	uint8_t i =0;
	//��Ϊ������Ĳ����ܽŲ�������һ��������Ҫ������ķ�ʽ����ȡgpio
	for(i=0;i<t->channel_len ;i ++)
	{
		hdl_set_rcc(t->gpio[i].gpio_rcc ,ENABLE,2);
		hdl_input_gpio(t->gpio[i].GPIO_x,t->gpio[i].GPIO_Pin,GPIO_Mode_AIN);
	}
}
