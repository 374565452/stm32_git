#ifndef __RDS_ENCODER_H
#define __RDS_ENCODER_H

#define RDS_PROTOCOLS_LENGTH 10
#define RDS_ALL_OPEN_BYTE 0x01
#define RDS_ALL_CLOSE_BYTE 0x02

#define RDS_PROTOCOL_HEAD 0xAA
#define RDS_PROTOCOL_TAIL 0x55

#define RDS_COMMAND_START 0xFE
#define RDS_COMMAND_STOP 0xFF
#define RDS_COMMAND_6E 0x6E
#define RDS_COMMAND_00 0x00
#define RDS_COMMAND_72 0x72
#define RDS_COMMAND_74 0x74
#define RDS_COMMAND_73 0x73
#define RDS_COMMAND_76 0x76
#define RDS_COMMAND_77 0x77


#define RDS_COMMAND_RESET_ADDRESS 0x71 
#define RDS_COMMAND_RESET_ADDRESS_VALUE 0x52

#define RDS_PROTOCOL_LENGTH 12
#define RDS_COMMAND_LENGTH 20

#include "hdl_include.h"
#include "m_util.h"

void rds_encoder_task();

void pack_rds_protcol(uint8_t * buffer,uint8_t len);

void send_all_open();
void send_all_close();
#endif