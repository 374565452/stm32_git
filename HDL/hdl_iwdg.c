/*
*********************************************************************************************************
*
*	模块名称 : HDL_IWDG模块
*	文件名称 : hdl_iwdg.c
* 版    本 ：v1.0
*	说    明 : stm32独立看门狗驱动程序
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2016-11-29        正式发布
*
* 注：
	 使能LSI  其中LSI为内部低速晶振
	RCC_LSICmd(ENABLE);
	
	等待直到LSI就绪 
	while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
	{}
*********************************************************************************************************
*/
#include "hdl_iwdg.h"

/****************************************************************************
* 名    称：hdl_init_iwdg
* 功    能：初始化独立看门狗，使能独立看门狗
* 入口参数：rlr看门狗重装载值
* 出口参数：
* 说    明：
* 调用方法：     
****************************************************************************/ 
void hdl_init_iwdg(uint16_t rlr)
{
	/* 检测系统是否从独立看门狗复位中恢复 */
	if (RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET)
	{		
		/* 清除复位标志 */
		RCC_ClearFlag();
	}
	/* 写入0x5555表示允许访问IWDG_PR 和IWDG_RLR寄存器 */
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);

  /* IWDG counter clock: 40KHz(LSI) / 32 = 1.25 KHz */
		/*  LSI/32 分频*/
	/*
	 *采用32分频后，计算公式为32*RLR/40
	 *例 rlr=40000/32 则看门狗溢出时间大致为32*40000/32/40 =1S
	*/
  IWDG_SetPrescaler(IWDG_Prescaler_32);

  /* Set counter reload value to 349 */
  IWDG_SetReload(rlr);

  /* Reload IWDG counter */
	/* 重载IWDG计数 */
  IWDG_ReloadCounter();

  /* Enable IWDG (the LSI oscillator will be enabled by hardware) */
	/* 使能 IWDG (LSI oscillator 由硬件使能) */
  IWDG_Enable();
	
}
/*
*********************************************************************************************************
*	函 数 名: hdl_feed_dog
*	功能说明: 喂狗函数
*	形    参：无
*	返 回 值: 无		        
*********************************************************************************************************
*/
void hdl_feed_dog()
{
	IWDG_ReloadCounter();
}