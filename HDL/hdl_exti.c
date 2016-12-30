/*
*********************************************************************************************************
*
*	模块名称 : HDL_EXTI模块
*	文件名称 : hdl_exti.c
*	说    明 : stm32中外部中断使用的函数库
*
* 注：此库中只是进行外部中断的初始化工作，并没有将外部中断处理函数加入进来
*
*           stm32中的GPIO外部中断，是以组为一个单位 ，同组间的外部中断同一时间只能有一个
								EXTI0~EXTI4这5个外部中断有着单独的中断响应函数
*								EXTI5_9共用一个中断响应函数		
*								EXTI10_15共用一个中断响应函数
*
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2016-12-5           正式发布
*
*********************************************************************************************************
*/
#include "hdl_exti.h"


static void hdl_exti_nvic(exti_t * t);
static void hdl_exti(exti_t *t);
static void hdl_exti_gpio(exti_t *t);

/*
*********************************************************************************************************
*	函 数 名: hdl_init_exti
*	功能说明: 外部中断的相应初始化操作
*	形    参: exti * t 封装exti参数的结构体变量
*	返 回 值: 无
* 
*********************************************************************************************************
*/
void hdl_init_exti(exti_t  * t)
{
	hdl_set_rcc(RCC_APB2Periph_AFIO,ENABLE ,2); //要设置RCC_APB2Periph_AFIO 这个RCC为ENABLE
	hdl_exti_gpio(t);
	hdl_exti(t);
	hdl_exti_nvic(t);
}
/*
*********************************************************************************************************
*	函 数 名: hdl_init_exti
*	功能说明: 外部中断的相应初始化操作  ,外部中断的相应初始化操作
*	形    参: exti * t 封装exti参数的结构体变量
*	返 回 值: 无
* 
*********************************************************************************************************
*/
static void hdl_exti_nvic(exti_t * t)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //这个由外部统一配置，类似于FreeRTOS一样，统一为   NVIC_PriorityGroup_4
	//NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;  
	NVIC_InitStructure.NVIC_IRQChannel = t->exti_irq;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = t->irq_priority; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;        
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                                   
  NVIC_Init(&NVIC_InitStructure);
}

/*
*********************************************************************************************************
*	函 数 名: hdl_init_exti
*	功能说明: 外部中断的相应初始化操作  exti外部中断相应功能配置
*	形    参: exti * t 封装exti参数的结构体变量
*	返 回 值: 无
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
*	函 数 名: hdl_init_exti
*	功能说明: 外部中断的相应初始化操作
*	形    参: exti * t 封装exti参数的结构体变量   exti外部的gpio功能配置
*	返 回 值: 无
* 
* 注： 此处的gpio功能设置为上拉输入模式，可以根据自己的业务需求来设定此模式
*********************************************************************************************************
*/
static void hdl_exti_gpio(exti_t *t)
{
	hdl_set_rcc(t->exti_gpio->gpio_rcc,ENABLE,2);
	hdl_input_gpio(t->exti_gpio->GPIO_x,t->exti_gpio->GPIO_Pin,GPIO_Mode_IPU);
}