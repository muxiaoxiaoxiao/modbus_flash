#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/LED/led.h"
#include "./BSP/RS485/rs485.h"
#include "./BSP/Modbus/modbus.h"
#include "./BSP/TIMER/timer.h"
#include "./BSP/STMFLASH/stmflash.h"
uint16_t g_text_buf[] = {
            0x0008,
            0x0008,
            0x0008,
            0x0008,
	        0x0008,
            0x0008,
            0x0008,
						0X0009,};

#define TEXT_LENTH sizeof(g_text_buf) /* 数组长度 */

/*SIZE表示半字长(2字节), 大小必须是2的整数倍, 如果不是的话, 强制对齐到2的整数倍 */
#define SIZE TEXT_LENTH / 2 + ((TEXT_LENTH % 2) ? 1 : 0)

#define FLASH_SAVE_ADDR 0x08010000 /* 设置FLASH 保存地址(必须为4的整数倍，且其值要大于本代码所占用FLASH的大小 + 0X08000000) */

int main(void)
{
    uint16_t datatemp[SIZE];
    HAL_Init();                                 /* 初始化HAL库 */
    sys_stm32_clock_init(RCC_PLL_MUL9);         /* 设置时钟, 72Mhz */
    delay_init(72);                             /* 延时初始化 */
    led_init();                                 /* 初始化LED */ 
    rs485_init(9600);
    Modbus_Init();
    gtim_timx_int_init(7200-1,10-1);
    while (1)
    {	
			Modbus_Event();
			LED1(0);
			LED0(0);
			delay_ms(200);
			if (Reg[0]==0x0001){Reg[0]=0x0000;
			stmflash_write(FLASH_SAVE_ADDR, (uint16_t *)g_text_buf, SIZE);
			}
			if(Reg[1]==0x0001){Reg[1]=0x0000;
			stmflash_read(FLASH_SAVE_ADDR, (uint16_t *)datatemp, SIZE);
			for (int i = 0; i < SIZE; i++) 
			{
				Reg[i] = datatemp[i];
			}
			}
			delay_ms(10);
		}
}

