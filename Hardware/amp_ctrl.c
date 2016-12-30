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
		xTimerStop(pxTimer,100); //停止定时器
		hdl_l(POWER_CTL_GPIO,POWER_CTL_PORT);
		amp_close_delay=0;
	}
	else
	{
		amp_close_delay++;
	}
	/****** 测试Timer定时器代码 *************/
//	if(flag >1)
//	{
//		printf("--------close softer timer-----------------\r\n");
//		xTimerStop(pxTimer,100); //停止定时器
//		flag=0;
//		timer_open_flag=0;
//	}
//	else
//	{
//		printf("--------softer timer running-----------------\r\n");
//		flag++;
//	}
	/****** 测试Timer定时器代码 *************/
}

void amp_init()
{
	amp_init_var();
	amp_init_hardware();
	amp_close_timer=xTimerCreate("amp close timer",          /* 定时器名字 */
									 1000,    /* 定时器周期,单位时钟节拍 */
									 pdTRUE,          /* 周期性 */
									 (void *) 1,      /* 定时器ID */
									 timer_call_back); /* 定时器回调函数 */
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
				//有动作，要开启功放，并且关闭软件定时器
				hdl_h(POWER_CTL_GPIO,POWER_CTL_PORT);
				xTimerStop(amp_close_timer,100);
				amp_close_delay=0; //这里需要将延迟计数用的累加器置0
				//进入临界区，对
				taskENTER_CRITICAL(); //此处会用到rds_protocol_buffer这个共享全局变量中 ，所以OS要进入到临界区
				send_all_open(); //发送全开
				taskEXIT_CRITICAL() ;  //不用rds_protocol_buffer ，OS退出临界区
				 //while(1) {} //如果有动作，直接不发送标志位，看系统是否复位  如果在此任务中，不发送事件标志位，则会进行复位操作
				//
				action_flag=1;
			}
		}
		if((switch_in==1 ) && (ac_in==0) && (dc_in==0))
		{
			if(action_flag==1)
			{
					//进入临界区，对
				taskENTER_CRITICAL(); //此处会用到rds_protocol_buffer这个共享全局变量中 ，所以OS要进入到临界区
				send_all_close(); //发送全开
				taskEXIT_CRITICAL() ;  //不用rds_protocol_buffer ，OS退出临界区
				//动作取消，开启软件定时器 ，关闭功放
				xTimerStart(amp_close_timer,100);
				action_flag=0;
			}
		}
			/* 发送事件标志 1，表示任务正常运行 */
		xEventGroupSetBits(dog_event_group_handle, TASK_BIT_1);
		vTaskDelay(30); //30ms检测一次
	}
	
	/****** 测试Timer定时器代码 *************/
//	while(1)
//	{
//		vTaskDelay(12000); //12S后，开始定时器
//		if(timer_open_flag ==0)
//		{
//			timer_open_flag=1;
//			vTaskDelay(5000); //再次延迟5S执行
//			xTimerStart(amp_close_timer,100);
//			printf("--------------start the sofeter timer ---------\r\n");
//		}
//		
//	}
	/****** 测试Timer定时器代码 *************/
}