#ifndef __modbus_H
#define __modbus_H
#include "stdio.h"	
#include "./SYSTEM/sys/sys.h"

typedef struct 
{
	//��Ϊ�ӻ�ʱʹ��
  uint8_t  myadd;        //���豸�ӻ���ַ
	uint8_t  rcbuf[100];   //modbus���ܻ�����
	uint8_t  timout;       //modbus���ݳ���ʱ��
	uint8_t  recount;      //modbus�˿ڽ��յ������ݸ���
	uint8_t  timrun;       //modbus��ʱ���Ƿ��ʱ��־
	uint8_t  reflag;       //modbusһ֡���ݽ�����ɱ�־λ
	uint8_t  sendbuf[100]; //modbus�ӷ��ͻ�����
	
}MODBUS;

extern MODBUS modbus;
extern uint16_t Reg[];
void Modbus_Init(void);
void Modbus_Func3(void);//���Ĵ�������
void Modbus_Func6(void);//��1���Ĵ�����д������
void Modbus_Func16(void);//������Ĵ�����д������
void Modbus_Event(void);
uint16_t  Modbus_CRC16( uint8_t *puchMsg, uint16_t usDataLen );

#endif


