/*
*********************************************************************************************************
*
*	模块名称 : M_MENU模块
*	文件名称 : m_menu.h
*	说    明 : 按键与屏联动操作所需要的menu菜单操作相关的数据结构
*
*
*********************************************************************************************************
*/
#ifndef __M_MENU_H
#define __M_MENU_H

#include "hdl_include.h"

#define menu_action_len 13  //在一个屏幕中，所对应的按键动作数组长度
#define menu_content_len 8  //一个屏幕中最多显示条目数据
#define clear_row_len 5  

#define menu_actions_len 255 //系统配置的最大的菜单动作数组长度值

typedef struct menu_content 
{
	uint8_t row; //显示在第几行
	uint8_t col;  //显示在第几列
	uint8_t character_flag;   //字符标识，0:写汉字  1:写数字与英文
	uint8_t reverse;          //是否反转
	uint8_t * content;  //所要显示的内容
}m_menu_content;
/*
*每一个屏中的动作，都有10个处理函数，没有的写0，并且排列序号为
*/
typedef struct menu_item
{
	uint8_t range_from;    //显示范围起始
	uint8_t range_to;      //显示范围结束
	uint8_t item_count;    //显示条目总数
	int8_t selected;      //当前选中的位置
	uint8_t clear_flag;   //为1在作屏幕跳转时，清屏，为2在作屏幕跳转时，清行
	int8_t clear_row_num[clear_row_len]; //清行号数组，表示行号，-1为不清除
	//void (*start_timer)(); //启动菜单内部定时器 初始为NULL
	//void (*stop_timer)();  //停止菜单内部定时器 初始为NULL
	uint16_t  menu_action_index[menu_action_len]; //动作
	m_menu_content content[menu_content_len];   //显示内容

	m_menu_content* title;
	struct menu_item ** children; //子菜单
	struct menu_item * parent;    //父菜单
	
}m_menu;

typedef struct
{
	uint16_t menu_action_index; //菜单动作索引号-------
	void (* call_back)();       //菜单动作的回调函数
}m_menu_action_t;  


void m_process_menu_actions(uint16_t  menu_action_index,m_menu_action_t * t);

#endif