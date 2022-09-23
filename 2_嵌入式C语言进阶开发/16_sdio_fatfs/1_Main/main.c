/**
 * @file main.c
 * @author 百问网开发团队
 * @brief 
 * @version 0.1
 * @date 2022-05-30
 * 
 * @copyright Copyright (c) 2022 深圳百问网科技有限公司
 * 
 */

#include "drv_led.h"
#include "drv_sysclk.h"
#include "drv_systick.h"
#include "drv_uart.h"
#include "hal_common.h"

#include "ffconf.h"
#include "ff.h"

#include <stdio.h>

FATFS fs;
FIL fil;
const TCHAR fs_drv[] = "1:/";
const TCHAR filname[] = "1:/hello.txt";
BYTE work[FF_MAX_SS]; /* Work area (larger is better for processing time) */
UINT bw, br;
#define FIL_BUFF_LEN 16u
uint8_t fil_write_buff[FIL_BUFF_LEN];
uint8_t fil_read_buff[FIL_BUFF_LEN];

FRESULT fs_ret;

uint8_t fil_write_seed = 0u;

/**
 * @brief ARM处理器程序入口函数
 * 
 * @return int 通常main函数不会有返回值
 */
int main(void)
{
    uint8_t c;
    uint32_t tmp32;
    /* 定义LED的状态变量，按键按下时取反 */
    LED_Status status = off;
    /* 初始化系统时钟频率为120MHz */
    SystemClockConfig();
    /* 配置滴答定时器的技术周期是1kHz */
    Drv_SysTick_Config();
    /* 初始化用户LED的GPIO和默认状态 */
    Drv_LED_Init();
    /* 初始化串口 */
    Drv_UART_Init();

    printf("fatfs_basic\r\n");

    while(1)
    {
        /* TDO Your Task */
        c = getchar();
        switch (c)
        {
            case 'a':
                printf("a: f_mkfs().\r\n");
            
                if ( !f_mkfs(fs_drv, 0u, work, sizeof(work)) )
                {
                    printf("success.\r\n");
                }
                else
                {
                    printf("failed.\r\n");
                }
                break;

            case 'b':
                printf("b: f_mount().\r\n");
                if( !f_mount(&fs, fs_drv ,1) )
                {
                    printf("success.\r\n");
                }
                else
                {
                    printf("failed.\r\n");
                }
                break;

            case 'c':
                printf("c: f_open() & f_write() & f_close().\r\n");
                
                printf("f_open().\r\n");
                fs_ret = f_open(&fil, filname, FA_CREATE_NEW | FA_WRITE );
                if ( fs_ret != FR_OK )
                {
                    printf("failed.\r\n");
                    break;
                }
                
                printf("f_write().\r\n");
                
                sprintf((char *)fil_write_buff, "hi, %d\r\n", fil_write_seed++), 

                fs_ret = f_write(&fil, fil_write_buff, FIL_BUFF_LEN, &bw);
                if (bw != FIL_BUFF_LEN)
                {
                    printf("failed.\r\n");
                    
                    f_close(&fil);
                    break;
                }
                
                printf("f_close().\r\n");
                f_close(&fil);
                
                printf("success.\r\n");

                break;
                
            case 'd':
                printf("d: f_open() & f_read() & f_close().\r\n");
                
                printf("f_open().\r\n");
                fs_ret = f_open(&fil, filname, FA_READ );
                if ( fs_ret != FR_OK )
                {
                    printf("failed.\r\n");
                    break;
                }
            
                printf("f_read()\r\n");
                fs_ret = f_read(&fil, fil_read_buff, FIL_BUFF_LEN, &br);
                if (br != FIL_BUFF_LEN)
                {
                    printf("failed.\r\n");
                    
                    f_close(&fil);
                    break;
                }
                
                for (uint32_t i  = 0u; i < FIL_BUFF_LEN; i++)
                {
                    printf("%c", fil_read_buff[i]);
                }
                printf("\r\n");
                
                printf("f_close().\r\n");
                f_close(&fil);
                
                printf("success.\r\n");
                
                break;
                
            case 'f':
                printf("f: f_unlink().\r\n");
            
                f_unlink(filname);
                
                printf("file removed.\r\n");

                break;
                
            default: /* help. */
                printf("\r\n");

                printf("a: f_mkfs().\r\n");
                printf("b: f_mount().\r\n");
                printf("c: f_open() & f_write() & f_close().\r\n");
                printf("d: f_open() & f_read() & f_close().\r\n");
                printf("f: f_unlink().\r\n");

                printf("\r\n");
                break;
        }
    }
}


