/*
*********************************************************************************************************
*
*	ģ������ : M_KEYģ��
*	�ļ����� : m_key.h
*	˵    �� :����ʽ���������������ݽṹ��ɨ���㷨����Ӧ�İ����������
*
*
*********************************************************************************************************
*/
#ifndef __M_KEY_H
#define __M_KEY_H

#include "hdl_include.h"

/*********************************
	�ṹ��
*********************************/
typedef struct
{
	GPIO_TypeDef * GPIO_x; //�˿����� A,B,C,D,E
	uint16_t GPIO_Pin;     //�˿ں�
}m_key_io;

//���������ṹ��
typedef struct 
{
	uint8_t key_flag;    //�����Ƿ񱻰��±�ʶ 
	void (*call_back)(void);  //�ص�����
}m_key_action_t;

//���󰴼��ṹ��
typedef struct
{
	
	uint8_t input_port_len; //����ܽ�����
	uint8_t output_port_len; //����ܽ�����
	m_key_io * input_keys;   //���е�����ܽ�
	m_key_io * output_keys;  //���е�����ܽ�
	m_key_action_t * actions; //���󰴼����� ,�������ĳ���Ӧ��Ϊinput_port_len* output_port_len
	
}m_matrix_keys;

void m_init_matrix_keys( m_matrix_keys * keys);

void m_matrix_scan(m_matrix_keys * keys);

void m_matrix_process(m_matrix_keys * keys);

#endif