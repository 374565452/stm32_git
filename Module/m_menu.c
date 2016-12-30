/*
*********************************************************************************************************
*
*	模块名称 : M_MENU模块
*	文件名称 : m_menu.c
*	说    明 : 按键与屏联动操作所需要的menu菜单操作相关的数据结构
*
*   注： 此模块中，只是定义一些菜单操作的简易数据结构，具体的业务实现还需要另外写
*         具体的业务中可以采用此模块中定义的数据结构
*
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2016-11-28        正式发布
*
*********************************************************************************************************
*/
#include "m_menu.h"

/*
*********************************************************************************************************
*	函 数 名: hdl_output_gpio_
*	功能说明: 在指定屏幕中，查找到当前按键动作所对应的菜单业务执行方法
*	形    参: uint16_t menu_action_index 菜单动作索引，这个值应该配置在m_menu结构体对象的menu_action_index数组中
*           m_menu_action_t*t,t为外部指定的菜单动作数组，其数组的最大长度为menu_actions_len，
						但在定义的时候，有些业务达不到这么长的数据长度，m_menu_action_t结构体对象的menu_action_index设置为0 ，
						  同时CALL_BACK函数指针设置为NULL，这样就可以在执行循环的过程中不需要每次都要循环执行255次
							
						例： menu_action_index menu_actions[menu_actions_len]=
								{
									{0,NULL}  //数组中的最后一个对象必定是此元素
								}
*	返 回 值: 无
* 
*********************************************************************************************************
*/
void m_process_menu_actions(uint16_t menu_action_index,m_menu_action_t * t)
{
	uint16_t i=0;
	if(menu_action_index ==0)
	{
		return;
	}
	if(menu_action_index >=menu_actions_len)
	{
		return;
	}
	for(i=0;i<menu_actions_len;i++)
	{
		if(t[i].menu_action_index==0) //此数组中没有定义可操作的菜单操作数组
		{
			return;
		}
		//找到相应的动作，直接调用回调函数，然后调出循环
		if(t[i].menu_action_index == menu_action_index)
		{
			if(t[i].call_back != NULL)
			{
				t[i].call_back();
			}
			break;
		}
		
	}
}