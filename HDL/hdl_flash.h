/*
*********************************************************************************************************
*
*	模块名称 : HDL_FLASH模块
*	文件名称 : hdl_flash.h
*	说    明 :stm32中使用mcu内部flash驱动库，可以直接按照结构体来存储与读取
						可以在指定位置进行连续写与连续读
*
*
*********************************************************************************************************
*/

#ifndef __HDL_FLASH_H
#define __HDL_FLASH_H

#include "hdl_include.h"

#define STM32_FLASH_SIZE  256
#define STM32_FLASH_BASE 0x08000000 	//STM32 FLASH的起始地址

#define FLASH_EN 0
#define FLASH_CONFIG_EN 0

#if FLASH_EN==1
	#if STM32_FLASH_SIZE < 256 
		#define STM32_SECTOR_SIZE 1024  //如果容量小于256其内部flash页面大小为1K
	#else
		#define STM32_SECTOR_SIZE 2048  //如果容量大于256，其内部flash页面大小为2K
	#endif
#endif

#if FLASH_CONFIG_EN==1
//在这里定义Config结构体，目的演示下如何实现flash按照完整的结构体长度来进行flash数据的存储与读取
//实际项目中，可根据所需要保存的数据进行统一配置
typedef struct{
	uint16_t test;
}Config;

//配置数据存放的基地址，这个基地址可根据具体的MCU来决定，一般配置数据存放在flash最后位置
#define CONFIG_BASE_ADDR 0x08007E00 

void hdl_read_config(Config * config);

void hdl_write_config(Config * config);

#endif


#if FLASH_EN==1
	u16 hdl_read_half_word(u32 addr);		  //读出半字  
	void hdl_write(u32 addr,u16 * buf,u16 num_to_write);		//从指定地址开始写入指定长度的数据
	void hdl_read(u32 addr,u16 * buf,u16 num_to_read);   		//从指定地址开始读出指定长度的数据
#endif
#endif