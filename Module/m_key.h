/*
*********************************************************************************************************
*
*	模块名称 : M_KEY模块
*	文件名称 : m_key.h
*	说    明 :矩阵式按键驱动程序，数据结构、扫描算法、相应的按键处理程序
*
*
*********************************************************************************************************
*/
#ifndef __M_KEY_H
#define __M_KEY_H

#include "hdl_include.h"

/*********************************
	结构体
*********************************/
typedef struct
{
	GPIO_TypeDef * GPIO_x; //端口类型 A,B,C,D,E
	uint16_t GPIO_Pin;     //端口号
}m_key_io;

//按键动作结构体
typedef struct 
{
	uint8_t key_flag;    //按键是否被按下标识 
	void (*call_back)(void);  //回调函数
}m_key_action_t;

//矩阵按键结构体
typedef struct
{
	
	uint8_t input_port_len; //输入管脚总数
	uint8_t output_port_len; //输出管脚总数
	m_key_io * input_keys;   //所有的输入管脚
	m_key_io * output_keys;  //所有的输出管脚
	m_key_action_t * actions; //矩阵按键动作 ,这个数组的长度应该为input_port_len* output_port_len
	
}m_matrix_keys;

void m_init_matrix_keys( m_matrix_keys * keys);

void m_matrix_scan(m_matrix_keys * keys);

void m_matrix_process(m_matrix_keys * keys);

#endif