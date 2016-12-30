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

/******************rds编码器的初始化操作****************/
//rds编码器参数变量初始化操作
static void rds_encoder_var_init()
{
	memset(rds_protocol_buffer,0,RDS_PROTOCOL_LENGTH);
}
//对rds编码器进行复位操作
static void rds_encoder_reset()
{
	uint8_t val=RDS_COMMAND_RESET_ADDRESS_VALUE;
	rds_command_pack(RDS_COMMAND_RESET_ADDRESS,&val,1);
}

static void rds_encoder_extsync() //强制rds编码器使用内部时钟源
{
	uint8_t val=0;
	rds_command_pack(RDS_COMMAND_6E,&val,1);
}
static void rds_encoder_pi() //设置rds编码器中的PI值 
{
	uint8_t val[2];
	val[0]=0x12;
	val[1]=0x34;
	rds_command_pack(RDS_COMMAND_00,val,2);
}

static void rds_encoder_ps() //dynamic ps设置
{
	uint8_t val=0;
	rds_command_pack(RDS_COMMAND_72,&val,1);
}

static void rds_encoder_labper() //dynamic PS 参数信息
{
	uint8_t val=0;
	rds_command_pack(RDS_COMMAND_74,&val,1);
}

static void rds_encoder_psmod() //dynamic ps mod 
{
	uint8_t val=0;
	rds_command_pack(RDS_COMMAND_73,&val,1);
}
/******************rds编码器的初始化操作****************/
static void send_rds_command(uint8_t * buffer,uint8_t len)
{
	hdl_uart_send_buffer(COM1,buffer,len);
}
//按照RDS编码器芯片资料，将数据打包，发送
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
	//发送
	send_rds_command(rds_encoder_command,3+len);
}

//按照RDS自定义协议，将buffer数据加上包头与包尾
void pack_rds_protcol(uint8_t * buffer,uint8_t len)
{
	rds_protocol_buffer[0]=RDS_PROTOCOL_HEAD;
	memcpy(rds_protocol_buffer+1,buffer,len);
	rds_protocol_buffer[RDS_PROTOCOL_LENGTH-1]=RDS_PROTOCOL_TAIL;
	//debug_rds_send();
}

//发送全开码
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
	//校验用的是 m_util中的计算方式
	rds_protocol[9]=m_check_unsigned_sum(rds_protocol,(RDS_PROTOCOLS_LENGTH-1));
	
	pack_rds_protcol(rds_protocol,RDS_PROTOCOLS_LENGTH);
	
}
//发送全关码
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
void send_rds_command_76(uint8_t flag) //flag 为0 发送00 为1发送0C
{
	uint8_t val=0;
	if(flag==1)
	{
		//一条RDS指令发送完成后，需要将发送这一条数据的长度
		//0x0C 是12，我们RDS所定义的数据长度为12
		val=0x0C;
	}
	rds_command_pack(RDS_COMMAND_76,&val,1);
}




//RDS编码器的任务
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
	
	
	//进行rds编码器的初始化操作
	
	rds_encoder_reset();
	//
	vTaskDelay(500); //复位后1.5S执行之后的操作 此处若不作处理，就会直接复位
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
			
			taskENTER_CRITICAL(); //此处会用到rds_protocol_buffer这个共享全局变量中 ，所以OS要进入到临界区
			send_rds_command_77(); //发送RDS指令协议数据
			taskEXIT_CRITICAL() ;  //不用rds_protocol_buffer ，OS退出临界区
			//printf("--------rds_protocol_buffer is sending --------\r\n");
			send_rds_command_76(1); //再把这次发送的RDS指令协议数据的长度发送给编码器芯片
		}
		//每隔5.6S发送一次RDS指令
			/* 发送事件标志 0，表示任务正常运行 */
		for(i=0;i<11;i++)
		{
			xEventGroupSetBits(dog_event_group_handle, TASK_BIT_0);
			vTaskDelay(500); //每隔500ms循环执行一次
		}
	}
}
