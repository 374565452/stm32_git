/*
    FreeRTOS V9.0.0 - Copyright (C) 2016 Real Time Engineers Ltd.
    All rights reserved

    VISIT http://www.FreeRTOS.org TO ENSURE YOU ARE USING THE LATEST VERSION.

    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation >>>> AND MODIFIED BY <<<< the FreeRTOS exception.

    ***************************************************************************
    >>!   NOTE: The modification to the GPL is included to allow you to     !<<
    >>!   distribute a combined work that includes FreeRTOS without being   !<<
    >>!   obliged to provide the source code for proprietary components     !<<
    >>!   outside of the FreeRTOS kernel.                                   !<<
    ***************************************************************************

    FreeRTOS is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE.  Full license text is available on the following
    link: http://www.freertos.org/a00114.html

    ***************************************************************************
     *                                                                       *
     *    FreeRTOS provides completely free yet professionally developed,    *
     *    robust, strictly quality controlled, supported, and cross          *
     *    platform software that is more than just the market leader, it     *
     *    is the industry's de facto standard.                               *
     *                                                                       *
     *    Help yourself get started quickly while simultaneously helping     *
     *    to support the FreeRTOS project by purchasing a FreeRTOS           *
     *    tutorial book, reference manual, or both:                          *
     *    http://www.FreeRTOS.org/Documentation                              *
     *                                                                       *
    ***************************************************************************

    http://www.FreeRTOS.org/FAQHelp.html - Having a problem?  Start by reading
    the FAQ page "My application does not run, what could be wrong?".  Have you
    defined configASSERT()?

    http://www.FreeRTOS.org/support - In return for receiving this top quality
    embedded software for free we request you assist our global community by
    participating in the support forum.

    http://www.FreeRTOS.org/training - Investing in training allows your team to
    be as productive as possible as early as possible.  Now you can receive
    FreeRTOS training directly from Richard Barry, CEO of Real Time Engineers
    Ltd, and the world's leading authority on the world's leading RTOS.

    http://www.FreeRTOS.org/plus - A selection of FreeRTOS ecosystem products,
    including FreeRTOS+Trace - an indispensable productivity tool, a DOS
    compatible FAT file system, and our tiny thread aware UDP/IP stack.

    http://www.FreeRTOS.org/labs - Where new FreeRTOS products go to incubate.
    Come and try FreeRTOS+TCP, our new open source TCP/IP stack for FreeRTOS.

    http://www.OpenRTOS.com - Real Time Engineers ltd. license FreeRTOS to High
    Integrity Systems ltd. to sell under the OpenRTOS brand.  Low cost OpenRTOS
    licenses offer ticketed support, indemnification and commercial middleware.

    http://www.SafeRTOS.com - High Integrity Systems also provide a safety
    engineered and independently SIL3 certified version for use in safety and
    mission critical applications that require provable dependability.

    1 tab == 4 spaces!
*/

#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

/*-----------------------------------------------------------
 * Application specific definitions.
 *
 * These definitions should be adjusted for your particular hardware and
 * application requirements.
 *
 * THESE PARAMETERS ARE DESCRIBED WITHIN THE 'CONFIGURATION' SECTION OF THE
 * FreeRTOS API DOCUMENTATION AVAILABLE ON THE FreeRTOS.org WEB SITE. 
 *
 * See http://www.freertos.org/a00110.html.
 *----------------------------------------------------------*/

#if defined(__ICCARM__) || defined(__CC_ARM) || defined(__GNUC__)
 #include <stdint.h>
 extern volatile uint32_t ulHighFrequencyTimerTicks;
#endif

#define configUSE_PREEMPTION		1        //使能抢占调度
#define configUSE_IDLE_HOOK			0        //不使用空闲任务钩子函数

#define configUSE_TICKLESS_IDLE         0                                   // 使用低功耗机制
#define configEXPECTED_IDLE_TIME_BEFORE_SLEEP   3                           // 最小唤醒节拍设为5个TICK(低功耗)

#define configUSE_TICK_HOOK			0
#define configCPU_CLOCK_HZ			( ( unsigned long ) 72000000 )	 //MCU主频为72MHZ
#define configTICK_RATE_HZ			( ( TickType_t ) 1000 )  //SysTick节拍频率
#define configMAX_PRIORITIES		( 15 ) //  最大任务优先级的数目 ,数值越大优先级越高
#define configMINIMAL_STACK_SIZE	( ( unsigned short ) 128 )   //最小任务堆栈分配
#define configTOTAL_HEAP_SIZE		( ( size_t ) ( 17 * 1024 ) ) //内存堆管理的内存空间为17K
#define configMAX_TASK_NAME_LEN		( 16 )  //任务名称最大长度
#define configUSE_TRACE_FACILITY	1 //只有设置为1，才可以执行vTaskGetRunTimeStates函数
#define configUSE_16_BIT_TICKS		0  // 不使用16位节拍数，使用32位节拍数
#define configIDLE_SHOULD_YIELD		0   //// 空闲任务不主动让出CPU使用权(不允许用户任务与空闲任务共享�


