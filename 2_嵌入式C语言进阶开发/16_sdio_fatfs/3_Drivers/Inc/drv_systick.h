/**
 * @file drv_systick.h
 * @author 百问网开发团队
 * @brief 
 * @version 0.1
 * @date 2022-05-30
 * 
 * @copyright Copyright (c) 2022 深圳百问网科技有限公司
 * 
 */

#ifndef __DRV_SYSTICK_H
#define __DRV_SYSTICK_H

/**
 * @brief 配置内核滴答定时器的驱动函数，默认周期是1kHz
 * 
 * @return int 
 * @retval 0 成功
 * @retval 1 失败
 */
int Drv_SysTick_Config(void);

/**
 * @brief 获取系统运行的tick计数值
 * 
 * @return unsigned int 滴答定时器以1kHz计数的值
 */
unsigned int HAL_GetTick(void);

/**
 * @brief 利用滴答定时器实现的延时函数，默认单位是ms
 * 
 * @param Delay 
 */
void HAL_Delay(unsigned int Delay);

#endif /* __DRV_SYSTICK_H */
