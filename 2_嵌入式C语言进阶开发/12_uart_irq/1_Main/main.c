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

#include <stdio.h>
#include <string.h>

/**
 * @brief ARM处理器程序入口函数
 * 
 * @return int 通常main函数不会有返回值
 */
int main(void)
{
    uint16_t i = 0;
    char buf[128] = {0};
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
    printf("Start->->->->\r\n");

    while(1)
    {
        /* TDO Your Task */
        if(Drv_UART_Read((unsigned char*)&buf[i%128], 128) == 0)    continue;
        i++;
        if(strstr(buf, "led on") != 0)
        {
            i = 0;
            Drv_LED_Write(user_led, on);    // 控制LED
            printf("LED状态: on\r\n");
            memset(buf, 0, 128);
        }
        else if(strstr(buf, "led off") != 0)
        {
            i = 0;
            Drv_LED_Write(user_led, off);   // 控制LED
            printf("LED状态: off\r\n");
            memset(buf, 0, 128);
        }
        else
            printf("%s\r", buf);
    }
}


