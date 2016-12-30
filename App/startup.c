#include "startup.h"

#include "stdarg.h"
#include "rds_encoder.h"
#include "amp_ctrl.h"
#include "common.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "croutine.h"
#include "semphr.h"
#include "event_groups.h"
/* 被系统调用 */
volatile uint32_t ulHighFrequencyTimerTicks = 0UL;
static TaskHandle_t rds_encoder_task_handle = NULL;
static TaskHandle_t iwdg_task_handle=NULL;
static TaskHandle_t amp_task_handle=NULL;

//喂狗事件标志组
EventGroupHandle_t dog_event_group_handle = NULL;
/*************
*   这里的喂狗不能直接通过延迟来解决，因为这是一个独立 的任务，不能任务不能打断其运行，
*   如果其他的任务发生错误 ，并不能阻止此任务 的运行，800MS时间一到，还是会执行喂狗操作
*   这里通过 FreeRTOS中的事件标志组，来解决此问题
***************/
static void dog_task()
{
	EventBits_t uxBits;
	const TickType_t feed_dog_wait = 800 / portTICK_PERIOD_MS; /* 最大延迟100ms */
	while(1)
	{
		
			/* 等待所有任务发来事件标志 */
		uxBits = xEventGroupWaitBits(dog_event_group_handle, /* 事件标志组句柄 */
							         TASK_BIT_ALL,       /* 等待TASK_BIT_ALL被设置 */
							         pdTRUE,             /* 退出前TASK_BIT_ALL被清除，这里是TASK_BIT_ALL都被设置才表示“退出”*/
							         pdTRUE,             /* 设置为pdTRUE表示等待TASK_BIT_ALL都被设置*/
							         feed_dog_wait); 	 /* 等待延迟时间 */
		if( (uxBits & TASK_BIT_ALL) == TASK_BIT_ALL)
		{
			//printf("---------system call the dog task---------\r\n");
			hdl_feed_dog();
		}
		else
		{
			 //超时，直接复位 
			printf("----------wait the bits time out -------------\r\n");
			//m_system_reset();
		}
//		printf("---------system call the dog task---------\r\n");
//		hdl_feed_dog();
//		//设置的是1S的独立看门狗，这里可以进行800MS的 延迟操作
//		vTaskDelay(800);
	}
}


static void app_create_obj()
{
	dog_event_group_handle=xEventGroupCreate();
}

//创建任务
static void app_task_create()
{
	xTaskCreate( dog_task,   	/* 任务函数  */
                 "dog_task",     	/* 任务名    */
                 50,               	/* 任务栈大小，单位word，也就是4字节 */
                 NULL,              	/* 任务参数  */
                 10,                 	/* 任务优先级*/
                 &iwdg_task_handle );  /* 任务句柄  */
	
		xTaskCreate( rds_encoder_task,   	/* 任务函数  */
                 "rds_encoder_task",     	/* 任务名    */
                 256,               	/* 任务栈大小，单位word，也就是4字节 */
                 NULL,              	/* 任务参数  */
                 9,                 	/* 任务优先级*/
                 &rds_encoder_task_handle );  /* 任务句柄  */
	
			xTaskCreate( amp_task,   	/* 任务函数  */
                 "amp_task",     	/* 任务名    */
                 256,               	/* 任务栈大小，单位word，也就是4字节 */
                 NULL,              	/* 任务参数  */
                 8,                 	/* 任务优先级*/
                 &amp_task_handle );  /* 任务句柄  */
}

static void hard_init()
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);             // 一定要将优先级分组设置分组方式4！！！
	
	hdl_init_iwdg(40000); //初始独立看门狗
	
	hdl_uart_init(); //初始化串口
}

void system_init()
{
	
	SystemInit();
	
	__set_PRIMASK(1); //系统初始化之前，关闭所有的中断
	
	hard_init();
	printf("--------System start--------\r\n");
	//debug_out("--------System start--------\r\n");
	app_task_create();
	//创建事件标志组
	app_create_obj() ;
	//printf("--------rds encoder and iwdg init success--------\r\n");
	
	vTaskStartScheduler();
	
}

/*
*********************************************************************************************************
*	函 数 名: vApplicationStackOverflowHook
*	功能说明: 栈溢出的钩子函数
*	形    参: xTask        任务句柄
*             pcTaskName   任务名
*	返 回 值: 无
* 注：如果想使用此钩子函数，则需要configCHECK_FOR_STACK_OVERFLOW 置为1
*********************************************************************************************************
*/
void vApplicationStackOverflowHook( TaskHandle_t xTask, signed char *pcTaskName )
{
	//printf("任务：%s 发现栈溢出\r\n", pcTaskName);
}