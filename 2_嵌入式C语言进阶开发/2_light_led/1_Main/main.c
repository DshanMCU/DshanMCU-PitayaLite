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
#include <hal_common.h>

/**
 * @brief 软件延时函数
 * 
 * @param x 延时时间
 */
static void sw_delay(uint32_t x)
{
    volatile uint32_t i = x;
    volatile uint32_t j = 100;
    while(j--)
    {
        for(i=0; i<x; i++)
        {
            
        }
    }
}

/**
 * @brief ARM处理器程序入口函数main函数
 * 
 * @return int 
 * @retval 通常情况下不会从main函数返回数值
 */
int main(void)
{
    /* 初始化LED */
    Drv_LED_Init();
    while(1)
    {
        /* TDO Your Task */
        Drv_LED_Write(user_led, on);    // 点亮LED
        sw_delay(1000);
        
        Drv_LED_Write(user_led, off);   // 熄灭LED
        sw_delay(1000);
    }
}


