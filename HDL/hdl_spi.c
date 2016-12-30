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
		SPI��������ʱ���Ƚ�Ƭѡ�źŽ������Ͳ�����������ݷ��Ͷ�����Ȼ���ٽ�Ƭѡ�źŽ������߲���
		
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    2016-11-28        ��ʽ����
*
*********************************************************************************************************
*/
#include "hdl_spi.h"

/*
*********************************************************************************************************
*	�� �� ��: hdl_init_spi
*	����˵��: spiӲ����ʼ������
*	��    �Σ�spi_t * t ,����spi�������� �Ľṹ��ָ��
*	�� �� ֵ: ��
*
*
* ע��spi��Ƭѡ�ź� Ӧ������Ϊ�������
			����Ķ˿�����Ϊ�����������
			�����������SPIǰҪ�Դ�ģʽ�˽�����������豸֧�ֵ�ʱ�ӷ�Χ��ģʽ��CPOL��CPHA��
*********************************************************************************************************
*/
void hdl_init_spi(spi_t * t)
{
	SPI_InitTypeDef   SPI_InitStructure;
	
	hdl_set_rcc(t->spi_rcc,ENABLE,t->spi_rcc_type);
	hdl_set_rcc(t->port_rcc,ENABLE,2);
	hdl_output_gpio(t->spi_gpio,t->cs_port,GPIO_Mode_Out_PP);//Ƭѡ�ź�����Ϊ�������
	hdl_output_gpio(t->spi_gpio,t->clk_port,GPIO_Mode_AF_PP);
	hdl_output_gpio(t->spi_gpio,t->miso_port,GPIO_Mode_AF_PP);
	hdl_output_gpio(t->spi_gpio,t->mosi_port,GPIO_Mode_AF_PP);
	
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//����SPI����ģʽ:����Ϊ��SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = t->spi_cpol;		//ѡ���˴���ʱ�ӵ���̬:ʱ�����ո�
	SPI_InitStructure.SPI_CPHA = t->spi_cpha;	//���ݲ����ڵڶ���ʱ����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
	SPI_InitStructure.SPI_BaudRatePrescaler = t->spi_baud_rate;		//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRCֵ����Ķ���ʽ
	SPI_Init(t->spix, &SPI_InitStructure);  //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
 
	SPI_Cmd(t->spix, ENABLE); //ʹ��SPI����
}

/*
*********************************************************************************************************
*	�� �� ��: hdl_send_write_byte
*	����˵��: spi��ȡ����һ���ֽ�����
*	��    �Σ�spi_t * t ,����spi�������� �Ľṹ��ָ��
						uint8_t data����Ҫ���͵�����
*	�� �� ֵ:  uint8_t ���ض�ȡ��������
*
*
* ע��ͨ��SPI����������д���ݣ����ǵ��ô˺���
			����Ƕ����ݣ���Ҫ�ȷ���һ���ֽڣ�����ֽڿ�����0x00��Ȼ��Ϳ��Զ�ȡ���ش���������
			Ϊʲô����������Ϊ��ģʽ�²����ṩʱ�ӣ����Զ�����Ҫ�ȷ���һ���ֽ�
*********************************************************************************************************
*/
uint8_t hdl_read_write_byte(spi_t * t,uint8_t data)
{
	//ȷ���ϴ��Ѿ��������
	 while (SPI_I2S_GetFlagStatus(t->spix, SPI_I2S_FLAG_TXE) == RESET);
  //���Ĵ�����д��һ���ֽ�
	SPI_I2S_SendData(t->spix, data);
	//�ȴ�����һ���ֽ�����
	/***
	*������һ������Ŀ�ģ���ȷ������ֽ��Ѿ����ͳ�ȥ����Ϊ����������ǲ���ͬ������
	*��˼�ǣ�����һ���ֽ���ζ�žͿ��Խ��յ�һ���ֽ�
	*������仰�ı�Ҫ�ԣ�������仰�Ļ����ĽӸ�SPI��cs�ź� 
	*��SPI_I2S_SendData����������CS�źţ���������ġ�
	**/
	while (SPI_I2S_GetFlagStatus(t->spix, SPI_I2S_FLAG_RXNE) == RESET);
	//���յ�һ������		
	return SPI_I2S_ReceiveData(SPI2);
}