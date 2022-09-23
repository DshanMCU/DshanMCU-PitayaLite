/**
 * @file drv_uart.h
 * @author 百问网开发团队
 * @brief 
 * @version 0.1
 * @date 2022-05-31
 * 
 * @copyright Copyright (c) 2022 深圳百问网科技有限公司
 * 
 */

#ifndef __DRV_UART_H
#define __DRV_UART_H

#define UART1_TX_PIN            GPIO_PIN_6
#define UART1_RX_PIN            GPIO_PIN_7
#define UART1_PORT              GPIOB
#define UART1_GPIO_CLK_EN()     RCC_EnableAHB1Periphs(RCC_AHB1_PERIPH_GPIOB, true)

#define UART_INSTANCE           UART1
#define UART_INSTANCE_CLK_EN()  RCC_EnableAPB2Periphs(RCC_APB2_PERIPH_UART1, true)

/**
 * @brief 初始化串口
 * -# 使能时钟
 * -# 初始化引脚
 * -# 配置参数：波特率115200，8位数据位，1位停止位，没有校验位，没有硬件控制流
 * 
 */
void Drv_UART_Init(void);

#endif /* __DRIVER_UART_H */
