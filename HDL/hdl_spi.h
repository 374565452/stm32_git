/*
*********************************************************************************************************
*
*	ģ������ : HDL_SPIģ��
*	�ļ����� : hdl_SPI.h
*	˵    �� :stm32��SPI���߷�װ���̣�������ֻ�ǽ�����ϵͳ���Դ�������SPI�ķ�ʽ������SPI����
						��û�а���ģ�ⷽʽ������SPI����
						stm32����2��spi ����spi1��spi2
						��spi1�У�PA4ΪCS ��PA5ΪCLK   PA6ΪMISO  PA7ΪMOSI
						��spi2�У�PB12ΪCS��PB13ΪCLK  PB14ΪMISO  PB15ΪMOSI
						
ע�������д��������ָ��MCU��ΪSPI������ʵ�ֹ���
		MOSI:��������
		��SPI����Ϊ����ʱ�ö˿ھ�Ϊ���ͣ�Master Output����������Ϊ�ӻ�ʱ�ö˿ھ�Ϊ���գ�Slave Input��
		MISO�����մӷ�
		��SPI����Ϊ����ʱ�ö˿ھ�Ϊ���գ�Master Input����������Ϊ�ӻ�ʱ�ö˿ھ�Ϊ���ͣ�Slave Output��
*
*
*********************************************************************************************************
*/
#ifndef __HDL_SPI_H
#define __HDL_SPI_H

#include "hdl_include.h"

typedef struct
{
	SPI_TypeDef* spix; //������һ��spi,spi1��spi2
	uint8_t spi_rcc_type; //spi rcc �����ĸ����� 1��2
	uint32_t spi_rcc;  //spi��ʱ��
	uint32_t port_rcc; //spi�����˿�����ʱ��
	GPIO_TypeDef* spi_gpio; //��SPI���ڵĶ˿�����
	uint16_t cs_port;  //Ƭѡ�˿ں�
	uint16_t clk_port; //ʱ�Ӷ˿ں�
	uint16_t miso_port; //MISO�˿ں�
	uint16_t mosi_port;//mosi�˿ں�
	//spi������Ԥ��Ƶ����������Ϊ����spi���ٶ�
	//��ȡֵΪSPI_BaudRatePrescaler_2��SPI_BaudRatePrescaler_4��
	//SPI_BaudRatePrescaler_8  SPI_BaudRatePrescaler_16
	//SPI_BaudRatePrescaler_32  SPI_BaudRatePrescaler_64
	//SPI_BaudRatePrescaler_128  SPI_BaudRatePrescaler_256
	uint16_t spi_baud_rate;
	//CLK����ʱ���ȶ�״̬ ��ȡֵ����ΪSPI_CPOL_Low  SPI_CPOL_High ����ʱ�ӵ��ȶ�״̬
	uint16_t spi_cpol;  
	//CLK�����ز��� �ӵڼ������ؽ��в�������  ȡֵ����ΪSPI_CPHA_1Edge SPI_CPHA_2Edge
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