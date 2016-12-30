/*
*********************************************************************************************************
*
*	模块名称 : HDL_FLASH模块
*	文件名称 : hdl_flash.c
* 版    本 ：v1.0
*	说    明 : stm32中使用mcu内部flash驱动库，可以直接按照结构体来存储与读取
*
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2016-11-28        正式发布
*
*
*********************************************************************************************************
*/

#include "hdl_flash.h"

#if FLASH_CONFIG_EN==1

volatile FLASH_Status FLASHStatus = FLASH_COMPLETE;
/*
*********************************************************************************************************
*	函 数 名: hdl_read_config
*	功能说明: 从flash的指定位置，按照指定数据结构长度来读取数据
*	形    参: void* config,config实际是应用程序自定义的结构数据
*	返 回 值: 无
* 注：
*********************************************************************************************************
*/
void hdl_read_config(Config * config)
{
	memcpy(config, (uint8_t*)CONFIG_BASE_ADDR, sizeof(*config));
	//uint8_t a=1;
}
/*
*********************************************************************************************************
*	函 数 名: hdl_write_config
*	功能说明: 从flash的指定位置，写入指定数据结构长度的配置数据
*	形    参: void* config,config实际是应用程序自定义的结构数据
*	返 回 值: 无
* 注：stm32MCU芯片中的flash操作是按照半字来进行写入的，所以在写的时候以16位数据的方式进行写入
*********************************************************************************************************
*/
void hdl_write_config(Config * config)
{
	uint8_t i = 0;
	uint16_t temp = 0;
	FLASH_Status s;
	FLASH_Unlock();
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR); 
	
  FLASHStatus = FLASH_ErasePage(CONFIG_BASE_ADDR);
	//debug_format("the status is %d \r\n",FLASHStatus);
	//debug_format("the length is %d\r\n", sizeof(*data));	
  for(i=0;i<sizeof(*config);i+=2)
  {
		memcpy(&temp,((uint8_t *)(config)+i),2);  
//debug_format("the temp is %d \r\n",temp);		
    //FLASH_ProgramByte((SYS_DATA_ADDR+i),temp); 
		while(FLASH_GetFlagStatus(FLASH_FLAG_BSY)==1){}	
		//s = FLASH_ProgramOptionByteData((StartAddress+i),temp);
		
		s=FLASH_ProgramHalfWord((CONFIG_BASE_ADDR+i),temp);
		//s=FLASH_ProgramWord((StartAddress+i),temp);
		
		//debug_format("the temp is %d \r\n",temp);
  }
}
#endif

#if FLASH_EN==1

