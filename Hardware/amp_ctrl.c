#include "amp_ctrl.h"
#include "rds_encoder.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "croutine.h"
#include "timers.h"
#include "event_groups.h"
#include "common.h"
extern EventGroupHandle_t dog_event_group_handle;

static TimerHandle_t amp_close_timer=NULL;

//volatile uint8_t timer_open_flag=0;

volatile uint8_t action_flag=0;
volatile uint8_t amp_close_delay=0;

static void amp_init_var()
{
	action_flag=0;
	amp_close_delay=0;
}

static void amp_init_hardware()
{
	hdl_set_rcc(RCC_APB2Periph_GPIOE ,ENABLE,2);
	hdl_input_gpio(SWITCH_IN_GPIO,SWITCH_IN_PORT,GPIO_Mode_IPU);
	hdl_input_gpio(AC_IN_GPIO,AC_IN_PORT,GPIO_Mode_IN_FLOATING);
	hdl_input_gpio(DC_IN_GPIO,DC_IN_PORT,GPIO_Mode_IN_FLOATING);
	hdl_output_gpio(POWER_CTL_GPIO,POWER_CTL_PORT,GPIO_Mode_Out_PP);
}

static void timer_call_back(TimerHandle_t pxTimer)
{
	if(amp_close_delay > 30)
	{
		xTimerStop(pxTimer,100); //ֹͣ��ʱ��
		hdl_l(POWER_CTL_GPIO,POWER_CTL_PORT);
		amp_close_delay=0;
	}
	else
	{
		amp_close_delay++;
	}
	/****** ����Timer��ʱ������ *************/
//	if(flag >1)
//	{
//		printf("--------close softer timer-----------------\r\n");
//		xTimerStop(pxTimer,100); //ֹͣ��ʱ��
//		flag=0;
//		timer_open_flag=0;
//	}
//	else
//	{
//		printf("--------softer timer running-----------------\r\n");
//		flag++;
//	}
	/****** ����Timer��ʱ������ *************/
}

void amp_init()
{
	amp_init_var();
	amp_init_hardware();
	amp_close_timer=xTimerCreate("amp close timer",          /* ��ʱ������ */
									 1000,    /* ��ʱ������,��λʱ�ӽ��� */
									 pdTRUE,          /* ������ */
									 (void *) 1,      /* ��ʱ��ID */
									 timer_call_back); /* ��ʱ���ص����� */
	xTimerStop(amp_close_timer,100);		
}

void amp_task()
{
	amp_init();
	while(1)
	{
		uint8_t switch_in=1;
		uint8_t ac_in=0;
		uint8_t dc_in=0;
		switch_in=hdl_read(SWITCH_IN_GPIO,SWITCH_IN_PORT);
		ac_in=hdl_read(AC_IN_GPIO,AC_IN_PORT);
		dc_in	=hdl_read(DC_IN_GPIO,DC_IN_PORT);
		if((switch_in == 0) || (ac_in == 1) || (dc_in==1))
		{
			if(action_flag == 0)
			{
				//�ж�����Ҫ�������ţ����ҹر������ʱ��
				hdl_h(POWER_CTL_GPIO,POWER_CTL_PORT);
				xTimerStop(amp_close_timer,100);
				amp_close_delay=0; //������Ҫ���ӳټ����õ��ۼ�����0
				//�����ٽ�������
				taskENTER_CRITICAL(); //�˴����õ�rds_protocol_buffer�������ȫ�ֱ����� ������OSҪ���뵽�ٽ���
				send_all_open(); //����ȫ��
				taskEXIT_CRITICAL() ;  //����rds_protocol_buffer ��OS�˳��ٽ���
				 //while(1) {} //����ж�����ֱ�Ӳ����ͱ�־λ����ϵͳ�Ƿ�λ  ����ڴ������У��������¼���־λ�������и�λ����
				//
				action_flag=1;
			}
		}
		if((switch_in==1 ) && (ac_in==0) && (dc_in==0))
		{
			if(action_flag==1)
			{
					//�����ٽ�������
				taskENTER_CRITICAL(); //�˴����õ�rds_protocol_buffer�������ȫ�ֱ����� ������OSҪ���뵽�ٽ���
				send_all_close(); //����ȫ��
				taskEXIT_CRITICAL() ;  //����rds_protocol_buffer ��OS�˳��ٽ���
				//����ȡ�������������ʱ�� ���رչ���
				xTimerStart(amp_close_timer,100);
				action_flag=0;
			}
		}
			/* �����¼���־ 1����ʾ������������ */
		xEventGroupSetBits(dog_event_group_handle, TASK_BIT_1);
		vTaskDelay(30); //30ms���һ��
	}
	
	/****** ����Timer��ʱ������ *************/
//	while(1)
//	{
//		vTaskDelay(12000); //12S�󣬿�ʼ��ʱ��
//		if(timer_open_flag ==0)
//		{
//			timer_open_flag=1;
//			vTaskDelay(5000); //�ٴ��ӳ�5Sִ��
//			xTimerStart(amp_close_timer,100);
//			printf("--------------start the sofeter timer ---------\r\n");
//		}
//		
//	}
	/****** ����Timer��ʱ������ *************/
}