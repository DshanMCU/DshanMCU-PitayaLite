/**
 * @file drv_led.h
 * @author 百问网开发团队
 * @brief  PitayaLite 开发板板载用户LED驱动头文件
 * @version 0.1
 * @date 2022-05-30
 * 
 * @copyright Copyright (c) 2022 深圳百问网科技有限公司
 * 
 */

#ifndef __DRV_LED_H
#define __DRV_LED_H

#define USER_LED_PIN             GPIO_PIN_1
#define USER_LED_PORT            GPIOA
#define USER_LED(STATUS)         GPIO_WriteBit(USER_LED_PORT, USER_LED_PIN, STATUS)

#define USER_LED_GPIO_CLK_EN()   RCC_EnableAHB1Periphs(RCC_AHB1_PERIPH_GPIOA, true)

/**
 * @brief LED状态枚举
 * @param on=0, LED亮
 * @param off=1, LED灭
 */
typedef enum{
    on  = 0,
    off = !on,
}LED_Status;

/**
 * @brief LED类型枚举
 * @param user_led=0，用户LED
 * @param reserved，保留类型，用于将来扩展
 */
typedef enum{
    user_led  = 0,
    reserved_led,
}LED_Type;

/**
 * @brief LED驱动初始化函数
 * 
 */
void Drv_LED_Init(void);

/**
 * @brief 控制LED状态的驱动函数
 * 
 * @param type LED类型
 * @param status LED状态
 */
void Drv_LED_Write(LED_Type type, LED_Status status);

#endif /* __DRV_LED_H */
