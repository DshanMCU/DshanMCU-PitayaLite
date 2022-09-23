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

#include "drv_sysclk.h"
#include "hal_common.h"

/**
 * @brief ARM处理器程序入口函数main函数
 * 
 * @return int 
 * @retval 通常情况下不会从main函数返回数值
 */
int main(void)
{
    /* 初始化系统时钟 */
    SystemClockConfig();

    /* 获取配置好的系统时钟频率 */
    /* 调试将sys_freq加入watch中观察验证 */
    volatile uint32_t sys_freq = Drv_GetSysClockFreq();
    while(1)
    {
        /* TDO Your Task */
    }
}


