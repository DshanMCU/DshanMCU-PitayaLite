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
 * -# 使用外部高速时钟HSE作为PLL时钟源
 * -# PLL将HSE进行2分频后再20倍频得到最高主频12MHz
 */
void SystemClockConfig(void)
{
    /* 使能总线外设时钟 */
    RCC->AHB1ENR |= (1u << 13u);    // 使能FLASH外设
    FLASH->ACR |= (4<<0);           // 设置Flash的等待周期
    
    /* 使能PWR/DBG */
    RCC->APB1ENR |= (1<<28);

    /* 如果系统时钟需要达到最大频率 120MHz，需要将 VOS 设置为 2’b10 即 1.7V */
    PWR->CR1 &= ~(2<<14);
    PWR->CR1 |= (2<<14);
    
    RCC->CR &= ~((1<<16) | (1<<24) ); // 关闭HSE/PLL
    
    /* 使能HSE */
    RCC->CR |= (1<<16);             // 使能HSE
    while(0 == ((RCC->CR)&(1<<17)));// 等待HSE稳定
    
    /* 配置PLL */
    RCC->PLLCFGR |= (1<<0);         // 配置PLL的时钟源HSE
    RCC->PLLCFGR &= ~(1<<1);        // 配置PLL的时钟源HSE不分频后再作为时钟输入源
    RCC->PLLCFGR &= ~(0x7F<<16);
    RCC->PLLCFGR |= (19<<16);       // 配置PLL的倍频系数:20倍
    RCC->PLLCFGR &= ~(0x7<<8);
    RCC->PLLCFGR |= (1<<8);         // 配置PLL的分频系数:2
    
    RCC->CR |= (1<<24);             // 使能PLL
    while(0 == (RCC->CR & (1<<25)));// 等待PLL时钟稳定

    /* 配置系统时钟、AHB、APB时钟 */
    RCC->CFGR &= ~(0xF<<4);         // AHB不分频
    RCC->CFGR |= (4<<8);            // APB1 2分频
    RCC->CFGR |= (4<<11);           // APB2 2分频
    RCC->CFGR |= (2<<22);           // PLL输出时钟3分频后输出给USB：120MHz/3=40MHz
    RCC->CFGR |= (7<<24);           // PLL输出时钟2分频后输出到MCO

    /* 选择PLL作为系统时钟源 */
    RCC->CFGR |= (2<<0);            // 00：选择 HSE 输出用作系统时钟; 01：选择 HSE 输出用作系统时钟; 10：选择 PLL 输出用作系统时钟; 11：选择 LSI 输出用作系统时钟
    while(0 == (RCC->CFGR&(2<<2))); // 等待PLL输出用作系统时钟稳定
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
