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
#include "drv_buffer.h"

#include "hal_rcc.h"
#include "hal_gpio.h"
#include "hal_uart.h"

#include <stdio.h>

#define __HAL_UART_GET_FLAG(__HANDLE__, __FLAG__)       (((__HANDLE__)->CSR & (__FLAG__)) == (__FLAG__))
#define __HAL_UART_GET_IT_FLAG(__HANDLE__, __FLAG__)    (((__HANDLE__)->ISR & (__FLAG__)) == (__FLAG__))
#define __HAL_UART_CLEAR_IT_FLAG(__HANDLE__, __FLAG__)  ((__HANDLE__)->ICR |= (__FLAG__))

static RingBuffer uart_buf;
volatile static uint8_t tx_cplt = 0;

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
    
    Drv_Buffer_Init(&uart_buf, 256);
    
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

    /* 使能收发中断 */
    UART_EnableInterrupts(UART_INSTANCE, UART_INT_RX_DONE|UART_IER_TXCIEN_MASK, true);
    NVIC_EnableIRQ(UART1_IRQn);
    /* 使能串口中断 */
    UART_Enable(UART_INSTANCE, true);
}

/**
 * @brief 读取串口数据
 * 
 * @param buf 数据保存地址
 * @param len 读取数据的个数
 * @return int 
 * @retval 0-ok
 * @retval -1-error
 */
int Drv_UART_Read(uint8_t *buf, uint16_t len)
{
    return Drv_Buffer_ReadBytes(&uart_buf, buf, len);
}

/**
 * @brief 发送串口数据
 * 
 * @param buf 数据保存地址
 * @return int 
 * @retval 0-ok
 * @retval -1-error
 */
int Drv_UART_Write(uint8_t *buf)
{
    while(*buf != '\0')
    {
        tx_cplt = 0;
        UART_PutData(UART_INSTANCE, (uint8_t)(*buf));
        while(!tx_cplt);
        buf++;
    }
    return 0;
}

void UART1_IRQHandler(void)
{
    // RX irq
    if(__HAL_UART_GET_IT_FLAG(UART1, UART_INT_RX_DONE) != 0)
    {
        if(__HAL_UART_GET_FLAG(UART1, UART_INT_RX_DONE) != 0)
        {
            __HAL_UART_CLEAR_IT_FLAG(UART1, UART_INT_RX_DONE);
            volatile uint8_t data = UART_GetData(UART1);
            Drv_Buffer_Write(&uart_buf, data);
        }
    }
    // TX irq
    if(__HAL_UART_GET_IT_FLAG(UART1, UART_ISR_TXCINTF_MASK) != 0)
    {
        if(__HAL_UART_GET_FLAG(UART1, UART_INT_TX_EMPTY) != 0)
        {
            __HAL_UART_CLEAR_IT_FLAG(UART1, UART_ISR_TXCINTF_MASK);
            tx_cplt = 1;
        }
    }
}

#if defined (__ARMCC_VERSION) && (__ARMCC_VERSION < 6010050)
struct __FILE{
   int handle;
};
#endif

FILE __stdout, __stdin;
int fputc(int ch, FILE *f) 
{
    /* Your implementation of fputc(). */
    tx_cplt = 0;
    UART_PutData(UART_INSTANCE, (uint8_t)(ch));
    while(!tx_cplt);
    return ch;
}

int fgetc(FILE *f)
{
    uint8_t data = 0;
    Drv_Buffer_Read(&uart_buf, &data);
    return data;
}
