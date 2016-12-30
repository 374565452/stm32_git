/*
*********************************************************************************************************
*
*	ģ������ : HDL_RTCģ��
*	�ļ����� : hdl_rtc.c
* ��    �� ��v1.0
*	˵    �� : ����stm32�е�ʵʱʵ�ӹ���
*
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    2016-12-9        ��ʽ����
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

//�ж��Ƿ������꺯��
//�·�   1  2  3  4  5  6  7  8  9  10 11 12
//����   31 29 31 30 31 30 31 31 30 31 30 31
//������ 31 28 31 30 31 30 31 31 30 31 30 31
//����:���
//���:������ǲ�������.1,��.0,����
static uint8_t is_leap_year(uint16_t year)
{			  
	if(year%4==0) //�����ܱ�4����
	{ 
		if(year%100==0) 
		{ 
			if(year%400==0)
			{
				return 1;//�����00��β,��Ҫ�ܱ�400���� 	   
			
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
*	�� �� ��: hdl_init_rtc
*	����˵��: stm32�е�RTCӲ����ʼ��
*	��    ��: ��
*	�� �� ֵ: ��
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
*	�� �� ��: hdl_rtc_rcc 
*	����˵��: stm32�е�RTCӲ����ʼ�� rccʱ�ӳ�ʼ��
*	��    ��: ��
*	�� �� ֵ: ��
* 
*********************************************************************************************************
*/
static void hdl_rtc_rcc()
{
	//RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
																	//ʹ��PWR��BKP����ʱ��   	
  hdl_set_rcc(RCC_APB1Periph_PWR,ENABLE,1);
	hdl_set_rcc(RCC_APB1Periph_BKP,ENABLE,1);
}

/*
*********************************************************************************************************
*	�� �� ��: hdl_rtc_rcc 
*	����˵��: stm32�е�RTCӲ����ʼ�� RTCȫ���ж�
*	��    ��: ��
*	�� �� ֵ: ��
* 
*********************************************************************************************************
*/
static void hdl_rtc_nvic()
{
		
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;		//RTCȫ���ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//��ռ���ȼ�1λ,�����ȼ�3λ
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	//��ռ���ȼ�0λ,�����ȼ�4λ
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		//ʹ�ܸ�ͨ���ж�
	NVIC_Init(&NVIC_InitStructure);		//����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
}
/*
*********************************************************************************************************
*	�� �� ��: hdl_rtc_rcc 
*	����˵��: stm32�е�RTCӲ����ʼ�� RTC�Ĵ����������⣬��������Ƿ�Ϊ��һ�ν�������RTC
*	��    ��: ��
*	�� �� ֵ: ��
* 
*********************************************************************************************************
*/
static void hdl_rtc()
{
	PWR_BackupAccessCmd(ENABLE);									//ʹ��RTC�ͺ󱸼Ĵ������� 
	
	if(BKP_ReadBackupRegister(BKP_DR1)!=0x5555)						//��ָ���ĺ󱸼Ĵ����ж������ݣ��ж��Ƿ�Ϊ��һ������
	{	
      	//printf("ʱ�����á�����\r\n");																
		BKP_DeInit();												//������BKP��ȫ���Ĵ�������Ϊȱʡֵ 	
		RCC_LSEConfig(RCC_LSE_ON);									//ʹ���ⲿ����ʱ�� 32.768KHz
		while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)			//���ָ����RCC��־λ�������,�ȴ����پ������
  		{}
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);						//����RTCʱ��(RTCCLK),ѡ��LSE��ΪRTCʱ��    
		RCC_RTCCLKCmd(ENABLE);										//ʹ��RTCʱ��  
		RTC_WaitForSynchro();										//�ȴ�RTC�Ĵ���(RTC_CNT,RTC_ALR��RTC_PRL)��RTC APBʱ��ͬ��
		RTC_WaitForLastTask();										//�ȴ����һ�ζ�RTC�Ĵ�����д�������
		RTC_ITConfig(RTC_IT_SEC, ENABLE);							//ʹ��RTC���ж�
		RTC_WaitForLastTask();										//�ȴ����һ�ζ�RTC�Ĵ�����д�������
		RTC_SetPrescaler(32767); 									//����RTCԤ��Ƶ��ֵ  RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1)
		RTC_WaitForLastTask();										//�ȴ����һ�ζ�RTC�Ĵ�����д�������
		//�ڵ�һ������RTCʱ���ʱ�� ��Ӧ�����ⲿ����������ʱ��
		time_set();													//ʱ������	
    
		BKP_WriteBackupRegister(BKP_DR1, 0x5555);					//��ָ���ĺ󱸼Ĵ�����д���û���������0X5555���жϱ�־										
	}																 	
	else															//���ǵ�һ������ ������ʱ
	{
		if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)			//���ָ����RCC��־λ�������:POR/PDR��λ
		{
      		//printf("�ϵ縴λ������\r\n");
		}											
		else if(RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)		//���ָ����RCC��־λ�������:�ܽŸ�λ
		{
      		//printf("�ⲿ��λ������\r\n");
		}
    	//printf("����Ҫ���á�����\r\n");
		
		RTC_WaitForSynchro();										//�ȴ����һ�ζ�RTC�Ĵ�����д�������
		
		RTC_ITConfig(RTC_IT_SEC, ENABLE);							//ʹ��RTC���ж�

		RTC_WaitForLastTask();										//�ȴ����һ�ζ�RTC�Ĵ�����д�������
	}		    				     
	
	time_get();														//����ʱ�� 
	
	RCC_ClearFlag();												//���RCC�ĸ�λ��־λ
}


