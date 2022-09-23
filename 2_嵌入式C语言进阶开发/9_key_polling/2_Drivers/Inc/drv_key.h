/**
 * @file drv_key.h
 * @author 百问网开发团队
 * @brief 
 * @version 0.1
 * @date 2022-05-30
 * 
 * @copyright Copyright (c) 2022 深圳百问网科技有限公司
 * 
 */

#ifndef __DRV_KEY_H
#define __DRV_KEY_H

#define USER_KEY_PIN             GPIO_PIN_0
#define USER_KEY_PORT            GPIOA
#define USER_KEY                 GPIO_ReadInDataBit(USER_KEY_PORT, USER_KEY_PIN)
#define USER_KEY_GPIO_CLK_EN()   RCC_EnableAHB1Periphs(RCC_AHB1_PERIPH_GPIOA, true)

/**
 * @brief 按键状态枚举
 * -# pressed=0, 按键按下
 * -# released=1， 按键松开
 */
typedef enum{
    pressed = 0,
    released = !pressed,
}KeyStatus_t;

/**
 * @brief 按键类型枚举
 * -# user_key 用户按键
 * -# reserved 保留，留作将来扩展用
 */
typedef enum{
    user_key = 0,
    reserved_key,
}KeyType_t;

/**
 * @brief 初始化按键的GPIO为内部上拉输入模式
 * 
 */
void Drv_Key_Init(void);

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
KeyStatus_t Drv_Key_Read(KeyType_t key);

#endif /* __DRV_LED_H */

