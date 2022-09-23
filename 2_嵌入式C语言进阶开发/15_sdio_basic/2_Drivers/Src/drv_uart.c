/**
 * @file drv_uart.c
 * @author 百问网开发团队
 * @brief 
 * @version 0.1
 * @date 2022-05-31
 * 
 * @copyright Copyright (c) 2022 深圳百问网科技有限公司
 * 
 */

#include "drv_uart.h"
#include "drv_sysclk.h"
#include "hal_rcc.h"
#include "hal_gpio.h"
#include "hal_uart.h"

#include <stdio.h>

/**
 * @brief 初始化串口的GPIO
 * 
 */
static void Drv_UART_GPIO_Init(void)
{
    GPIO_Init_Type gpio_init;
    
    UART1_GPIO_CLK_EN();
    /* PA9  - UART1_TX. */
    /* PA10 - UART1_RX. */
    gpio_init.Pins  = UART1_TX_PIN;
    gpio_init.PinMode  = GPIO_PinMode_AF_PushPull;  // 复用推挽输出
    gpio_init.Speed = GPIO_Speed_50MHz;
    GPIO_Init(UART1_PORT, &gpio_init);
    GPIO_PinAFConf(UART1_PORT, gpio_init.Pins, GPIO_AF_7);
    
    gpio_init.Pins  = UART1_RX_PIN;
    gpio_init.PinMode  = GPIO_PinMode_In_Floating;  // 浮空输入
    gpio_init.Speed = GPIO_Speed_50MHz;
    GPIO_Init(UART1_PORT, &gpio_init);
    GPIO_PinAFConf(UART1_PORT, gpio_init.Pins, GPIO_AF_7);
}

/**
 * @brief 初始化串口
 * -# 使能时钟
 * -# 初始化引脚
 * -# 配置参数：波特率115200，8位数据位，1位停止位，没有校验位，没有硬件控制流
 * 
 */
void Drv_UART_Init(void)
{
    UART_Init_Type uart_init;
    
    UART_INSTANCE_CLK_EN();
    Drv_UART_GPIO_Init();

    uart_init.ClockFreqHz   = Drv_GetSysClockFreq()>>1;
    uart_init.BaudRate      = 115200;
    uart_init.WordLength    = UART_WordLength_8b;
    uart_init.StopBits      = UART_StopBits_1;
    uart_init.Parity        = UART_Parity_None;
    uart_init.XferMode      = UART_XferMode_RxTx;
    uart_init.HwFlowControl = UART_HwFlowControl_None;
    UART_Init(UART_INSTANCE, &uart_init);
    UART_Enable(UART_INSTANCE, true);
}


#if defined (__ARMCC_VERSION) && (__ARMCC_VERSION < 6010050)
struct __FILE{
   int handle;
};
#endif

int fputc(int ch, FILE *f) 
{
    /* Your implementation of fputc(). */
    while ( 0u == (UART_STATUS_TX_EMPTY & UART_GetStatus(UART_INSTANCE)) )
    {}
    UART_PutData(UART_INSTANCE, (uint8_t)(ch));
    return ch;
}

int fgetc(FILE *f)
{
    while ( 0u == (UART_STATUS_RX_DONE & UART_GetStatus(UART_INSTANCE)) )
    {}
    return UART_GetData(UART_INSTANCE);
}

