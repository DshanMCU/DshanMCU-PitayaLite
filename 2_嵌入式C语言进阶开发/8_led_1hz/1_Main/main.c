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
#include "hal_common.h"

/**
 * @brief ARM处理器程序入口函数
 * 
 * @return int 通常main函数不会有返回值
 */
int main(void)
{
    /* 初始化系统时钟频率为120MHz */
    SystemClockConfig();
    /* 配置滴答定时器的技术周期是1kHz */
    Drv_SysTick_Config();
    /* 初始化用户LED的GPIO和默认状态 */
    Drv_LED_Init();

    while(1)
    {
        /* TDO Your Task */
        Drv_LED_Write(user_led, on);    // 点亮LED
        HAL_Delay(500);                 // 延时500ms
        Drv_LED_Write(user_led, off);   // 熄灭LED
        HAL_Delay(500);                 // 延时500ms
    }
}


