/*
*********************************************************************************************************
*
*	模块名称 : HDL_WWDG模块
*	文件名称 : hdl_wwdg.c
*	说    明 :stm32窗口看门狗驱动程序
*
*           窗口看门狗有上窗口与下窗口，下窗口固定值为0x40 上窗口最大值为0x7F 
						所有窗口值应该设置在0x40~0x7F之间
*           窗口看门狗溢出时间计算公式
						分频值  (PCLK1 (48M)/4096)/8=1464HZ ~683us  （此处按照8分频来计算的）
						计算器范围设置为0x40~0x7F
						计数个数为0x7F-0x40=63
						最大喂狗时间为683us*63 = ~430ms
						
						
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2016-11-28        正式发布
*
*********************************************************************************************************
*/
#include "hdl_wwdg.h"

static void init_wwdg_nvic();

/*
*********************************************************************************************************
*	函 数 名: hdl_init_wwdg
*	功能说明: 对stm32中WWDG窗口看门狗进行初始化操作，使能窗口看门狗，以及使能窗口看门狗中断服务
*	形    参: uint8_t tr 指窗口看门狗 计算值
						uint8_t wr 指窗口看门狗 上窗口值
						可以设置tr为0x7F wr为0x50
*	返 回 值: 无
* 注：      这里的分频值默认为8分频，可以根据实际项目需要另行设定

						一定要先设定WWDG_Enable(tr)后，才能清除中断标志
						
*********************************************************************************************************
*/
void hdl_init_wwdg(uint8_t tr,uint8_t wr)
{
	/* 检测系统是否从窗口看门狗复位中恢复 */
	if (RCC_GetFlagStatus(RCC_FLAG_WWDGRST) != RESET)
	{ 	
		/* 清除复位标志 */
		RCC_ClearFlag();
	}
	/* 使能WWDG时钟 */
	//RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);
	hdl_set_rcc(RCC_APB1Periph_WWDG,ENABLE,1);
	
	/* 
	   窗口看门狗分频设置：
	   比如选择WWDG_Prescaler_8
	   (PCLK1 (48MHz)/4096)/8 = 1464 Hz (~683 us)  
	*/
	WWDG_SetPrescaler(WWDG_Prescaler_8);
	
	/* 
	 设置窗口值是wr，用户必须在小于wr且大于0x40时刷新计数
	 器，要不会造成系统复位。
    */
	WWDG_SetWindowValue(wr);
	
	/* 
	 使能WWDG，设置计数器
	 比如设置tr=127 8分频时，那么溢出时间就是= ~683 us * 64 
	 如果wr值为0x50
	 窗口看门狗的刷新时间段是: ~683 * (127-80)  < 刷新窗口看门狗 < ~683 * 64 
	*/
	WWDG_Enable(tr);
	/* 清除EWI中断标志 */
	WWDG_ClearFlag();	

	/* 使能EW中断 */
	WWDG_EnableIT();
	init_wwdg_nvic();
}
/*
*********************************************************************************************************
*	函 数 名: init_wwdg_nvic
*	功能说明: 对stm32中WWDG中断初始化并使能
*	形    参: 
*	返 回 值: 无
* 注：     

*********************************************************************************************************
*/
static void init_wwdg_nvic()
{
	NVIC_InitTypeDef NVIC_InitStructure;  /* 设置 WWDG 的NVIC */
	NVIC_InitStructure.NVIC_IRQChannel = WWDG_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
}

/*
*********************************************************************************************************
*	函 数 名: WWDG_IRQHandler
*	功能说明: 窗口看门狗调用中断服务程序。
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void WWDG_IRQHandler(void)
{
	/*特别注意，下面两句的执行顺序不能反了，反了就出问题了*/
	/* 清除EWI标志 */
	if (WWDG_GetFlagStatus != RESET)
	{ 	
		/* 清除复位标志 */
		WWDG_ClearFlag();
		
		/* 更新窗口看门狗计数器 */
		WWDG_SetCounter(127);
	}
}