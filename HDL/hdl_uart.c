/*
*********************************************************************************************************
*
*	模块名称 : HDL_UART模块
*	文件名称 : hdl_uart.c
* 版    本 ：v1.0
*	说    明 : 设置stm32的通用异步收发器功能，uart1~uart5
						接收由中断来接收
						缓冲区类型为环形缓冲区，其内部大小由位段 自动控制
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2016-11-28        正式发布
*
*
*********************************************************************************************************
*/

#include "hdl_uart.h"

#if (UART1_EN==1)
	static uint8_t usart1_ring_data[BUFFER_BYTE];
	static uint8_t usart1_recv_data[BUFFER_DATA_LEN];
	static data_t usart1_data;
	static USART_RING_BUF usart1_ring_buf;
	static USART_T usart1_t;
#endif

#if (UART2_EN==1)
	static uint8_t usart2_ring_data[BUFFER_BYTE];
	static uint8_t usart2_recv_data[BUFFER_DATA_LEN];
	static data_t usart2_data;
	static USART_RING_BUF usart2_ring_buf;
	static USART_T usart2_t;
#endif

#if (UART3_EN==1)
	static uint8_t usart3_ring_data[BUFFER_BYTE];
	static uint8_t usart3_recv_data[BUFFER_DATA_LEN];
	static data_t usart3_data;
	static USART_RING_BUF usart3_ring_buf;
	static USART_T usart3_t;
#endif

#if (UART4_EN==1)
	static uint8_t usart4_ring_data[BUFFER_BYTE];
	static uint8_t usart4_recv_data[BUFFER_DATA_LEN];
	static data_t usart4_data;
	static USART_RING_BUF usart4_ring_buf;
	static USART_T usart4_t;
#endif

#if (UART5_EN==1)
	static uint8_t usart5_ring_data[BUFFER_BYTE];
	static uint8_t usart5_recv_data[BUFFER_DATA_LEN];
	static data_t usart5_data;
	static USART_RING_BUF usart5_ring_buf;
	static USART_T usart5_t;
#endif

static void init_usart_var();
static void init_usart_hard();
static void init_usart_nvic();
static void usart_receive(COM_E com,USART_T * usart_t);
static USART_TypeDef * com_to_usart(COM_E com);
static USART_T * com_to_usart_t(COM_E com);

static void usart_send(USART_TypeDef* USARTx,uint8_t dat)
{
	while(!USART_GetFlagStatus(USARTx,USART_FLAG_TXE));	
	USART_SendData(USARTx,dat);

}

/****************************************************************************
* 名    称：void usart_read
* 功    能：将串口环形缓冲区中的数据转存到实际处理的缓冲区中
* 入口参数：usart_ring_buf 环形缓冲区结构体指针，代表具体执行的环形缓冲区
* 出口参数：
* 说    明：
* 调用方法：     
****************************************************************************/ 
static int usart_read_to_data_t(USART_T * usart_t)
{
	uint16_t len=0;
	if(usart_t->buf->tx.bit.index==usart_t->buf->rx.bit.index)
	{
		return 0;
	}
	while(usart_t->buf->tx.bit.index !=usart_t->buf->rx.bit.index)
	{
		//debug_format("the ring buf received buf is %s \r\n",buf->data);
		
		usart_t->data_t->data[usart_t->data_t->len]=usart_t->buf->data[usart_t->buf->tx.bit.index];
		//buffer[len]=buf->data[buf->tx.bit.index];
		usart_t->data_t->len ++;
		usart_t->buf->tx.bit.index++;
		len++;
	}
	//debug_format("the ring buf received buf is %s \r\n",gprs_data.data);
	return len;
}
/****************************************************************************
* 名    称：process_receive
* 功    能：处理串口接收到的数据
* 入口参数：usart_ring_buf 环形缓冲区结构体指针，代表具体执行的环形缓冲区
* 出口参数：
* 说    明：
* 调用方法：     
****************************************************************************/  
void process_receive(COM_E com)
{
	USART_T * usart_t;
	usart_t = com_to_usart_t(com);
	if(usart_t==0)
	{
		return;
	}
	usart_read_to_data_t(usart_t);
}

