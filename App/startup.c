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
/* ��ϵͳ���� */
volatile uint32_t ulHighFrequencyTimerTicks = 0UL;
static TaskHandle_t rds_encoder_task_handle = NULL;
static TaskHandle_t iwdg_task_handle=NULL;
static TaskHandle_t amp_task_handle=NULL;

//ι���¼���־��
EventGroupHandle_t dog_event_group_handle = NULL;
/*************
*   �����ι������ֱ��ͨ���ӳ����������Ϊ����һ������ �����񣬲��������ܴ�������У�
*   ������������������� ����������ֹ������ �����У�800MSʱ��һ�������ǻ�ִ��ι������
*   ����ͨ�� FreeRTOS�е��¼���־�飬�����������
***************/
static void dog_task()
{
	EventBits_t uxBits;
	const TickType_t feed_dog_wait = 800 / portTICK_PERIOD_MS; /* ����ӳ�100ms */
	while(1)
	{
		
			/* �ȴ������������¼���־ */
		uxBits = xEventGroupWaitBits(dog_event_group_handle, /* �¼���־���� */
							         TASK_BIT_ALL,       /* �ȴ�TASK_BIT_ALL������ */
							         pdTRUE,             /* �˳�ǰTASK_BIT_ALL�������������TASK_BIT_ALL�������òű�ʾ���˳���*/
							         pdTRUE,             /* ����ΪpdTRUE��ʾ�ȴ�TASK_BIT_ALL��������*/
							         feed_dog_wait); 	 /* �ȴ��ӳ�ʱ�� */
		if( (uxBits & TASK_BIT_ALL) == TASK_BIT_ALL)
		{
			//printf("---------system call the dog task---------\r\n");
			hdl_feed_dog();
		}
		else
		{
			 //��ʱ��ֱ�Ӹ�λ 
			printf("----------wait the bits time out -------------\r\n");
			//m_system_reset();
		}
//		printf("---------system call the dog task---------\r\n");
//		hdl_feed_dog();
//		//���õ���1S�Ķ������Ź���������Խ���800MS�� �ӳٲ���
//		vTaskDelay(800);
	}
}


static void app_create_obj()
{
	dog_event_group_handle=xEventGroupCreate();
}

//��������
static void app_task_create()
{
	xTaskCreate( dog_task,   	/* ������  */
                 "dog_task",     	/* ������    */
                 50,               	/* ����ջ��С����λword��Ҳ����4�ֽ� */
                 NULL,              	/* �������  */
                 10,                 	/* �������ȼ�*/
                 &iwdg_task_handle );  /* ������  */
	
		xTaskCreate( rds_encoder_task,   	/* ������  */
                 "rds_encoder_task",     	/* ������    */
                 256,               	/* ����ջ��С����λword��Ҳ����4�ֽ� */
                 NULL,              	/* �������  */
                 9,                 	/* �������ȼ�*/
                 &rds_encoder_task_handle );  /* ������  */
	
			xTaskCreate( amp_task,   	/* ������  */
                 "amp_task",     	/* ������    */
                 256,               	/* ����ջ��С����λword��Ҳ����4�ֽ� */
                 NULL,              	/* �������  */
                 8,                 	/* �������ȼ�*/
                 &amp_task_handle );  /* ������  */
}

static void hard_init()
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);             // һ��Ҫ�����ȼ��������÷��鷽ʽ4������
	
	hdl_init_iwdg(40000); //��ʼ�������Ź�
	
	hdl_uart_init(); //��ʼ������
}

void system_init()
{
	
	SystemInit();
	
	__set_PRIMASK(1); //ϵͳ��ʼ��֮ǰ���ر����е��ж�
	
	hard_init();
	printf("--------System start--------\r\n");
	//debug_out("--------System start--------\r\n");
	app_task_create();
	//�����¼���־��
	app_create_obj() ;
	//printf("--------rds encoder and iwdg init success--------\r\n");
	
	vTaskStartScheduler();
	
}

/*
*********************************************************************************************************
*	�� �� ��: vApplicationStackOverflowHook
*	����˵��: ջ����Ĺ��Ӻ���
*	��    ��: xTask        ������
*             pcTaskName   ������
*	�� �� ֵ: ��
* ע�������ʹ�ô˹��Ӻ���������ҪconfigCHECK_FOR_STACK_OVERFLOW ��Ϊ1
*********************************************************************************************************
*/
void vApplicationStackOverflowHook( TaskHandle_t xTask, signed char *pcTaskName )
{
	//printf("����%s ����ջ���\r\n", pcTaskName);
}