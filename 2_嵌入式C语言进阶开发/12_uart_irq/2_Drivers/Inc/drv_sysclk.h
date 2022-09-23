/**
 * @file drv_sysclk.h
 * @author 百问网开发团队
 * @brief 
 * @version 0.1
 * @date 2022-05-30
 * 
 * @copyright Copyright (c) 2022 2022 深圳百问网科技有限公司
 * 
 */

#ifndef __DRV_SYSCLK_H
#define __DRV_SYSCLK_H

/**
 * @brief 系统时钟配置函数
 * -# 使用内部高速时钟HSI作为系统时钟源
 */
void SystemClockConfig(void);

/**
 * @brief 获取系统时钟频率
 * 
 * @return uint32_t 
 * @retval 系统时钟频率
 */
unsigned int Drv_GetSysClockFreq(void);

#endif /* __DRV_SYSCLK_H */