/*
*********************************************************************************************************
*	函 数 名: hdl_uart_init
*	功能说明: 初始化stm32的使用，包括数据变量，以及硬件通道、接收中断等
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void hdl_uart_init()
{
	init_usart_var();
	init_usart_hard();
	init_usart_nvic();
}
/*
*********************************************************************************************************
*	函 数 名: hdl_uart_send_byte
*	功能说明: 向指定的串口中发送一个字节
*	形    参: COM_E 指COM1~COM5,类似PC机上的串口
*	返 回 值: 无
*********************************************************************************************************
*/
void hdl_uart_send_byte(COM_E com,uint8_t one)
{
	USART_TypeDef * USARTX;
	USARTX=com_to_usart(com);
	if(USARTX==0)
	{
		return;
	}
	usart_send(USARTX,one);
}
/*
*********************************************************************************************************
*	函 数 名: hdl_uart_send_buffer
*	功能说明: 向指定的串口中发送多个字符
*	形    参: COM_E 指COM1~COM5,类似PC机上的串口
						uint8_t * buffer,数据缓冲区
						uint16_t len,数据缓冲区长度
*	返 回 值: 无
*********************************************************************************************************
*/
void hdl_uart_send_buffer(COM_E com,uint8_t * buffer,uint16_t len)
{
	uint16_t i=0;
	USART_TypeDef * USARTX;
	USART_T * usart_t;
	USARTX=com_to_usart(com);
//	usart_t=com_to_usart_t(com);
//	if(usart_t !=0)
//	{
//		if(usart_t->send_before!=NULL)
//		{
//			usart_t->send_before();
//		}
//	}
	
	if(USARTX==0)
	{
		return;
	}
	
	for(i=0;i<len;i++)
	{
		usart_send(USARTX,buffer[i]);
	}
	
//	if(usart_t != 0)
//	{
//		if(usart_t->send_over != NULL)
//		{
//			usart_t->send_over();
//		}
//	}
}
/*
*********************************************************************************************************
*	函 数 名: hdl_uart_send_string
*	功能说明: 向指定的串口中发送多个字符
*	形    参: COM_E 指COM1~COM5,类似PC机上的串口
						uint8_t * buffer,数据缓冲区
						uint16_t len,数据缓冲区长度
*	返 回 值: 无
*********************************************************************************************************
*/
void hdl_uart_send_string(COM_E com,char * buffer,uint16_t len)
{
	hdl_uart_send_buffer(com,(uint8_t*)buffer,len);
}

