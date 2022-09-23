/**
 * @file drv_key.c
 * @author 百问网开发团队
 * @brief 
 * @version 0.1
 * @date 2022-05-30
 * 
 * @copyright Copyright (c) 2022 深圳百问网科技有限公司
 * 
 */

#include "drv_key.h"
#include "hal_gpio.h"
#include "hal_rcc.h"

/**
 * @brief 初始化按键的GPIO为内部上拉输入模式
 * 
 */
void Drv_Key_Init(void)
{
    /* 调用GPIO的HAL库对象结构体声明GPIO对象 */
    GPIO_Init_Type gpio_init;
    
    /* 使能GPIO的时钟 */
    USER_KEY_GPIO_CLK_EN();
    
    /* 设置GPIO的模式 */
    gpio_init.PinMode  = GPIO_PinMode_In_PullUp;
    gpio_init.Speed = GPIO_Speed_50MHz;
    
    /* 选择引脚初始化 */
    gpio_init.Pins  = USER_KEY_PIN;
    GPIO_Init(USER_KEY_PORT, &gpio_init);
}

/**
 * @brief 根据按键类型读取该按键的状态
 * 
 * @param key 示例只有用户按键
 * -# user_key 用户按键
 * 
 * @return KeyStatus_t 
 * @retval pressed 按键按下
 * @retval released 按键松开
 */
KeyStatus_t Drv_Key_Read(KeyType_t key)
{
    volatile KeyStatus_t status = released;
    switch(key)
    {
        case user_key:
        {
            status = USER_KEY;
            break;
        }
        case reserved_key:  break;
        default: break;
    }

    return status;
}

