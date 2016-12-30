/*
*********************************************************************************************************
*
*	ģ������ : M_KEYģ��
*	�ļ����� : m_key.c
*	˵    �� :����ʽ���������������ݽṹ��ɨ���㷨����Ӧ�İ����������
*
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    2016-12-1           ��ʽ����
*********************************************************************************************************
*/
#include "m_key.h"

#define key_action_index_init  -1

// ���������±�ʶ
#define key_pressed_flag 1
//������һ�α�����ʱ����¼�˰����������ټ��һ�Σ���������ͬ�����ж��˰���������
static int8_t key_action_before = key_action_index_init;
//�����°��������ڵ�������
static uint8_t key_action_index=0;
/*
*********************************************************************************************************
*	�� �� ��: m_init_matrix_keys
*	����˵��: ���󰴼�����������˿ڳ�ʼ������
*	��    ��: m_matrix_keys * keys ��װ�����ھ��󰴼�ɨ���㷨�Ľṹ��
*	�� �� ֵ: ��
* 
*********************************************************************************************************
*/
void m_init_matrix_keys( m_matrix_keys * keys)
{
	uint8_t i=0;
	for(i=0;i<keys->input_port_len ;i++)
	{
		hdl_input_gpio(keys->input_keys[i].GPIO_x,keys->input_keys[i].GPIO_Pin,GPIO_Mode_IPU); //���е����붨��Ϊ��������
	}
	for(i=0;i<keys->output_port_len;i++)
	{
		hdl_output_gpio(keys->output_keys[i].GPIO_x,keys->output_keys[i].GPIO_Pin,GPIO_Mode_Out_PP); //�������
		hdl_h(keys->output_keys[i].GPIO_x,keys->output_keys[i].GPIO_Pin); //�����е�����ܽ���1������
	}
	
}
/*
*********************************************************************************************************
*	�� �� ��: m_matrix_scan
*	����˵��: ���󰴼�ɨ���㷨����һ����ÿһ������ߣ������������ϵĹܽ�
*	��    ��: m_matrix_keys * keys ��װ�����ھ��󰴼�ɨ���㷨�Ľṹ��
*	�� �� ֵ: ��
* ע��
*   �˷���Ҫ������������
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
		hdl_l(keys->output_keys[i].GPIO_x,keys->output_keys[i].GPIO_Pin); //����һ����
		for(j=0;j<keys->input_port_len;j++)
		{
			value=hdl_read(keys->input_keys[j].GPIO_x,keys->input_keys[j].GPIO_Pin); //��һ������
			if(value==0)
			{
				//�˴����ǽ��е�������������ɨ����ʱ����������ȡ����
				if(key_action_index == key_action_before) //�жϵ�ǰ������������֮ǰ���µ��Ƿ�Ϊͬһ������
				{
					//�˴�Ҳ����ֱ�ӽ��а���������ִ�У�������Ҫ������λ��1��Ȼ����ִ��m_matrix_process���а�����������
					//���ַ�ʽ��ȡ�ᣬ�������Ӧ�ó���������
					//�������RTOSʵʱ����ϵͳ��������Է���һ���ź���֪ͨ������������а���������
//					if(keys->actions[key_action_index].call_back != NULL)
//					{
//						keys->actions[key_action_index].call_back();
//					}
					keys->actions[key_action_index].key_flag=key_pressed_flag; //���һ����������������ʶλ��1
					key_action_before=key_action_index_init;
				}else
				{
					key_action_before=key_action_index;
				}
				//��return ���֮ǰ��һ��Ҫ������������
				hdl_h(keys->output_keys[i].GPIO_x,keys->output_keys[i].GPIO_Pin);
				return;
			}
			key_action_index++;
		}
		hdl_h(keys->output_keys[i].GPIO_x,keys->output_keys[i].GPIO_Pin); //��һ����󣬽���һ��������
	}
	key_action_before=key_action_index_init;
}

/*
*********************************************************************************************************
*	�� �� ��: m_matrix_process
*	����˵��: ���󰴼�����ִ�� ������˷�������ҪĿ�ģ�������ִ�а�������֮ǰִ��һЩ�����Ķ����������LCD����� 
*	��    ��: m_matrix_keys * keys ��װ�����ھ��󰴼�ɨ���㷨�Ľṹ��
*	�� �� ֵ: ��
* ע��
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
				//��������ִ��ǰ��ִ��һЩ��������
				keys->actions[i].call_back();			
				//��������ִ�к�ִ��һЩ��������
			}
		}
	}
}