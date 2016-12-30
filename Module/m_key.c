/*
*********************************************************************************************************
*
*	模块名称 : M_KEY模块
*	文件名称 : m_key.c
*	说    明 :矩阵式按键驱动程序，数据结构、扫描算法、相应的按键处理程序
*
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2016-12-1           正式发布
*********************************************************************************************************
*/
#include "m_key.h"

#define key_action_index_init  -1

// 按键被按下标识
#define key_pressed_flag 1
//按键第一次被按下时，记录此按键索引。再检测一次，如两次相同，则判定此按键被按下
static int8_t key_action_before = key_action_index_init;
//被按下按键的所在的索引号
static uint8_t key_action_index=0;
/*
*********************************************************************************************************
*	函 数 名: m_init_matrix_keys
*	功能说明: 矩阵按键输入与输出端口初始化操作
*	形    参: m_matrix_keys * keys 封装的用于矩阵按键扫描算法的结构体
*	返 回 值: 无
* 
*********************************************************************************************************
*/
void m_init_matrix_keys( m_matrix_keys * keys)
{
	uint8_t i=0;
	for(i=0;i<keys->input_port_len ;i++)
	{
		hdl_input_gpio(keys->input_keys[i].GPIO_x,keys->input_keys[i].GPIO_Pin,GPIO_Mode_IPU); //所有的输入定义为上拉输入
	}
	for(i=0;i<keys->output_port_len;i++)
	{
		hdl_output_gpio(keys->output_keys[i].GPIO_x,keys->output_keys[i].GPIO_Pin,GPIO_Mode_Out_PP); //推挽输出
		hdl_h(keys->output_keys[i].GPIO_x,keys->output_keys[i].GPIO_Pin); //将所有的输出管脚置1，拉高
	}
	
}
/*
*********************************************************************************************************
*	函 数 名: m_matrix_scan
*	功能说明: 矩阵按键扫描算法，逐一拉低每一根输出线，读此输入线上的管脚
*	形    参: m_matrix_keys * keys 封装的用于矩阵按键扫描算法的结构体
*	返 回 值: 无
* 注：
*   此方法要进行消抖操作
*********************************************************************************************************
*/
void m_matrix_scan(m_matrix_keys * keys)
{
	uint8_t i=0;
	uint8_t j=0;
	uint8_t value=0;
	key_action_index=0;
	//debug_format("the receive len ---------------------- \r\n");
	for(i=0;i<keys->output_port_len;i++)
	{
		hdl_l(keys->output_keys[i].GPIO_x,keys->output_keys[i].GPIO_Pin); //拉低一条线
		for(j=0;j<keys->input_port_len;j++)
		{
			value=hdl_read(keys->input_keys[j].GPIO_x,keys->input_keys[j].GPIO_Pin); //逐一读输入
			if(value==0)
			{
				//此处就是进行的消抖操作，在扫描间隔时间内连续读取两次
				if(key_action_index == key_action_before) //判断当前按键的索引与之前按下的是否为同一个按键
				{
					//此处也可以直接进行按键动作的执行，并不需要将标致位置1，然后再执行m_matrix_process进行按键动作处理
					//两种方式的取舍，看具体的应用场景来定制
					//如果采用RTOS实时操作系统，这里可以发送一个信号来通知按键处理程序有按键被按下
//					if(keys->actions[key_action_index].call_back != NULL)
//					{
//						keys->actions[key_action_index].call_back();
//					}
					keys->actions[key_action_index].key_flag=key_pressed_flag; //如果一样，将按键动作标识位置1
					key_action_before=key_action_index_init;
				}else
				{
					key_action_before=key_action_index;
				}
				//在return 语句之前，一定要将这条线拉高
				hdl_h(keys->output_keys[i].GPIO_x,keys->output_keys[i].GPIO_Pin);
				return;
			}
			key_action_index++;
		}
		hdl_h(keys->output_keys[i].GPIO_x,keys->output_keys[i].GPIO_Pin); //逐一读完后，将这一条线拉高
	}
	key_action_before=key_action_index_init;
}

/*
*********************************************************************************************************
*	函 数 名: m_matrix_process
*	功能说明: 矩阵按键动作执行 ，定义此方法的主要目的：可以在执行按键动作之前执行一些其他的动作，比如打开LCD背光等 
*	形    参: m_matrix_keys * keys 封装的用于矩阵按键扫描算法的结构体
*	返 回 值: 无
* 注：
*********************************************************************************************************
*/
void m_matrix_process(m_matrix_keys * keys)
{
	uint8_t i=0;
	uint16_t key_action_len =(keys->input_port_len) * (keys->output_port_len);       
	for(i=0;i<key_action_len;i++)
	{
		if(keys->actions[i].key_flag==key_pressed_flag)
		{
			keys->actions[i].key_flag=0;
			if(keys->actions[i].call_back != NULL)
			{
				//按键动作执行前，执行一些其他动作
				keys->actions[i].call_back();			
				//按键动作执行后，执行一些其他动作
			}
		}
	}
}