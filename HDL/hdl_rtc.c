/*
*********************************************************************************************************
*
*	模块名称 : HDL_RTC模块
*	文件名称 : hdl_rtc.c
* 版    本 ：v1.0
*	说    明 : 设置stm32中的实时实钟功能
*
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2016-12-9        正式发布
*
*
*********************************************************************************************************
*/
#include "hdl_rtc.h"

static void hdl_rtc_rcc();
static void hdl_rtc_nvic();
static void hdl_rtc();

static uint8_t is_leap_year(uint16_t year);

static void time_set(void);

date_t date_timer;

//判断是否是闰年函数
//月份   1  2  3  4  5  6  7  8  9  10 11 12
//闰年   31 29 31 30 31 30 31 31 30 31 30 31
//非闰年 31 28 31 30 31 30 31 31 30 31 30 31
//输入:年份
//输出:该年份是不是闰年.1,是.0,不是
static uint8_t is_leap_year(uint16_t year)
{			  
	if(year%4==0) //必须能被4整除
	{ 
		if(year%100==0) 
		{ 
			if(year%400==0)
			{
				return 1;//如果以00结尾,还要能被400整除 	   
			
			}
			else 
			{
				return 0;
			}
		}else return 1;   
	}else return 0;	
}
/*
*********************************************************************************************************
*	函 数 名: hdl_init_rtc
*	功能说明: stm32中的RTC硬件初始化
*	形    参: 无
*	返 回 值: 无
* 
*********************************************************************************************************
*/
void hdl_init_rtc()
{
	hdl_rtc_rcc();
	hdl_rtc_nvic();
	hdl_rtc();
}

/*
*********************************************************************************************************
*	函 数 名: hdl_rtc_rcc 
*	功能说明: stm32中的RTC硬件初始化 rcc时钟初始化
*	形    参: 无
*	返 回 值: 无
* 
*********************************************************************************************************
*/
static void hdl_rtc_rcc()
{
	//RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
																	//使能PWR和BKP外设时钟   	
  hdl_set_rcc(RCC_APB1Periph_PWR,ENABLE,1);
	hdl_set_rcc(RCC_APB1Periph_BKP,ENABLE,1);
}

/*
*********************************************************************************************************
*	函 数 名: hdl_rtc_rcc 
*	功能说明: stm32中的RTC硬件初始化 RTC全局中断
*	形    参: 无
*	返 回 值: 无
* 
*********************************************************************************************************
*/
static void hdl_rtc_nvic()
{
		
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;		//RTC全局中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//先占优先级1位,从优先级3位
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	//先占优先级0位,从优先级4位
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		//使能该通道中断
	NVIC_Init(&NVIC_InitStructure);		//根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
}
/*
*********************************************************************************************************
*	函 数 名: hdl_rtc_rcc 
*	功能说明: stm32中的RTC硬件初始化 RTC寄存器设置问题，包括检测是否为第一次进行配置RTC
*	形    参: 无
*	返 回 值: 无
* 
*********************************************************************************************************
*/
static void hdl_rtc()
{
	PWR_BackupAccessCmd(ENABLE);									//使能RTC和后备寄存器访问 
	
	if(BKP_ReadBackupRegister(BKP_DR1)!=0x5555)						//从指定的后备寄存器中读出数据，判断是否为第一次配置
	{	
      	//printf("时钟配置。。。\r\n");																
		BKP_DeInit();												//将外设BKP的全部寄存器重设为缺省值 	
		RCC_LSEConfig(RCC_LSE_ON);									//使能外部低速时钟 32.768KHz
		while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)			//检查指定的RCC标志位设置与否,等待低速晶振就绪
  		{}
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);						//设置RTC时钟(RTCCLK),选择LSE作为RTC时钟    
		RCC_RTCCLKCmd(ENABLE);										//使能RTC时钟  
		RTC_WaitForSynchro();										//等待RTC寄存器(RTC_CNT,RTC_ALR和RTC_PRL)与RTC APB时钟同步
		RTC_WaitForLastTask();										//等待最近一次对RTC寄存器的写操作完成
		RTC_ITConfig(RTC_IT_SEC, ENABLE);							//使能RTC秒中断
		RTC_WaitForLastTask();										//等待最近一次对RTC寄存器的写操作完成
		RTC_SetPrescaler(32767); 									//设置RTC预分频的值  RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1)
		RTC_WaitForLastTask();										//等待最近一次对RTC寄存器的写操作完成
		//在第一次配置RTC时间的时候 ，应该由外部来触发设置时间
		time_set();													//时间设置	
    
		BKP_WriteBackupRegister(BKP_DR1, 0x5555);					//向指定的后备寄存器中写入用户程序数据0X5555做判断标志										
	}																 	
	else															//不是第一次配置 继续计时
	{
		if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)			//检查指定的RCC标志位设置与否:POR/PDR复位
		{
      		//printf("上电复位。。。\r\n");
		}											
		else if(RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)		//检查指定的RCC标志位设置与否:管脚复位
		{
      		//printf("外部复位。。。\r\n");
		}
    	//printf("不需要配置。。。\r\n");
		
		RTC_WaitForSynchro();										//等待最近一次对RTC寄存器的写操作完成
		
		RTC_ITConfig(RTC_IT_SEC, ENABLE);							//使能RTC秒中断

		RTC_WaitForLastTask();										//等待最近一次对RTC寄存器的写操作完成
	}		    				     
	
	time_get();														//更新时间 
	
	RCC_ClearFlag();												//清除RCC的复位标志位
}


