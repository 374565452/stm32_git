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

#define configUSE_PREEMPTION		1        //Ê¹ÄÜÇÀÕ¼µ÷¶È
#define configUSE_IDLE_HOOK			0        //²»Ê¹ÓÃ¿ÕÏĞÈÎÎñ¹³×Óº¯Êı

#define configUSE_TICKLESS_IDLE         0                                   // Ê¹ÓÃµÍ¹¦ºÄ»úÖÆ
#define configEXPECTED_IDLE_TIME_BEFORE_SLEEP   3                           // ×îĞ¡»½ĞÑ½ÚÅÄÉèÎª5¸öTICK(µÍ¹¦ºÄ)

#define configUSE_TICK_HOOK			0
#define configCPU_CLOCK_HZ			( ( unsigned long ) 72000000 )	 //MCUÖ÷ÆµÎª72MHZ
#define configTICK_RATE_HZ			( ( TickType_t ) 1000 )  //SysTick½ÚÅÄÆµÂÊ
#define configMAX_PRIORITIES		( 15 ) //  ×î´óÈÎÎñÓÅÏÈ¼¶µÄÊıÄ¿ ,ÊıÖµÔ½´óÓÅÏÈ¼¶Ô½¸ß
#define configMINIMAL_STACK_SIZE	( ( unsigned short ) 128 )   //×îĞ¡ÈÎÎñ¶ÑÕ»·ÖÅä
#define configTOTAL_HEAP_SIZE		( ( size_t ) ( 17 * 1024 ) ) //ÄÚ´æ¶Ñ¹ÜÀíµÄÄÚ´æ¿Õ¼äÎª17K
#define configMAX_TASK_NAME_LEN		( 16 )  //ÈÎÎñÃû³Æ×î´ó³¤¶È
#define configUSE_TRACE_FACILITY	1 //Ö»ÓĞÉèÖÃÎª1£¬²Å¿ÉÒÔÖ´ĞĞvTaskGetRunTimeStatesº¯Êı
#define configUSE_16_BIT_TICKS		0  // ²»Ê¹ÓÃ16Î»½ÚÅÄÊı£¬Ê¹ÓÃ32Î»½ÚÅÄÊı
#define configIDLE_SHOULD_YIELD		0   //// ¿ÕÏĞÈÎÎñ²»Ö÷¶¯ÈÃ³öCPUÊ¹ÓÃÈ¨(²»ÔÊĞíÓÃ»§ÈÎÎñÓë¿ÕÏĞÈÎÎñ¹²ÏíÓ


#define configUSE_MUTEXES				1                                   // Ê¹ÓÃ»¥³âĞÅºÅÁ¿
#define configQUEUE_REGISTRY_SIZE		8
//#define configCHECK_FOR_STACK_OVERFLOW	0                                   // ²»Ê¹ÓÃ¶ÑÕ»Òç³ö¼ì²âµÄ·½·¨
//Èç¹ûÉèÖÃconfigCHECK_FOR_STACK_OVERFLOW Îª1 Ôò±ØĞëÒª¶¨ÒåvApplicationStackOverflowHookº¯Êı£¬·ñÔò±àÒë²»Í¨¹ı
#define configCHECK_FOR_STACK_OVERFLOW	1                                   // ²»Ê¹ÓÃ¶ÑÕ»Òç³ö¼ì²âµÄ·½·¨
#define configUSE_RECURSIVE_MUTEXES		0
#define configUSE_MALLOC_FAILED_HOOK	0
#define configUSE_APPLICATION_TASK_TAG	0
#define configUSE_COUNTING_SEMAPHORES	0                                   // ²»Ê¹ÓÃ¼ÆÊıĞÅºÅÁ¿
#define configGENERATE_RUN_TIME_STATS	0



/* Co-routine definitions. */
#define configUSE_CO_ROUTINES 		0  //²»Ê¹ÓÃĞ­³Ì
#define configMAX_CO_ROUTINE_PRIORITIES ( 2 )  //Ğ­³ÌÓÅÏÈ¼¶

/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function. */

#define INCLUDE_vTaskPrioritySet		1 //ÊÇ·ñÔÊĞíÖØÉèÈÎÎñÓÅÏÈ¼¶
#define INCLUDE_uxTaskPriorityGet		1 //ÊÇ·ñÔÊĞí»ñÈ¡ÈÎÎñÓÅÏÈ¼¶
#define INCLUDE_vTaskDelete				1   //ÊÇ·ñÔÊĞíÉ¾³ıÈÎÎñ
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
//ÓÃÓÚ¼ì²âÈÎÎñÔËĞĞ×´Ì¬
/*******
*
* Èç¹ûÏë²é¿´FreeRTOSÖĞµÄÈÎÎñÔËĞĞ×´Ì¬£¬Ôò±ØÃæÉèÖÃÈçÏÂ²ÎÊıĞÅÏ¢
*
*******/
#define configGENERATE_RUN_TIME_STATS                0
#define configUSE_STATS_FORMATTING_FUNCTIONS         0
#define portCONFIGURE_TIMER_FOR_RUN_TIME_STATS()     (ulHighFrequencyTimerTicks = 0ul)
#define portGET_RUN_TIME_COUNTER_VALUE()             ulHighFrequencyTimerTicks

//Èç¹ûÆôÓÃ¶¨Ê±Æ÷£¬FreeRTOSÖĞ»á´´½¨Ò»¸öÃûÎªTmr Svc ÈÎÎñ£¬
#define configUSE_TIMERS 1    //Æô¶¯ÓÃ»§¶¨Ê±Æ÷£¬FreeRTOSÖĞµÄÈí¼ş¶¨Ê±Æ÷
//Èç¹ûconfigUSE_TIMERS ÖÃÎª1£¬Ôò´´½¨µÄTmr SvcÈÎÎñµÄÓÅÏÈ¼¶Îª2
#define configTIMER_TASK_PRIORITY   (2)
#define configTIMER_QUEUE_LENGTH   5 //ÅäÖÃ¶¨Ê±Æ÷ÈÎÎñÓÃµ½µÄÏûÏ¢¶ÓÁĞµÄ´óĞ¡£¬ÄÜ¹»´æ´¢µÄÏûÏ¢¸öÊı
//Ö®Ç°ÉèÖÃµÄ¶¨Ê±Æ÷ÈÎÎñÕ»¿Õ¼ä´óĞ¡Îª  50 £¬µ¼ÖÂ´´½¨2¸ö¶¨Ê±Æ÷ÈÎÎñÊ±£¬³öÏÖTmr SvcÈÎÎñÕ»Òç³ö´íÎó
//ËùÒÔ¸ÄÓÃÏÂÃæµÄ Õ»¿Õ¼ä´óĞ¡Öµ
#define configTIMER_TASK_STACK_DEPTH (configMINIMAL_STACK_SIZE*2) //ÅäÖÃ¶¨Ê±Æ÷ÈÎÎñµÄÈÎÎñÕ»´óĞ¡£¬(ÏµÍ³´´½¨µÄ×îĞ¡¶ÑÕ»Êı*2£©

//¶¨Òå²Ù×÷ÏµÍ³ÖĞ¶ÏÎªstm32¿âµ×²ãÖĞ¶Ï
#define vPortSVCHandler SVC_Handler
#define xPortPendSVHandler PendSV_Handler
#define xPortSysTickHandler SysTick_Handler

//Ìí¼ÓÒ»¸ö²ÎÊı¼ì²â¹¦ÄÜ
#define configASSERT(x)  if((x)==0){taskDISABLE_INTERRUPTS();for(;;);}

#endif /* FREERTOS_CONFIG_H */

