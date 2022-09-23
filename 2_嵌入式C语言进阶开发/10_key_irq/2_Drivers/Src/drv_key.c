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
#include "drv_systick.h"

#include "hal_gpio.h"
#include "hal_rcc.h"
#include "hal_syscfg.h"
#include "hal_exti.h"

#define __HAL_GPIO_EXTI_GET_IT(__EXTI_LINE__) (EXTI->PR & (__EXTI_LINE__))
#define __HAL_GPIO_EXTI_CLEAR_IT(__EXTI_LINE__) (EXTI->PR = (__EXTI_LINE__))
static volatile uint32_t trigger_time = 0;

volatile KeyEvent_t key_event = {0};

/**
 * @brief 外部中断初始化函数
 * -# 使能SYSCFG
 * -# 将外部中断线设置为复用模式
 * -# 设置外部中断线的触发方式
 * -# 使能外部中断线
 * -# 使能对应的外部中断
 */
static void Drv_Exti_Init(void)
{
    /* 使能SYSCFG的时钟 */
    RCC_EnableAPB2Periphs(RCC_APB2_PERIPH_SYSCFG, true);
    /* 将外部中断线设置为复用模式 */
    SYSCFG_SetExtIntMux(SYSCFG_EXTIPort_GPIOA, SYSCFG_EXTILine_0);
    /* 设置外部中断线的触发方式 */
    EXTI_SetTriggerIn(EXTI, EXTI_LINE_0, EXTI_TriggerIn_BothEdges); // 双边沿触发
    /* 使能外部中断线 */
    EXTI_EnableLineInterrupt(EXTI, EXTI_LINE_0, true);
    /* 使能对应的外部中断 */
    NVIC_EnableIRQ(EXTI0_IRQn);
}

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

    /* 初始化外部中断 */
    Drv_Exti_Init();
}

/**
 * @brief 根据按键类型读取该按键的状态
 * 
 * @param key 示例只有用户按键
 * -# user_key 用户按键
 * 
 * @return KeyEvent_t 
 */
KeyEvent_t Drv_Key_Read(KeyType_t key)
{
    volatile KeyEvent_t keyevent = {0};
    switch(key)
    {
        case user_key:
        {
            if(key_event.type == user_key && key_event.time != 0)
            {
                keyevent = key_event;
                key_event.time = 0;
                key_event.type = 0;
            }
            
            break;
        }
        case reserved_key:  break;
        default: break;
    }

    return keyevent;
}

void Drv_Key_ProcessCallBack(uint32_t tick)
{
    static uint32_t pressed_tick = 0; 
    static uint32_t released_tick = 0; 
    if(tick == trigger_time)
    {
        if(USER_KEY == pressed)     pressed_tick = tick;
        else                        released_tick = tick;

        if(pressed_tick!=0 && released_tick!=0 && pressed_tick<released_tick)
        {
            key_event.type = user_key;
            key_event.time = released_tick - pressed_tick;
            released_tick = pressed_tick = 0;
        }
    }
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if(USER_KEY_PIN == GPIO_Pin)
    {
        trigger_time = HAL_GetTick() + 50;
    }
}

void HAL_GPIO_EXTI_IRQHandler(uint16_t GPIO_Pin)
{
    if (__HAL_GPIO_EXTI_GET_IT(GPIO_Pin) != 0x00u)
    {
        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_Pin);
        HAL_GPIO_EXTI_Callback(GPIO_Pin);
    }
}

void EXTI0_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(USER_KEY_PIN);
}

