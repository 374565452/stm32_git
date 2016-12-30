/*
*********************************************************************************************************
*
*	ģ������ : HDL_IWDGģ��
*	�ļ����� : hdl_iwdg.c
* ��    �� ��v1.0
*	˵    �� : stm32�������Ź���������
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    2016-11-29        ��ʽ����
*
* ע��
	 ʹ��LSI  ����LSIΪ�ڲ����پ���
	RCC_LSICmd(ENABLE);
	
	�ȴ�ֱ��LSI���� 
	while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
	{}
*********************************************************************************************************
*/
#include "hdl_iwdg.h"

/****************************************************************************
* ��    �ƣ�hdl_init_iwdg
* ��    �ܣ���ʼ���������Ź���ʹ�ܶ������Ź�
* ��ڲ�����rlr���Ź���װ��ֵ
* ���ڲ�����
* ˵    ����
* ���÷�����     
****************************************************************************/ 
void hdl_init_iwdg(uint16_t rlr)
{
	/* ���ϵͳ�Ƿ�Ӷ������Ź���λ�лָ� */
	if (RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET)
	{		
		/* �����λ��־ */
		RCC_ClearFlag();
	}
	/* д��0x5555��ʾ�������IWDG_PR ��IWDG_RLR�Ĵ��� */
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);

  /* IWDG counter clock: 40KHz(LSI) / 32 = 1.25 KHz */
		/*  LSI/32 ��Ƶ*/
	/*
	 *����32��Ƶ�󣬼��㹫ʽΪ32*RLR/40
	 *�� rlr=40000/32 ���Ź����ʱ�����Ϊ32*40000/32/40 =1S
	*/
  IWDG_SetPrescaler(IWDG_Prescaler_32);

  /* Set counter reload value to 349 */
  IWDG_SetReload(rlr);

  /* Reload IWDG counter */
	/* ����IWDG���� */
  IWDG_ReloadCounter();

  /* Enable IWDG (the LSI oscillator will be enabled by hardware) */
	/* ʹ�� IWDG (LSI oscillator ��Ӳ��ʹ��) */
  IWDG_Enable();
	
}
/*
*********************************************************************************************************
*	�� �� ��: hdl_feed_dog
*	����˵��: ι������
*	��    �Σ���
*	�� �� ֵ: ��		        
*********************************************************************************************************
*/
void hdl_feed_dog()
{
	IWDG_ReloadCounter();
}