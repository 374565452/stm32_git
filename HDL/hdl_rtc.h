/*
*********************************************************************************************************
*
*	ģ������ : HDL_RTCģ��
*	�ļ����� : hdl_rtc.h
*	˵    �� :����stm32�е�ʵʱʵ�ӹ���
*				 
*
*
*********************************************************************************************************
*/
#ifndef __HDL_RTC_H
#define __HDL_RTC_H

#include "hdl_include.h"
typedef struct 
{
	uint8_t hour;
	uint8_t min;
	uint8_t sec;			
	//������������
	uint16_t w_year;
	uint8_t  w_month;
	uint8_t  w_date;
	uint8_t  week;
}date_t;


void hdl_init_rtc();

uint8_t time_update(uint16_t syear,uint8_t smon,uint8_t sday,uint8_t hour,uint8_t min,uint8_t sec);
uint8_t time_get(void);

#endif