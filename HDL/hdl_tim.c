/*
*********************************************************************************************************
*
*	模块名称 : HDL_TIM模块
*	文件名称 : hdl_tim.c
*	说    明 : TIM1~TIM8定时器中断 硬件初始化模块
*
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2016-12-9           正式发布
*
*********************************************************************************************************
*/
#include "hdl_tim.h"

static void hdl_tim_rcc(tim_t *t);
static void hdl_tim_nvic(tim_t *t);
static void hdl_tim(tim_t * t);

/*
*********************************************************************************************************
*	函 数 名: hdl_init_tim
*	功能说明: 定时器中断初始化操作
*	形    参: tim_t * t 封装tim_t参数的结构体变量
*	返 回 值: 无
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
*	函 数 名: hdl_init_tim
*	功能说明: 定时器中断初始化操作 时钟初始化
*	形    参: tim_t * t 封装tim_t参数的结构体变量
*	返 回 值: 无
* 
*********************************************************************************************************
*/
static void hdl_tim_rcc(tim_t *t)
{
	hdl_set_rcc(t->tim_rcc,ENABLE,t->tim_rcc_type);
}
/*
*********************************************************************************************************
*	函 数 名: hdl_init_tim
*	功能说明: 定时器中断初始化操作 定时器中断向量设计
*	形    参: tim_t * t 封装tim_t参数的结构体变量
*	返 回 值: 无
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
*	函 数 名: hdl_init_tim
*	功能说明: 定时器中断初始化操作  内部tim结构体变量设计
*	形    参: tim_t * t 封装tim_t参数的结构体变量
*	返 回 值: 无
* 
*********************************************************************************************************
*/
static void hdl_tim(tim_t * t)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_DeInit(t->tim_x); //重新设置TIM为缺省值
	//TIM_InternalClockConfig(TIM2);
	TIM_TimeBaseStructure.TIM_Prescaler = t->prescaler - 1; //设置Tim的预分频
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //设置计数器模式为向上计数模式
	TIM_TimeBaseStructure.TIM_Period = t->period - 1; //设置Tim的预加载值
	TIM_TimeBaseInit(t->tim_x,&TIM_TimeBaseStructure);
	
	TIM_ClearFlag(t->tim_x, TIM_FLAG_Update); //清除定时器的中断标识
	//此值 的意思 是，外部是否可以修改已经设置好的period值，
	//如果在中断服务函数中，想重新设置period值的，则要设置为ENABLE
	TIM_ARRPreloadConfig(t->tim_x, DISABLE); //此处设置为禁止，不允许ARR预装载缓冲区
	TIM_ITConfig(t->tim_x,TIM_IT_Update,ENABLE); //使能中断
		/* TIMx enable counter */
	TIM_Cmd(t->tim_x, ENABLE);
}
