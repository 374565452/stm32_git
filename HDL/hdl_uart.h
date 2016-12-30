/*
*********************************************************************************************************
*
*	模块名称 : HDL_UART模块
*	文件名称 : hdl_uart.h
*	说    明 :设置stm32的通用异步收发器功能，uart1~uart5
						接收由中断来接收
						缓冲区类型为环形缓冲区，其内部大小由位段 自动控制
*
*
*********************************************************************************************************
*/

#ifndef __HDL_UART_H
#define __HDL_UART_H

#include "hdl_include.h"

#define BUF_512_EN 1

#define UART1_EN 1
#define UART2_EN 0
#define UART3_EN 1
#define UART4_EN 0
#define UART5_EN 0

#if BUF_128_EN==1 
	#define BUFFER_BYTE 128 
	#define BUFFER_DATA_LEN 128
	#define BUF_T buf_128_t
	#define USART_RING_BUF usart_buf_128
	#define USART_T usart_128_t
#endif

#if BUF_256_EN ==1 
	#define BUFFER_BYTE 256 
	#define BUFFER_DATA_LEN 256
	#define BUF_T buf_253_t
	#define USART_RING_BUF usart_buf_256
	#define USART_T usart_256_t
#endif

#if BUF_512_EN ==1
	#define BUFFER_BYTE 512 
	#define BUFFER_DATA_LEN 512
	#define BUF_T buf_512_t
	#define USART_RING_BUF usart_buf_512
	#define USART_T usart_512_t
#endif

#if BUF_1024_EN ==1
	#define BUFFER_BYTE 1024 
	#define BUFFER_DATA_LEN 1024
	#define BUF_T buf_1024_t
	#define USART_RING_BUF usart_buf_1024
	#define USART_T usart_1024_t
#endif

#if BUF_2048_EN ==1
	#define BUFFER_BYTE 2048 
	#define BUFFER_DATA_LEN 2048
	#define BUF_T buf_2048_t
	#define USART_RING_BUF usart_buf_2048
	#define USART_T usart_2048_t
#endif

#if BUF_4096_EN ==1
	#define BUFFER_BYTE 4096 
	#define BUFFER_DATA_LEN 4096
	#define BUF_T buf_4096_t
	#define USART_RING_BUF usart_buf_4096
	#define USART_T usart_4096_t
#endif


/* 定义端口号 */
typedef enum
{
	COM1 = 0,	/* USART1  PA9, PA10 */
	COM2 = 1,	/* USART2,  PA2, PA3 */
	COM3 = 2,	/* USART3, PB10, PB11 */
	COM4 = 3,	/* UART4, PC10, PC11 */
	COM5 = 4,	/* UART5, PC12, PD2 */
}COM_E;
/***7bit 128byte***/
typedef union
{
	struct
	{
		unsigned  index   : 7 ;
	}bit;
	uint16_t max ;
}buf_128_t;

/***8bit 256byte***/
typedef union
{
	struct
	{
		unsigned  index   : 8 ;
	}bit;
	uint16_t max ;
}buf_256_t;

/***9bit 512byte***/
typedef union
{
	struct
	{
		unsigned  index   : 9 ;
	}bit;
	uint16_t max ;
}buf_512_t;

/********10bits 1024byte****/
typedef union
{
	struct 
	{
		unsigned index : 10;
	}bit;
	uint16_t max ;
}buf_1024_t ;	


/********11bits 2048byte****/
typedef union
{
	struct 
	{
		unsigned index : 11;
	}bit;
	uint16_t max ;
}buf_2048_t ;	

/********12bits 4096byte****/
typedef union
{
	struct 
	{
		unsigned index : 12;
	}bit;
	uint16_t max ;
}buf_4096_t ;	
/*设置由中断接收后的数据存放的缓冲区，将uart中断接收的数据存放*/
typedef struct 
{
	uint16_t               buffer_len_max;       //最长数据长度
	uint16_t               len;              //当前数据长度
	uint8_t                *data;                //数据指针
}data_t;

/***缓存设置***/
typedef struct
{
	uint8_t     *data; //数据
	buf_128_t   rx;  //接收
	buf_128_t   tx;  //发送
	uint16_t    buf_len_max; //最大长度
}usart_buf_128;

typedef struct
{
	uint8_t     *data; //数据
	buf_256_t   rx;  //接收
	buf_256_t   tx;  //发送
	uint16_t    buf_len_max; //最大长度
}usart_buf_256;

typedef struct
{
	uint8_t     *data; //数据
	buf_512_t   rx;  //接收
	buf_512_t   tx;  //发送
	uint16_t    buf_len_max; //最大长度

}usart_buf_512;


typedef struct
{
	uint8_t     *data; //数据
	buf_1024_t   rx;  //接收
	buf_1024_t   tx;  //发送
	uint16_t    buf_len_max; //最大长度
}usart_buf_1024;


typedef struct
{
	uint8_t     *data; //数据
	buf_2048_t   rx;  //接收
	buf_2048_t   tx;  //发送
	uint16_t    buf_len_max; //最大长度
}usart_buf_2048;

typedef struct
{
	uint8_t     *data; //数据
	buf_4096_t   rx;  //接收
	buf_4096_t   tx;  //发送
	uint16_t    buf_len_max; //最大长度
	
}usart_buf_4096;

typedef struct
{
	usart_buf_128 *buf;
	data_t      *data_t; //数据处理后放置的缓冲区 
	
	void(*send_before)(); //数据发送之前调用
	void(*send_over)();   //数据发送完成后调用
	
}usart_128_t;


typedef struct
{
	usart_buf_256 *buf;
	data_t      *data_t; //数据处理后放置的缓冲区 
	void(*send_before)(); //数据发送之前调用
	void(*send_over)();   //数据发送完成后调用
}usart_256_t;


typedef struct
{
	usart_buf_512 *buf;
	data_t      *data_t; //数据处理后放置的缓冲区 
	void(*send_before)(); //数据发送之前调用
	void(*send_over)();   //数据发送完成后调用
}usart_512_t;


typedef struct
{
	usart_buf_1024 *buf;
	data_t      *data_t; //数据处理后放置的缓冲区 
	void(*send_before)(); //数据发送之前调用
	void(*send_over)();   //数据发送完成后调用
}usart_1024_t;


typedef struct
{
	usart_buf_2048 *buf;
	data_t      *data_t; //数据处理后放置的缓冲区 
	
	void(*send_before)(); //数据发送之前调用
	void(*send_over)();   //数据发送完成后调用
}usart_2048_t;


typedef struct
{
	usart_buf_4096 *buf;
	data_t      *data_t; //数据处理后放置的缓冲区 
	void(*send_before)(); //数据发送之前调用
	void(*send_over)();   //数据发送完成后调用
}usart_4096_t;

void hdl_uart_init();
void hdl_uart_send_byte(COM_E com,uint8_t one);
void hdl_uart_send_buffer(COM_E com,uint8_t * buffer,uint16_t len);
void hdl_uart_send_string(COM_E com,char * buffer,uint16_t len);
void process_receive(COM_E com);
#endif