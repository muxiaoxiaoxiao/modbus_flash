#ifndef __STMFLASH_H
#define __STMFLASH_H

#include "./SYSTEM/sys/sys.h"


#define STM32_FLASH_SIZE        0x80000        
#define STM32_FLASH_BASE        0x08000000      

#if STM32_FLASH_SIZE < 256 * 1024
#define STM32_SECTOR_SIZE   1024               
#else
#define STM32_SECTOR_SIZE   2048               
#endif

#define STM32_FLASH_KEY1        0X45670123
#define STM32_FLASH_KEY2        0XCDEF89AB

static void stmflash_unlock(void);                 
static void stmflash_lock(void);                    /* 锁定STM32 内部FLASH */
static uint8_t stmflash_get_error_status(void);     /* 获取FLASH错误状态 */
static uint8_t stmflash_wait_done(uint32_t time);   /* 等待操作完成 */
static uint8_t stmflash_erase_sector(uint32_t saddr);                   /* 擦除扇区 */
static uint8_t stmflash_write_halfword(uint32_t faddr, uint16_t data);  /* FLASH写半字 */

uint16_t stmflash_read_halfword(uint32_t faddr);                        /* FLASH读半字 */
void stmflash_read(uint32_t raddr, uint16_t *pbuf, uint16_t length);    /* 从指定地址开始读出指定长度的数据 */
void stmflash_write(uint32_t waddr, uint16_t *pbuf, uint16_t length);   /* 在FLASH 指定位置, 写入指定长度的数据(自动擦除) */

void test_write(uint32_t waddr, uint16_t wdata);

#endif


