/*
*********************************************************************************************************
*	函 数 名: init_usart_var
*	功能说明: 初始化通用异步收发器相关操作的变量，主要是可用于环形缓冲区操作的各个变量
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void init_usart_var()
{
	#if (UART1_EN==1)
	usart1_ring_buf.data=usart1_ring_data;
	usart1_data.buffer_len_max=BUFFER_DATA_LEN;
	usart1_data.len=0;
	usart1_data.data=usart1_recv_data;
	
	if(usart1_data.data == NULL)
	{
		usart1_data.data=malloc(BUFFER_DATA_LEN);
	}
	memset(usart1_data.data,0,BUFFER_DATA_LEN);
	usart1_t.buf=&usart1_ring_buf;
	usart1_t.data_t=&usart1_data;
	usart1_t.send_before=NULL;
	usart1_t.send_over=NULL;
	#endif
	
	#if (UART2_EN==1)
	usart2_ring_buf.data=usart2_ring_data;
	usart2_data.buffer_len_max=BUFFER_DATA_LEN;
	usart2_data.len=0;
	usart2_data.data=usart2_recv_data;

	if(usart2_data.data == NULL)
	{
		usart2_data.data=malloc(BUFFER_DATA_LEN);
	}
	memset(usart2_data.data,0,BUFFER_DATA_LEN);
	usart2_t.buf=&usart2_ring_buf;
	usart2_t.data_t=&usart2_data;
	usart2_t.send_before=NULL;
	usart2_t.send_over=NULL;
	#endif
	
	
	#if (UART3_EN==1)
	usart3_ring_buf.data=usart3_ring_data;
	usart3_data.buffer_len_max=BUFFER_DATA_LEN;
	usart3_data.len=0;
	usart3_data.data=usart3_recv_data;

	if(usart3_data.data == NULL)
	{
		usart3_data.data=malloc(BUFFER_DATA_LEN);
	}
	memset(usart3_data.data,0,BUFFER_DATA_LEN);
	usart3_t.buf=&usart3_ring_buf;
	usart3_t.data_t=&usart3_data;
	usart3_t.send_before=NULL;
	usart3_t.send_over=NULL;
	#endif
	
	
	#if (UART4_EN==1)
	usart4_ring_buf.data=usart4_ring_data;
	usart4_data.buffer_len_max=BUFFER_DATA_LEN;
	usart4_data.len=0;
	usart4_data.data=usart4_recv_data;
	
	if(usart4_data.data == NULL)
	{
		usart4_data.data=malloc(BUFFER_DATA_LEN);
	}
	memset(usart4_data.data,0,BUFFER_DATA_LEN);
	usart4_t.buf=&usart4_ring_buf;
	usart4_t.data_t=&usart4_data;
	usart4_t.send_before=NULL;
	usart4_t.send_over=NULL;
	#endif
	
	#if (UART5_EN==1)
	usart5_ring_buf.data=usart5_ring_data;
	usart5_data.buffer_len_max=BUFFER_DATA_LEN;
	usart5_data.len=0;
	usart5_data.data=usart5_recv_data;
	
	if(usart5_data.data == NULL)
	{
		usart5_data.data=malloc(BUFFER_DATA_LEN);
	}
	memset(usart5_data.data,0,BUFFER_DATA_LEN);
	usart5_t.buf=&usart5_ring_buf;
	usart5_t.data_t=&usart5_data;
	usart5_t.send_before=NULL;
	usart5_t.send_over=NULL;
	#endif
	
}

/*
*********************************************************************************************************
*	函 数 名: init_usart_hard
*	功能说明: 初始化通用异步收发器相关硬件操作设置
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void init_usart_hard()
{
	USART_InitTypeDef USART_InitStruct;
	//NVIC_InitTypeDef NVIC_InitStruct;	
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	
	#if (UART1_EN==1) //PA9 ,PA10,PA9为TX，PA10为RX
	hdl_set_rcc(RCC_APB2Periph_GPIOA,ENABLE,2);
	hdl_set_rcc(RCC_APB2Periph_USART1,ENABLE,2);
	hdl_output_gpio(GPIOA,GPIO_Pin_9,GPIO_Mode_AF_PP);
	hdl_input_gpio(GPIOA,GPIO_Pin_10,GPIO_Mode_IN_FLOATING);
	
	USART_ClearFlag(USART1, USART_IT_RXNE);
	USART_InitStruct.USART_BaudRate  = 19200;
	USART_InitStruct.USART_WordLength  = USART_WordLength_8b;
	USART_InitStruct.USART_StopBits  = USART_StopBits_1;
	USART_InitStruct.USART_Parity  = USART_Parity_No;
	USART_InitStruct.USART_Mode  = USART_Mode_Rx|USART_Mode_Tx;
	USART_InitStruct.USART_HardwareFlowControl  = USART_HardwareFlowControl_None;
	USART_Init(USART1,&USART_InitStruct);
	//使能接收中断
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	USART_Cmd(USART1,ENABLE);
		/* CPU的小缺陷：串口配置好，如果直接Send，则第1个字节发送不出去
		如下语句解决第1个字节无法正确发送出去的问题 */
	USART_ClearFlag(USART1, USART_FLAG_TC);     /* 清发送完成标志，Transmission Complete flag */
	#endif
	
	#if (UART2_EN==1) //PA2 ,PA3,PA2为TX，PA3为RX
	hdl_set_rcc(RCC_APB2Periph_GPIOA,ENABLE,2);
	hdl_set_rcc(RCC_APB1Periph_USART2,ENABLE,1);
	hdl_output_gpio(GPIOA,GPIO_Pin_2,GPIO_Mode_AF_PP);
	hdl_input_gpio(GPIOA,GPIO_Pin_3,GPIO_Mode_IN_FLOATING);
	
	USART_ClearFlag(USART2, USART_IT_RXNE);
	USART_InitStruct.USART_BaudRate  = 115200;
	USART_InitStruct.USART_WordLength  = USART_WordLength_8b;
	USART_InitStruct.USART_StopBits  = USART_StopBits_1;
	USART_InitStruct.USART_Parity  = USART_Parity_No;
	USART_InitStruct.USART_Mode  = USART_Mode_Rx|USART_Mode_Tx;
	USART_InitStruct.USART_HardwareFlowControl  = USART_HardwareFlowControl_None;
	USART_Init(USART2,&USART_InitStruct);
	//使能接收中断
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
	USART_Cmd(USART2,ENABLE);
		/* CPU的小缺陷：串口配置好，如果直接Send，则第1个字节发送不出去
		如下语句解决第1个字节无法正确发送出去的问题 */
	USART_ClearFlag(USART2, USART_FLAG_TC);     /* 清发送完成标志，Transmission Complete flag */
	#endif
	
	
	#if (UART3_EN==1) //PB10 ,PB11,PB10为TX，PB11为RX
	hdl_set_rcc(RCC_APB2Periph_GPIOB,ENABLE,2);
	hdl_set_rcc(RCC_APB1Periph_USART3,ENABLE,1);
	hdl_output_gpio(GPIOB,GPIO_Pin_10,GPIO_Mode_AF_PP);
	hdl_input_gpio(GPIOB,GPIO_Pin_11,GPIO_Mode_IN_FLOATING);
	
	USART_ClearFlag(USART3, USART_IT_RXNE);
	USART_InitStruct.USART_BaudRate  = 9600;
	USART_InitStruct.USART_WordLength  = USART_WordLength_8b;
	USART_InitStruct.USART_StopBits  = USART_StopBits_1;
	USART_InitStruct.USART_Parity  = USART_Parity_No;
	USART_InitStruct.USART_Mode  = USART_Mode_Rx|USART_Mode_Tx;
	USART_InitStruct.USART_HardwareFlowControl  = USART_HardwareFlowControl_None;
	USART_Init(USART3,&USART_InitStruct);
	//使能接收中断
	USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);
	USART_Cmd(USART3,ENABLE);
		/* CPU的小缺陷：串口配置好，如果直接Send，则第1个字节发送不出去
		如下语句解决第1个字节无法正确发送出去的问题 */
	USART_ClearFlag(USART3, USART_FLAG_TC);     /* 清发送完成标志，Transmission Complete flag */
	#endif
	
	
	#if (UART4_EN==1) //PC10 ,PC11,PC10为TX，PC11为RX
	hdl_set_rcc(RCC_APB2Periph_GPIOC,ENABLE,2);
	hdl_set_rcc(RCC_APB1Periph_UART4,ENABLE,1);
	hdl_output_gpio(GPIOC,GPIO_Pin_10,GPIO_Mode_AF_PP);
	hdl_input_gpio(GPIOC,GPIO_Pin_11,GPIO_Mode_IN_FLOATING);
	
	USART_ClearFlag(UART4, USART_IT_RXNE);
	USART_InitStruct.USART_BaudRate  = 115200;
	USART_InitStruct.USART_WordLength  = USART_WordLength_8b;
	USART_InitStruct.USART_StopBits  = USART_StopBits_1;
	USART_InitStruct.USART_Parity  = USART_Parity_No;
	USART_InitStruct.USART_Mode  = USART_Mode_Rx|USART_Mode_Tx;
	USART_InitStruct.USART_HardwareFlowControl  = USART_HardwareFlowControl_None;
	USART_Init(UART4,&USART_InitStruct);
	//使能接收中断
	USART_ITConfig(UART4,USART_IT_RXNE,ENABLE);
	USART_Cmd(UART4,ENABLE);
		/* CPU的小缺陷：串口配置好，如果直接Send，则第1个字节发送不出去
		如下语句解决第1个字节无法正确发送出去的问题 */
	USART_ClearFlag(UART4, USART_FLAG_TC);     /* 清发送完成标志，Transmission Complete flag */
	#endif
	
	
	#if (UART5_EN==1) //PC12 ,PD2,PC12为TX，PD2为RX
	hdl_set_rcc(RCC_APB2Periph_GPIOC,ENABLE,2);
	hdl_set_rcc(RCC_APB1Periph_UART5,ENABLE,1);
	hdl_set_rcc(RCC_APB2Periph_GPIOD,ENABLE,2);
	hdl_output_gpio(GPIOC,GPIO_Pin_12,GPIO_Mode_AF_PP);
	hdl_input_gpio(GPIOD,GPIO_Pin_2,GPIO_Mode_IN_FLOATING);
	
	USART_ClearFlag(UART5, USART_IT_RXNE);
	USART_InitStruct.USART_BaudRate  = 115200;
	USART_InitStruct.USART_WordLength  = USART_WordLength_8b;
	USART_InitStruct.USART_StopBits  = USART_StopBits_1;
	USART_InitStruct.USART_Parity  = USART_Parity_No;
	USART_InitStruct.USART_Mode  = USART_Mode_Rx|USART_Mode_Tx;
	USART_InitStruct.USART_HardwareFlowControl  = USART_HardwareFlowControl_None;
	USART_Init(UART5,&USART_InitStruct);
	//使能接收中断
	USART_ITConfig(UART5,USART_IT_RXNE,ENABLE);
	USART_Cmd(UART5,ENABLE);
		/* CPU的小缺陷：串口配置好，如果直接Send，则第1个字节发送不出去
		如下语句解决第1个字节无法正确发送出去的问题 */
	USART_ClearFlag(UART5, USART_FLAG_TC);     /* 清发送完成标志，Transmission Complete flag */
	#endif
}

