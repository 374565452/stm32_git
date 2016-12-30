#include "rds_encoder.h"
#include "FreeRTOS.h" 
#include "task.h"
#include "event_groups.h"
#include "common.h"
extern EventGroupHandle_t dog_event_group_handle;

uint8_t rds_protocol_buffer[RDS_PROTOCOL_LENGTH];

uint8_t rds_encoder_command[RDS_COMMAND_LENGTH];

uint8_t rds_protocol[RDS_PROTOCOLS_LENGTH];

static void rds_command_pack(uint8_t addr,uint8_t * data,uint8_t len);

/******************rds�������ĳ�ʼ������****************/
//rds����������������ʼ������
static void rds_encoder_var_init()
{
	memset(rds_protocol_buffer,0,RDS_PROTOCOL_LENGTH);
}
//��rds���������и�λ����
static void rds_encoder_reset()
{
	uint8_t val=RDS_COMMAND_RESET_ADDRESS_VALUE;
	rds_command_pack(RDS_COMMAND_RESET_ADDRESS,&val,1);
}

static void rds_encoder_extsync() //ǿ��rds������ʹ���ڲ�ʱ��Դ
{
	uint8_t val=0;
	rds_command_pack(RDS_COMMAND_6E,&val,1);
}
static void rds_encoder_pi() //����rds�������е�PIֵ 
{
	uint8_t val[2];
	val[0]=0x12;
	val[1]=0x34;
	rds_command_pack(RDS_COMMAND_00,val,2);
}

static void rds_encoder_ps() //dynamic ps����
{
	uint8_t val=0;
	rds_command_pack(RDS_COMMAND_72,&val,1);
}

static void rds_encoder_labper() //dynamic PS ������Ϣ
{
	uint8_t val=0;
	rds_command_pack(RDS_COMMAND_74,&val,1);
}

static void rds_encoder_psmod() //dynamic ps mod 
{
	uint8_t val=0;
	rds_command_pack(RDS_COMMAND_73,&val,1);
}
/******************rds�������ĳ�ʼ������****************/
static void send_rds_command(uint8_t * buffer,uint8_t len)
{
	hdl_uart_send_buffer(COM1,buffer,len);
}
//����RDS������оƬ���ϣ������ݴ��������
static void rds_command_pack(uint8_t addr,uint8_t * data,uint8_t len)
{
	uint8_t i =0 ;
	rds_encoder_command[0]=RDS_COMMAND_START;
	rds_encoder_command[1]=addr;
	for(i=0;i<len;i++)
	{
		rds_encoder_command[2+i]=data[i];
	}
	rds_encoder_command[2+len]=RDS_COMMAND_STOP;
	//����
	send_rds_command(rds_encoder_command,3+len);
}

//����RDS�Զ���Э�飬��buffer���ݼ��ϰ�ͷ���β
void pack_rds_protcol(uint8_t * buffer,uint8_t len)
{
	rds_protocol_buffer[0]=RDS_PROTOCOL_HEAD;
	memcpy(rds_protocol_buffer+1,buffer,len);
	rds_protocol_buffer[RDS_PROTOCOL_LENGTH-1]=RDS_PROTOCOL_TAIL;
	//debug_rds_send();
}

//����ȫ����
void send_all_open()
{
	//gprs_send_close=1;
	rds_protocol[0]=RDS_ALL_OPEN_BYTE;
	rds_protocol[1]=RDS_ALL_OPEN_BYTE;
	rds_protocol[2]=RDS_ALL_OPEN_BYTE;
	rds_protocol[3]=RDS_ALL_OPEN_BYTE;
	rds_protocol[4]=RDS_ALL_OPEN_BYTE;
	rds_protocol[5]=0x00;
	rds_protocol[6]=0x00;
	rds_protocol[7]=0x00;
	rds_protocol[8]=0x00;
	//У���õ��� m_util�еļ��㷽ʽ
	rds_protocol[9]=m_check_unsigned_sum(rds_protocol,(RDS_PROTOCOLS_LENGTH-1));
	
	pack_rds_protcol(rds_protocol,RDS_PROTOCOLS_LENGTH);
	
}
//����ȫ����
void send_all_close()
{
	//gprs_send_close=0;
	rds_protocol[0]=RDS_ALL_CLOSE_BYTE;
	rds_protocol[1]=RDS_ALL_CLOSE_BYTE;
	rds_protocol[2]=RDS_ALL_CLOSE_BYTE;
	rds_protocol[3]=RDS_ALL_CLOSE_BYTE;
	rds_protocol[4]=RDS_ALL_CLOSE_BYTE;
	rds_protocol[5]=0x00;
	rds_protocol[6]=0x00;
	rds_protocol[7]=0x00;
	rds_protocol[8]=0x00;
	
	rds_protocol[9]=m_check_unsigned_sum(rds_protocol,(RDS_PROTOCOLS_LENGTH-1));
	pack_rds_protcol(rds_protocol,RDS_PROTOCOLS_LENGTH);
}
void send_rds_command_77()
{
	
	rds_command_pack(RDS_COMMAND_77,rds_protocol_buffer,RDS_PROTOCOL_LENGTH);
}
void send_rds_command_76(uint8_t flag) //flag Ϊ0 ����00 Ϊ1����0C
{
	uint8_t val=0;
	if(flag==1)
	{
		//һ��RDSָ�����ɺ���Ҫ��������һ�����ݵĳ���
		//0x0C ��12������RDS����������ݳ���Ϊ12
		val=0x0C;
	}
	rds_command_pack(RDS_COMMAND_76,&val,1);
}




