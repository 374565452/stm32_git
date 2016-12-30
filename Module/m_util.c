/*
*********************************************************************************************************
*
*	ģ������ : M_UTILģ��
*	�ļ����� : m_util.c
* ��    �� ��v1.0
*	˵    �� : ���õĹ����ຯ�������ڴ��ļ��н���ͳһ����
			1.�޷���У��ͼ��㺯��
			
*			2.	ʵ��printf��scanf�����ض���ͨ���첽�շ����У���֧��printf��Ϣ��USART1�������첽�շ���ѡ��USART1��
*				ʵ���ض���ֻ��Ҫ����2������:
*				int fputc(int ch, FILE *f);
*				int fgetc(FILE *f);
*				����KEIL MDK������������ѡ������Ҫ��MicorLibǰ��򹳣����򲻻������ݴ�ӡ��USART1��
				����������£�
						�ڹ��̵������У�"Target" -> "Code Generation"  ѡ���й�ѡ"Use MicroLIB"
						MicroLIB��ȱʡC�ı��ݿ⣬
*
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    2016-12-1           ��ʽ����
*   V1.1    2016-12-7           bug�޸ģ���fputc fgetc���������е�USART1ȫ��ת��DEBUG_UART������printfҲ��������ʹ��
*                               ����ϵͳ������λ����
*
*********************************************************************************************************
*/
#include "m_util.h"
/*
*********************************************************************************************************
*	�� �� ��: m_system_reset
*	����˵��: ʹ��������������λ����
*	��    ��: 
						
*	�� �� ֵ: ��
* 
*
*********************************************************************************************************
*/
__asm void m_system_reset(void)
{
 MOV R0, #1           //; 
 MSR FAULTMASK, R0    //; ���FAULTMASK  ��ֹһ���жϲ���
 LDR R0, =0xE000ED0C  //;
 LDR R1, =0x05FA0004  //; 
 STR R1, [R0]         //; ϵͳ������λ  
 
deadloop
    B deadloop        //; ��ѭ���������в�������Ĵ���
}
 

/*
*********************************************************************************************************
*	�� �� ��: m_check_unsigned_sum
*	����˵��: �����޷���У��ͣ�����ָ���Ļ����������ݼ�����8λ�޷���У���
*	��    ��: uint8_t * buffer,��Ҫ����У�������
						uint32_t len ,���ݳ���
*	�� �� ֵ: ��������õ����޷���У�������
* 
* ע���˺������⣺֮ǰ���õ���Դ��չ��ʱ�����ڵ�ǰ�����ݳ��ȱ���������д��uint8_t���´�������ݳ���
*			�������255���������������У��Ͳ���ȷ���˴������ݳ�������Ӧ��Ϊuint32_t
*     У����㷨Ϊ�������е������ۼ���ӣ��õ��ĺ�ֻȡ��8λ
*
*********************************************************************************************************
*/
uint8_t m_check_unsigned_sum(uint8_t * buffer, uint32_t len)
{
	uint32_t i = 0 ;
	uint8_t sumchek=0;
	for(i = 0 ;i<len;i++)
	{
		sumchek += buffer[i];
	}
	return sumchek;
}

/*
*********************************************************************************************************
*	�� �� ��: fputc
*	����˵��: �ض���putc��������������ʹ��printf������ָ���Ĵ����д�ӡ���
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
#if M_PRINTF_EN ==1
int fputc(int ch, FILE *f)
{
	
	/* дһ���ֽڵ�USART1 */
	USART_SendData(DEBUG_UART, (uint8_t) ch);

	/* �ȴ����ͽ��� */
	while (USART_GetFlagStatus(DEBUG_UART, USART_FLAG_TC) == RESET)
	{}

	return ch;
	
}
#endif
/*
*********************************************************************************************************
*	�� �� ��: fgetc
*	����˵��: �ض���getc��������������ʹ��scanff������ָ���Ĵ�������������
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
#if M_PRINTF_EN ==1
int fgetc(FILE *f)
{
	/* �ȴ�����1�������� */
	while (USART_GetFlagStatus(DEBUG_UART, USART_FLAG_RXNE) == RESET);

	return (int)USART_ReceiveData(DEBUG_UART);
}
#endif