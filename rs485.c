#include "./BSP/RS485/rs485.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/Modbus/modbus.h"
UART_HandleTypeDef g_rs458_handler;     /* RS485���ƾ��(����) */

#ifdef RS485_EN_RX /* ���ʹ���˽��� */


void RS485_UX_IRQHandler(void)
{
    uint8_t res;

    if ((__HAL_UART_GET_FLAG(&g_rs458_handler, UART_FLAG_RXNE) != RESET)) /* ���յ����� */
    {
        HAL_UART_Receive(&g_rs458_handler, &res, 1, 1000);

        if( modbus.reflag==1)  //�����ݰ����ڴ���
	  {
		   return ;
		}		
	  modbus.rcbuf[modbus.recount++] = res;
		modbus.timout = 0;
		if(modbus.recount == 1)  //�Ѿ��յ��˵ڶ����ַ�����
		{
		  modbus.timrun = 1;  //����modbus��ʱ����ʱ
		}
    }
}

#endif

void rs485_init(uint32_t baudrate)
{
    RS485_RE_GPIO_CLK_ENABLE(); /* ʹ�� RS485_RE ��ʱ�� */
    RS485_TX_GPIO_CLK_ENABLE(); /* ʹ�� ����TX�� ʱ�� */
    RS485_RX_GPIO_CLK_ENABLE(); /* ʹ�� ����RX�� ʱ�� */
    RS485_UX_CLK_ENABLE();      /* ʹ�� ���� ʱ�� */

    GPIO_InitTypeDef gpio_initure;
    gpio_initure.Pin = RS485_TX_GPIO_PIN;
    gpio_initure.Mode = GPIO_MODE_AF_PP;
    gpio_initure.Pull = GPIO_PULLUP;
    gpio_initure.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(RS485_TX_GPIO_PORT, &gpio_initure); /* ����TX �� ģʽ���� */

    gpio_initure.Pin = RS485_RX_GPIO_PIN;
    gpio_initure.Mode = GPIO_MODE_AF_INPUT;
    HAL_GPIO_Init(RS485_RX_GPIO_PORT, &gpio_initure); /* ����RX �� �������ó�����ģʽ */

    gpio_initure.Pin = RS485_RE_GPIO_PIN;
    gpio_initure.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_initure.Pull = GPIO_PULLUP;
    gpio_initure.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(RS485_RE_GPIO_PORT, &gpio_initure); /* RS485_RE �� ģʽ���� */

    /* USART ��ʼ������ */
    g_rs458_handler.Instance = RS485_UX;                  /* ѡ��485��Ӧ�Ĵ��� */
    g_rs458_handler.Init.BaudRate = baudrate;             /* ������ */
    g_rs458_handler.Init.WordLength = UART_WORDLENGTH_8B; /* �ֳ�Ϊ8λ���ݸ�ʽ */
    g_rs458_handler.Init.StopBits = UART_STOPBITS_1;      /* һ��ֹͣλ */
    g_rs458_handler.Init.Parity = UART_PARITY_NONE;       /* ����żУ��λ */
    g_rs458_handler.Init.HwFlowCtl = UART_HWCONTROL_NONE; /* ��Ӳ������ */
    g_rs458_handler.Init.Mode = UART_MODE_TX_RX;          /* �շ�ģʽ */
    HAL_UART_Init(&g_rs458_handler);                      /* HAL_UART_Init()��ʹ��UART2 */

#if RS485_EN_RX 
    __HAL_UART_ENABLE_IT(&g_rs458_handler, UART_IT_RXNE); /* ���������ж� */
    HAL_NVIC_EnableIRQ(RS485_UX_IRQn);                    /* ʹ��USART2�ж� */
    HAL_NVIC_SetPriority(RS485_UX_IRQn, 2, 3);            /* ��ռ���ȼ�3�������ȼ�3 */
#endif
    RS485_RX_ENABLE; /* Ĭ��Ϊ����ģʽ */
}
void Modbus_Send_Byte(uint8_t *buf)
{
    HAL_UART_Transmit(&g_rs458_handler, buf, 1, 1000); /* ����2�������� */
}