u16 STMFLASH_BUF[STM32_SECTOR_SIZE/2];//最多是2K字节
/*
*********************************************************************************************************
*	函 数 名: stmflash_write_no_check
*	功能说明: 向flash的指定位置处，写入指定长度的数据，其中数据是16位显示，供内部使用
*	形    参: u32 addrs flash中内部地址
						u16* buf 待写入数据的缓存区或
						u16 num_to_write 写入数据的长度
*	返 回 值: 无
* 注：stm32MCU芯片中的flash操作是按照半字来进行写入的，所以在写的时候以16位数据的方式进行写入
*********************************************************************************************************
*/
void stmflash_write_no_check(u32 addrs,u16 * buf,u16 num_to_write)
{
	u16 i=0;
	for(i=0;i<num_to_write;i++)
	{
		FLASH_ProgramHalfWord(addrs,buf[i]);
		addrs += 2;
	}
}
/*
*********************************************************************************************************
*	函 数 名: stmflash_write_no_check
*	功能说明: 在指定地址上，读取半字
*	形    参: u32 addrs flash中内部地址
*	返 回 值: 读取到的半字
* 注：stm32MCU芯片中的flash操作是按照半字来进行写入的，所以在写的时候以16位数据的方式进行写入
*********************************************************************************************************
*/
u16 hdl_read_half_word(u32 addr)	  //读出半字   
{
	return *(vu16 *)addr;
}
/*
*********************************************************************************************************
*	函 数 名: stmflash_write_no_check
*	功能说明: 向flash的指定位置处，写入指定长度的数据，其中数据是16位显示，供外部应用程序来调用
*	形    参: u32 addrs flash中内部地址
						u16* buf 待写入数据的缓存区或
						u16 num_to_write 写入数据的长度
*	返 回 值: 无
* 注：stm32MCU芯片中的flash操作是按照半字来进行写入的，所以在写的时候以16位数据的方式进行写入
*********************************************************************************************************
*/
void hdl_write(u32 addr,u16 * buf,u16 num_to_writes)		//从指定地址开始写入指定长度的数据
{
	u32 offaddr;//实际偏移地址，从0x080000000开始
	u32 sector_pos; //扇区地址
	u16 sector_offset; //扇区内实际偏移
	u16 sector_remain; //扇区内剩余空间
	u16 i=0;
	
	if( (addr < STM32_FLASH_BASE) || (addr >= (STM32_FLASH_BASE+1024*STM32_FLASH_SIZE)))
	{
		return;
	}
	
	FLASH_Unlock();
	offaddr=addr-STM32_FLASH_BASE;
	sector_pos=offaddr / STM32_SECTOR_SIZE;
	sector_offset=(offaddr % STM32_SECTOR_SIZE)/2;
	sector_remain=(STM32_SECTOR_SIZE/2) - sector_offset;
	while(1)  //写数据
	{
		hdl_read(sector_pos*STM32_SECTOR_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,STM32_SECTOR_SIZE/2);//读出整个扇区的内容
		for(i=0;i<sector_remain;i++)//校验数据 ，判断是否需要进行探险操作，具体查看当前扇区内剩余的空间内是否有数据
		{
			if(STMFLASH_BUF[sector_offset+i]!=0XFFFF)
				break;//需要擦除  	  
		}
		if(i<sector_remain) //此时需要擦除flash
		{
			//这里应该是sector_pos ,而不应该是sector_offset
			FLASH_ErasePage(sector_pos*STM32_SECTOR_SIZE+STM32_FLASH_BASE);//擦除这个扇区
			for(i=0;i<sector_remain;i++)//复制
			{
				STMFLASH_BUF[i+sector_offset]=buf[i];	  
			}
				//这里应该是sector_pos ,而不应该是sector_offset
			//stmflash_write_no_check(sector_offset*STM32_SECTOR_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,STM32_SECTOR_SIZE/2);//写入整个扇区  
			stmflash_write_no_check(sector_pos*STM32_SECTOR_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,STM32_SECTOR_SIZE/2);
		}
		else
		{
			stmflash_write_no_check(addr,buf,sector_remain);//写已经擦除了的,直接写入扇区剩余区间. 
		}
		if(num_to_writes <= sector_remain) //已经写完，退出循环
		{
			break;
		}
		else
		{
			sector_pos ++; //扇区地址向后移
			sector_offset=0; //扇区内依稀为0，因为之前已经将整个扇区写完，sector_pos已经向后偏移至一个新的扇区内
			num_to_writes -= sector_remain; //计算剩余数据
			addr += sector_remain;  //写入数据的地址向后偏移
			buf += sector_remain;   //数据指针向后偏移
			
			if(num_to_writes > STM32_SECTOR_SIZE/2)  //计算出扇区内还需要多少空间
			{
				sector_remain=(STM32_SECTOR_SIZE/2);
			}
			else
			{
				sector_remain=num_to_writes;
			}
		}
	}
	
	FLASH_Lock();
}

/*
*********************************************************************************************************
*	函 数 名: stmflash_write_no_check
*	功能说明: 向flash的指定位置处，读取指定长度的16位数据
*	形    参: u32 addrs flash中内部地址
						u16* buf 读取数据存放缓冲区
						u16 num_to_write 要读取数据长度
*	返 回 值: 无
* 注：stm32MCU芯片中的flash操作是按照半字来进行写入的，所以在写的时候以16位数据的方式进行写入
*********************************************************************************************************
*/
void hdl_read(u32 addr,u16 * buf,u16 num_to_read)  		//从指定地址开始读出指定长度的数据 
{
	u16 i=0;
	for(i=0;i<num_to_read;i++)
	{
		buf[i]=hdl_read_half_word(addr); //读取2个字节
		addr += 2;  //地址向后偏移2个字节
	}
}

#endif
