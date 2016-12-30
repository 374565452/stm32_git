/*
*********************************************************************************************************
*
*	ģ������ : HDL_FLASHģ��
*	�ļ����� : hdl_flash.c
* ��    �� ��v1.0
*	˵    �� : stm32��ʹ��mcu�ڲ�flash�����⣬����ֱ�Ӱ��սṹ�����洢���ȡ
*
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    2016-11-28        ��ʽ����
*
*
*********************************************************************************************************
*/

#include "hdl_flash.h"

#if FLASH_CONFIG_EN==1

volatile FLASH_Status FLASHStatus = FLASH_COMPLETE;
/*
*********************************************************************************************************
*	�� �� ��: hdl_read_config
*	����˵��: ��flash��ָ��λ�ã�����ָ�����ݽṹ��������ȡ����
*	��    ��: void* config,configʵ����Ӧ�ó����Զ���Ľṹ����
*	�� �� ֵ: ��
* ע��
*********************************************************************************************************
*/
void hdl_read_config(Config * config)
{
	memcpy(config, (uint8_t*)CONFIG_BASE_ADDR, sizeof(*config));
	//uint8_t a=1;
}
/*
*********************************************************************************************************
*	�� �� ��: hdl_write_config
*	����˵��: ��flash��ָ��λ�ã�д��ָ�����ݽṹ���ȵ���������
*	��    ��: void* config,configʵ����Ӧ�ó����Զ���Ľṹ����
*	�� �� ֵ: ��
* ע��stm32MCUоƬ�е�flash�����ǰ��հ���������д��ģ�������д��ʱ����16λ���ݵķ�ʽ����д��
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

u16 STMFLASH_BUF[STM32_SECTOR_SIZE/2];//�����2K�ֽ�
/*
*********************************************************************************************************
*	�� �� ��: stmflash_write_no_check
*	����˵��: ��flash��ָ��λ�ô���д��ָ�����ȵ����ݣ�����������16λ��ʾ�����ڲ�ʹ��
*	��    ��: u32 addrs flash���ڲ���ַ
						u16* buf ��д�����ݵĻ�������
						u16 num_to_write д�����ݵĳ���
*	�� �� ֵ: ��
* ע��stm32MCUоƬ�е�flash�����ǰ��հ���������д��ģ�������д��ʱ����16λ���ݵķ�ʽ����д��
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
*	�� �� ��: stmflash_write_no_check
*	����˵��: ��ָ����ַ�ϣ���ȡ����
*	��    ��: u32 addrs flash���ڲ���ַ
*	�� �� ֵ: ��ȡ���İ���
* ע��stm32MCUоƬ�е�flash�����ǰ��հ���������д��ģ�������д��ʱ����16λ���ݵķ�ʽ����д��
*********************************************************************************************************
*/
u16 hdl_read_half_word(u32 addr)	  //��������   
{
	return *(vu16 *)addr;
}
/*
*********************************************************************************************************
*	�� �� ��: stmflash_write_no_check
*	����˵��: ��flash��ָ��λ�ô���д��ָ�����ȵ����ݣ�����������16λ��ʾ�����ⲿӦ�ó���������
*	��    ��: u32 addrs flash���ڲ���ַ
						u16* buf ��д�����ݵĻ�������
						u16 num_to_write д�����ݵĳ���
*	�� �� ֵ: ��
* ע��stm32MCUоƬ�е�flash�����ǰ��հ���������д��ģ�������д��ʱ����16λ���ݵķ�ʽ����д��
*********************************************************************************************************
*/
void hdl_write(u32 addr,u16 * buf,u16 num_to_writes)		//��ָ����ַ��ʼд��ָ�����ȵ�����
{
	u32 offaddr;//ʵ��ƫ�Ƶ�ַ����0x080000000��ʼ
	u32 sector_pos; //������ַ
	u16 sector_offset; //������ʵ��ƫ��
	u16 sector_remain; //������ʣ��ռ�
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
	while(1)  //д����
	{
		hdl_read(sector_pos*STM32_SECTOR_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,STM32_SECTOR_SIZE/2);//������������������
		for(i=0;i<sector_remain;i++)//У������ ���ж��Ƿ���Ҫ����̽�ղ���������鿴��ǰ������ʣ��Ŀռ����Ƿ�������
		{
			if(STMFLASH_BUF[sector_offset+i]!=0XFFFF)
				break;//��Ҫ����  	  
		}
		if(i<sector_remain) //��ʱ��Ҫ����flash
		{
			//����Ӧ����sector_pos ,����Ӧ����sector_offset
			FLASH_ErasePage(sector_pos*STM32_SECTOR_SIZE+STM32_FLASH_BASE);//�����������
			for(i=0;i<sector_remain;i++)//����
			{
				STMFLASH_BUF[i+sector_offset]=buf[i];	  
			}
				//����Ӧ����sector_pos ,����Ӧ����sector_offset
			//stmflash_write_no_check(sector_offset*STM32_SECTOR_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,STM32_SECTOR_SIZE/2);//д����������  
			stmflash_write_no_check(sector_pos*STM32_SECTOR_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,STM32_SECTOR_SIZE/2);
		}
		else
		{
			stmflash_write_no_check(addr,buf,sector_remain);//д�Ѿ������˵�,ֱ��д������ʣ������. 
		}
		if(num_to_writes <= sector_remain) //�Ѿ�д�꣬�˳�ѭ��
		{
			break;
		}
		else
		{
			sector_pos ++; //������ַ�����
			sector_offset=0; //��������ϡΪ0����Ϊ֮ǰ�Ѿ�����������д�꣬sector_pos�Ѿ����ƫ����һ���µ�������
			num_to_writes -= sector_remain; //����ʣ������
			addr += sector_remain;  //д�����ݵĵ�ַ���ƫ��
			buf += sector_remain;   //����ָ�����ƫ��
			
			if(num_to_writes > STM32_SECTOR_SIZE/2)  //����������ڻ���Ҫ���ٿռ�
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
*	�� �� ��: stmflash_write_no_check
*	����˵��: ��flash��ָ��λ�ô�����ȡָ�����ȵ�16λ����
*	��    ��: u32 addrs flash���ڲ���ַ
						u16* buf ��ȡ���ݴ�Ż�����
						u16 num_to_write Ҫ��ȡ���ݳ���
*	�� �� ֵ: ��
* ע��stm32MCUоƬ�е�flash�����ǰ��հ���������д��ģ�������д��ʱ����16λ���ݵķ�ʽ����д��
*********************************************************************************************************
*/
void hdl_read(u32 addr,u16 * buf,u16 num_to_read)  		//��ָ����ַ��ʼ����ָ�����ȵ����� 
{
	u16 i=0;
	for(i=0;i<num_to_read;i++)
	{
		buf[i]=hdl_read_half_word(addr); //��ȡ2���ֽ�
		addr += 2;  //��ַ���ƫ��2���ֽ�
	}
}

#endif
