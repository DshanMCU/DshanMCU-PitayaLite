/**
 * @file drv_systick.c
 * @author 百问网开发团队
 * @brief 
 * @version 0.1
 * @date 2022-05-30
 * 
 * @copyright Copyright (c) 2022 深圳百问网科技有限公司
 * 
 */

#include "hal_common.h"
#include "drv_sysclk.h"

/**
 * @brief 静态全局变量
 * @param uwTick 滴答定时器控制的计数值，用以表明系统运行的tick数以及延时函数用
 */
static uint32_t uwTick = 0;

/**
 * @brief 配置内核滴答定时器的驱动函数，默认周期是1kHz
 * 
 * @return int 
 * @retval 0 成功
 * @retval 1 失败
 */
int Drv_SysTick_Config(void)
{
    volatile uint32_t prioritygroup = 0x00U;
    volatile uint32_t SystemCoreClock = Drv_GetSysClockFreq();
    /* Configure the SysTick to have interrupt in 1ms time basis*/
    if(SysTick_Config(SystemCoreClock/1000) > 0)
    {
        return -1;
    }
    /* Configure the SysTick IRQ priority */
    prioritygroup = NVIC_GetPriorityGrouping();
    NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(prioritygroup, 15, 0));
    
    return 0;
}

/**
 * @brief 内核滴答定时器的中断服务函数
 * 
 */
void SysTick_Handler(void)
{
    uwTick++;
}

/**
 * @brief 获取系统运行的tick计数值
 * 
 * @return unsigned int 滴答定时器以1kHz计数的值
 */
unsigned int HAL_GetTick(void)
{
    return uwTick;
}

/**
 * @brief 利用滴答定时器实现的延时函数，默认单位是ms
 * 
 * @param Delay 
 */
void HAL_Delay(unsigned int Delay)
{
  volatile uint32_t tickstart = HAL_GetTick();
  volatile uint32_t wait = Delay;

  /* Add a freq to guarantee minimum wait */
  if (wait < 0xFFFFFFFFU)
  {
    wait += (uint32_t)(1);
  }

  while ((HAL_GetTick() - tickstart) < wait)
  {
  }
}