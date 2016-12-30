/*
*********************************************************************************************************
*
*	ģ������ : M_MENUģ��
*	�ļ����� : m_menu.h
*	˵    �� : ��������������������Ҫ��menu�˵�������ص����ݽṹ
*
*
*********************************************************************************************************
*/
#ifndef __M_MENU_H
#define __M_MENU_H

#include "hdl_include.h"

#define menu_action_len 13  //��һ����Ļ�У�����Ӧ�İ����������鳤��
#define menu_content_len 8  //һ����Ļ�������ʾ��Ŀ����
#define clear_row_len 5  

#define menu_actions_len 255 //ϵͳ���õ����Ĳ˵��������鳤��ֵ

typedef struct menu_content 
{
	uint8_t row; //��ʾ�ڵڼ���
	uint8_t col;  //��ʾ�ڵڼ���
	uint8_t character_flag;   //�ַ���ʶ��0:д����  1:д������Ӣ��
	uint8_t reverse;          //�Ƿ�ת
	uint8_t * content;  //��Ҫ��ʾ������
}m_menu_content;
/*
*ÿһ�����еĶ���������10����������û�е�д0�������������Ϊ
*/
typedef struct menu_item
{
	uint8_t range_from;    //��ʾ��Χ��ʼ
	uint8_t range_to;      //��ʾ��Χ����
	uint8_t item_count;    //��ʾ��Ŀ����
	int8_t selected;      //��ǰѡ�е�λ��
	uint8_t clear_flag;   //Ϊ1������Ļ��תʱ��������Ϊ2������Ļ��תʱ������
	int8_t clear_row_num[clear_row_len]; //���к����飬��ʾ�кţ�-1Ϊ�����
	//void (*start_timer)(); //�����˵��ڲ���ʱ�� ��ʼΪNULL
	//void (*stop_timer)();  //ֹͣ�˵��ڲ���ʱ�� ��ʼΪNULL
	uint16_t  menu_action_index[menu_action_len]; //����
	m_menu_content content[menu_content_len];   //��ʾ����

	m_menu_content* title;
	struct menu_item ** children; //�Ӳ˵�
	struct menu_item * parent;    //���˵�
	
}m_menu;

typedef struct
{
	uint16_t menu_action_index; //�˵�����������-------
	void (* call_back)();       //�˵������Ļص�����
}m_menu_action_t;  


void m_process_menu_actions(uint16_t  menu_action_index,m_menu_action_t * t);

#endif