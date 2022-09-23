/**
 * @file drv_led.c
 * @author 百问网开发团队
 * @brief  PitayaLite 开发板板载用户LED驱动源文件
 * @version 0.1
 * @date 2022-05-30
 * 
 * @copyright Copyright (c) 2022 深圳百问网科技有限公司
 * 
 */

#include "drv_led.h"
#include "hal_rcc.h"
#include "hal_gpio.h"

/**
 * @brief LED驱动初始化函数
 * 
 */
void Drv_LED_Init(void)
{
    /* 调用GPIO的HAL库对象结构体声明GPIO对象 */
    GPIO_Init_Type gpio_init;
    
    /* 使能GPIO的时钟 */
    USER_LED_GPIO_CLK_EN();
    
    /* 设置GPIO的模式 */
    gpio_init.PinMode  = GPIO_PinMode_Out_PushPull;
    gpio_init.Speed = GPIO_Speed_50MHz;
    
    /* 选择引脚初始化 */
    gpio_init.Pins  = USER_LED_PIN;
    GPIO_Init(USER_LED_PORT, &gpio_init);
    
    /* 设置LED的默认状态：熄灭 */
    USER_LED(off);
}

/**
 * @brief 控制LED状态的驱动函数
 * 
 * @param type LED类型
 * @param status LED状态
 */
void Drv_LED_Write(LED_Type type, LED_Status status)
{
    switch(type)
    {
        case user_led:
        {
            USER_LED(status);
            break;
        }
        case reserved_led:  break;
        default:break;
    }
}
