/**
 * @file drv_sysclk.c
 * @author 百问网开发团队
 * @brief 
 * @version 0.1
 * @date 2022-05-30
 * 
 * @copyright Copyright (c) 2022 2022 深圳百问网科技有限公司
 * 
 */

#include "drv_sysclk.h"

/**
 * @brief 系统时钟配置函数
 * -# 使用外部高速时钟HSE作为系统时钟源
 */
void SystemClockConfig(void)
{
    /* 使能总线外设时钟 */
    RCC->AHB1ENR |= (1u << 13u);    // 使能FLASH外设
    FLASH->ACR |= (4<<0);           // 设置Flash的等待周期
    
    /* 使能PWR/DBG */
    RCC->APB1ENR |= (1<<28);
    
    RCC->CR &= ~((1<<16) | (1<<24) ); // 关闭HSE/PLL
    
    /* 使能HSE */
    RCC->CR |= (1<<16);
    while(0 == (RCC->CR&(1<<17)));// 等待HSE稳定
    
    /* 选择HSI作为系统时钟源 */
    RCC->CFGR |= (1<<0);     // 00：选择 HSE 输出用作系统时钟; 01：选择 HSE 输出用作系统时钟; 10：选择 PLL 输出用作系统时钟; 11：选择 LSI 输出用作系统时钟
    while(0 == (RCC->CFGR&(1<<2))); // 等待HSE用作系统时钟变
}

/**
 * @brief 获取系统时钟频率
 * 
 * @return uint32_t 
 * @retval 系统时钟频率
 */
uint32_t Drv_GetSysClockFreq(void)
{
    uint32_t tmpreg = 0U, prediv = 0U, pllclk = 0U, pllmul = 0U;
    uint32_t sysclockfreq = 0U;
    
    tmpreg = RCC->CFGR;
    /* 获取系统时钟源 */
    switch(tmpreg & RCC_CFGR_SWS_MASK)
    {
        case RCC_SYSCLKSOURCE_STATUS_HSI:
        {
            sysclockfreq = HSI_VALUE;
            break;
        }
        case RCC_SYSCLKSOURCE_STATUS_HSE:
        {
            sysclockfreq = HSE_VALUE;
            break;
        }
        case RCC_SYSCLKSOURCE_STATUS_LSI:
        {
            sysclockfreq = LSI_VALUE;
            break;
        }
        case RCC_SYSCLKSOURCE_STATUS_PLLCLK:
        {
            /* 获取PLL的输入时钟源 */
            if(RCC->PLLCFGR&0x01)       // HSE用作PLL的输入时钟
            {
                if(RCC->PLLCFGR&0x02)   // HSE二分频后输入给PLL
                {
                    pllclk = HSE_VALUE>>1;
                }
                else                    // HSE部分变频直接输出给PLL
                {
                    pllclk = HSE_VALUE;
                }
            }
            else    // HSI用作PLL的输入时钟
            {
                pllclk = HSI_VALUE;
            }
            prediv = (RCC->PLLCFGR>>8)&0x07;    // PLL的分频系数：PLLCFGR[10:8]
            pllmul = (RCC->PLLCFGR>>16)&0x7F;   // PLL的倍频系数: PLLCFGR[22:16]
            sysclockfreq = pllclk * (pllmul+1) / (prediv+1);
            
            break;
        }
        default:break;
    }
    
    return sysclockfreq;
}
