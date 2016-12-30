/*
*********************************************************************************************************
*
*	模块名称 : HDL_GPIO模块
*	文件名称 : hdl_gpio.c
* 版    本 ：v1.0
*	说    明 : 对stm32系统库里设置gpio功能管脚进行简单封装。屏蔽应用层频繁调用库中函数
*
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2016-11-28        正式发布
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
*	函 数 名: hdl_output_gpio_
*	功能说明: 设置指定管脚为输出管脚
*	形    参: GPIOX为管脚a-g，port为0-15 ，mode管脚输出类型
*	返 回 值: 无
* 
*********************************************************************************************************
*/
void hdl_output_gpio(GPIO_TypeDef* gpio,uint16_t port,GPIOMode_TypeDef mode)
{
	hdl_set_gpio(gpio,port,mode);
}
/*
*********************************************************************************************************
*	函 数 名: hdl_set_rcc
*	功能说明: 设置指定管脚为输入管脚
*	形    参: GPIOX为管脚a-g，port为0-15 ，mode管脚输出类型
						
*	返 回 值: 无
*
*********************************************************************************************************
*/
void hdl_input_gpio(GPIO_TypeDef* gpio,uint16_t port,GPIOMode_TypeDef mode)
{
	hdl_set_gpio(gpio,port,mode);
}