/*
*********************************************************************************************************
*
*	ģ������ : HDL_FLASHģ��
*	�ļ����� : hdl_flash.h
*	˵    �� :stm32��ʹ��mcu�ڲ�flash�����⣬����ֱ�Ӱ��սṹ�����洢���ȡ
						������ָ��λ�ý�������д��������
*
*
*********************************************************************************************************
*/

#ifndef __HDL_FLASH_H
#define __HDL_FLASH_H

#include "hdl_include.h"

#define STM32_FLASH_SIZE  256
#define STM32_FLASH_BASE 0x08000000 	//STM32 FLASH����ʼ��ַ

#define FLASH_EN 0
#define FLASH_CONFIG_EN 0

#if FLASH_EN==1
	#if STM32_FLASH_SIZE < 256 
		#define STM32_SECTOR_SIZE 1024  //�������С��256�����ڲ�flashҳ���СΪ1K
	#else
		#define STM32_SECTOR_SIZE 2048  //�����������256�����ڲ�flashҳ���СΪ2K
	#endif
#endif

#if FLASH_CONFIG_EN==1
//�����ﶨ��Config�ṹ�壬Ŀ����ʾ�����ʵ��flash���������Ľṹ�峤��������flash���ݵĴ洢���ȡ
//ʵ����Ŀ�У��ɸ�������Ҫ��������ݽ���ͳһ����
typedef struct{
	uint16_t test;
}Config;

//�������ݴ�ŵĻ���ַ���������ַ�ɸ��ݾ����MCU��������һ���������ݴ����flash���λ��
#define CONFIG_BASE_ADDR 0x08007E00 

void hdl_read_config(Config * config);

void hdl_write_config(Config * config);

#endif


#if FLASH_EN==1
	u16 hdl_read_half_word(u32 addr);		  //��������  
	void hdl_write(u32 addr,u16 * buf,u16 num_to_write);		//��ָ����ַ��ʼд��ָ�����ȵ�����
	void hdl_read(u32 addr,u16 * buf,u16 num_to_read);   		//��ָ����ַ��ʼ����ָ�����ȵ�����
#endif
#endif