//RDS������������
void rds_encoder_task()
{
	uint8_t i=0;
	uint8_t init_delay=10;
	rds_encoder_var_init();
	
//	vTaskDelay(500);
//	xEventGroupSetBits(dog_event_group_handle, TASK_BIT_0);
//	vTaskDelay(500);
//	xEventGroupSetBits(dog_event_group_handle, TASK_BIT_0);
//	vTaskDelay(500);
//	xEventGroupSetBits(dog_event_group_handle, TASK_BIT_0);
//	
//		vTaskDelay(500);
//	xEventGroupSetBits(dog_event_group_handle, TASK_BIT_0);
//	vTaskDelay(500);
//	xEventGroupSetBits(dog_event_group_handle, TASK_BIT_0);
//	vTaskDelay(500);
//	xEventGroupSetBits(dog_event_group_handle, TASK_BIT_0);
//	
//	vTaskDelay(500);
//	xEventGroupSetBits(dog_event_group_handle, TASK_BIT_0);
//	
	
	
	//����rds�������ĳ�ʼ������
	
	rds_encoder_reset();
	//
	vTaskDelay(500); //��λ��1.5Sִ��֮��Ĳ��� �˴������������ͻ�ֱ�Ӹ�λ
	xEventGroupSetBits(dog_event_group_handle, TASK_BIT_0);
	vTaskDelay(500);
	xEventGroupSetBits(dog_event_group_handle, TASK_BIT_0);
	vTaskDelay(500);
	xEventGroupSetBits(dog_event_group_handle, TASK_BIT_0);
	printf("--------rds encoder reset suc--------\r\n");
	rds_encoder_extsync();
	//printf("--------rds encoder set sync--------\r\n");
	vTaskDelay(init_delay);
	rds_encoder_pi();
	
	vTaskDelay(init_delay);
	rds_encoder_ps();

	vTaskDelay(init_delay);
	rds_encoder_labper();

	vTaskDelay(init_delay);
	rds_encoder_psmod();

	vTaskDelay(init_delay);

	while(1)
	{
		//printf("--------rds encoder main logic running--------\r\n");
		if(rds_protocol_buffer[0] == RDS_PROTOCOL_HEAD)
		{
			//
			send_rds_command_76(0);
			//xEventGroupSetBits(dog_event_group_handle, TASK_BIT_0);
			vTaskDelay(init_delay);
			
			taskENTER_CRITICAL(); //�˴����õ�rds_protocol_buffer�������ȫ�ֱ����� ������OSҪ���뵽�ٽ���
			send_rds_command_77(); //����RDSָ��Э������
			taskEXIT_CRITICAL() ;  //����rds_protocol_buffer ��OS�˳��ٽ���
			//printf("--------rds_protocol_buffer is sending --------\r\n");
			send_rds_command_76(1); //�ٰ���η��͵�RDSָ��Э�����ݵĳ��ȷ��͸�������оƬ
		}
		//ÿ��5.6S����һ��RDSָ��
			/* �����¼���־ 0����ʾ������������ */
		for(i=0;i<11;i++)
		{
			xEventGroupSetBits(dog_event_group_handle, TASK_BIT_0);
			vTaskDelay(500); //ÿ��500msѭ��ִ��һ��
		}
	}
}
