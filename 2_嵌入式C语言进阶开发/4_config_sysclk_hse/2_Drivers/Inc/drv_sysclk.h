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

#include "hal_rcc.h"

#define LSI_VALUE           (40000)
#define HSI_VALUE           (8000000)
#define HSE_VALUE           (12000000)

#define __USE_HSE           (0)
#define __USE_HSE_PLL       (1)
#define __USE_HSI_PLL       (0)

#define RCC_CFGR_SWS_Pos    (2U)                              
#define RCC_CFGR_SWS_Msk    (0x3UL << RCC_CFGR_SWS_Pos)        /*!< 0x0000000C */

#define RCC_CFGR_SWS_HSI    0x00000000U                       /*!< HSI oscillator used as system clock */
#define RCC_CFGR_SWS_HSE    0x00000004U                       /*!< HSE oscillator used as system clock */
#define RCC_CFGR_SWS_PLL    0x00000008U                       /*!< PLL used as system clock */
#define RCC_CFGR_SWS_LSI    0x0000000CU                       /*!< LSI used as system clock */

#define RCC_SYSCLKSOURCE_STATUS_HSI         RCC_CFGR_SWS_HSI            /*!< HSI used as system clock */
#define RCC_SYSCLKSOURCE_STATUS_HSE         RCC_CFGR_SWS_HSE            /*!< HSE used as system clock */
#define RCC_SYSCLKSOURCE_STATUS_PLLCLK      RCC_CFGR_SWS_PLL            /*!< PLL used as system clock */
#define RCC_SYSCLKSOURCE_STATUS_LSI         RCC_CFGR_SWS_LSI            /*!< LSI used as system clock */


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
uint32_t Drv_GetSysClockFreq(void);

#endif /* __DRV_SYSCLK_H */