/*****************************************************************************
** 函数名称: RTC_IRQHandler
** 功能描述: RTC中断服务函数
				每秒触发一次  
*	形    参: 无
*	返 回 值: 无
*****************************************************************************/
void RTC_IRQHandler(void)
{							    
	if(RTC_GetITStatus(RTC_IT_SEC))			//秒钟中断
	{							
		//time_get();							//更新时间 	1秒钟触发一次，这里可以获取秒钟时间
			//rtc_flag=1;
	}
	if(RTC_GetITStatus(RTC_IT_ALR))			//闹钟中断
	{
		RTC_ClearITPendingBit(RTC_IT_ALR);	//清闹钟中断		  								 				
	}
	RTC_ClearITPendingBit(RTC_IT_SEC);		//清除溢出，秒钟中断标志		  								 
	RTC_WaitForLastTask();					//等待RTC寄存器操作完成
}

uint8_t const table_week[12]={0,3,3,6,1,4,6,2,5,0,3,5}; 			//月修正数据表	  

const uint8_t mon_table[12]={31,28,31,30,31,30,31,31,30,31,30,31};	//平年的月份日期表
/*****************************************************************************
** 函数名称: time_get
** 功能描述: 获取RTC时间函数，由外部来调用 
*	形    参: 无
*	返 回 值: 无
*****************************************************************************/
uint8_t time_get(void)
{
	static uint16_t daycnt=0;
	uint32_t timecount=0; 
	uint32_t temp=0;
	uint16_t temp1=0;	  
	 
	timecount = RTC_GetCounter();									//获得 RTC 计数器值(秒钟数)			 
	temp=timecount/86400;   //得到天数(秒钟数对应的)
	if(daycnt!=temp)//超过一天了
	{	  
		daycnt=temp;
		temp1=1970;	//从1970年开始
		while(temp>=365)
		{				 
			if(is_leap_year(temp1))//是闰年
			{
				if(temp>=366)temp-=366;//闰年的秒钟数
				else {temp1++;break;}  
			}
			else temp-=365;	  //平年 
			temp1++;  
		}   
		date_timer.w_year=temp1;//得到年份
		temp1=0;
		while(temp>=28)//超过了一个月
		{
			if(is_leap_year(date_timer.w_year)&&temp1==1)//当年是不是闰年/2月份
			{
				if(temp>=29)temp-=29;//闰年的秒钟数
				else break; 
			}
			else 
			{
				if(temp>=mon_table[temp1])temp-=mon_table[temp1];//平年
				else break;
			}
			temp1++;  
		}
		date_timer.w_month=temp1+1;//得到月份
		date_timer.w_date=temp+1;  //得到日期 
	}
	temp=timecount%86400;     //得到秒钟数   	   
	date_timer.hour=temp/3600;     //小时
	date_timer.min=(temp%3600)/60; //分钟	
	date_timer.sec=(temp%3600)%60; //秒钟
	//timer.week=rtc_get_week(timer.w_year,timer.w_month,timer.w_date);//获取星期 
	return 0;
}
/*****************************************************************************
** 函数名称: Time_Set
** 功能描述: 由检测是否为第一次设置RTC时调用
					包括年月日时分秒
*	形    参: 无
*	返 回 值: 无
*****************************************************************************/
static void time_set(void)
{
	uint16_t buf[6];	
	buf[0]=2015;
	buf[1]=9;
	buf[2]=23;
	buf[3]=19;
	buf[4]=10;
	buf[5]=28;
	time_update(buf[0],buf[1],buf[2],buf[3],buf[4],buf[5]);
}

//设置时钟
//把输入的时钟转换为秒钟
//以1970年1月1日为基准
//1970~2099年为合法年份
//返回值:0,成功;其他:错误代码.
//月份数据表											 

/*****************************************************************************
** 函数名称: time_update
** 功能描述: 外部调用 来设置RTC时间，校时作用
					包括年月日时分秒
*	形    参: 无
*	返 回 值: 无
*****************************************************************************/
uint8_t time_update(uint16_t syear,uint8_t smon,uint8_t sday,uint8_t hour,uint8_t min,uint8_t sec)
{
	uint16_t t;
	uint32_t seccount=0;
	if(syear<1970||syear>2099)return 1;	   
	for(t=1970;t<syear;t++)											//把所有年份的秒钟相加
		{
		if(is_leap_year(t))seccount+=31622400;						//闰年的秒钟数
		else seccount+=31536000;			  						//平年的秒钟数
		}
	smon-=1;
	for(t=0;t<smon;t++)	   											//把前面月份的秒钟数相加
		{
		seccount+=(uint32_t)mon_table[t]*86400;						//月份秒钟数相加
		if(is_leap_year(syear)&&t==1)seccount+=86400;				//闰年2月份增加一天的秒钟数	   
		}
	seccount+=(uint32_t)(sday-1)*86400;								//把前面日期的秒钟数相加 
	seccount+=(uint32_t)hour*3600;									//小时秒钟数
	seccount+=(uint32_t)min*60;	 									//分钟秒钟数
	seccount+=sec;													//最后的秒钟加上去
											    
	RTC_WaitForLastTask();											//等待最近一次对RTC寄存器的写操作完成
	RTC_SetCounter(seccount);										//设置RTC计数器的值
	RTC_WaitForLastTask();											//等待最近一次对RTC寄存器的写操作完成  	
	return 0;		
}