/*****************************************************************************
** ��������: RTC_IRQHandler
** ��������: RTC�жϷ�����
				ÿ�봥��һ��  
*	��    ��: ��
*	�� �� ֵ: ��
*****************************************************************************/
void RTC_IRQHandler(void)
{							    
	if(RTC_GetITStatus(RTC_IT_SEC))			//�����ж�
	{							
		//time_get();							//����ʱ�� 	1���Ӵ���һ�Σ�������Ի�ȡ����ʱ��
			//rtc_flag=1;
	}
	if(RTC_GetITStatus(RTC_IT_ALR))			//�����ж�
	{
		RTC_ClearITPendingBit(RTC_IT_ALR);	//�������ж�		  								 				
	}
	RTC_ClearITPendingBit(RTC_IT_SEC);		//�������������жϱ�־		  								 
	RTC_WaitForLastTask();					//�ȴ�RTC�Ĵ����������
}

uint8_t const table_week[12]={0,3,3,6,1,4,6,2,5,0,3,5}; 			//���������ݱ�	  

const uint8_t mon_table[12]={31,28,31,30,31,30,31,31,30,31,30,31};	//ƽ����·����ڱ�
/*****************************************************************************
** ��������: time_get
** ��������: ��ȡRTCʱ�亯�������ⲿ������ 
*	��    ��: ��
*	�� �� ֵ: ��
*****************************************************************************/
uint8_t time_get(void)
{
	static uint16_t daycnt=0;
	uint32_t timecount=0; 
	uint32_t temp=0;
	uint16_t temp1=0;	  
	 
	timecount = RTC_GetCounter();									//��� RTC ������ֵ(������)			 
	temp=timecount/86400;   //�õ�����(��������Ӧ��)
	if(daycnt!=temp)//����һ����
	{	  
		daycnt=temp;
		temp1=1970;	//��1970�꿪ʼ
		while(temp>=365)
		{				 
			if(is_leap_year(temp1))//������
			{
				if(temp>=366)temp-=366;//�����������
				else {temp1++;break;}  
			}
			else temp-=365;	  //ƽ�� 
			temp1++;  
		}   
		date_timer.w_year=temp1;//�õ����
		temp1=0;
		while(temp>=28)//������һ����
		{
			if(is_leap_year(date_timer.w_year)&&temp1==1)//�����ǲ�������/2�·�
			{
				if(temp>=29)temp-=29;//�����������
				else break; 
			}
			else 
			{
				if(temp>=mon_table[temp1])temp-=mon_table[temp1];//ƽ��
				else break;
			}
			temp1++;  
		}
		date_timer.w_month=temp1+1;//�õ��·�
		date_timer.w_date=temp+1;  //�õ����� 
	}
	temp=timecount%86400;     //�õ�������   	   
	date_timer.hour=temp/3600;     //Сʱ
	date_timer.min=(temp%3600)/60; //����	
	date_timer.sec=(temp%3600)%60; //����
	//timer.week=rtc_get_week(timer.w_year,timer.w_month,timer.w_date);//��ȡ���� 
	return 0;
}
/*****************************************************************************
** ��������: Time_Set
** ��������: �ɼ���Ƿ�Ϊ��һ������RTCʱ����
					����������ʱ����
*	��    ��: ��
*	�� �� ֵ: ��
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

//����ʱ��
//�������ʱ��ת��Ϊ����
//��1970��1��1��Ϊ��׼
//1970~2099��Ϊ�Ϸ����
//����ֵ:0,�ɹ�;����:�������.
//�·����ݱ�											 

/*****************************************************************************
** ��������: time_update
** ��������: �ⲿ���� ������RTCʱ�䣬Уʱ����
					����������ʱ����
*	��    ��: ��
*	�� �� ֵ: ��
*****************************************************************************/
uint8_t time_update(uint16_t syear,uint8_t smon,uint8_t sday,uint8_t hour,uint8_t min,uint8_t sec)
{
	uint16_t t;
	uint32_t seccount=0;
	if(syear<1970||syear>2099)return 1;	   
	for(t=1970;t<syear;t++)											//��������ݵ��������
		{
		if(is_leap_year(t))seccount+=31622400;						//�����������
		else seccount+=31536000;			  						//ƽ���������
		}
	smon-=1;
	for(t=0;t<smon;t++)	   											//��ǰ���·ݵ����������
		{
		seccount+=(uint32_t)mon_table[t]*86400;						//�·����������
		if(is_leap_year(syear)&&t==1)seccount+=86400;				//����2�·�����һ���������	   
		}
	seccount+=(uint32_t)(sday-1)*86400;								//��ǰ�����ڵ���������� 
	seccount+=(uint32_t)hour*3600;									//Сʱ������
	seccount+=(uint32_t)min*60;	 									//����������
	seccount+=sec;													//�������Ӽ���ȥ
											    
	RTC_WaitForLastTask();											//�ȴ����һ�ζ�RTC�Ĵ�����д�������
	RTC_SetCounter(seccount);										//����RTC��������ֵ
	RTC_WaitForLastTask();											//�ȴ����һ�ζ�RTC�Ĵ�����д�������  	
	return 0;		
}