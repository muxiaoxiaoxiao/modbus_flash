#include "./BSP/RS485/rs485.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/Modbus/modbus.h"
UART_HandleTypeDef g_rs458_handler;     /* RS485控制句柄(串口) */

#ifdef RS485_EN_RX /* 如果使能了接收 */


void RS485_UX_IRQHandler(void)
{
    uint8_t res;

    if ((__HAL_UART_GET_FLAG(&g_rs458_handler, UART_FLAG_RXNE) != RESET)) /* 接收到数据 */
    {
        HAL_UART_Receive(&g_rs458_handler, &res, 1, 1000);

        if( modbus.reflag==1)  //有数据包正在处理
	  {
		   return ;
		}		
	  modbus.rcbuf[modbus.recount++] = res;
		modbus.timout = 0;
		if(modbus.recount == 1)  //已经收到了第二个字符数据
		{
		  modbus.timrun = 1;  //开启modbus定时器计时
		}
    }
}

#endif

void rs485_init(uint32_t baudrate)
{
    RS485_RE_GPIO_CLK_ENABLE(); /* 使能 RS485_RE 脚时钟 */
    RS485_TX_GPIO_CLK_ENABLE(); /* 使能 串口TX脚 时钟 */
    RS485_RX_GPIO_CLK_ENABLE(); /* 使能 串口RX脚 时钟 */
    RS485_UX_CLK_ENABLE();      /* 使能 串口 时钟 */

    GPIO_InitTypeDef gpio_initure;
    gpio_initure.Pin = RS485_TX_GPIO_PIN;
    gpio_initure.Mode = GPIO_MODE_AF_PP;
    gpio_initure.Pull = GPIO_PULLUP;
    gpio_initure.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(RS485_TX_GPIO_PORT, &gpio_initure); /* 串口TX 脚 模式设置 */

    gpio_initure.Pin = RS485_RX_GPIO_PIN;
    gpio_initure.Mode = GPIO_MODE_AF_INPUT;
    HAL_GPIO_Init(RS485_RX_GPIO_PORT, &gpio_initure); /* 串口RX 脚 必须设置成输入模式 */

    gpio_initure.Pin = RS485_RE_GPIO_PIN;
    gpio_initure.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_initure.Pull = GPIO_PULLUP;
    gpio_initure.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(RS485_RE_GPIO_PORT, &gpio_initure); /* RS485_RE 脚 模式设置 */

    /* USART 初始化设置 */
    g_rs458_handler.Instance = RS485_UX;                  /* 选择485对应的串口 */
    g_rs458_handler.Init.BaudRate = baudrate;             /* 波特率 */
    g_rs458_handler.Init.WordLength = UART_WORDLENGTH_8B; /* 字长为8位数据格式 */
    g_rs458_handler.Init.StopBits = UART_STOPBITS_1;      /* 一个停止位 */
    g_rs458_handler.Init.Parity = UART_PARITY_NONE;       /* 无奇偶校验位 */
    g_rs458_handler.Init.HwFlowCtl = UART_HWCONTROL_NONE; /* 无硬件流控 */
    g_rs458_handler.Init.Mode = UART_MODE_TX_RX;          /* 收发模式 */
    HAL_UART_Init(&g_rs458_handler);                      /* HAL_UART_Init()会使能UART2 */

#if RS485_EN_RX 
    __HAL_UART_ENABLE_IT(&g_rs458_handler, UART_IT_RXNE); /* 开启接收中断 */
    HAL_NVIC_EnableIRQ(RS485_UX_IRQn);                    /* 使能USART2中断 */
    HAL_NVIC_SetPriority(RS485_UX_IRQn, 2, 3);            /* 抢占优先级3，子优先级3 */
#endif
    RS485_RX_ENABLE; /* 默认为接收模式 */
}
void Modbus_Send_Byte(uint8_t *buf)
{
    HAL_UART_Transmit(&g_rs458_handler, buf, 1, 1000); /* 串口2发送数据 */
}
