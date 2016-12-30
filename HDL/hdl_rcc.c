/*
*********************************************************************************************************
*
*	模块名称 : HDL_RCC模块
*	文件名称 : hdl_rcc.c
* 版    本 ：v1.0
*	说    明 : 对stm32系统库里设置RCC时钟进行简单封装，避免重复使用系统库中的函数
*				    屏蔽系统库操作，为上层应用统一提供接口
*
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2016-11-28        正式发布
*   V1.1    2016-12-7            修改此函数，增加一个rccType选中，表明是APB2还是APB1，因为不能直接通过值来比较，这
                                 样会造成错误，在调试USART3时，出现错误一直输出乱码
*
*
*********************************************************************************************************
*/

#include "hdl_rcc.h"


/*
*********************************************************************************************************
*	函 数 名: hdl_set_rcc
*	功能说明: 设置各个功能模块，或功能管脚rcc时钟，
*	形    参: rcc需要设置的时钟类型，参考stm32库中所指示的时钟类型
						newState状态类型，值为DISABLE、ENABLE
						rccType为时钟类型选择 1为APB1  2为APB2
*	返 回 值: 无
* 注：UART4与UART5的时钟后缀为UART4与UART5，并不是USART4、USART5
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