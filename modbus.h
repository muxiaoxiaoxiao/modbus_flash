#ifndef __modbus_H
#define __modbus_H
#include "stdio.h"	
#include "./SYSTEM/sys/sys.h"

typedef struct 
{
	//作为从机时使用
  uint8_t  myadd;        //本设备从机地址
	uint8_t  rcbuf[100];   //modbus接受缓冲区
	uint8_t  timout;       //modbus数据持续时间
	uint8_t  recount;      //modbus端口接收到的数据个数
	uint8_t  timrun;       //modbus定时器是否计时标志
	uint8_t  reflag;       //modbus一帧数据接受完成标志位
	uint8_t  sendbuf[100]; //modbus接发送缓冲区
	
}MODBUS;

extern MODBUS modbus;
extern uint16_t Reg[];
void Modbus_Init(void);
void Modbus_Func3(void);//读寄存器数据
void Modbus_Func6(void);//往1个寄存器中写入数据
void Modbus_Func16(void);//往多个寄存器中写入数据
void Modbus_Event(void);
uint16_t  Modbus_CRC16( uint8_t *puchMsg, uint16_t usDataLen );

#endif