/*
*********************************************************************************************************
*	函 数 名: init_usart_nvic
*	功能说明: 初始化通用异步收发器接收中断相关设置
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void init_usart_nvic()
{
	NVIC_InitTypeDef NVIC_InitStructure;
	#if UART1_EN==1
	/* 使能串口1中断 */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	#endif
	
	#if UART2_EN==1
	/* 使能串口2中断 */
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	#endif
	
	#if UART3_EN==1
	/* 使能串口3中断t */
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	#endif
	
	#if UART4_EN==1
	/* 使能串口4中断t */
	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	#endif
	
	#if UART5_EN==1
	/* 使能串口5中断t */
	NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	#endif
}

/*
*********************************************************************************************************
*	函 数 名: USART1_IRQHandler
*	功能说明: 通用串口1的中断数据接收函数
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
#if UART1_EN==1
void USART1_IRQHandler(void)
{
	usart_receive(COM1,&usart1_t);
	//usart_receive(USART1,&usart1_ring_buf);
}
#endif
/*
*********************************************************************************************************
*	函 数 名: USART2_IRQHandler
*	功能说明: 通用串口2的中断数据接收函数
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
#if UART2_EN==1
void USART2_IRQHandler(void)
{

	usart_receive(COM2,&usart2_t);

	//usart_receive(USART2,&usart2_ring_buf);
}
#endif
/*
*********************************************************************************************************
*	函 数 名: USART3_IRQHandler
*	功能说明: 通用串口3的中断数据接收函数
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
#if UART3_EN==1
void USART3_IRQHandler(void)
{
	usart_receive(COM3,&usart3_t);
	//set_gpio_high();
	//usart_receive(USART3,&usart3_ring_buf);
	//char c;
		//RxBuffer[RxCounter++] = USART_ReceiveData(USART3);   
		//c = (char)(USART_ReceiveData(USART3)&0xff);
		//usart_send(USART3,c);
}
#endif
/*
*********************************************************************************************************
*	函 数 名: UART4_IRQHandler
*	功能说明: 通用串口4的中断数据接收函数
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
#if UART4_EN==1
void UART4_IRQHandler(void)
{
	usart_receive(COM4,&usart4_t);
	//set_gpio_high();
	//usart_receive(UART4,&usart4_ring_buf);
}
#endif
/*
*********************************************************************************************************
*	函 数 名: UART5_IRQHandler
*	功能说明: 通用串口5的中断数据接收函数
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
#if UART5_EN==1
void UART5_IRQHandler(void)
{
	usart_receive(COM5,&usart5_t);
}
#endif

/*
*********************************************************************************************************
*	函 数 名: com_to_usart_t
*	功能说明: 将COM端口号转换为 USART_T* 指针类型
*	形    参: _ucPort: 端口号(COM1 - COM6)
*	返 回 值: USART_T * ，&usart1_t,&usart2_t,&usart3_t,&usart4_t,&usart5_t
*********************************************************************************************************
*/
static USART_T * com_to_usart_t(COM_E com)
{
	if(com==COM1)
	{
		#if (UART1_EN==1)
		return &usart1_t;
		#else 
		return 0;
		#endif
	}
	else if(com==COM2)
	{
		#if (UART2_EN==1)
		return &usart2_t;
		#else 
		return 0;
		#endif
	}
	else if(com==COM3)
	{
		#if (UART3_EN==1)
		return &usart3_t;
		#else 
		return 0;
		#endif
	}
	else if(com==COM4)
	{
		#if (UART4_EN==1)
		return &usart4_t;
		#else 
		return 0;
		#endif
	}
	else if(com==COM5)
	{
		#if (UART5_EN==1)
		return &usart5_t;
		#else 
		return 0;
		#endif
	}
	else 
	{
		return 0;
	}
}
/*
*********************************************************************************************************
*	函 数 名: com_to_usart
*	功能说明: 将COM端口号转换为 USART_TypeDef* USARTx
*	形    参: _ucPort: 端口号(COM1 - COM6)
*	返 回 值: USART_TypeDef*,  USART1, USART2, USART3, UART4, UART5
*********************************************************************************************************
*/
static USART_TypeDef * com_to_usart(COM_E com)
{
	if(com==COM1)
	{
		#if (UART1_EN==1)
		return USART1;
		#else 
		return 0;
		#endif
	}
	else if(com==COM2)
	{
		#if (UART2_EN==1)
		return USART2;
		#else 
		return 0;
		#endif
	}
	else if(com==COM3)
	{
		#if (UART3_EN==1)
		return USART3;
		#else 
		return 0;
		#endif
	}
	else if(com==COM4)
	{
		#if (UART4_EN==1)
		return UART4;
		#else 
		return 0;
		#endif
	}
	else if(com==COM5)
	{
		#if (UART5_EN==1)
		return UART5;
		#else 
		return 0;
		#endif
	}
	else 
	{
		return 0;
	}
}

/*
*********************************************************************************************************
*	函 数 名: usart_receive
*	功能说明: 将串口中断中接收到的数据存放入各个环形缓冲区中
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void usart_receive(COM_E com,USART_T * usart_t)
{
	uint8_t byte=0x00;
	USART_TypeDef* usartX;
	usartX=com_to_usart(com);
	if(usartX==0)
	{
		return;
	}
	if(USART_GetITStatus(usartX, USART_IT_RXNE) != RESET)	   
	{	
			byte = (USART_ReceiveData(usartX)&0xff);
			if((usart_t->buf->rx.bit.index+1)!=(usart_t->buf->tx.bit.index))
			{
				usart_t->buf->data[usart_t->buf->rx.bit.index]=byte;
				
			}	
			usart_t->buf->rx.bit.index++;
	}
	if(USART_GetITStatus(usartX, USART_IT_TXE) != RESET)                  
	{ 
	 USART_ITConfig(usartX, USART_IT_TXE, DISABLE);					    
	}	
}