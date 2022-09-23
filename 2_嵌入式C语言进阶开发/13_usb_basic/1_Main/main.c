/**
 * @file main.c
 * @author 百问网开发团队
 * @brief  本例程USB的驱动源文件来源于上海灵动微科技有限公司提供的MindSDK
 * @version 0.1
 * @date 2022-05-30
 * 
 * @copyright Copyright (c) 2022 深圳百问网科技有限公司
 * 
 */

#include "drv_sysclk.h"
#include "drv_systick.h"
#include "drv_usb.h"
#include "hal_common.h"

/**
 * @brief ARM处理器程序入口函数
 * 
 * @return int 通常main函数不会有返回值
 */
int main(void)
{
    /* 初始化系统时钟频率为96MHz */
    /* USB时钟只能工作在48MHz下 */
    SystemClockConfig();
    /* 配置滴答定时器的技术周期是1kHz */
    Drv_SysTick_Config();
    /* 初始化USB */
    Drv_USB_Init();

    while(1)
    {
        /* TDO Your Task */
    }
}


