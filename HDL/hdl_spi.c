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
		SPI发送数据时，先将片选信号进行拉低操作，完成数据发送动作，然后再将片选信号进行拉高操作
		
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2016-11-28        正式发布
*
*********************************************************************************************************
*/
#include "hdl_spi.h"

/*
*********************************************************************************************************
*	函 数 名: hdl_init_spi
*	功能说明: spi硬件初始化功能
*	形    参：spi_t * t ,带有spi基本数据 的结构体指针
*	返 回 值: 无
*
*
* 注：spi的片选信号 应该设置为推挽输出
			其余的端口设置为复用推挽输出
			另外对于配置SPI前要对从模式了解清楚，包括设备支持的时钟范围与模式（CPOL与CPHA）
*********************************************************************************************************
*/
void hdl_init_spi(spi_t * t)
{
	SPI_InitTypeDef   SPI_InitStructure;
	
	hdl_set_rcc(t->spi_rcc,ENABLE,t->spi_rcc_type);
	hdl_set_rcc(t->port_rcc,ENABLE,2);
	hdl_output_gpio(t->spi_gpio,t->cs_port,GPIO_Mode_Out_PP);//片选信号设置为推挽输出
	hdl_output_gpio(t->spi_gpio,t->clk_port,GPIO_Mode_AF_PP);
	hdl_output_gpio(t->spi_gpio,t->miso_port,GPIO_Mode_AF_PP);
	hdl_output_gpio(t->spi_gpio,t->mosi_port,GPIO_Mode_AF_PP);
	
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//设置SPI工作模式:设置为主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//设置SPI的数据大小:SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = t->spi_cpol;		//选择了串行时钟的稳态:时钟悬空高
	SPI_InitStructure.SPI_CPHA = t->spi_cpha;	//数据捕获于第二个时钟沿
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	SPI_InitStructure.SPI_BaudRatePrescaler = t->spi_baud_rate;		//定义波特率预分频的值:波特率预分频值为256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC值计算的多项式
	SPI_Init(t->spix, &SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
 
	SPI_Cmd(t->spix, ENABLE); //使能SPI外设
}

/*
*********************************************************************************************************
*	函 数 名: hdl_send_write_byte
*	功能说明: spi读取或发送一个字节内容
*	形    参：spi_t * t ,带有spi基本数据 的结构体指针
						uint8_t data，需要发送的数据
*	返 回 值:  uint8_t 返回读取到的数据
*
*
* 注：通过SPI来读数据与写数据，都是调用此函数
			如果是读数据，就要先发送一个字节，这个字节可以是0x00。然后就可以读取到回传来的数据
			为什么这样做？因为从模式下不能提供时钟，所以读数据要先发送一个字节
*********************************************************************************************************
*/
uint8_t hdl_read_write_byte(spi_t * t,uint8_t data)
{
	//确保上次已经发送完成
	 while (SPI_I2S_GetFlagStatus(t->spix, SPI_I2S_FLAG_TXE) == RESET);
  //往寄存器中写入一个字节
	SPI_I2S_SendData(t->spix, data);
	//等待接收一个字节数据
	/***
	*加上下一面语句的目的，是确保这个字节已经发送出去，因为发送与接收是并行同步进行
	*意思是，发送一个字节意味着就可以接收到一个字节
	*加上这句话的必要性：不加这句话的会过早的接高SPI的cs信号 
	*在SPI_I2S_SendData后立即拉高CS信号，会有问题的。
	**/
	while (SPI_I2S_GetFlagStatus(t->spix, SPI_I2S_FLAG_RXNE) == RESET);
	//接收到一个数据		
	return SPI_I2S_ReceiveData(SPI2);
}