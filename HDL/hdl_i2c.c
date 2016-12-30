/*
*********************************************************************************************************
*
*	模块名称 : HDL_I2C模块
*	文件名称 : hdl_i2c.c
* 版    本 ：v1.0
*	说    明 : stm32中使用模拟I2C通信驱动库
*
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2016-11-28        正式发布
*
*
*********************************************************************************************************
*/
#include "hdl_i2c.h"

void i2c_delay (struct i2c_base_t * t)
{
	uint32_t i=t->delay_time;
	while(i--);
}
uint8_t i2c_start(struct i2c_base_t * t)
{
	SDA_H(t->sda_gpio_port,t->sda_gpio_pin);
	i2c_delay(t);
	SCL_H(t->scl_gpio_port,t->scl_gpio_pin);
	i2c_delay(t);
	if(!SDA_STAT(t->sda_gpio_port,t->sda_gpio_pin))
	{
		return i2c_bus_busy;
	}
	SDA_L(t->sda_gpio_port,t->sda_gpio_pin);
	i2c_delay(t);
	SCL_L(t->scl_gpio_port,t->scl_gpio_pin);
	i2c_delay(t);
	if(SDA_STAT(t->sda_gpio_port,t->sda_gpio_pin))
	{
		return i2c_bus_error;
	}
	return i2c_ready;
	//max(1,2);
}
void i2c_stop(struct i2c_base_t * t)
{
	SDA_L(t->sda_gpio_port,t->sda_gpio_pin);
	i2c_delay(t);
	SCL_H(t->scl_gpio_port,t->scl_gpio_pin);
	i2c_delay(t);
	SDA_H(t->sda_gpio_port,t->sda_gpio_pin);
	i2c_delay(t);
	
}
void i2c_send_ack( struct i2c_base_t * t)
{
	SDA_L(t->sda_gpio_port,t->sda_gpio_pin);
	i2c_delay(t);
	SCL_H(t->scl_gpio_port,t->scl_gpio_pin);
	i2c_delay(t);
	SCL_L(t->scl_gpio_port,t->scl_gpio_pin);
	i2c_delay(t);
}
void i2c_send_nack( struct i2c_base_t * t)
{
	SDA_H(t->sda_gpio_port,t->sda_gpio_pin);
	i2c_delay(t);
	SCL_H(t->scl_gpio_port,t->scl_gpio_pin);
	i2c_delay(t);
	SCL_L(t->scl_gpio_port,t->scl_gpio_pin);
	i2c_delay(t);
}
uint8_t i2c_wait_ack( struct i2c_base_t * t)
{
	SDA_H(t->sda_gpio_port,t->sda_gpio_pin);
	i2c_delay(t);
	SCL_H(t->scl_gpio_port,t->scl_gpio_pin);
	i2c_delay(t);
	if(SDA_STAT(t->sda_gpio_port,t->sda_gpio_pin))
	{
		SCL_L(t->scl_gpio_port,t->scl_gpio_pin);
		return i2c_nack;
	}else
	{
		SCL_L(t->scl_gpio_port,t->scl_gpio_pin);
		return i2c_ack;
	}
}
uint8_t i2c_send_byte(struct i2c_base_t * t ,uint8_t data)
{
	u8 i;
 SCL_L(t->scl_gpio_port,t->scl_gpio_pin);
 for(i=0;i<8;i++)
 {  
 
  if(data&0x80)
  {
   SDA_H(t->sda_gpio_port,t->sda_gpio_pin);
  }
  else
  {
   SDA_L(t->sda_gpio_port,t->sda_gpio_pin);
  } 
  data<<=1;
  i2c_delay(t);
  SCL_H(t->scl_gpio_port,t->scl_gpio_pin);
  i2c_delay(t);
  SCL_L(t->scl_gpio_port,t->scl_gpio_pin);
  i2c_delay(t);
 }
 return i2c_wait_ack(t);
}
uint8_t i2c_receive_byte(struct i2c_base_t * t)
{
	u8 i,Dat;
 SDA_H(t->sda_gpio_port,t->sda_gpio_pin);
 SCL_L(t->scl_gpio_port,t->scl_gpio_pin);
 Dat=0;
 for(i=0;i<8;i++)
 {
  SCL_H(t->scl_gpio_port,t->scl_gpio_pin);
  i2c_delay(t);
  Dat<<=1;
  if(SDA_STAT(t->sda_gpio_port,t->sda_gpio_pin)) 
  {
   Dat|=0x01; 
  }   
  SCL_L(t->scl_gpio_port,t->scl_gpio_pin);
  i2c_delay(t);
 }
 //////DebugPrint("TWI2_Dat:%x\n",Dat);
 return Dat;
}

uint8_t _i2c_write_byte( struct i2c_base_t * t,uint8_t SendByte )
{
	 uint8_t  flag;
    //if(!i2c_start())
	if(i2c_start(t))
	{
		return 0;
	}
    
	flag = i2c_send_byte(t,t->write_addr & 0xFE);
	if(flag)
	{
		flag = i2c_send_byte(t,SendByte);
	}
  
	i2c_stop(t);
   
  return flag;
}

uint8_t _i2c_write_bytes(struct i2c_base_t * t,uint8_t * buffer,uint16_t len)
{
	uint8_t  flag;
	uint8_t i=0;
  
	uint8_t send_count=0;
  
	while(i2c_start(t)!=i2c_ready)
	{
		send_count++;
		if(send_count >= I2C_TRYE_COUNT)
		{
			return 0;
		}
	}

	flag=i2c_send_byte(t,t->write_addr );
	
	for(i=0;i<len;i++)
	{
		flag=i2c_send_byte(t,buffer[i]);
		i2c_delay(t);
		i2c_delay(t); 
	}
	i2c_stop(t);
	return flag;
}

void _i2c_read_bytes(struct i2c_base_t * t,uint8_t * buffer,uint16_t len)
{
	
	i2c_start(t);
	i2c_send_byte(t,t->read_addr ); //写地址
	i2c_delay(t); 
	i2c_delay(t); 
	while(len != 1)
	{
		*buffer=i2c_receive_byte(t);
		i2c_send_ack(t);
		i2c_delay(t); 
		buffer++;
		len--;
		i2c_delay(t);  
		i2c_delay(t); 
	}
	*buffer=i2c_receive_byte(t);
	
	i2c_send_nack(t);
	
	i2c_stop(t);
}