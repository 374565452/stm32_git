/*
*********************************************************************************************************
*
*	模块名称 : HDL_SPI模块
*	文件名称 : hdl_SPI.h
*	说    明 :stm32中SPI总线封装过程，这里面只是将利用系统库自带的配置SPI的方式来驱动SPI总线
						并没有包括模拟方式来驱动SPI总线
						stm32中有2个spi ，，spi1与spi2
						在spi1中，PA4为CS ，PA5为CLK   PA6为MISO  PA7为MOSI
						在spi2中，PB12为CS，PB13为CLK  PB14为MISO  PB15为MOSI
						
注：这里的写的驱动是指，MCU作为SPI主机来实现功能
		MOSI:主发从收
		当SPI配置为主机时该端口就为发送（Master Output），当配置为从机时该端口就为接收（Slave Input）
		MISO：主收从发
		当SPI配置为主机时该端口就为接收（Master Input），当配置为从机时该端口就为发送（Slave Output）
*
*
*********************************************************************************************************
*/
#ifndef __HDL_SPI_H
#define __HDL_SPI_H

#include "hdl_include.h"

typedef struct
{
	SPI_TypeDef* spix; //属于哪一个spi,spi1或spi2
	uint8_t spi_rcc_type; //spi rcc 属于哪个类型 1或2
	uint32_t spi_rcc;  //spi的时钟
	uint32_t port_rcc; //spi各个端口类型时钟
	GPIO_TypeDef* spi_gpio; //该SPI所在的端口类型
	uint16_t cs_port;  //片选端口号
	uint16_t clk_port; //时钟端口号
	uint16_t miso_port; //MISO端口号
	uint16_t mosi_port;//mosi端口号
	//spi的配置预分频数，个人认为即是spi的速度
	//其取值为SPI_BaudRatePrescaler_2，SPI_BaudRatePrescaler_4，
	//SPI_BaudRatePrescaler_8  SPI_BaudRatePrescaler_16
	//SPI_BaudRatePrescaler_32  SPI_BaudRatePrescaler_64
	//SPI_BaudRatePrescaler_128  SPI_BaudRatePrescaler_256
	uint16_t spi_baud_rate;
	//CLK空闲时的稳定状态 ，取值可以为SPI_CPOL_Low  SPI_CPOL_High 串行时钟的稳定状态
	uint16_t spi_cpol;  
	//CLK上升沿采样 从第几个边沿进行采样动作  取值可以为SPI_CPHA_1Edge SPI_CPHA_2Edge
  uint16_t spi_cpha;      
}spi_t;

typedef enum
{
	SPI_1=1,
	SPI_2
}spi_e;

void hdl_init_spi(spi_t * t);
uint8_t hdl_read_write_byte(spi_t *t ,uint8_t data);

#endif