#define configUSE_MUTEXES				1                                   // 使用互斥信号量
#define configQUEUE_REGISTRY_SIZE		8
//#define configCHECK_FOR_STACK_OVERFLOW	0                                   // 不使用堆栈溢出检测的方法
//如果设置configCHECK_FOR_STACK_OVERFLOW 为1 则必须要定义vApplicationStackOverflowHook函数，否则编译不通过
#define configCHECK_FOR_STACK_OVERFLOW	1                                   // 不使用堆栈溢出检测的方法
#define configUSE_RECURSIVE_MUTEXES		0
#define configUSE_MALLOC_FAILED_HOOK	0
#define configUSE_APPLICATION_TASK_TAG	0
#define configUSE_COUNTING_SEMAPHORES	0                                   // 不使用计数信号量
#define configGENERATE_RUN_TIME_STATS	0



/* Co-routine definitions. */
#define configUSE_CO_ROUTINES 		0  //不使用协程
#define configMAX_CO_ROUTINE_PRIORITIES ( 2 )  //协程优先级

/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function. */

#define INCLUDE_vTaskPrioritySet		1 //是否允许重设任务优先级
#define INCLUDE_uxTaskPriorityGet		1 //是否允许获取任务优先级
#define INCLUDE_vTaskDelete				1   //是否允许删除任务
#define INCLUDE_vTaskCleanUpResources	0 //
#define INCLUDE_vTaskSuspend			1
#define INCLUDE_vTaskDelayUntil			1
#define INCLUDE_vTaskDelay				1

/* This is the raw value as per the Cortex-M3 NVIC.  Values can be 255
(lowest) to 0 (1?) (highest). */
#define configKERNEL_INTERRUPT_PRIORITY 		255
/* !!!! configMAX_SYSCALL_INTERRUPT_PRIORITY must not be set to zero !!!!
See http://www.FreeRTOS.org/RTOS-Cortex-M3-M4.html. */
//#define configMAX_SYSCALL_INTERRUPT_PRIORITY 	191 /* equivalent to 0xb0, or priority 11. */
#define configMAX_SYSCALL_INTERRUPT_PRIORITY 	0x1F 

/* This is the value being used as per the ST library which permits 16
priority values, 0 to 15.  This must correspond to the
configKERNEL_INTERRUPT_PRIORITY setting.  Here 15 corresponds to the lowest
NVIC value of 255. */
#define configLIBRARY_KERNEL_INTERRUPT_PRIORITY	15


/* Run time and task stats gathering related definitions. */
//用于检测任务运行状态
/*******
*
* 如果想查看FreeRTOS中的任务运行状态，则必面设置如下参数信息
*
*******/
#define configGENERATE_RUN_TIME_STATS                0
#define configUSE_STATS_FORMATTING_FUNCTIONS         0
#define portCONFIGURE_TIMER_FOR_RUN_TIME_STATS()     (ulHighFrequencyTimerTicks = 0ul)
#define portGET_RUN_TIME_COUNTER_VALUE()             ulHighFrequencyTimerTicks

//如果启用定时器，FreeRTOS中会创建一个名为Tmr Svc 任务，
#define configUSE_TIMERS 1    //启动用户定时器，FreeRTOS中的软件定时器
//如果configUSE_TIMERS 置为1，则创建的Tmr Svc任务的优先级为2
#define configTIMER_TASK_PRIORITY   (2)
#define configTIMER_QUEUE_LENGTH   5 //配置定时器任务用到的消息队列的大小，能够存储的消息个数
//之前设置的定时器任务栈空间大小为  50 ，导致创建2个定时器任务时，出现Tmr Svc任务栈溢出错误
//所以改用下面的 栈空间大小值
#define configTIMER_TASK_STACK_DEPTH (configMINIMAL_STACK_SIZE*2) //配置定时器任务的任务栈大小，(系统创建的最小堆栈数*2）

//定义操作系统中断为stm32库底层中断
#define vPortSVCHandler SVC_Handler
#define xPortPendSVHandler PendSV_Handler
#define xPortSysTickHandler SysTick_Handler

//添加一个参数检测功能
#define configASSERT(x)  if((x)==0){taskDISABLE_INTERRUPTS();for(;;);}

#endif /* FREERTOS_CONFIG_H */

