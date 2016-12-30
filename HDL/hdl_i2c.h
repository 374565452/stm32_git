/*
*********************************************************************************************************
*
*	模块名称 : HDL_I2C模块
*	文件名称 : hdl_i2c.h
*	说    明 :stm32中使用模拟I2C通信驱动库
*
*
*********************************************************************************************************
*/

#ifndef __HDL_I2C_H
#define __HDL_I2C_H

#include "hdl_include.h"

#define PORT(p) ((GPIO_TypeDef*)(p))

#define SCL_H(p,s) (PORT(p)->BSRR=(s))
#define SCL_L(p,s) (PORT(p)->BRR=(s))

#define SDA_H(p,s) (PORT(p)->BSRR=(s))
#define SDA_L(p,s) (PORT(p)->BRR=(s))
#define SDA_STAT(p,s) (PORT(p)->IDR &(s))
#define I2C_TRYE_COUNT 3

struct i2c_base_t{
	GPIO_TypeDef * scl_gpio_port; 
	uint16_t scl_gpio_pin;     
	
	GPIO_TypeDef * sda_gpio_port;
	uint16_t sda_gpio_pin;
	
	uint16_t delay_time;
	
	uint16_t read_addr;
	uint16_t write_addr;
};

enum i2c_reply
{
	i2c_nack=0, 
	i2c_ack=1
};
enum i2c_bus_state
{
	i2c_ready=0,
	i2c_bus_busy=1,
	i2c_bus_error=2
};
void i2c_delay (struct i2c_base_t * t);
uint8_t i2c_start(struct i2c_base_t * t);
void i2c_stop(struct i2c_base_t * t);
void i2c_send_ack( struct i2c_base_t * t);
void i2c_send_nack( struct i2c_base_t * t);
uint8_t i2c_wait_ack( struct i2c_base_t * t);
uint8_t i2c_send_byte(struct i2c_base_t * t,uint8_t data);

uint8_t i2c_receive_byte(struct i2c_base_t * t);

uint8_t _i2c_write_byte( struct i2c_base_t * t,uint8_t SendByte );

uint8_t _i2c_write_bytes(struct i2c_base_t * t,uint8_t * buffer,uint16_t len);

void _i2c_read_bytes(struct i2c_base_t * t,uint8_t * buffer,uint16_t len);

#endif